#pragma once

#include <optional>
#include "configuration.hpp"

namespace spancopy::cli {

std::optional<configuration> load(int argc, char** argv) noexcept;

} // spancopy
