//-----------------------------------------------------------------------------
#include "menu.hpp"
#include "game.hpp"

#include <algorithm>
//-----------------------------------------------------------------------------
using namespace codejam26;
//-----------------------------------------------------------------------------
static constexpr olc::vf2d MENU_MARGIN   {8.0, 8.0};
static constexpr auto      MENU_HMARGIN  {MENU_MARGIN.x};
static constexpr auto      MENU_YMARGIN  {MENU_MARGIN.y};
static constexpr auto      MENU_ITEM_GAP {3.0};
//-----------------------------------------------------------------------------

bool menu_t::onClicked(olc::vf2d const& pos)
{
    //-- If the click was done in the menu area...
    if(isHit(pos))
    {   //-- Calculate the relative pointer position. (Relative to the menu)
        auto relative_pos {pos - m_pos};
        //-- Get the item clicked (if any)
        auto clicked_itm  { clickedItem(relative_pos) };
        //-- Call callback (if item clicked)
        return clicked_itm && clicked_itm->onClicked ? clicked_itm->onClicked() : false;
    }
    return false;
}
//-----------------------------------------------------------------------------

inline bool menu_t::isHit(olc::vf2d const& pos)
{
    auto right  { m_pos.x + dimensions.x };
    auto bottom { m_pos.y + dimensions.y };
    return (m_pos.x <= pos.x) && (pos.x < right) && (m_pos.y <= pos.y) && (pos.y < bottom);
}
//-----------------------------------------------------------------------------

void menu_t::draw(olc::vf2d const& pos)
{
    auto& ge_draw { GAME.GetDraw() };

    //-- If position has been informed, update it
    if (&pos != &INVALID_VF2D)
        m_pos = pos;

    //-- Store the total size of the menu
    olc::vf2d menu_sz {0.0, 0.0};
    //-- Calculate total size of menu from its items
    for (auto& itm : m_items)
    {
        auto isz {ge_draw.GetTextSize(itm->text, true)};
        //-- WARN: Assume all items in this menu are arrenged vertically
        menu_sz.x  = std::max<float>(isz.x + MENU_HMARGIN, menu_sz.x) + MENU_HMARGIN;
        menu_sz.y += (isz.y + MENU_ITEM_GAP);
    }
    //-- Resize menu to accomodate all its entries
    Resize(menu_sz + MENU_MARGIN);

    //-- Start drawing the menu
    ge_draw.SetTarget(*this);
    ge_draw.Clear(olc::Colour::WHITE);

    //-- Draw a rectangle around the "menu"
    auto msz = ge_draw.GetTargetSize();
    ge_draw.RoundedRect({1.0f, 1.0f}, {msz.x - 1.0f, msz.y - 1.0f}, 2.0f, olc::Colour::VERY_DARK_GREY);

    //-- Draw elements
    olc::vf2d ipos {MENU_MARGIN};
    for (auto& itm : m_items)
    { //-- WARN: We are assuming menu has items in a vertical list!!!
        ge_draw.StringProp(ipos, itm->text, olc::Colour::VERY_DARK_GREY);
        ipos.y += ge_draw.GetTextSize(itm->text, true).y + MENU_ITEM_GAP;
    }
    //-- End drawing menu

    //-- Draw the actual menu on the window
    ge_draw.SetTarget(GAME.GetScreen());
    ge_draw.Image(*this, m_pos);
}
//-----------------------------------------------------------------------------

menu::item_s menu_t::clickedItem(olc::vf2d const& relpos)
{
    //-- This is called once it has been checked that the click has been
    //   done in this menu. The coordinates of the clicked within the menu
    //   is provided in relpos
    olc::vf2d msz { MENU_MARGIN };
    for(auto& itm : m_items)
    {
        auto isz { GAME.GetDraw().GetTextSize(itm->text, true) };
        //-- If adding the next element goes below the click on the mouse,
        //   the current item is the one being clicked!!
        if((msz + isz).y > relpos.y)
        {
            MN_LOGGER.debug("Clicked on element '{}'", itm->text);
            return itm;
        }
        msz.y += (isz.y + MENU_ITEM_GAP);
    }
    MN_LOGGER.debug("Could not find the menu element clicked upon");
    return nullptr;
}
//-----------------------------------------------------------------------------










menu_s menu::manager_t::spawnMenu()
{
    auto mn { m_menus.emplace_back(new menu_t) };
    MN_LOGGER.debug(" - menu address 0x{:x}", reinterpret_cast<std::uintptr_t>(mn.get()));
    GAME.CreateImage(*mn, {10, 10});
    MN_LOGGER.debug(" - menu image created");
    return mn;
}
//-----------------------------------------------------------------------------

menu_s menu::manager_t::menuAt(olc::vf2d const& pos)
{   //-- The vector order is the stack order from bottom to top, as the last
    //   element is the last to be drawn (shown on top), so we need to check
    //   in reverse order
    auto it = std::find_if(m_menus.rbegin(), m_menus.rend(),
        [&pos](auto const& _menu)
        {
            return _menu ? _menu->isHit(pos) : false;
        });
    return it != m_menus.rend() ? *it : nullptr;
}
//-----------------------------------------------------------------------------

void menu::manager_t::draw()
{
    for(auto& menu : m_menus)
        menu->draw();
}
//-----------------------------------------------------------------------------
