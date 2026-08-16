# SdlZombies

SdlZombies is a game about tricking zombies into falling into holes.
It starts simple, but the zombies gradually move faster and harder to outmaneuver.

The game is based on a very old game that ran on the ZX81 (Sinclair).

![SdlZombies ScreenShot](/doc/sdlzombies.png)

---

## Version

**1.0.0 Release 3** — ArcaOS / OS/2 SDL2 port

---

## Platform

ArcaOS 5.x / OS/2 Warp 4.5 (32-bit)

---

## License

GNU General Public License v2 — see `COPYING`.

---

## Authors

- Original game: Philippe Brochard
- OS/2 initial port: Dave Yeo
- SDL2 migration (Release 3): ArcaOS community

---

## Requirements

The following libraries must be installed via **ANPM** before building or running:

| Package | Version |
|---|---|
| SDL2 | 2.x |
| SDL2_image | 2.x |
| SDL2_mixer | 2.x |
| GCC | 9.2 (RPM: gcc) |

The build also requires **Open Watcom** (`wl.exe`) on the PATH for the WLINK linker
backend used by the `emxomfld` wrapper.

---

## Building

Open an OS/2 command prompt in the game directory and run:

```
compile.cmd
```

This sets the required `EMXOMFLD_*` environment variables and invokes
`make -f makefile.os2`. On success the executable `sdlzomb.exe` is produced
in the same directory.

To clean object files:

```
make -f makefile.os2 clean
```

---

## Running

```
sdlzomb.exe [options]
```

Double-clicking `sdlzomb.exe` from a Workplace Shell folder also works.

The game window opens at **960 × 720** pixels (1.5× the 640 × 480 logical
resolution). All game graphics are scaled to fit.

### Command-line options

| Option | Description |
|---|---|
| `-f`, `--fullscreen` | Start in fullscreen mode |
| `-w`, `--window` | Start in windowed mode |
| `-ns`, `--nosound` | Disable sound |
| `-s`, `--sound` | Enable sound |

---

## Controls

| Key | Action |
|---|---|
| Arrow keys | Move the penguin |
| Mouse | Move the penguin (when mouse mode active) |
| `K` | Toggle keyboard / mouse control |
| `M`, `Ctrl` | Cycle map display (none → transparent → under → over) |
| `S`, `Shift` | Cycle sound volume (none → low → medium → high) |
| `F` | Toggle fullscreen / window (in-game key) |
| `Alt`+`Enter` | Toggle fullscreen / window (works in all screens) |
| `P`, `Space` | Pause / unpause |
| `Escape` | Quit / back to menu |

---

## Options file

When the game starts it reads and saves settings to `sdlzombies.dat` in the
game directory. To reset all options to their defaults, delete this file.

---

## Links

- Original game page: http://hocwp.free.fr/sdlzombies.html
- ArcaOS Port: https://github.com/OS2World/GAME-SDL-ACTION-sdlZombies
