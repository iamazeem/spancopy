#include "configuration.hpp"

namespace spancopy {

configuration::configuration(
    const std::uintmax_t threshold,
    const fs::path source,
    const fs::path target) noexcept
    :
    m_threshold{threshold},
    m_source{source},
    m_target{target}
{
}

std::uintmax_t configuration::threshold() const noexcept
{
    return m_threshold;
}

fs::path configuration::source() const noexcept
{
    return m_source;
}

fs::path configuration::target() const noexcept
{
    return m_target;
}

} // spancopy
