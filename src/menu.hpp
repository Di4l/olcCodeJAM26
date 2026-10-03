//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <olcPixelGameEngine3.h>

#include <functional>
#include <vector>
#include <memory>
#include "logger.hpp"
//-----------------------------------------------------------------------------
#define MN_LOGGER       codejam26::Logger::get("menu")
//-----------------------------------------------------------------------------

namespace codejam26
{
    //-------------------------------------------------------------------------
    static constexpr olc::vf2d INVALID_VF2D {};
    //-------------------------------------------------------------------------

    namespace menu
    {
        //---------------------------------------------------------------------

        namespace item
        {
            using action = std::function<bool(void)>;
        }
        //---------------------------------------------------------------------

        struct item_t
        {
            std::string  text{};
            item::action onClicked{};
        };
        //---------------------------------------------------------------------
        using item_s = std::shared_ptr<item_t>;
        using items  = std::vector<item_s>;
        //---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------

    class menu_t : public olc::Image
    {
    public:
        inline olc::vf2d& position() { return m_pos; }
        inline menu::items& items() { return m_items; }

        inline void moveTo(olc::vf2d const& pos)
        {
            m_pos = pos;
        }

        bool onClicked(olc::PixelGameEngine* engine, olc::vf2d const& pos);

        void draw(olc::PixelGameEngine* engine, olc::vf2d const& pos = INVALID_VF2D);

    protected:
        menu::item_s clickedItem(olc::PixelGameEngine* engine, olc::vf2d const& relpos);

    private:
        olc::vf2d   m_pos{0, 0};
        menu::items m_items{};

        inline bool isInside(olc::vf2d const& pos);
    };

    //-------------------------------------------------------------------------
    using menu_s = std::shared_ptr<menu_t>;
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------