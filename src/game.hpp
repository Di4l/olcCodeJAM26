//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>

//-- Both this files need to be included in the header, since the extension is
//   not a full interface and uses types defined in miniaudio.h
#include <miniaudio.h>
#include <olcPGEX3_Miniaudio.h>

#include "menu.hpp"
//-----------------------------------------------------------------------------

namespace codejam26
{
    //-------------------------------------------------------------------------

    class Game : public olc::PixelGameEngine
    {
    public:
        Game();

        bool OnUserCreate() override;
        bool OnUserUpdate(float fElapsedTime) override;

        void playClickSound();

    private:
        olc::ext::Miniaudio::AudioEngine m_audio;
        olc::ext::Miniaudio::Sound       m_sound_click;

        menu_s m_menu { nullptr };

        void initializeAudioEngine();
    };
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
