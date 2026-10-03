//-----------------------------------------------------------------------------
#include "game.hpp"
#include "main_menu_options.hpp"

#include <iostream>
//-- Include asset file headers
// #include <retro_blop_18.hpp>
#include <keyboard001.hpp>
//-----------------------------------------------------------------------------
using namespace codejam26;
//-----------------------------------------------------------------------------

bool Game::OnUserCreate()
{
    //-- Feed the rand() function with a seed
    LOGGER.info("Initialize random seed");
    std::srand(std::time({}));
    //-- Initialize the audio engine: Install the extension and load sounds
    LOGGER.info("Initialize audio engine");
    initializeAudioEngine();
    LOGGER.info("Initialize menus");
    configureMap();

    // LOGGER.info("Create a sample menu");
    // m_menu = std::make_shared<menu_t>();

    // m_menu->items().emplace_back(std::make_shared<menu::item_t>("File", [&](){ playClickSound(); std::cout
    // << "Pressed 'File'\n"; return true; }));
    // m_menu->items().emplace_back(std::make_shared<menu::item_t>("Open (Ctrl+O)", [&](){ playClickSound();
    // std::cout << "Pressed 'Open'\n"; return true; }));
    // m_menu->items().emplace_back(std::make_shared<menu::item_t>("Exit", [&](){ playClickSound(); std::cout
    // << "Pressed 'Exit'\n"; return true; }));

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

    auto window_position = GetWindowPosition();
    window_position.y = window_position.y / 2;
    window_position.x = window_position.x / 2;

    CreateImage(*main_menu, window_position);

    createRandomMenu();
    CreateImage(*m_menu, {10, 10});

    LOGGER.info("Game instance created and initialized");
    return true;
}
//-----------------------------------------------------------------------------

bool Game::OnUserUpdate(float /*fElapsedTime*/)
{
    bool running = true;
    //-- Store where the mouse was first pressed

    //-- Clear the screen (grey => "dark white")
    draw.Clear(olc::Colour::GREY);

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
    // Configure the map with menu items and their corresponding messages to show as hints
    m_map.clear(); //-- Unnecesary, since this method is called at contruction and the map is empty then.
    //-- I'd rather use: m_map["File"] = "Every journey begins with a couple of words.";
    m_map.insert({"File", "Every journey begins with a couple of words."});
    m_map.insert({"Open", "Some things are meant to be opened, others not..."});
    m_map.insert({"Close Menu", "Sometimes the way forward is to leave."});
    m_map.insert({"New Menu", "A fresh start can change everything. Hopefully."});
    m_map.insert({"Delete", "What is gone cannot bother you anymore, not always but sometimes."});
    m_map.insert({"Copy", "Why make another when you can duplicate it? Don't forget to correct it."});
    m_map.insert({"Paste", "I think it is in my clipboard, but I am not sure. It is worth a try."});
    m_map.insert({"Cut", "The best way to get something in small pieces."});
    m_map.insert({"Help", "Should I have to read the manual? I think it is worth a try."});
    m_map.insert({"Sound", "It would be awesome to have this control on my baby's crying."});
    m_map.insert({"Settings", "Everything has a way to be adjusted. Probably."});
    m_map.insert({"Save", "Some things are worth keeping. Others are better forgotten."});
    m_map.insert({"Save As", "Perhaps it deserves a different name. Perhaps you do."});
    m_map.insert({"Share", "Good things are rarely kept alone. Bad things neither."});
    m_map.insert({"Preferences", "Everyone has their own way of doing things. Mine is probably wrong."});
    m_map.insert({"Close Window", "There is always another way out. Unless there isn't."});
    m_map.insert({"Undo", "Mistakes don't always have to be permanent. Convenient, isn't it?"});
    m_map.insert({"Search", "The answer may already be somewhere. The question is where."});
    m_map.insert({"Select All", "Why choose only one? That sounds like unnecessary work."});
    m_map.insert({"New Game", "Every adventure needs a first step. This might be one."});
    m_map.insert({"Continue Game", "The story isn't over yet. Apparently."});
    m_map.insert({"Options", "There is more than one way forward. Most of them are probably wrong."});
    m_map.insert({"Graphics", "Sometimes seeing is believing. Sometimes it is just bad graphics."});
    m_map.insert({"Credits", "Someone had to make all this. Sorry."});
    m_map.insert({"Cancel", "Not every decision needs to be final. Especially this one."});
    m_map.insert(
        {"Quit Game(On development)", "Every game eventually comes to an end. This one can end sooner."}
    );
    m_map.insert({"Video", "Some things are easier to understand when you see them. Some aren't."});
    m_map.insert({"Language", "Words mean different things to different people. Especially mine."});
    m_map.insert({"Start", "Everything has to begin somewhere. This seems as good a place as any."});
    m_map.insert({"Stop", "Knowing when to stop is important. I should probably follow my own advice."});
    m_map.insert({"<Invalid>", "Even mistakes can point you somewhere. Usually somewhere useless."});
    m_map.insert(
        {"------", "Sometimes the least obvious path is worth following. Sometimes it is just a line."}
    );
    m_map.insert({"Do not click!", "They wouldn't warn you without a reason."});
    m_map.insert({"Click Me!", "At least this one is honest."});
    m_map.insert({"*******", "Some secrets are better left unexplained. Like this one."});
    m_map.insert(
        {"Reset password",
         "Sometimes forgetting is the first step. Remembering the password would be better."}
    );
    m_map.insert({"Create Account", "Every identity has to start somewhere. This is probably legal."});
    m_map.insert({"Log  in", "Someone is expecting you. Hopefully it is you."});
    m_map.insert({"Log out", "Even visitors have to leave eventually. Probably."});
    m_map.insert({"Security", "Not everything should be trusted. Especially menus."});
    m_map.insert({"This is not a valid option", "Perhaps the wrong choice is hiding the right one."});
    m_map.insert(
        {"This is neither a valid option", "Two wrong answers still leave one possibility. I think."}
    );
    m_map.insert({"Stop? ", "You may want to reconsider before going further. Or don't."});
    m_map.insert({"Reload", "Sometimes a second look changes everything. Sometimes it just reloads."});
    m_map.insert(
        {"Quit Game",
         "Every game eventually comes to an end. Congratulations on making it this far! You have completed "
         "the game and reached the end. Thank you for playing!"}
    );

    // Populate the menu items vector with the keys from the map to insert them into the menu in a random
    // order
    m_buttons_text.clear();
    m_unused_indices.clear();

    int index = 0;
    for (const auto& it : m_map)
    {
        m_buttons_text.push_back(it.first);
        m_unused_indices.push_back(
            index
        ); // Store the index of the item that has been stored as a random button to click in the game. This
           // will be used to avoid showing the same item again in the menu.
        ++index;
    }

    LOGGER.info("Map reconfigured. {0:d} available items", m_map.size());
}

