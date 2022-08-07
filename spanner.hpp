#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace spancopy {

class spanner final
{
public:
    spanner(const double size_in_gbs, const fs::path source, const fs::path target) noexcept;
    bool span() const noexcept;

private:
    std::string get_target_root_dir_name() const noexcept;
    std::uintmax_t gbs_to_bytes(const double gbs) const noexcept;
    double bytes_to_gbs(const std::uintmax_t bytes) const noexcept;
    bool is_space_available(const std::uintmax_t source_dir_size_in_bytes) const noexcept;

    const double m_size_in_gbs{0.0};
    const fs::path m_source;
    const fs::path m_target;
};

} // spancopy
