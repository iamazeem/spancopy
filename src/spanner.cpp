#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <ctime>
#include <map>
#include "spanner.hpp"
#include "utils.hpp"

namespace spancopy {

spanner::spanner(const configuration& configuration) noexcept
    :
    m_configuration{configuration}
{
}

bool spanner::span() const noexcept
{
    std::cout << "[INF] source: [" << m_configuration.source().generic_string() << "]\n";
    std::cout << "[INF] target: [" << m_configuration.target().generic_string() << "]\n";

    const auto target_root_dir_path = generate_target_root_dir_path();
    std::cout << "[INF] target root directory: [" << target_root_dir_path.generic_string() << "]\n";
    remove_target_dir_if_exists(target_root_dir_path);

    // { key: filename, value: size } (sorted filenames)
    using source_file_info_t = std::map<fs::path, std::uintmax_t>;

    // { key: parent path, value: { filename, size } } (sorted parent paths)
    using source_file_mapping_t = std::map<fs::path, source_file_info_t>;

    std::size_t source_file_counter{0};
    std::uintmax_t source_dir_size_in_bytes{0};
    source_file_mapping_t source_file_mapping;
    for (const auto& entry : fs::recursive_directory_iterator(m_configuration.source()))
    {
        if (entry.is_regular_file())
        {
            const auto path = entry.path();
            const auto size = entry.file_size();
            source_dir_size_in_bytes += size;

            ++source_file_counter;

            if (auto it = source_file_mapping.find(path.parent_path());
                (it == source_file_mapping.end()))
            {
                const auto source_file_info = source_file_info_t
                {
                    { path.filename(), size }
                };
                source_file_mapping.emplace(path.parent_path(), source_file_info);
            }
            else
            {
                it->second.emplace(path.filename(), size);
            }
        }
    }

    if (source_file_counter == 0)
    {
        std::cerr << "[ERR] no source files found to copy!\n";
        return false;
    }

    std::cout << "[INF] source directory info: [files: "
              << source_file_counter << ", size (bytes): "
              << source_dir_size_in_bytes << ", size (GB): "
              << std::fixed
              << utils::bytes_to_gbs(source_dir_size_in_bytes)
              << "]\n";

    if (!utils::is_space_available(m_configuration.target(), source_dir_size_in_bytes))
    {
        std::cerr << "[ERR] required space not available! ["
                  << source_dir_size_in_bytes << " bytes]\n";
        return false;
    }

    // const auto threshold_in_bytes = utils::gbs_to_bytes(m_configuration.threshold());
    const auto threshold_in_bytes = m_configuration.threshold();

    std::size_t target_subdir_counter{1};
    std::uintmax_t target_subdir_size{0};
    fs::path target_subdir_root_path;
    for (const auto& [source_parent_path, source_file_info] : source_file_mapping)
    {
        for (const auto& [source_filename, source_file_size] : source_file_info)
        {
            if ((target_subdir_size + source_file_size) > threshold_in_bytes)
            {
                target_subdir_size = 0;
                ++target_subdir_counter;
            }

            if (target_subdir_size == 0)
            {
                const auto subdir_name = std::to_string(target_subdir_counter);
                target_subdir_root_path = target_root_dir_path / subdir_name;
            }

            const auto source_file_path = source_parent_path / source_filename;
            const auto target_file_path = generate_target_file_path(source_file_path, target_subdir_root_path);
            fs::create_directories(target_file_path.parent_path());
            fs::copy(source_file_path, target_file_path);

            target_subdir_size += source_file_size;

            std::cout << "[INF] copying [" << source_file_path.generic_string() << "] ("
                    << source_file_size << ") >> "
                    << "[" << target_file_path.generic_string() << "] ("
                    << target_subdir_size << ")\n";
        }
    }

    std::cout << "[INF] summary: total " << source_file_counter << " files spanned over "
              << target_subdir_counter << " subdirectories\n";

    std::cout << "[INF] --- [DONE] ---\n";
    return true;
}

fs::path spanner::generate_target_root_dir_path() const noexcept
{
    using namespace std::chrono;
    std::ostringstream oss;
    const auto now = system_clock::to_time_t(system_clock::now());
    oss << std::put_time(std::localtime(&now), "%Y%m%d");
    return m_configuration.target() / oss.str();
}

void spanner::remove_target_dir_if_exists(const fs::path& target_root_dir_path) const noexcept
{
    if (fs::exists(target_root_dir_path))
    {
        std::cerr << "[WRN] target root directory exists! [" << target_root_dir_path.generic_string() << "]\n";
        for (const auto& entry : fs::directory_iterator{target_root_dir_path})
        {
            fs::remove_all(entry.path());
        }
        fs::remove(target_root_dir_path);
        std::cerr << "[WRN] target root directory removed! [" << target_root_dir_path.generic_string() << "]\n";
    }
}

fs::path spanner::generate_target_file_path(
    const fs::path& source_file_path,
    const fs::path& target_subdir_root_path) const noexcept
{
    static const auto source_root_dir_path_str = m_configuration.source().generic_string();
    const auto source_file_path_str = source_file_path.generic_string();
    const auto relative_source_file_path_str = source_file_path_str.substr(source_root_dir_path_str.length());
    const auto target_subdir_root_path_str = target_subdir_root_path.generic_string();
    const auto target_file_path = fs::path{target_subdir_root_path_str + relative_source_file_path_str};
    return target_file_path;
}

} // spancopy
