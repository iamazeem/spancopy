#include "utils.hpp"

namespace spancopy::utils {

bool is_space_available(const fs::path& at, const std::uintmax_t bytes) noexcept
{
    return (fs::space(at).available > bytes);
}

std::uintmax_t gbs_to_bytes(const double gbs) noexcept
{
    return (gbs * 1024 * 1024 * 1024);
}

double bytes_to_gbs(const std::uintmax_t bytes) noexcept
{
    return (bytes / 1024. / 1024. / 1024.);
}

} // spancopy::utils
