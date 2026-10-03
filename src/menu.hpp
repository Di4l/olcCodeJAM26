//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include "logger.hpp"
#include <functional>
#include <memory>
#include <olcPixelGameEngine3.h>
#include <vector>
//-----------------------------------------------------------------------------
#define MN_LOGGER codejam26::Logger::get("menu")
//-----------------------------------------------------------------------------

namespace codejam26
{
    //-------------------------------------------------------------------------
    static constexpr olc::vf2d INVALID_VF2D {};
    //-------------------------------------------------------------------------

    namespace menu
    {
        //---------------------------------------------------------------------
        class manager_t;
        //---------------------------------------------------------------------

        namespace item
        {
        using action = std::function<bool(void)>;
        } // namespace item

        //---------------------------------------------------------------------

        struct item_t
        {
            std::string text {};
            item::action onClicked {};
        };

        //---------------------------------------------------------------------
        using item_s = std::shared_ptr<item_t>;
        using items = std::vector<item_s>;
        //---------------------------------------------------------------------
    } // namespace menu
    //-------------------------------------------------------------------------

    class menu_t : public olc::Image
    {
    public:
        [[nodiscard]] inline olc::vf2d&   position() { return m_pos;   }
        [[nodiscard]] inline menu::items& items()    { return m_items; }

        inline void moveTo(olc::vf2d const& pos) { m_pos = pos; }

        [[maybe_unused]] bool onClicked(olc::vf2d const& pos);
        [[nodiscard]]    bool isHit(olc::vf2d const& pos);
        
        void configureMouse(olc::PixelGameEngine* engine, olc::hw::Mouse& mouse);

        void draw(olc::vf2d const& pos = INVALID_VF2D);

    protected:
        [[nodiscard]] menu::item_s clickedItem(olc::vf2d const& relpos);

    private:
        olc::vf2d   m_pos   {0,0};
        menu::items m_items {};
    };
    //-------------------------------------------------------------------------
    using menu_s  = std::shared_ptr<menu_t>;
    using menus_t = std::vector<menu_s>;
    //-------------------------------------------------------------------------

    namespace menu
    {
        //---------------------------------------------------------------------

        class manager_t
        {
        public:
            [[nodiscard]] menus_t& menus() { return m_menus;        }
            [[nodiscard]] size_t   size()  { return m_menus.size(); }

            [[nodiscard]] menu_s spawnMenuAt(olc::vf2d const& pos);
            [[nodiscard]] menu_s menuAt(olc::vf2d const& pos);

            void draw();

        protected:
        private:
            menus_t m_menus{};
        };
        //---------------------------------------------------------------------
    }
    //-------------------------------------------------------------------------
} // namespace codejam26
//-----------------------------------------------------------------------------

