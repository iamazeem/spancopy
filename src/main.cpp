#include <cstdlib>
#include <CLI11.hpp>
#include "version.hpp"
#include "spanner.hpp"

int main(int argc, char** argv)
{
    const auto exe = fs::path{argv[0]}.filename().string();
    const auto exe_with_version = exe + ' ' + VERSION;

    CLI::App app{exe_with_version + " - a CLI tool to copy files from source to destination per threshold"};
    app.footer(
        "Notes:\n"
        "- The `threshold` unit may be bytes, KB, MB, GB, etc.\n"
        "- The `threshold` must be less or equal to all source files' sizes.\n"
        "- A main subdirectory under `destination` is created to avoid conflicts.\n"
        "    Format:  YYYYMMDDTHHMMSSMS\n"
        "    Example: 20220820T170159946\n"
        "- The `destination` directory tree is removed if it exists already.\n"
        "- Only files are copied. Empty directories are ignored.\n"
        "- On all platforms, the `/` is used as the path separator.\n"
        // "\n"
        // "Examples:\n"
        // "  # With Linux style options\n"
        // "  spancopy --threshold 500mb --source <source> --destination <destination>\n"
        // "\n"
        // "  # With Windows style options\n"
        // "  spancopy /threshold 100kb /source <source> /destination <destination>\n"
        "\n"
        "For any feedback or issues, please open an issue on GitHub.\n"
        "GitHub: https://github.com/iamazeem/spancopy\n"
        "\n"
        "Written by: AZEEM SAJID <azeem.sajid@gmail.com>\n"
    );

    app.allow_windows_style_options();

    app.set_help_flag("-h,--help", "show help and exit");
    app.set_version_flag("-v,--version", exe_with_version, "show version and exit");

    std::uintmax_t threshold = 0;
    app.add_option("-t,--threshold", threshold, "threshold size to span files")
        ->required()
        ->transform(CLI::AsSizeValue{false})
        ->check(CLI::PositiveNumber);

    fs::path source;
    app.add_option("SOURCE", source, "source directory")
        ->required()
        ->check(CLI::ExistingDirectory);

    fs::path destination;
    app.add_option("DEST", destination, "destination directory")
        ->required()
        ->check(CLI::ExistingDirectory);

    try
    {
        app.parse(argc, argv);
        std::cout << "after parse" << std::endl;
    }
    catch (const CLI::Success& e)
    {
        // -h,--help or -v,--version
        app.exit(e);
        return EXIT_SUCCESS;
    }
    catch (const CLI::ParseError& e)
    {
        app.exit(e);
        return EXIT_FAILURE;
    }

    const spancopy::spanner spanner{threshold, source, destination};
    if (!spanner.span())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
