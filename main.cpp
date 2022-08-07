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

    double size{0.0};
    app.add_option("--size", size, "threshold to span (copy) files [GB]")
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

    spancopy::spanner spanner{size, source, target, dry_run};
    if (!spanner.span())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
