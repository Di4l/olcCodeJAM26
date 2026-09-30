//-----------------------------------------------------------------------------
#include "game.hpp"

#include <iostream>
//-----------------------------------------------------------------------------
using namespace codejam26;
//-----------------------------------------------------------------------------

Game::Game()
{
}
//-----------------------------------------------------------------------------

bool Game::OnUserCreate()
{
    m_menu = std::make_shared<menu_t>(
        [&](olc::vf2d const& pos)
        {
            //-- Calculate the relative pointer position. (Relative to the menu)
            auto relative_pos { pos - m_menu->position() };
            //-- Get the item clicked (if any)
            auto clicked_itm  { m_menu->clickedItem(this, relative_pos) };
            //-- Call callback (if item clicked)
            return clicked_itm && clicked_itm->onClicked ? clicked_itm->onClicked() : false;
        });

    m_menu->items().emplace_back(std::make_shared<menu::item_t>("File", [](){ std::cout << "Pressed 'File'\n"; return true; }));
    m_menu->items().emplace_back(std::make_shared<menu::item_t>("Open (Ctrl+O)", [](){ std::cout << "Pressed 'Open'\n"; return true; }));
    m_menu->items().emplace_back(std::make_shared<menu::item_t>("Exit", [](){ std::cout << "Pressed 'Exit'\n"; return true; }));

    CreateImage(*m_menu, {10, 10});

    return true;
}
//-----------------------------------------------------------------------------

bool Game::OnUserUpdate(float fElapsedTime)
{
    //-- Store where the mouse was first pressed
    static olc::vf2d mouse_pos_pressed {};
    //-- This is just for testing purposes. Final text should be somewhere else
    static constexpr std::string_view texto {"Aqui empezaria el lio"};

    //-- Clear the screen (grey => "dark white")
    draw.Clear(olc::Colour::GREY);

    //-- Draw text centered on window
    auto tsz { draw.GetTextSize(texto.data(), true) };  //-- Text size
    auto wsz { draw.GetTargetSize() };                  //-- Window size
    draw.StringProp((wsz - tsz) / 2, texto.data());

    //-- If we wanted to draw the manu at a different coordinates, we could
    //   pass a second parameter to draw() with the desired position
    m_menu->draw(this);

    //-- Get the mouse position
    auto lft_btn_status {mouse.GetButton(0)};
    if(lft_btn_status.bPressed)
    {
        mouse_pos_pressed = mouse.GetPosition();
    }
    else if(lft_btn_status.bReleased)
    { //-- Get the menu that lies at original button press
        m_menu->onClicked(mouse_pos_pressed);
    }

    return true;
}
//-----------------------------------------------------------------------------
