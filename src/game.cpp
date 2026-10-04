//-----------------------------------------------------------------------------
#include "game.hpp"

#include <map>
#include <string_view>
#include <random>
#include <functional>

//-- Include asset file headers
// #include <retro_blop_18.hpp>
#include <click_button.hpp>
#include <action_wrong.hpp>
#include <action_correct.hpp>
//-----------------------------------------------------------------------------
using namespace codejam26;
//-----------------------------------------------------------------------------
static constexpr olc::Pixel CLR_BACKGROUND(80,180,180);
static constexpr std::string_view SV_HINT {"Hint: "};
static constexpr std::string_view SV_INTRO
{
    "  Where to start... how did I get into this mess? I was just...\n" \
    "trying to unsubscribe from that p-hub service that is driving me\n" \
    "nuts. 'Log in, then account, click in here, then over there...'\n" \
    "hold on... who is that on what video?? Oh f*** it!! Forget about\n" \
    "it!! Let's do what we came here to do... ... ... naaah, just this\n" \
    "one little click... it'll be 5 minutes.\n\n\n\n" \
    "  Why did I bloody clicked? How can I be soooo dumb? You moron...\n" \
    "Look at the mess you've gotten into. Now what...\n\n\n" \
    "       How do I get out of this application?\n"
};
//-----------------------------------------------------------------------------

static const std::map<std::string_view, std::string_view> MITM_MAP
{
    {"File", "Every journey begins with a couple of words."},
    {"Open", "Some things are meant to be opened, others not..."},
    {"Close Menu", "Sometimes the way forward is to leave."},
    {"New Menu", "A fresh start can change everything. Hopefully."},
    {"Delete", "What is gone cannot bother you anymore, not always but sometimes."},
    {"Copy", "Why make another when you can duplicate it? Don't forget to correct it."},
    {"Paste", "I think it is in my clipboard, but I am not sure. It is worth a try."},
    {"Cut", "The best way to get something in small pieces."},
    {"Help", "Should I have to read the manual? I think it is worth a try."},
    {"Sound", "It would be awesome to have this control on my baby's crying."},
    {"Settings", "Everything has a way to be adjusted. Probably."},
    {"Save", "Some things are worth keeping. Others are better forgotten."},
    {"Save As", "Perhaps it deserves a different name. Perhaps you do."},
    {"Share", "Good things are rarely kept alone. Bad things neither."},
    {"Preferences", "Everyone has their own way of doing things. Mine is probably wrong."},
    {"Close Window", "There is always another way out. Unless there isn't."},
    {"Undo", "Mistakes don't always have to be permanent. Convenient, isn't it?"},
    {"Search", "The answer may already be somewhere. The question is where."},
    {"Select All", "Why choose only one? That sounds like unnecessary work."},
    {"New Game", "Every adventure needs a first step. This might be one."},
    {"Continue Game", "The story isn't over yet. Apparently."},
    {"Options", "There is more than one way forward. Most of them are probably wrong."},
    {"Graphics", "Sometimes seeing is believing. Sometimes it is just bad graphics."},
    {"Credits", "Someone had to make all this. Sorry."},
    {"Cancel", "Not every decision needs to be final. Especially this one."},
    {"Quit Game(On development)", "Every game eventually comes to an end. This one can end sooner."},
    {"Video", "Some things are easier to understand when you see them. Some aren't."},
    {"Language", "Words mean different things to different people. Especially mine."},
    {"Start", "Everything has to begin somewhere. This seems as good a place as any."},
    {"Stop", "Knowing when to stop is important. I should probably follow my own advice."},
    {"<Invalid>", "Even mistakes can point you somewhere. Usually somewhere useless."},
    {"------", "Sometimes the least obvious path is worth following. Sometimes it is just a line."},
    {"Do not click!", "They wouldn't warn you without a reason."},
    {"Click Me!", "At least this one is honest."},
    {"*******", "Some secrets are better left unexplained. Like this one."},
    {"Reset password", "Sometimes forgetting is the first step. Remembering the password would be better."},
    {"Create Account", "Every identity has to start somewhere. This is probably legal."},
    {"Log  in", "Someone is expecting you. Hopefully it is you."},
    {"Log out", "Even visitors have to leave eventually. Probably."},
    {"Security", "Not everything should be trusted. Especially menus."},
    {"This is not a valid option", "Perhaps the wrong choice is hiding the right one."},
    {"This is neither a valid option", "Two wrong answers still leave one possibility. I think."},
    {"Stop? ", "You may want to reconsider before going further. Or don't."},
    {"Reload", "Sometimes a second look changes everything. Sometimes it just reloads."},
    {"Quit Game", "Every game eventually comes to an end. Congratulations on making it this far! You have completed the game and reached the end. Thank you for playing!)"}
};
//-----------------------------------------------------------------------------










