#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace spancopy {

class config final
{
public:
    config(
        const std::uintmax_t threshold,
        const fs::path source,
        const fs::path target) noexcept
        :
        m_threshold{threshold},
        m_source{source},
        m_target{target}
    {
    }

    std::uintmax_t threshold() const noexcept { return m_threshold; }
    fs::path source() const noexcept { return m_source; }
    fs::path target() const noexcept { return m_target; }

private:
    const std::uintmax_t m_threshold{};
    const fs::path m_source;
    const fs::path m_target;
};

} // spancopy
