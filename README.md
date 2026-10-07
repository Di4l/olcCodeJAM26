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

### Use of Artificial Intelligence
AI has been used to create the .cmake files in the cmake folder.
For the web version, AI has guided us with the modifications to CMakeLists.txt to be able to build and compile for web.

## Building and compiling

### Windows/Linux

#### Building the project
At the console, at the main project's folder, execute:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
```
where the type can be *Debug* or *Release*

This will create a folder *build* within the maun project folder where the framework will be downloaded, a patch applied to the downloaded code and some headers created where the sound assests are defined as arrays of hex values. The project is now ready to build

#### Compiling and linking the game
At the console, at the main project's folder, execute:
```bash
cmake --build build -j$(nproc)
```

#### Running the game
In the *build* directory of the main folder you could find the compiled binary after the project has been compiled successfully. Just the binary is needed to run the game; no extra assets (all embedded in the binary).

### Web (Emscripten)

For this to be a reeality I trsuted AI, as I know not how to create the toolchain needed to compile and distribute a web application.
Yo need to have emscripten sdk installed and activated. With this we will be able to follow the building and then compiling of the project.

#### Building the project

Pretty similar to the 'propper' binaries scenario, we build the project by executing a flavour of cmake:
```bash
emcmake cmake -B build-web -DCMAKE_BUILD_TYPE=Release
```

#### Compiling and linking

At the console and once the project has been built, execute:
```bash
cmake --build build-web -j$(nproc)
```

This should produce an .wasm and .js file. Those two, combined with the index.html in the assets folder can be put together to run the game in a browser.

#### Running the web game

Copy the index.html file in the asset folder to the place where the .js and .wasm files are. The three files need to be in the same folder. On a terminal, navigate to the path where the three files are and launch a web server:
```bash
python3 -m http.server 8000
```
Now, open your web browser and navigate to [http://localhost:8000/](http://localhost:8000/). A page with the game should load. Enjoy!!!

## [OLC CodeJAM 2026](https://itch.io/jam/olc-codejam-2026)
This game has been submitted to the olc CodeJAM 2026 and can be found here: https://wowlinh.itch.io/exitme2

We would like to take the opportunity to congratulate *One Lone Coder* and the *team of collaborators* that have managed to produce a neat product, very simple to use and multiplatform...

## Authors
This has been coded by a small team of 3 people:
1. Alejandro Vaquero
2. Alejandro Romero
3. Raúl Hermoso (aka wowLinh, Linh, Di4l).

Special thanks must be given to **Daniel Lianes** for the ideas given at the initial stages of the project.
