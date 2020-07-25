# Space Invaders

<p align="center">
  <img src="https://github.com/AlexOberhofer/Space-Invaders/raw/master/doc/attract.PNG" alt="Chip 8 Image"/>
</p>

A multiplatform Emulation of the Intel 8080 version of Space Invaders, implemented in C using SDL2. <br>

The main cpu core is still in development. A debug core is hooked in and provides functionality in its place.


## Building

Clone this repository

```
$ git clone https://github.com/AlexOberhofer/Space-Invaders.git
```
Switch to working directory

```
$ cd Space-Invaders
```

### UNIX / MACOS


A makefile is provided for gnu/linux systems. This requires the SDL2 library, clang, and make to build. These can all be installed through brew on Mac OSX, or your favorite unix based package manager for *nix systems.


```
$ make
```

### WINDOWS

A bash script to cross compile a windows executable using mingw-w64 is provided in the /scripts/ directory 

```
$ ./build_win.sh
```

## Running

### UNIX / MACOS

This emulator requires a singular rom file "invaders.rom" to be located in the /res/ directory. A single rom file can be created from the individual space invaders roms by following the shell command below.

```
$ cat invaders.h invaders.g invaders.f invaders.e > invaders.rom
```

The invaders.rom file must be placed in the same directory as the space invaders executable. The executable is output in the /bin/ folder.

```
$ cd bin/
```

```
$ ./invaders 
```

### Windows

The executable requires the invaders.rom file to be in the same directory as invaders.exe. SDL2.dll is also required to be placed in the same directory as the windows executable.

The emulator can be started by simply clicking invaders.exe.

## Controls

General Controls

```
P - PAUSE GAME
```

```
R - RESUME GAME
```

```
ESC - QUIT 
```

```
C - INSERT COIN
```

```
X - START GAME
```

```
Z - COLOR MODE TOGGLE
```

Gameplay Controls

```
A - LEFT 
```

```
D - RIGHT 
```

```
S - FIRE 
```

```
T - TILT 
```



## Author

Alex Oberhofer

## License

This project is licensed under the MIT License.
