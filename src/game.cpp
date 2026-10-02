//-----------------------------------------------------------------------------
#include "game.hpp"

#include <iostream>
//-- Include asset file headers
// #include <retro_blop_18.hpp>
#include <keyboard001.hpp>
//-----------------------------------------------------------------------------
using namespace codejam26;
//-----------------------------------------------------------------------------

bool Game::OnUserCreate()
{
    //-- Initialize the audio engine: Install the extension and load sounds
    LOGGER.info("Initialize Audio Engine");
    initializeAudioEngine();

    LOGGER.info("Create a sample menu");
    m_menu = std::make_shared<menu_t>();

    m_menu->items().emplace_back(std::make_shared<menu::item_t>("File", [&](){ playClickSound(); std::cout << "Pressed 'File'\n"; return true; }));
    m_menu->items().emplace_back(std::make_shared<menu::item_t>("Open (Ctrl+O)", [&](){ playClickSound(); std::cout << "Pressed 'Open'\n"; return true; }));
    m_menu->items().emplace_back(std::make_shared<menu::item_t>("Exit", [&](){ playClickSound(); std::cout << "Pressed 'Exit'\n"; return true; }));

    CreateImage(*m_menu, {10, 10});

    LOGGER.info("Game instance created and initialized");
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
    m_menu->draw(this, {20, 10});

    //-- Get the mouse position
    auto lft_btn_status {mouse.GetButton(0)};
    if(lft_btn_status.bPressed)
    {
        mouse_pos_pressed = mouse.GetPosition();
    }
    else if(lft_btn_status.bReleased)
    { //-- Get the menu that lies at original button press
        m_menu->onClicked(this, mouse_pos_pressed);
    }

    return true;
}
//-----------------------------------------------------------------------------

void Game::playClickSound()
{
    if(m_sound_click.IsLoaded())
        m_sound_click.Play();
    else
        LOGGER.error("Click sound not loaded. Cannot play it!");
}
//-----------------------------------------------------------------------------

void Game::initializeAudioEngine()
{
    //-- Load the miniaudio extension
    if(!InstallSystemExtension(&m_audio))
    {
        LOGGER.error("Failed to install olcPGEX3_miniaudio");
        throw std::runtime_error("Failed to install olcPGEX3_miniaudio");
    }

    if(!m_audio.CreateSoundFromMemory(m_sound_click, assets::keyboard001.data(), assets::keyboard001.size()))
        LOGGER.error("Could not load audio 'keyboard001' from memory");
}
//-----------------------------------------------------------------------------