//-----------------------------------------------------------------------------

void Game::createRandomMenu()
{
#ifdef SPDLOG_ACTIVE_LEVEL
#    if (SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG)
    LOGGER.debug("Unused: ");
    for (int index : m_unused_indices)
    {
        LOGGER.debug("'{}'", m_buttons_text[index]);
    }
#    endif
#endif
    m_menu.reset(new menu_t());

    //-- Declare as size_t to avoid warning on the for loop below when comparing variables...
    size_t randomNumberOfItems
        = rand() % 7
        + 1; // Generate a random number between 1 and 7 this will be the elements shown in the menu

    int selectedIndex = selectRandomIndex();

    const std::string& buttonText = m_buttons_text.at(m_unused_indices.at(selectedIndex));

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

    for (std::size_t i = 0; i < m_unused_indices.size() && i < randomNumberOfItems; ++i)
    {
        int randomIndex {0};

        do
        {
            randomIndex = rand() % m_unused_indices.size();
        } while (m_buttons_text.at(m_unused_indices.at(randomIndex)) == "Quit Game");

        int itemIndex = m_unused_indices.at(randomIndex);
        eraseUsedIndices(randomIndex);

        bool randomBool = rand() % 2; // Generate a random boolean to decide if we will introduce the item at
                                      // the beginning or at the end of the menu. This is to avoid having the
                                      // same item always at the same position in the menu.

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
        selectedIndex
            = rand()
            % (m_unused_indices.size()); // Generate a random index to get the correct item to click on the
                                         // game. If the counter is less than 2, we will not show the last
                                         // item in the menu, which is "Quit Game" to avoid ending the game
                                         // too soon. After 2 clicks, we will allow the last item to be shown.
    } while (! showQuitGame
             && m_buttons_text.at(m_unused_indices.at(selectedIndex))
                    == "Quit Game"); // If the counter is less than 2, we will not show the last item in the
                                     // menu, which is "Quit Game" to avoid ending the game too soon. After 2
                                     // clicks, we will allow the last item to be shown.

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
