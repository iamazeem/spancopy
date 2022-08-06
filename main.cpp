#include <cstdlib>
#include <CLI11.hpp>
#include "spanner.hpp"

#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    CLI::App app{"spancopy - utility to span (copy) files into subdirectories"};
    app.allow_windows_style_options();

    app.set_help_flag("--help", "show usage help");

    float size{0.0f};
    app.add_option("--size", size, "threshold in GBs to span (copy) files")
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

    bool dry_run{false};
    app.add_flag("--dry-run", dry_run, "print span listings only, do not copy")
        ->default_str("false");

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError& e)
    {
        return app.exit(e);
    }

    if (source == target)
    {
        std::cerr << "[ERR] source and target must not be the same!";
        return EXIT_FAILURE;
    }

    spancopy::spanner spanner;
    if (!spanner.span(size, source, target, dry_run))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
