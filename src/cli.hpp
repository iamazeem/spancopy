#pragma once

#include <optional>
#include "config.hpp"

namespace spancopy::cli {

std::optional<config> load(int argc, char** argv) noexcept;

} // spancopy
