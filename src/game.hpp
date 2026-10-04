//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>

//-- Both this files need to be included in the header, since the extension is
//   not a full interface and uses types defined in miniaudio.h
#include <miniaudio.h>
#include <olcPGEX3_Miniaudio.h>

#include "logger.hpp"
//#include "main_menu_options.hpp"
#include "menu.hpp"

#include <map>
#include <vector>
#include <memory>
#include <random>
#include <exception>
//-----------------------------------------------------------------------------
#define CJGAME    codejam26::Game::instance()
//-----------------------------------------------------------------------------

namespace codejam26
{
    //-------------------------------------------------------------------------

    enum class GameState : uint8_t
    {
        GAME,
        EXPLANATION,
        MAIN_MENU,
        EXIT
    };
    //-------------------------------------------------------------------------

    class Game : public olc::PixelGameEngine
    {
    public:
        static Game& instance();

        bool OnUserCreate() override;
        bool OnUserUpdate(float /*fElapsedTime*/) override;

        void playClickSound()     { playSound(m_sound_click); }
        void playCorrectSound()   { playSound(m_sound_right); }
        void playIncorrectSound() { playSound(m_sound_wrong); }

        template <typename T, T min, T max>
        T random()
        {
            if constexpr (std::is_integral_v<T>)
                return std::uniform_int_distribution<T>{min, max}(m_rnd_engine);
            else if constexpr (std::is_floating_point_v<T>)
                return std::uniform_real_distribution<T>{min, max}(m_rnd_engine);
            else
                static_assert(std::is_arithmetic_v<T>,
                    "random() requires an integral or floating-point type");
        }

        template <typename T>
        T random(T min, T max)
        {
            if constexpr (std::is_integral_v<T>)
                return std::uniform_int_distribution<T>{min, max}(m_rnd_engine);
            else if constexpr (std::is_floating_point_v<T>)
                return std::uniform_real_distribution<T>{min, max}(m_rnd_engine);
            else
                static_assert(std::is_arithmetic_v<T>,
                    "random() requires an integral or floating-point type");
        }

        bool random() { return std::uniform_int_distribution<int>{0, 1}(m_rnd_engine); }

    private:
        Game() = default;

        void createRandomMenu();
        void createMainMenu();

        void initializeAudioEngine();
        void playSound(olc::ext::Miniaudio::Sound& sound);

        void configureMap();
        void removeAllGameMenus();

        bool incorrectButtonClicked();
        bool correctButtonClicked();

        olc::ext::Miniaudio::AudioEngine m_audio       {};
        olc::ext::Miniaudio::Sound       m_sound_click {};
        olc::ext::Miniaudio::Sound       m_sound_right {};
        olc::ext::Miniaudio::Sound       m_sound_wrong {};

        static std::mt19937 m_rnd_engine;

        menu_s          m_main_menu {nullptr};        //-- Starting menu
        menu_s          m_hints     {nullptr};        //-- The hints will be drawn here
        menu::manager_t m_menu_mgr  {};               //-- Menu manager

        std::vector<std::string> m_available_items{};
        size_t                   m_counter {0};       //-- Counter to keep track of how many times the user has clicked
                                                      //   on the menu items. This is used to determine when to show the
                                                      //   "Quit Game" option in the menu and to know how many times the
                                                      //   user has clicked on the menu items before the game ends.
        GameState m_game_state {GameState::MAIN_MENU};
    };
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
