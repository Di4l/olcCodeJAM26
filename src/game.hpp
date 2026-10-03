//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>

//-- Both this files need to be included in the header, since the extension is
//   not a full interface and uses types defined in miniaudio.h
#include <miniaudio.h>
#include <olcPGEX3_Miniaudio.h>

#include "logger.hpp"
#include "main_menu_options.hpp"
#include "menu.hpp"

#include <map>
#include <vector>
#include <memory>
//-----------------------------------------------------------------------------
#define GAME    codejam26::Game::instance()
//-----------------------------------------------------------------------------

namespace codejam26
{
    //-------------------------------------------------------------------------

    class Game : public olc::PixelGameEngine
    {
    public:
        static Game& instance();

        bool OnUserCreate() override;
        bool OnUserUpdate(float /*fElapsedTime*/) override;

        void createRandomMenu();
        void playClickSound();

    private:
        void initializeAudioEngine();
        void configureMap();

        void eraseUsedIndices(int index);
        bool incorrectButtonClicked();
        bool correctButtonClicked();
        int  selectRandomIndex();

        olc::ext::Miniaudio::AudioEngine m_audio       {};
        olc::ext::Miniaudio::Sound       m_sound_click {};

        menu_s          main_menu  {nullptr};                    //-- Starting menu
        menu_s          m_menu     {nullptr};                    //-- To be removed
        menu_s          m_hints    {nullptr};                    //-- The hints will be drawn here
        menu::manager_t m_menu_mgr {};                           //-- Menu manager

        std::map<std::string, std::string>  m_map {};            //-- Map to store the items and their corresponding hints
        std::vector<std::string>            m_buttons_text {};   //-- Vector to store the items that will be shown in the menu as
                                                                 //   the text of the buttons to click in the game.
        std::vector<int>                    m_unused_indices {}; //-- Vector to store the indices of the items that have been used
        size_t                              m_counter {0};       //-- Counter to keep track of how many times the user has clicked
                                                                 //   on the menu items. This is used to determine when to show the
                                                                 //   "Quit Game" option in the menu and to know how many times the
                                                                 //   user has clicked on the menu items before the game ends.
        std::pair<std::string, std::string> m_current_pair {};   //-- Pair to store the current item and its corresponding hint that
                                                                 //   the user has to click on in the game. This is used to know
                                                                 //   which item the user has to click on in the game and to show
                                                                 //   the hint for that item.
        MainMenuOptions m_current_state {MainMenuOptions::MAIN_MENU};
    };
    //-------------------------------------------------------------------------
} // namespace codejam26
//-----------------------------------------------------------------------------
