//-----------------------------------------------------------------------------
#include "game.hpp"
//-----------------------------------------------------------------------------

int main(int /*argc*/, char** /*argv*/)
{
    constexpr std::string_view APP_NAME {"TBD"};
    constexpr auto             PXL_SZ {2};
    constexpr olc::vi2d        WINDOW_SZ { 640, 400 };

    //-- The game instance, our main application
    auto& game { CJGAME };
    //-- Main game configuration
    olc::PGEConfig cfg;

    cfg.bResizeable   = false;
    cfg.sAppName      = APP_NAME.data();
    cfg.vScreenSize   = WINDOW_SZ;
    cfg.vPixelSize    = { PXL_SZ, PXL_SZ };
    cfg.vWindowOffset = { 10, 10 };   //-- Does not work on Linux with X11
    cfg.bVSync        = false;

    //-- Construct and run the game
    if(game.Construct(cfg))
    {
        game.Start();
    }

    return 0;
}
//-----------------------------------------------------------------------------
