#pragma once

#include <map>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

namespace spancopy {

class spanner final
{
public:
    spanner(
        const std::uintmax_t threshold,
        const fs::path& source,
        const fs::path& destination) noexcept;

    bool span() const noexcept;

private:
    fs::path generate_destination_root_dir_path() const noexcept;
    bool is_destination_space_available(const std::uintmax_t source_dir_size) const noexcept;
    void remove_destination_dir_if_exists() const noexcept;
    fs::path generate_destination_file_path(
        const fs::path& source_file_path,
        const fs::path& destination_subdir_root_path) const noexcept;

    // sorted source file names with their sizes per directory
    // i.e. { key: filename, value: file size (bytes) }
    using source_file_t = std::map<fs::path, std::uintmax_t>;

    // sorted source file parent paths with files and their sizes
    // i.e. { key: parent path, value: { filename, file size } }
    using source_file_map_t = std::map<fs::path, source_file_t>;

    std::optional<source_file_map_t> generate_source_file_map() const noexcept;
    void copy_source_to_destination(const source_file_map_t& source_file_map) const noexcept;

    const std::uintmax_t m_threshold{};
    const fs::path m_source;
    const fs::path m_destination;

    const fs::path m_destination_root_dir_path;
};

} // spancopy
