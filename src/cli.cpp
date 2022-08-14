#include <CLI11.hpp>
#include "version.hpp"
#include "cli.hpp"

namespace spancopy::cli {

std::optional<configuration> load(int argc, char** argv) noexcept
{
    const auto exe = fs::path{argv[0]}.filename().string();
    const auto exe_with_version = exe + ' ' + spancopy::version;

    CLI::App app{exe_with_version + " - utility to span (copy) files into subdirectories"};
    app.allow_windows_style_options();

    app.set_help_flag("--help", "show help");
    app.set_version_flag("--version", exe_with_version, "show version");

    double threshold{0.0};
    app.add_option("--threshold", threshold, "threshold in GBs")
        ->required()
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

    return configuration
    {
        threshold,
        source,
        target
    };
}

} // spancopy
