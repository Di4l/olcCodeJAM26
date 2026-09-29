//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>
#include <string_view>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class Game : public olc::PixelGameEngine
{
public:
    Game();

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
};
//-----------------------------------------------------------------------------
