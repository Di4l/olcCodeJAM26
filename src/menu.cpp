//-----------------------------------------------------------------------------
#include "menu.hpp"
//-----------------------------------------------------------------------------
using namespace codejam26;
//-----------------------------------------------------------------------------
static constexpr olc::vf2d MENU_MARGIN {8.0, 8.0};
static constexpr auto MENU_HMARGIN {MENU_MARGIN.x};
static constexpr auto MENU_YMARGIN {MENU_MARGIN.y};
static constexpr auto MENU_ITEM_GAP {3.0};
//-----------------------------------------------------------------------------

bool menu_t::onClicked(olc::PixelGameEngine* engine, olc::vf2d const& pos)
{
    if (! engine)
    {
        return false;
    }
    //-- If the click was done in the menu area...
    if (isInside(pos))
    { //-- Calculate the relative pointer position. (Relative to the menu)
        auto relative_pos {pos - m_pos};
        //-- Get the item clicked (if any)
        auto clicked_itm {clickedItem(engine, relative_pos)};
        //-- Call callback (if item clicked)
        return clicked_itm && clicked_itm->onClicked ? clicked_itm->onClicked() : false;
    }
    return false;
}

//-----------------------------------------------------------------------------

menu::item_s menu_t::clickedItem(olc::PixelGameEngine* engine, olc::vf2d const& relpos)
{
    if (! engine)
    {
        return nullptr;
    }
    //-- This is called once it has been checked that the click has been
    //   done in this menu. The coordinates of the clicked within the menu
    //   is provided in relpos
    olc::vf2d msz {MENU_MARGIN};
    for (auto& itm : m_items)
    {
        auto isz {engine->GetDraw().GetTextSize(itm->text, true)};
        //-- If adding the next element goes below the click on the mouse,
        //   the current item is the one being clicked!!
        if ((msz + isz).y > relpos.y)
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

void menu_t::draw(olc::PixelGameEngine* engine, olc::vf2d const& pos)
{
    if (! engine)
    {
        return;
    }

    auto& ge_draw {engine->GetDraw()};

    //-- If position has been informed, update it
    if (&pos != &INVALID_VF2D)
    {
        m_pos = pos;
    }

    //-- Store the total size of the menu
    olc::vf2d menu_sz {0.0, 0.0};
    //-- Calculate total size of menu from its items
    for (auto& itm : m_items)
    {
        auto isz {ge_draw.GetTextSize(itm->text, true)};
        //-- WARN: Assume all items in this menu are arrenged vertically
        menu_sz.x = std::max<float>(isz.x + MENU_HMARGIN, menu_sz.x) + MENU_HMARGIN;
        menu_sz.y += (isz.y + MENU_ITEM_GAP);
    }
    //-- Resize menu to accomodate all its entries
    Resize(menu_sz + MENU_MARGIN + MENU_MARGIN);

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
    ge_draw.SetTarget(engine->GetScreen());
    ge_draw.Image(*this, m_pos);
}

//-----------------------------------------------------------------------------
void menu_t::configureMouse(olc::PixelGameEngine* engine, olc::hw::Mouse& mouse)
{

    auto lft_btn_status {mouse.GetButton(0)};
    if (lft_btn_status.bPressed)
    {
        m_mouse_pos_pressed = mouse.GetPosition();
    }
    if (lft_btn_status.bReleased)
    { //-- Get the menu that lies at original button press
        this->onClicked(engine, m_mouse_pos_pressed);
    }
}

inline bool menu_t::isInside(olc::vf2d const& pos)
{
    auto right {m_pos.x + dimensions.x};
    auto bottom {m_pos.y + dimensions.y};
    return (m_pos.x <= pos.x) && (pos.x < right) && (m_pos.y <= pos.y) && (pos.y < bottom);
}

//-----------------------------------------------------------------------------
