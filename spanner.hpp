#pragma once

#include <filesystem>
namespace fs = std::filesystem;

namespace spancopy {

class spanner final
{
public:
    bool span(const float size, const fs::path source, const fs::path target, const bool dry_run) const noexcept;

private:
    std::string get_current_date() const noexcept;
    bool is_free_space_available() const noexcept;
};

} // spancopy
