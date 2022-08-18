#pragma once

#include "configuration.hpp"

namespace spancopy {

class spanner final
{
public:
    spanner(const configuration& configuration) noexcept;
    bool span() const noexcept;

private:
    bool is_target_space_available(const std::uintmax_t source_dir_size) const noexcept;
    fs::path generate_target_root_dir_path() const noexcept;
    void remove_target_dir_if_exists(const fs::path& target_root_dir_path) const noexcept;
    fs::path generate_target_file_path(
        const fs::path& source_file_path,
        const fs::path& target_subdir_root_path) const noexcept;

    const configuration m_configuration;
};

} // spancopy
