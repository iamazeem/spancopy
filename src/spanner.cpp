#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include "spanner.hpp"

namespace spancopy {

spanner::spanner(const config& config) noexcept
    :
    m_config{config},
    m_target_root_dir_path{generate_target_root_dir_path()}
{
    std::cout << "[INF] threshold: [" << m_config.threshold() << " bytes]\n"
              << "[INF] source:    [" << m_config.source().generic_string() << "]\n"
              << "[INF] target:    [" << m_config.target().generic_string() << "]\n"
              << "[INF] target root directory: [" << m_target_root_dir_path.generic_string() << "]\n";
}

bool spanner::span() const noexcept
{
    remove_target_dir_if_exists();

    if (const auto source_file_map = generate_source_file_map(); !source_file_map)
    {
        return false;
    }
    else
    {
        copy_source_to_target(*source_file_map);
    }

    return true;
}

fs::path spanner::generate_target_root_dir_path() const noexcept
{
    using namespace std::chrono;
    std::ostringstream oss;
    const auto now = system_clock::to_time_t(system_clock::now());
    oss << std::put_time(std::localtime(&now), "%Y%m%d");
    return m_config.target() / oss.str();
}

bool spanner::is_target_space_available(const std::uintmax_t bytes) const noexcept
{
    return (fs::space(m_config.target()).available > bytes);
}

void spanner::remove_target_dir_if_exists() const noexcept
{
    if (!fs::exists(m_target_root_dir_path))
    {
        return;
    }

    std::cout << "[WRN] removing existing target root directory [" << m_target_root_dir_path.generic_string() << "]\n";
    for (const auto& entry : fs::directory_iterator{m_target_root_dir_path})
    {
        fs::remove_all(entry.path());
    }
    fs::remove(m_target_root_dir_path);
    std::cout << "[WRN] existing target root directory removed! [" << m_target_root_dir_path.generic_string() << "]\n";
}

fs::path spanner::generate_target_file_path(
    const fs::path& source_file_path,
    const fs::path& target_subdir_root_path) const noexcept
{
    const auto source_root_dir_path_str = m_config.source().generic_string();
    const auto source_file_path_str = source_file_path.generic_string();
    const auto relative_source_file_path_str = source_file_path_str.substr(source_root_dir_path_str.length());
    const auto target_subdir_root_path_str = target_subdir_root_path.generic_string();
    const auto target_file_path = fs::path{target_subdir_root_path_str + relative_source_file_path_str};
    return target_file_path;
}

std::optional<spanner::source_file_map_t> spanner::generate_source_file_map() const noexcept
{
    std::size_t unspanable_source_file_count{0};
    std::size_t total_source_file_count{0};
    std::uintmax_t source_dir_size{0};
    source_file_map_t source_file_map;
    for (const auto& entry : fs::recursive_directory_iterator(m_config.source()))
    {
        if (entry.is_regular_file())
        {
            const auto path = entry.path();
            const auto size = entry.file_size();

            if (size > m_config.threshold())
            {
                std::cerr << "[ERR] file cannot be spanned! [" << path.generic_string() << "] (" << size << ")\n";
                ++unspanable_source_file_count;
            }

            source_dir_size += size;
            ++total_source_file_count;

            if (auto it = source_file_map.find(path.parent_path());
                (it == source_file_map.end()))
            {
                const auto source_file = source_file_t
                {
                    { path.filename(), size }
                };
                source_file_map.emplace(path.parent_path(), source_file);
            }
            else
            {
                it->second.emplace(path.filename(), size);
            }
        }
    }

    if (source_file_map.empty())
    {
        std::cerr << "[ERR] no source files found to copy!\n";
        return std::nullopt;
    }

    if (unspanable_source_file_count > 0)
    {
        std::cerr << "[ERR] file sizes must be less than or equal to threshold!\n"
                  << "[ERR] summary: " << unspanable_source_file_count << " out of "
                  << total_source_file_count << " files cannot be spanned!\n";
        return std::nullopt;
    }

    std::cout << "[INF] source directory info: [files: " << total_source_file_count
              << ", size: " << source_dir_size << " bytes]\n";

    if (!is_target_space_available(source_dir_size))
    {
        std::cerr << "[ERR] required space not available! ["
                  << source_dir_size << " bytes]\n";
        return std::nullopt;
    }

    return source_file_map;
}

void spanner::copy_source_to_target(const source_file_map_t& source_file_map) const noexcept
{
    std::cout << "[INF] copying source files to target\n";

    std::size_t total_target_file_count{0};
    std::size_t target_subdir_count{1};
    std::uintmax_t target_subdir_size{0};
    fs::path target_subdir_root_path;
    for (const auto& [source_parent_path, source_file] : source_file_map)
    {
        for (const auto& [source_filename, source_file_size] : source_file)
        {
            if ((target_subdir_size + source_file_size) > m_config.threshold())
            {
                target_subdir_size = 0;
                ++target_subdir_count;
            }

            if (target_subdir_size == 0)
            {
                const auto subdir_name = std::to_string(target_subdir_count);
                target_subdir_root_path = m_target_root_dir_path / subdir_name;
            }

            const auto source_file_path = source_parent_path / source_filename;
            const auto target_file_path = generate_target_file_path(source_file_path, target_subdir_root_path);
            fs::create_directories(target_file_path.parent_path());
            fs::copy(source_file_path, target_file_path);

            target_subdir_size += source_file_size;
            ++total_target_file_count;

            std::cout << "[INF] copying file # " << total_target_file_count << ' '
                      << "[" << source_file_path.generic_string() << "] ("
                      << source_file_size << ") => "
                      << "[" << target_file_path.generic_string() << "] ("
                      << target_subdir_size << ")\n";
        }
    }

    std::cout << "[INF] summary: total " << total_target_file_count << " files spanned over "
              << target_subdir_count << " subdirectories\n";
}

} // spancopy
