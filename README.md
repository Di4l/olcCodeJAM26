# olcCodeJAM26
Code for the [OLC Code JAM 2026](https://itch.io/jam/olc-codejam-2026)

(WIP)

## The game
The idea of the game is to navigate through the popping menus and clicking in the correct item until the "Quit Game" is found. There are some hints in the rectangle below to "aid" the player guess the correct menu item to click.

If you click in the correct item in the menu, a new menu will pop at a random place on the Window, with new entries to click on. If you click and incorrect element, the game starts all over again: all opened menus close, and a new one is created as if the game has just started.

All the menus are generated randomly at random positions, and it is guaranteed that no menu items are repeated across the menus.

## The project
The project uses the [OLC PixelGameEngine3](https://github.com/OneLoneCoder/olcPixelGameEngine3) framework, and the [miniaudio extension](https://github.com/OneLoneCoder/olcPixelGameEngine3/tree/main/extensions/miniaudio) to play audio.

A very simple and featureless menu manager has been written from scratch, using an olc::Image as the bases for drawing every menu. Items in the menu are always located vertically.

Managing the download and generation of building environment is done via cmake. The project should compile in Windows and Linux and generate a final single binary file. That is all that is needed to run and play the game.

### A
AI has been used just to create the .cmake files in the cmake folder.

### Building the project
At the console, at the main project's folder, execute:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```
where the type can be *Debug* or *Release*

This will create a folder *build* within the maun project folder where the framework will be downloaded, a patch applied to the downloaded code and some headers created where the sound assests are defined as arrays of hex values. The project is now ready to build

### Compiling and linking the game
At the console, at the main project's folder, execute:
```bash
cmake --build build
```

### Running the game
In the *build* directory of the main folder you could find the compiled binary after the project has been compiled successfully. Just the binary is needed to run the game; no extra assets (all embedded in the binary).

## [OLC CodeJAM 2026](https://itch.io/jam/olc-codejam-2026)
This game has been submitted to the olc CodeJAM 2026 and can be found here: https://wowlinh.itch.io/exitme2

We would like to take the opportunity to congratulate *One Lone Coder* and the *team of collaborators* that have managed to produce a neat product, very simple to use and multiplatform...

## Authors
This has been coded by a small team of 3 people:
    Alejandro Vaquero
    Alejandro Romero
    Raúl Hermoso (aka wowLinh, Linh, Di4l).

Special thanks must be given to **Daniel Lianes** for the ideas given at the initial stages of the project.
