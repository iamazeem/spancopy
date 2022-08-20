#include <cstdlib>
#include "cli.hpp"
#include "spanner.hpp"

int main(int argc, char** argv)
{
    if (const auto config = spancopy::cli::load(argc, argv); !config)
    {
        return EXIT_FAILURE;
    }
    else if (const auto spanner = spancopy::spanner{*config}; !spanner.span())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
