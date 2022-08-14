#include <cstdlib>
#include "cli.hpp"
#include "spanner.hpp"

int main(int argc, char** argv)
{
    const auto configuration = spancopy::cli::load(argc, argv);
    if (!configuration)
    {
        return EXIT_FAILURE;
    }

    spancopy::spanner spanner{*configuration};
    if (!spanner.span())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
