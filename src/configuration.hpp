#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace spancopy {

class configuration final
{
public:
    configuration(
        const std::uintmax_t threshold,
        const fs::path source,
        const fs::path target) noexcept;

    std::uintmax_t threshold() const noexcept;
    fs::path source() const noexcept;
    fs::path target() const noexcept;

private:
    const std::uintmax_t m_threshold{};
    const fs::path m_source;
    const fs::path m_target;
};

} // spancopy
