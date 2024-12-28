# ICG HW4

## Environment Set-up
1. Follow the instructions in `Environment Setup Guide.pdf`

<br>

## Install Instructions
1. Clone repository
```
git clone https://github.com/vch2128/2024_ICG_HW4
```
2. Re-install glm
```
cd 2024_ICG_HW4/extern
rmdir glm
git clone https://github.com/g-truc/glm.git
```
3. Re-build the project
```
cd ..
rm -rf build
mkdir build
cd build
cmake ..
```

<br>

## Operation Instructions
### Original Settings
UFO is on the left hand side, out of the viewing window. Rotate camera(`D`) or move UFO right(`L`) to see it.

Alien is not showing. Press `I` to show it.

<br>

### Available Operations
1. Camera

`A`: Rotate left

`D`: Rotate right

`W`: Zoom in

`S`: Zoom out

<br>

2. UFO

`J`: Move left

`L`: Move right

<br>

3. Alien

`I`: Show alien

`K`: Alien rotate around UFO

<br>

4. Effects

`1`: Alien: not showing, UFO: grow effect

`2`: Alien: not showing, UFO: alert effect

`3`: Alien: gouraud effect, UFO: alert effect

`4`: Alien: fur effect, UFO: alert effect

`5`: Alien: fur effect, UFO: noise effect

`6`: Alien: split effect, UFO: noise effect

`7`: Alien: explosion effect, UFO: explosion effect