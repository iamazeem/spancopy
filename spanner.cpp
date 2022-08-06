#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <map>
#include "spanner.hpp"

namespace spancopy {

bool spanner::span(const float size, const fs::path source, const fs::path target, const bool dry_run) const noexcept
{
    std::cout << "source:           " << source << "\n";
    std::cout << "parent_path:      " << source.parent_path() << '\n';

    std::cout << "target:           " << target << "\n";
    std::cout << "parent_path:      " << target.parent_path() << '\n';

    const auto target_dir = target / get_current_date();
    std::cout << "[INF] target_dir: " << target_dir << '\n';

    // std::map<fs::path, fs::path> path_map;
    for (const auto& entry : fs::recursive_directory_iterator(source))
    {
        if (entry.is_regular_file())
        {
            std::cout << entry.path() << "\t[" << entry.file_size() << "]\n";
        }
    }

    return true;
}

std::string spanner::get_current_date() const noexcept
{
    std::ostringstream oss;
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    oss << std::put_time(std::localtime(&now), "%Y%m%d");
    return oss.str();
}

bool spanner::is_free_space_available() const noexcept
{
    return true;
}

} // spancopy