std::mt19937 Game::m_rnd_engine { std::random_device{}() };
//-----------------------------------------------------------------------------

Game& Game::instance()
{
    static std::unique_ptr<Game> m_singleton { new Game };
    return *m_singleton;
}
//-----------------------------------------------------------------------------

bool Game::OnUserCreate()
{
    //-- Initialize the audio engine: Install the extension and load sounds
    LOGGER.info("Initialize audio engine");
    initializeAudioEngine();
    LOGGER.info("Initialize available menu items");
    configureMap();
    #ifdef SPDLOG_ACTIVE_LEVEL
    #  if (SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG)
        LOGGER.debug("Available menu items:");
        for (auto const& itm : MITM_MAP)
            LOGGER.debug("'{}: {}'", itm.first, itm.second);
    #  endif
    #endif

    LOGGER.info("Create hints rectangle");
    m_hints = std::make_shared<menu_t>();
    // m_hints->items().push_back(std::make_shared<menu::item_t>(SV_HINT.data(), nullptr));
    CreateImage(*m_hints, {10, 10});

    LOGGER.info("Create main menu");
    createMainMenu();

    // LOGGER.info("Create starting menu");
    // createRandomMenu();

    LOGGER.info("Game instance created and initialized");
    return true;
}
//-----------------------------------------------------------------------------

bool Game::OnUserUpdate(float /*fElapsedTime*/)
{
    bool running = true;
    //-- Store where the mouse was first pressed
    static olc::vf2d mouse_pos_pressed {};
    static olc::vf2d menu_offset_pos {};
    static menu_s    menu_pressed { nullptr };
    static bool      menu_moving { false };

    //-- Clear the screen and fill it in the background color
    draw.Clear(CLR_BACKGROUND);

    //-- Draw text centered on window
    auto tsz { draw.GetTextSize(SV_INTRO.data(), true) };  //-- Text size
    auto wsz { draw.GetTargetSize() };                  //-- Window size
    draw.StringProp((wsz - tsz) / 2, SV_INTRO.data());

    //-- draw the Hints first, so if anything is overlapped, the Hints are at the bottom
    if(m_hints->items().size())
    {
        auto hsz { m_hints->Size() };
        m_hints->draw({ (wsz.x - hsz.x) / 2.0f, (wsz.y - hsz.y) - 10.0f });
    }

    //-- Draw all the other menus
    m_menu_mgr.draw();
    if(m_main_menu->moveable() && m_main_menu->visible())
    {   //-- Draw the main menu in the middle of the Window and then make it fixed (unmoveable)
        auto mmsz  { m_main_menu->Size() };
        auto mmpos { (wsz - mmsz) / 2.0f };
        mmpos.y -= (tsz.y + mmsz.y) / 2.0f + 5.0f;
        m_main_menu->moveTo(mmpos);
        m_main_menu->moveable() = false;    //-- Lock main menu in place
    }

    //-- Get the left mouse button status and handle it
    auto lft_btn_status {mouse.GetButton(0)};
    if(lft_btn_status.bPressed)
    {   //-- Get the mouse position
        mouse_pos_pressed = mouse.GetPosition();
        menu_pressed      = m_menu_mgr.menuAt(mouse_pos_pressed);
        menu_offset_pos   = menu_pressed ? (mouse_pos_pressed - menu_pressed->position()) : olc::vf2d(0.0, 0.0);
    }
    else if (lft_btn_status.bHeld)
    {
        if(menu_pressed)
        {   //-- Has it actually moved??
            auto mdisplaced = mouse.GetPosition() - mouse_pos_pressed;
            if(mdisplaced.abs() > olc::vf2d(2.0f, 2.0f) )
            {
                if(!menu_moving)
                    LOGGER.debug("Moving menu...");
                menu_moving = true;
                menu_pressed->moveTo(mouse.GetPosition() - menu_offset_pos);
            }
        }
    }
    else if(lft_btn_status.bReleased)
    { //-- Get the menu that lies at original button press
        if(menu_moving)
        {
            m_menu_mgr.bringToFront(menu_pressed);

            menu_moving     = false;
            menu_pressed    = nullptr;
            menu_offset_pos = {0.0f,0.0f};
            LOGGER.debug("...stop moving menu");
        }
        else
        {
            m_menu_mgr.onClick(mouse_pos_pressed);
        }
    }

    // if (m_game_state == GameState::MAIN_MENU)
    // {
    //     main_menu->draw(halfScreen);
    //     main_menu->configureMouse(this, mouse);
    // }
    // else if (m_game_state == GameState::EXPLANATION)
    // {
    //     std::string explicationText = "Valid text";
    //     main_menu->draw(halfScreen);
    //     main_menu->configureMouse(this, mouse);

    //     auto explicationPosition = halfScreen;
    //     explicationPosition.x = explicationPosition.x + main_menu->Size().x + 40;
    //     draw.StringProp(explicationPosition, explicationText);
    // }
    // else if (m_game_state == GameState::GAME)
    // {
    //     //-- If we wanted to draw the manu at a different coordinates, we could
    //     //   pass a second parameter to draw() with the desired position
    //     m_menu->draw({20, 10});
    //     m_menu->configureMouse(this, mouse);
    // }
    // else if (m_game_state == GameState::EXIT)
    // {
    //     running = false;
    // }

    return running;
}
//-----------------------------------------------------------------------------

