//-----------------------------------------------------------------------------
#include "game.hpp"
#include "main_menu_options.hpp"

#include <map>
#include <string_view>
#include <random>

//-- Include asset file headers
// #include <retro_blop_18.hpp>
#include <keyboard001.hpp>
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
    LOGGER.info("Initialize menus");
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
    m_hints->items().push_back(std::make_shared<menu::item_t>(SV_HINT.data(), nullptr));

    main_menu = std::make_shared<menu_t>();
    main_menu->items().emplace_back(
        std::make_shared<menu::item_t>(
            "Start Game!",
            [this]()
            {
                LOGGER.info("Init presses");
                m_current_state = MainMenuOptions::GAME;

                return true;
            }
        )
    );

    main_menu->items().emplace_back(
        std::make_shared<menu::item_t>(
            "Instructions",
            [this]()
            {
                LOGGER.info("EXPLANATION");
                m_current_state = MainMenuOptions::EXPLANATION;
                return true;
            }
        )
    );

    main_menu->items().emplace_back(
        std::make_shared<menu::item_t>(
            "Exit",
            [this]()
            {
                LOGGER.info("EXIT");
                m_current_state = MainMenuOptions::EXIT;
                return true;
            }
        )
    );

    LOGGER.debug("Try creating a menu within the manager...");
    auto mn { m_menu_mgr.spawnMenu() };
    LOGGER.debug(" - Menu created with address 0x{:x}", reinterpret_cast<std::uintptr_t>(mn.get()));
    if(mn)
    {
        LOGGER.debug(" - Place menu at 500, 50");
        mn->moveTo({500.0f, 50.0f});
        LOGGER.debug(" - Create menu item...");
        mn->items().push_back(std::make_shared<menu::item_t>("Probando", nullptr));
        LOGGER.debug(" - ...done!");
    }

    LOGGER.info("Create starting menu");
    createRandomMenu();

    CreateImage(*m_menu,  {10, 10});
    CreateImage(*main_menu, {10, 10});
    CreateImage(*m_hints, {0, 350});

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
    auto hsz { draw.GetTextSize(m_hints->items()[0]->text, true) };
    m_hints->draw({ (wsz.x - hsz.x) / 2.0f, (wsz.y - hsz.y) - 10.0f });

    //-- If we wanted to draw the manu at a different coordinates, we could
    //   pass a second parameter to draw() with the desired position
    m_menu->draw({20, 10});

    m_menu_mgr.draw();

    //-- Get the mouse position
    auto lft_btn_status {mouse.GetButton(0)};
    if(lft_btn_status.bPressed)
    {
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
            menu_moving     = false;
            menu_pressed    = nullptr;
            menu_offset_pos = {0.0f,0.0f};
            LOGGER.debug("...stop moving menu");
        }
        else
        {
            m_menu->onClicked(mouse_pos_pressed);
        }
    }
    
    auto halfScreen = (draw.GetTargetSize() - main_menu->Size()) / 2;
    if (m_current_state == MainMenuOptions::MAIN_MENU)
    {
        main_menu->draw(this, halfScreen);
        main_menu->configureMouse(this, mouse);
    }
    else if (m_current_state == MainMenuOptions::EXPLANATION)
    {
        std::string explicationText = "Valid text";
        main_menu->draw(this, halfScreen);
        main_menu->configureMouse(this, mouse);

        auto explicationPosition = halfScreen;
        explicationPosition.x = explicationPosition.x + main_menu->Size().x + 40;
        draw.StringProp(explicationPosition, explicationText);
    }
    else if (m_current_state == MainMenuOptions::GAME)
    {
        //-- If we wanted to draw the manu at a different coordinates, we could
        //   pass a second parameter to draw() with the desired position
        m_menu->draw(this, {20, 10});
        m_menu->configureMouse(this, mouse);
    }
    else if (m_current_state == MainMenuOptions::EXIT)
    {
        running = false;
    }

    return running;
}
//-----------------------------------------------------------------------------

void Game::configureMap()
{
    // Populate the menu items vector with the keys from the map to insert them into the menu in a random order
    m_buttons_text.clear();
    m_unused_indices.clear();

    int index { 0 };
    for(const auto& it : MITM_MAP)
    {
        m_buttons_text.push_back(it.first.data());
        m_unused_indices.push_back(index++); // Store the index of the item that has been stored as a random button to click in the game. This will be used to avoid showing the same item again in the menu. 
    }
}

//-----------------------------------------------------------------------------

