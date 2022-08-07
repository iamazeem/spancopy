#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace spancopy {

class spanner final
{
public:
    spanner(const double size, const fs::path source, const fs::path target, const bool dry_run) noexcept;
    bool span() const noexcept;

private:
    std::string get_current_date() const noexcept;
    std::uintmax_t gbs_to_bytes(const double gbs) const noexcept;
    double bytes_to_gbs(const std::uintmax_t bytes) const noexcept;
    bool is_space_available(const std::uintmax_t source_size_in_bytes) const noexcept;

    const double m_size{0.0};
    const fs::path m_source;
    const fs::path m_target;
    const bool m_dry_run{false};
};

} // spancopy
