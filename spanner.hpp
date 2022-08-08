#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace spancopy {

class spanner final
{
public:
    spanner(const double size, const fs::path source, const fs::path target) noexcept;
    bool span() const noexcept;

private:
    fs::path get_target_root_dir_path() const noexcept;
    std::uintmax_t gbs_to_bytes(const double gbs) const noexcept;
    double bytes_to_gbs(const std::uintmax_t bytes) const noexcept;
    bool is_space_available(const std::uintmax_t source_dir_size_in_bytes) const noexcept;
    void remove_target_dir_if_exists(const fs::path& target_root_dir_path) const noexcept;
    fs::path get_target_file_path(const fs::path& source_file_path, const fs::path& target_subdir_root_path) const noexcept;

    const double m_size{0.0};
    const fs::path m_source;
    const fs::path m_target;
};

} // spancopy