void Game::createRandomMenu()
{
    m_menu.reset(new menu_t());

    //-- Declare as size_t to avoid warning on the for loop below when comparing variables...
    auto randomNumberOfItems = random<size_t, 1, 7>(); // Generate a random number between 1 and 7 this will be the elements shown in the menu

    int selectedIndex = selectRandomIndex();

    auto const hint = MITM_MAP.at(buttonText);

    auto hint = m_map.at(buttonText);

    m_current_pair = std::make_pair(
        buttonText,
        hint
    ); // Store the item and its corresponding hint that the user has to click on in the game. This is used to
       // know which item the user has to click on in the game and to show the hint for that item.

    eraseUsedIndices(
        selectedIndex
    ); // Remove the index of the item that has been stored as the correct button to click in the game. This
       // will be used to avoid showing the same item again in the menu.

    m_menu->items().emplace_back(
        std::make_shared<menu::item_t>(buttonText, std::bind_front(&Game::correctButtonClicked, this))
    ); // Add the correct item to the menu with its corresponding callback
    LOGGER.debug("Correct button: '{}'", m_current_pair.first);
    LOGGER.debug(
        " - Hint: '{}'",
        m_current_pair.second
    ); // Show the hint for the item that the user has to click on in the game.

    m_hints->items()[0]->text = std::string(SV_HINT.data()) + hint.data();

    for(std::size_t i = 0; i < m_unused_indices.size() && i < randomNumberOfItems; ++i)
    {            
        int randomIndex{0};

        do
        {
            randomIndex = random<size_t>(0, m_unused_indices.size());
        }
        while(m_buttons_text.at(m_unused_indices.at(randomIndex)) == "Quit Game");

        int itemIndex = m_unused_indices.at(randomIndex);
        eraseUsedIndices(randomIndex);
        
        bool randomBool = random(); // Generate a random boolean to decide if we will introduce the item at the beginning or at the end of the menu. This is to avoid having the same item always at the same position in the menu.

        // Add the item to the menu with its corresponding callback
        if (randomBool)
        {
            m_menu->items().emplace_back(
                std::make_shared<menu::item_t>(
                    m_buttons_text.at(itemIndex),
                    std::bind_front(&Game::incorrectButtonClicked, this)
                )
            );
        }
        else
        {
            m_menu->items().insert(
                m_menu->items().begin(),
                std::make_shared<menu::item_t>(
                    m_buttons_text.at(itemIndex),
                    std::bind_front(&Game::incorrectButtonClicked, this)
                )
            );
        }
    }

    CreateImage(*m_menu, {10, 10});
}

//-----------------------------------------------------------------------------

void Game::eraseUsedIndices(int index)
{
    m_unused_indices.erase(
        m_unused_indices.begin() + index
    ); // Store the index of the item that has been stored as the correct button to click in the game. This
       // will be used to avoid showing the same item again in the menu.
}

//-----------------------------------------------------------------------------

bool Game::incorrectButtonClicked()
{
    LOGGER.debug("Incorrect!! Pressed '{}'", m_current_pair.first);
    m_counter = 0; // Reset the counter to 0 if the user clicks on an incorrect item. This is to avoid showing
                   // the "Quit Game" option too soon.
    configureMap();
    createRandomMenu();
    playClickSound();
    return true;
}

//-----------------------------------------------------------------------------

bool Game::correctButtonClicked()
{
    LOGGER.debug("Correct!! Pressed '{}'", m_current_pair.first);
    ++m_counter; // Increment the counter if the user clicks on the correct item. This is used to determine
                 // when to show the "Quit Game" option in the menu and to know how many times the user has
                 // clicked on the menu items before the game ends.
    createRandomMenu();
    playClickSound();
    LOGGER.info("Hint: {}", m_current_pair.second);
    return true;
}

//-----------------------------------------------------------------------------

int Game::selectRandomIndex()
{
    bool showQuitGame {m_counter >= 2}; // Show the "Quit Game" option only after 2 clicks have been made in
                                        // the game. This is to avoid ending the game too soon.
    int selectedIndex {0};
    do
    {
        selectedIndex = random<int>(0, m_unused_indices.size()) ; // Generate a random index to get the correct item to click on the game. If the counter is less than 2, we will not show the last item in the menu, which is "Quit Game" to avoid ending the game too soon. After 2 clicks, we will allow the last item to be shown.
    }
    while(!showQuitGame && m_buttons_text.at(m_unused_indices.at(selectedIndex)) == "Quit Game"); // If the counter is less than 2, we will not show the last item in the menu, which is "Quit Game" to avoid ending the game too soon. After 2 clicks, we will allow the last item to be shown.

    return selectedIndex;
}

//-----------------------------------------------------------------------------

void Game::playClickSound()
{
    if (m_sound_click.IsLoaded())
    {
        m_sound_click.Play();
    }
    else
    {
        LOGGER.error("Click sound not loaded. Cannot play it!");
    }
}

//-----------------------------------------------------------------------------

void Game::initializeAudioEngine()
{
    //-- Load the miniaudio extension
    if (! InstallSystemExtension(&m_audio))
    {
        LOGGER.error("Failed to install olcPGEX3_miniaudio");
        throw std::runtime_error("Failed to install olcPGEX3_miniaudio");
    }

    if (! m_audio
              .CreateSoundFromMemory(m_sound_click, assets::keyboard001.data(), assets::keyboard001.size()))
    {
        LOGGER.error("Could not load audio 'keyboard001' from memory");
    }
}

//-----------------------------------------------------------------------------
