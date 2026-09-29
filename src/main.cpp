//-----------------------------------------------------------------------------
#include "game.hpp"
//-----------------------------------------------------------------------------

int main(int /*argc*/, char** /*argv*/)
{
    //-- The game instance, our main application
    Game game;
    //-- Main game configuration
    olc::PGEConfig cfg;

    cfg.bResizeable   = false;
    cfg.sAppName      = Game::appName();
    cfg.vScreenSize   = { 320, 200 };
    cfg.vPixelSize    = {   4,   4 };
    cfg.vWindowOffset = {  10,  10 };   //-- Does not work on Linux with X11

    //-- Construct and run the game
    if(game.Construct(cfg))
    {
        game.Start();
    }

    return 0;
}
//-----------------------------------------------------------------------------
