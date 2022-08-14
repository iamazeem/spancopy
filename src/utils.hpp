#pragma once

#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

namespace spancopy::utils {

bool is_space_available(const fs::path& at, const std::uintmax_t bytes) noexcept;

std::uintmax_t gbs_to_bytes(const double gbs) noexcept;
double bytes_to_gbs(const std::uintmax_t bytes) noexcept;

} // spancopy::utils