void Game::configureMap()
{
    LOGGER.debug("Populating available items with ALL possible");
    for(auto const& [txt, _] : MITM_MAP)
        m_available_items.push_back(txt.data());
}
//-----------------------------------------------------------------------------

void Game::createRandomMenu()
{
    //-- If no more items available, cannot create a menu
    if(!m_available_items.size())
        return;

    //-- Create a new menu
    auto menu { m_menu_mgr.spawnMenu() };
    //-- Choose a random number of menu elements between 1 and 7 (or available items left)
    auto randomNumberOfItems = random<size_t>(1, std::min<size_t>(7, m_available_items.size()));

    //-- Choose a random element out of the available vectors...
    //   This shall be the correct menu item to click
    auto       selectedIndex = random<int>(0, m_available_items.size());
    auto const buttonText    = m_available_items[selectedIndex];
    auto const hint          = MITM_MAP.at(buttonText);

    //-- Remove the selected element from the list ov available items.. do not want to pick
    //   it up more than once
    m_available_items.erase(m_available_items.begin() + selectedIndex);

    //-- Add the correct item to the menu with its corresponding callback
    menu->items().emplace_back(new menu::item_t(buttonText.data(), std::bind_front(&Game::correctButtonClicked, this)));
    //-- Add the hint to the hints rectangle
    m_hints->items().emplace_back(new menu::item_t(std::string(SV_HINT.data()) + hint.data(), nullptr));

    LOGGER.debug("Correct button: '{}'", buttonText.data());
    LOGGER.debug(" - Hint: '{}'",        hint.data()); // Show the hint for the item that the user has to click on in the game.

    //-- Populate the rest of the menu with incorrect items
    while(menu->items().size() < randomNumberOfItems)
    {            
        int randomIndex{0};
        do
        {   //-- Make sure we do not pick "Quit Game" which is always the correct menu item
            randomIndex = random<size_t>(0, m_available_items.size());
        }
        while(m_available_items[randomIndex] == "Quit Game");

        //-- Get the name of the item to add
        auto const map_key { m_available_items[randomIndex] };
        //-- Remove from available list
        m_available_items.erase(m_available_items.begin() + randomIndex);

        //-- Generate a random boolean to decide if we will introduce the item at the beginning or at the
        //   end of the menu. This is to avoid having the same item always at the same position in the menu.
        bool randomBool = random();
        // Add the item to the menu with its corresponding callback
        if (randomBool)
        {
            menu->items().emplace_back(new menu::item_t(
                    map_key.data(),
                    std::bind_front(&Game::incorrectButtonClicked, this)));
        }
        else
        {
            menu->items().insert(
                menu->items().begin(),
                std::make_shared<menu::item_t>(
                    map_key.data(),
                    std::bind_front(&Game::incorrectButtonClicked, this)));
        }
    }
}
//-----------------------------------------------------------------------------

