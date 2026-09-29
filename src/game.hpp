//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>
#include <string_view>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class Game : public olc::PixelGameEngine
{
public:
    Game();

    static constexpr std::string_view appName() { return m_appName; }

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
    static constexpr std::string_view m_appName {"TBD"};
};
//-----------------------------------------------------------------------------
