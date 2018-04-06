#include "game.h"

#include <iostream>

int main()
{
    try
    {
        Game game;
        game.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return 0;
}
