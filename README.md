# olcCodeJAM26
Code for the OLC Code JAM 2026

(WIP)

This has been created by a small team of 3 people:
    Alejandro Vaquero
    Alejandro Romero
    Raúl Hermoso (aka wowLinh, Linh, Di4l).

Special thanks must be given to Daniel Lianes for the ideas given for the initial stages of the project.

The idea of the game is to navigate through the popping menus and clicking in the correct item until a menu with the "Quit Game" is found. There are some hints in the rectangle below to "help" you guess the correct menu item.

The application should compile in Windows and Linux. Just the binary needed to run, no extra assets (all embedded in the binary)

This game has been submitted to the OLC CodeJAM 2026: https://wowlinh.itch.io/exitme2

AI has been used just to create the .cmake files in the cmake folder.

## Building and compiling
To build the project, from the console, inside the project's folder execute:

'''cmake -B build -DCMAKE_BUILD_TYPE=Debug'''

where the type can be Debug on of Debug or Release

Then, to compile the project execute command:

'''cmake --build build'''
