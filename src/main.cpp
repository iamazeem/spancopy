#include <cstdlib>
#include "cli.hpp"
#include "spanner.hpp"

int main(int argc, char** argv)
{
    const auto config = spancopy::cli::load(argc, argv);
    if (!config)
    {
        return EXIT_FAILURE;
    }

    const auto spanner = spancopy::spanner{*config};
    if (!spanner.span())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