void Game::removeAllGameMenus()
{
    //-- Need to erase all menus (but the main one)
    LOGGER.debug("Erase all game menus...");
    auto& menus { m_menu_mgr.menus() };
    while(menus.size() > 1)
    {
        auto it { menus.begin() };
        while(*it == m_main_menu) ++it;
        menus.erase(it);
    }
    //-- And remove all hints
    m_hints->items().clear();
    //-- Then make all menu items available again
    configureMap();
}
//-----------------------------------------------------------------------------

bool Game::incorrectButtonClicked()
{
    LOGGER.debug("Incorrect!!");
    m_counter = 0; // Reset the counter to 0 if the user clicks on an incorrect item. This is to avoid showing
                   // the "Quit Game" option too soon.
    playIncorrectSound();
    removeAllGameMenus();
    createRandomMenu();
    return true;
}
//-----------------------------------------------------------------------------

bool Game::correctButtonClicked()
{
    LOGGER.debug("Correct!!");
    ++m_counter; // Increment the counter if the user clicks on the correct item. This is used to determine
                 // when to show the "Quit Game" option in the menu and to know how many times the user has
                 // clicked on the menu items before the game ends.
    playCorrectSound();
    createRandomMenu();
    return true;
}
//-----------------------------------------------------------------------------

void Game::createMainMenu()
{
    m_main_menu = m_menu_mgr.spawnMenu();

    m_main_menu->items().push_back(std::make_shared<menu::item_t>("Start Game!",
            [&]()
            {
                LOGGER.info("Init presses");
                m_game_state = GameState::GAME;
                createRandomMenu();
                m_main_menu->visible() = false;
                return true;
            }
        )
    );

    //-- Explanation is on the main Window, no need for this option
    // m_main_menu->items().push_back(std::make_shared<menu::item_t>("Instructions",
    //         [&]()
    //         {
    //             LOGGER.info("EXPLANATION");
    //             m_game_state = GameState::EXPLANATION;
    //             return true;
    //         }
    //     )
    // );

    m_main_menu->items().push_back(std::make_shared<menu::item_t>("Exit",
            [&]()
            {
                LOGGER.info("EXIT");
                m_game_state = GameState::EXIT;
                return true;
            }
        )
    );
}
//-----------------------------------------------------------------------------

void Game::playSound(olc::ext::Miniaudio::Sound& sound)
{
    if(sound.IsLoaded())
        sound.Play();
    else
        LOGGER.error("Sound not loaded. Cannot play it!");
}
//-----------------------------------------------------------------------------

void Game::initializeAudioEngine()
{
    //-- Load the miniaudio extension
    if (!InstallSystemExtension(&m_audio))
    {
        LOGGER.error("Failed to install olcPGEX3_miniaudio");
        throw std::runtime_error("Failed to install olcPGEX3_miniaudio");
    }

    if(!m_audio.CreateSoundFromMemory(m_sound_click, assets::click_button.data(), assets::click_button.size()))
        LOGGER.error("Could not load audio 'click_button' from memory");
    if(!m_audio.CreateSoundFromMemory(m_sound_right, assets::action_correct.data(), assets::action_correct.size()))
        LOGGER.error("Could not load audio 'action_correct' from memory");
    if(!m_audio.CreateSoundFromMemory(m_sound_wrong, assets::action_wrong.data(), assets::action_wrong.size()))
        LOGGER.error("Could not load audio 'action_wrong' from memory");
}
//-----------------------------------------------------------------------------
