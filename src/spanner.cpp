#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include "spanner.hpp"

namespace spancopy {

spanner::spanner(
    const std::uintmax_t threshold,
    const fs::path& source,
    const fs::path& destination) noexcept
    :
    m_threshold{threshold},
    m_source{source},
    m_destination{destination},
    m_destination_root_dir_path{generate_destination_root_dir_path()}
{
    std::cout << "[INF] threshold:        [" << m_threshold << " bytes]\n"
              << "[INF] source:           [" << m_source.generic_string() << "]\n"
              << "[INF] destination:      [" << m_destination.generic_string() << "]\n"
              << "[INF] destination root: [" << m_destination_root_dir_path.generic_string() << "]"
              << std::endl;
}

bool spanner::span() const noexcept
{
    if (!remove_destination_dir_if_exists())
    {
        return false;
    }

    if (const auto source_file_map = generate_source_file_map(); !source_file_map)
    {
        return false;
    }
    else if (!copy_source_to_destination(*source_file_map))
    {
        return false;
    }

    return true;
}

fs::path spanner::generate_destination_root_dir_path() const noexcept
{
    using namespace std::chrono;
    std::ostringstream oss;
    const auto now = system_clock::now();
    const auto now_time_t = system_clock::to_time_t(now);
    const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    oss << std::put_time(std::localtime(&now_time_t), "%Y%m%dT%H%M%S") << ms.count();
    return m_destination / oss.str();
}

bool spanner::is_destination_space_available(const std::uintmax_t bytes) const noexcept
{
    if (const auto space_info = fs::space(m_destination, m_ec); m_ec)
    {
        std::cerr << "[ERR] unable to query available space! " << m_ec.message() << '\n';
        return false;
    }
    else
    {
        return (space_info.available > bytes);
    }
}

bool spanner::remove_destination_dir_if_exists() const noexcept
{
    if (!fs::exists(m_destination_root_dir_path, m_ec) && !m_ec)
    {
        std::cout << "[INF] no existing destination root directory found!\n";
        return true;
    }
    else if (m_ec)
    {
        std::cerr << "[ERR] unable to check existence of destination root directory! " << m_ec.message() << '\n';
        return false;
    }

    std::cout << "[WRN] removing existing destination root directory [" << m_destination_root_dir_path.generic_string() << "]\n";
    for (const auto& entry : fs::directory_iterator{m_destination_root_dir_path, m_ec})
    {
        if (m_ec)
        {
            std::cerr << "[ERR] unable to iterate over destination root directory path! " << m_ec.message() << '\n';
            return false;
        }

        const auto path = entry.path();
        if (fs::remove_all(path, m_ec) < 0)
        {
            std::cerr << "[ERR] unable to remove destination subdirectory path! [" << path << "] " << m_ec.message() << '\n';
            return false;
        }
    }

    if (!fs::remove(m_destination_root_dir_path, m_ec))
    {
        std::cerr << "[ERR] unable to remove destination root directory path! ["
                  << m_destination_root_dir_path.generic_string() << "] "
                  << m_ec.message() << '\n';
        return false;
    }

    std::cout << "[INF] existing destination root directory removed! [" << m_destination_root_dir_path.generic_string() << "]\n";
    return true;
}

fs::path spanner::generate_destination_file_path(
    const fs::path& source_file_path,
    const fs::path& destination_subdir_root_path) const noexcept
{
    const auto source_root_dir_path_str = m_source.generic_string();
    const auto source_file_path_str = source_file_path.generic_string();
    const auto relative_source_file_path_str = source_file_path_str.substr(source_root_dir_path_str.length());
    const auto destination_subdir_root_path_str = destination_subdir_root_path.generic_string();
    const auto destination_file_path = fs::path{destination_subdir_root_path_str + relative_source_file_path_str};
    return destination_file_path;
}

std::optional<spanner::source_file_map_t> spanner::generate_source_file_map() const noexcept
{
    std::size_t unspanable_source_file_count{0};
    std::size_t total_source_file_count{0};
    std::uintmax_t source_dir_size{0};
    source_file_map_t source_file_map;
    for (const auto& entry : fs::recursive_directory_iterator{m_source, m_ec})
    {
        if (m_ec)
        {
            std::cerr << "[ERR] unable to iterate over source directory! [" << m_source.generic_string() << "] " << m_ec.message() << '\n';
            return std::nullopt;
        }

        if (entry.is_regular_file(m_ec))
        {
            const auto path = entry.path();
            if (m_ec)
            {
                std::cerr << "[WRN] unable to check if it is a regular file! [" << path.generic_string() << "] " << m_ec.message() << '\n';
                continue;
            }

            const auto size = entry.file_size();
            if (size > m_threshold)
            {
                std::cerr << "[WRN] file cannot be spanned! [" << path.generic_string() << "] (" << size << ")\n";
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
                  << "[ERR] SUMMARY: " << unspanable_source_file_count << " out of "
                  << total_source_file_count << " files cannot be spanned!\n";
        return std::nullopt;
    }

    std::cout << "[INF] source directory info: [files: " << total_source_file_count
              << ", size: " << source_dir_size << " bytes]\n";

    if (!is_destination_space_available(source_dir_size))
    {
        std::cerr << "[ERR] required space not available! ["
                  << source_dir_size << " bytes]\n";
        return std::nullopt;
    }

    return source_file_map;
}

bool spanner::copy_source_to_destination(const source_file_map_t& source_file_map) const noexcept
{
    std::cout << "[INF] copying source files to destination\n";

    std::size_t total_destination_file_count{0};
    std::size_t destination_subdir_count{1};
    std::uintmax_t destination_subdir_size{0};
    fs::path destination_subdir_root_path;
    for (const auto& [source_parent_path, source_file] : source_file_map)
    {
        for (const auto& [source_filename, source_file_size] : source_file)
        {
            if ((destination_subdir_size + source_file_size) > m_threshold)
            {
                destination_subdir_size = 0;
                ++destination_subdir_count;
            }

            if (destination_subdir_size == 0)
            {
                const auto subdir_name = std::to_string(destination_subdir_count);
                destination_subdir_root_path = m_destination_root_dir_path / subdir_name;
            }

            const auto source_file_path = source_parent_path / source_filename;
            const auto destination_file_path = generate_destination_file_path(source_file_path, destination_subdir_root_path);
            const auto destination_parent_path = destination_file_path.parent_path();

            fs::create_directories(destination_parent_path, m_ec);
            if (m_ec)
            {
                std::cerr << "[ERR] unable to create destination parent path! [" << destination_parent_path << "] "
                            << m_ec.message() << '\n';
                return false;
            }

            fs::copy(source_file_path, destination_file_path, m_ec);
            if (m_ec)
            {
                std::cerr << "[ERR] unable to copy file! [" << source_filename << "] " << m_ec.message() << '\n';
                return false;
            }

            destination_subdir_size += source_file_size;
            ++total_destination_file_count;

            std::cout << "[INF] copying file # " << total_destination_file_count << ' '
                      << "[" << source_file_path.generic_string() << "] ("
                      << source_file_size << ") => "
                      << "[" << destination_file_path.generic_string() << "] ("
                      << destination_subdir_size << ")\n";
        }
    }

    std::cout << "[INF] SUMMARY: total " << total_destination_file_count << " files spanned over "
              << destination_subdir_count << " subdirectories\n";
    return true;
}

} // spancopy
