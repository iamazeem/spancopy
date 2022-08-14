#include <cstdlib>
#include <CLI11.hpp>
#include "version.hpp"
#include "spanner.hpp"

int main(int argc, char** argv)
{
    const auto exe = fs::path{argv[0]}.filename().string();
    const auto exe_with_version = exe + ' ' + spancopy::version;

    CLI::App app{exe_with_version + " - utility to span (copy) files into subdirectories"};
    app.allow_windows_style_options();

    app.set_help_flag("--help", "show usage info");
    app.set_version_flag("--version", exe_with_version, "show version");

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
