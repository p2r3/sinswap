## Building
This is a Linux-only project, as the hack depends on a shared glibc library for extracting the original trig functions, and uses `LD_PRELOAD` to inject itself into the game. You can probably do this on Windows, but this code won't be of any help to you.

You'll need 32-bit compatible `gcc` to compile, which you likely already have. There are no other dependencies. Run `build.sh`, and it should generate `pi4.so` and `sinswap.so`. You can use `file` to make sure it reports an "ELF 32-bit LSB shared object". 

## Installation

1. Move the libraries into your "Portal 2" directory.
2. Edit `portal2.sh` to add `export LD_PRELOAD=pi4.so` right under `cd "$GAMEROOT"`
3. Lock your pitch at 0deg in-game with console commands `cl_pitchup 0` and `cl_pitchdown 0`

If using `sinswap.so` or some other unstable build, you may use the ""physics engine"" I hacked together in 10 minutes. It's nothing more than basic collision detection and gravity simulation. To install it, move the `portal2_dlc4` directory in this repository to your `Portal 2` folder. If you don't already have a `portal2_dlc3` there, rename it to that. If you already have `portal2_dlc4` or higher, rename it to the lowest available number. Then, when in game, before opening a map, run `sv_cheats 1` and `phys_timescale 0` to disable the game's existing physics.

## Contributing
Please don't. I don't intend on checking pull requests or issues.
