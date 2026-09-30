//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>
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

    private:
        menu_s m_menu { nullptr };
    };
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
