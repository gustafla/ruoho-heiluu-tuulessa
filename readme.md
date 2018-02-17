# Instanssidemo2018

A demo to be released at Instanssi 2018

Binary requires a modern 64-bit GNU/Linux (libc.so.6), OpenGL 3.3 and SDL 2.0

Recommended Intel Ivy Bridge Core i5 M-line processor and HD Graphics 4000 GPU

## Building

Install dependencies

- a c++11 compiler
- SDL2.0 and headers
- OpenGL drivers and headers (probably satisfied by installing sdl2)
- GLM (OpenGL Mathematics)

Make a build directory (if doesn't exist) and cd to it

```
mkdir release
cd release
```

Run cmake and if its successful, run make

```
cmake ..
make -j $(( $(nproc) + 1 )) -l $(nproc)
```

Get the rocket track files and music.ogg from the demo release package

<https://www.pouet.net/party.php?which=1544&when=2018>
