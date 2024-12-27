# ICG HW4

## Environment Set-up
1. Follow the instructions in `Environment Setup Guide.pdf`


## Install Instructions
1. Clone repository
```
git clone https://github.com/vch2128/2024_ICG_HW4
```
2. Re-install glm
```
cd 2024_ICG_HW4
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


## Operation Instructions
### Original Settings
UFO is at the left hand side, out of the viewing window. Rotate camera(`D`) or move UFO right(`L`) to see it.

Alien is not showing. Press `I` to show it.

### Available Operations
1. Camera
`A`: Rotate left

`D`: Rotate right

`W`: Zoom in

`S`: Zoom out


2. UFO
`J`: Move left

`L`: Move right


3. Alien
`I`: Show alien

`K`: Alien rotate around UFO


4. Effects
`1`: Alien: not showing, UFO: grow effect

`2`: Alien: not showing, UFO: alert effect

`3`: Alien: gouraud effect, UFO: alert effect

`4`: Alien: fur effect, UFO: alert effect

`5`: Alien: fur effect, UFO: noise effect

`6`: Alien: split effect, UFO: noise effect

`7`: Alien: explosion effect, UFO: explosion effect