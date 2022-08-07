#include <cstdlib>
#include <CLI11.hpp>
#include "spanner.hpp"

int main(int argc, char** argv)
{
    CLI::App app{"spancopy - utility to span (copy) files into subdirectories"};
    app.allow_windows_style_options();

    app.set_help_flag("--help", "show usage info");

    double size{0.0};
    app.add_option("--size", size, "threshold in GBs")
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
        return app.exit(e);
    }

    spancopy::spanner spanner{size, source, target};
    if (!spanner.span())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
