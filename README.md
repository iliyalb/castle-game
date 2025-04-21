# Castle Game Server

An experimental game server for real-time strategy games

## Build & Run

The build system used in this project is meson, setup a build directory:

```sh
mkdir build && cd build
meson ..
```

After generating the build files, build with ninja:

```sh
ninja
```

Finally run the executable:

```sh
./castle-game
```