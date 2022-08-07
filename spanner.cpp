#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <map>
#include "spanner.hpp"

namespace spancopy {

spanner::spanner(const double size_in_gbs, const fs::path source, const fs::path target) noexcept
    :
    m_size_in_gbs{size_in_gbs},
    m_source{source},
    m_target{target}
{
}

bool spanner::span() const noexcept
{
    std::cout << "[INF] source: [" << m_source << "]\n";
    std::cout << "[INF] target: [" << m_target << "]\n";

    std::size_t source_dir_file_counter{0};
    std::uintmax_t source_dir_size_in_bytes = 0;
    std::map<fs::path, std::uintmax_t> ordered_source_paths;
    for (const auto& entry : fs::recursive_directory_iterator(m_source))
    {
        if (entry.is_regular_file())
        {
            const auto path = entry.path();
            const auto size = entry.file_size();
            ordered_source_paths.emplace(path, size);
            source_dir_size_in_bytes += size;
            ++source_dir_file_counter;
        }
    }

    if (source_dir_file_counter == 0)
    {
        std::cerr << "[ERR] no source files found!\n";
        return false;
    }

    std::cout << "[INF] source directory info: [files: "
              << source_dir_file_counter << ", size (bytes): "
              << source_dir_size_in_bytes << ", size (GB): "
              << std::fixed
              << bytes_to_gbs(source_dir_size_in_bytes)
              << "]\n";

    if (!is_space_available(source_dir_size_in_bytes))
    {
        std::cerr << "[ERR] required space not available! ["
                  << source_dir_size_in_bytes << " bytes]\n";
        return false;
    }

    const auto target_root_dir_path = m_target / get_target_root_dir_name();
    std::cout << "[INF] target root directory: [" << target_root_dir_path << "]\n";
    if (fs::exists(target_root_dir_path))
    {
        std::cerr << "[ERR] target directory already exists! [" << target_root_dir_path << "]\n";
        return false;
    }

    const auto source_root_dir_path_str = m_source.generic_string();

    // const auto size_threshold_in_bytes = gbs_to_bytes(m_size_in_gbs);
    const auto size_threshold_in_bytes = m_size_in_gbs;

    std::size_t target_subdir_counter{1};
    std::uintmax_t target_subdir_size{0};
    fs::path target_subdir_root_path;
    for (const auto& [source_file_path, source_file_size] : ordered_source_paths)
    {
        if ((target_subdir_size + source_file_size) > size_threshold_in_bytes)
        {
            target_subdir_size = 0;
            ++target_subdir_counter;
        }

        if (target_subdir_size == 0)
        {
            const auto subdir_name = std::to_string(target_subdir_counter);
            target_subdir_root_path = target_root_dir_path / subdir_name;
        }

        const auto source_file_path_str = source_file_path.generic_string();
        const auto relative_source_file_path_str = source_file_path_str.substr(source_root_dir_path_str.length());
        const auto target_subdir_root_path_str = target_subdir_root_path.generic_string();
        const auto target_file_path = fs::path{target_subdir_root_path_str + relative_source_file_path_str};
        fs::create_directories(target_file_path.parent_path());
        fs::copy(source_file_path, target_file_path);

        target_subdir_size += source_file_size;

        std::cout << "[INF] copying [" << source_file_path << "] ("
                  << source_file_size << ") >> "
                  << "[" << target_file_path << "] ("
                  << target_subdir_size << ")\n";
    }

    std::cout << "[INF] --- [DONE] ---\n";
    return true;
}

std::string spanner::get_target_root_dir_name() const noexcept
{
    using namespace std::chrono;
    std::ostringstream oss;
    const auto now = system_clock::to_time_t(system_clock::now());
    oss << std::put_time(std::localtime(&now), "%Y%m%d");
    return oss.str();
}

std::uintmax_t spanner::gbs_to_bytes(const double gbs) const noexcept
{
    return (gbs * 1024 * 1024 * 1024);
}

double spanner::bytes_to_gbs(const std::uintmax_t bytes) const noexcept
{
    return (bytes / 1024. / 1024. / 1024.);
}

bool spanner::is_space_available(const std::uintmax_t source_dir_size_in_bytes) const noexcept
{
    const auto target_space_info = fs::space(m_target);
    return (target_space_info.available > source_dir_size_in_bytes);
}

} // spancopy
