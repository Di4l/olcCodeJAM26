//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>
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
