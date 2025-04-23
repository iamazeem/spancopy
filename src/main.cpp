#include <cstdlib>
#include <CLI11.hpp>
#include "version.hpp"
#include "spanner.hpp"

int main(int argc, char** argv)
{
    const auto exe = fs::path{argv[0]}.filename().string();
    const auto exe_with_version = exe + " v" + VERSION;

    CLI::App app{exe_with_version + " - a CLI tool to span (copy) files with size threshold"};
    app.footer(
        "Notes:\n"
        "- On all platforms, the `/` is used as the path separator.\n"
        "- Empty directories are ignored.\n"
        "- The sizes of all the `source` files must be less than or equal to the\n"
        "  `threshold` value. So, choose a `threshold` value accordingly.\n"
        "- The `destination` directory must have enough space for all the `source`\n"
        "  files to be copied successfully.\n"
        "- A main subdirectory under `destination` is created to avoid conflicts.\n"
        "  Following naming convention is used for this subdirectory:\n"
        "    Format:  YYYYMMDDTHHMMSSMS\n"
        "    Example: 20220820T170159946\n"
        "- The main subdirectory under `destination` is removed if it already exists.\n"
        "- For each `threshold` reached, the file(s) with their complete paths are\n"
        "  copied under their respective subdirectories i.e. 1, 2, 3, and so on.\n"
        "  For example, if for a `source` directory with some files, the `threshold`\n"
        "  is reached three times then there will three subdirectories e.g.:\n"
        "    Main subdir: 20220820T170159946\n"
        "    Threshold 1: 20220820T170159946/1/.../<file(s)>\n"
        "    Threshold 2: 20220820T170159946/2/.../<file(s)>\n"
        "    Threshold 3: 20220820T170159946/3/.../<file(s)>\n"
        "\n"
        "Examples:\n"
        "  # spancopy from `src` to `dst` with threshold 500 bytes\n"
        "  spancopy --threshold 500b --source ./src --destination ./dst\n"
        "\n"
        "  # spancopy from `src` to `dst` with threshold 100KB\n"
        "  spancopy --threshold 100kb --source ./src --destination ./dst\n"
        "\n"
        "For any feedback or to report any issues, please open an issue on\n"
        "GitHub: https://github.com/iamazeem/spancopy/issues\n"
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
    app.add_option("-s,--source", source, "source directory")
        ->required()
        ->check(CLI::ExistingDirectory);

    fs::path destination;
    app.add_option("-d,--destination", destination, "destination directory")
        ->required()
        ->check(CLI::ExistingDirectory);

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::CallForVersion& e)
    {
        return app.exit(e);
    }
    catch (const CLI::CallForHelp& e)
    {
        return app.exit(e);
    }
    catch (const CLI::ParseError& e)
    {
        if (argc == 1)
        {
            return app.exit(CLI::CallForHelp{});
        }

        return app.exit(e);
    }

    const spancopy::spanner spanner{threshold, source, destination};
    if (!spanner.span())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
