#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <map>
#include "spanner.hpp"

namespace spancopy {

spanner::spanner(const double size, const fs::path source, const fs::path target, const bool dry_run) noexcept
    :
    m_size{size},
    m_source{source},
    m_target{target},
    m_dry_run{dry_run}
{
}

bool spanner::span() const noexcept
{
    std::cout << "source: " << m_source << "\n";
    std::cout << "target: " << m_target << "\n";

    const auto target_dir = m_target / get_current_date();
    std::cout << "[INF] target_dir: " << target_dir << '\n';
    if (fs::exists(target_dir))
    {
        std::cerr << "[ERR] target directory exists! [" << target_dir << "]\n";
        // fs::remove_all(target_dir);
        return false;
    }

    std::uintmax_t source_size_in_bytes = 0;
    std::map<fs::path, std::uintmax_t> source_path_with_size_map;
    for (const auto& entry : fs::recursive_directory_iterator(m_source))
    {
        if (entry.is_regular_file())
        {
            const auto path = entry.path();
            const auto size = entry.file_size();
            source_path_with_size_map.emplace(path, size);
            source_size_in_bytes += size;
        }
    }

    if (!is_space_available(source_size_in_bytes))
    {
        std::cerr << "[ERR] required space not available! ["
                  << source_size_in_bytes << " bytes]\n";
        return false;
    }

    for (const auto& [path, size] : source_path_with_size_map)
    {
        std::cout << path << "\t[" << size << "]\n";
    }

    std::cout << "[INF] source directory size: "
              << source_size_in_bytes << " bytes ["
              << std::fixed
              << bytes_to_gbs(source_size_in_bytes)
              << " GB (approx)]\n";

    return true;
}

std::string spanner::get_current_date() const noexcept
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

bool spanner::is_space_available(const std::uintmax_t source_size_in_bytes) const noexcept
{
    const auto target_space_info = fs::space(m_target);
    return (target_space_info.available > source_size_in_bytes);
}

} // spancopy
