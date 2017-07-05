# Bilateral filter implementation in GLSL

Core implementation is written in fragment shader.

See [fshader.frag](https://github.com/tranvansang/bilateral-filter/blob/master/fshader.frag) for implementation detail.

## Usage

- Install [Qt Creator](https://www.qt.io/ide/) (free)
- Open BilatteralFiltering.pro
- Hit run

## Note:
- This project is created using Qt 5.9.1
- This project require GLSL compiler ver 4.10
- Tested under window 7
- Currently, cannot compiled in Ubuntu because of OpenGL driver problem
- Execute `BilatteralFiltering.exe` directly in Window to run program without compiling (non tested)

Screenshots:

![alt text](https://githubusercontent.com/tranvansang/bilateral-filter/master/screenshots/sc1.png)

![alt text](https://githubusercontent.com/tranvansang/bilateral-filter/master/screenshots/sc2.png)
