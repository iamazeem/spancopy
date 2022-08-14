#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace spancopy {

class configuration final
{
public:
    configuration(
        const double threshold,
        const fs::path source,
        const fs::path target) noexcept;

    double threshold() const noexcept;
    fs::path source() const noexcept;
    fs::path target() const noexcept;

private:
    const double m_threshold{0.0};
    const fs::path m_source;
    const fs::path m_target;
};

} // spancopy
