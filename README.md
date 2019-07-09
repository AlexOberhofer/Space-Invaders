# Space Invaders

<p align="center">
  <img src="https://github.com/AlexOberhofer/Space-Invaders/raw/master/doc/attract.PNG" alt="I8080"/>
</p>

An Emulation of the Intel 8080 version of Space Invaders, implemented in C using SDL2. This emulator is still under development.


## Building
A makefile is provided for gnu/linux systems. This requires the SDL2 library, clang, and make to build.

Clone this repository

```
$ git clone https://github.com/AlexOberhofer/Space-Invaders.git
```
Switch to working directory

```
$ cd Space-Invaders
```

Build

```
$ make
```

## Running

This emulator requires a singular rom file "invaders.rom" to be located in the /res/ directory. A single rom file can be created from the individual space invaders roms by following the shell command below.

```
$ cat invaders.h invaders.g invaders.f invaders.e > invaders.rom
```

The program can be run with the following command when compiled from the root project directory.

```
$ ./bin/invaders
```

This project is currently a work in progress. 

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
