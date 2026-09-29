//-----------------------------------------------------------------------------
#include "game.hpp"
//-----------------------------------------------------------------------------

Game::Game()
{
}
//-----------------------------------------------------------------------------

bool Game::OnUserCreate()
{
    return true;
}
//-----------------------------------------------------------------------------

bool Game::OnUserUpdate(float /*fElapsedTime*/)
{
    //-- This is just for testing purposes. Final text should be somewhere else
    static constexpr std::string_view texto {"Aqui empezaria el lio"};

    //-- Clear the screen (grey => "dark white")
    draw.Clear(olc::Colour::GREY);

    //-- Draw text centered on window
    auto tsz { draw.GetTextSize(texto.data(), true) };  //-- Text size
    auto wsz { draw.GetTargetSize() };                  //-- Window size
    draw.StringProp((wsz - tsz) / 2, texto.data());

    return true;
}
//-----------------------------------------------------------------------------
