#include <CLI11.hpp>
#include "version.hpp"
#include "cli.hpp"

namespace spancopy::cli {

std::optional<config> load(int argc, char** argv) noexcept
{
    const auto exe = fs::path{argv[0]}.filename().string();
    const auto exe_with_version = exe + ' ' + VERSION;

    CLI::App app{exe_with_version + " - CLI tool to copy files from source to target per threshold"};
    app.footer(
        "Author: AZEEM SAJID <azeem.sajid@gmail.com>\n"
        "GitHub: https://github.com/iamazeem/spancopy\n"
    );

    app.allow_windows_style_options();

    app.set_help_flag("--help", "show help");
    app.set_version_flag("--version", exe_with_version, "show version");

    std::uintmax_t threshold{};
    app.add_option("--threshold", threshold, "threshold size")
        ->required()
        ->transform(CLI::AsSizeValue{false})
        ->check(CLI::PositiveNumber);

    fs::path source;
    app.add_option("--source", source, "source directory")
        ->required()
        ->check(CLI::ExistingDirectory);

    fs::path target;
    app.add_option("--target", target, "target directory")
        ->required()
        ->check(CLI::ExistingDirectory);

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError& e)
    {
        app.exit(e);
        return std::nullopt;
    }

    return config
    {
        threshold,
        source,
        target
    };
}

} // spancopy::cli
