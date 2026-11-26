# SPACE DEFENDER

A keyboard-controlled terminal space shooter written in **C** with the **ncurses** library.
You are the last line of defence: enemies that slip past your ship damage the **base**.
Lose the base or all of your ships and the mission is over.

Final project for *Pengenalan Pemrograman*.

## Features
- Main menu, difficulty selection (Easy / Normal / Hard), How to Play, Hall of Fame
- **Base defence**: the line at the bottom of the screen is your base; its health is shown as a bar
- 4 enemy types: basic, fast (zigzag), tank (3 hits), shooter (fires back)
- **Boss fight** every 5 levels with a health bar and a second, enraged phase
- 5 power-ups: extra ship, shield, triple shot, bomb, base repair
- **Bomb** (B): clears the screen, wipes enemy bullets and hurts the boss
- **Combo multiplier** (up to x4) for destroying enemies without taking damage
- Explosions, parallax starfield, colours, pause, auto-fire toggle
- End-of-mission statistics: rank, accuracy, best combo, time, bosses beaten
- Top-5 Hall of Fame with player names, saved to a file

## Controls
| Key | Action |
|---|---|
| Left / Right or A / D | Move |
| SPACE | Fire |
| B | Bomb |
| F | Toggle auto-fire |
| P | Pause |
| Q | End the mission |

## Build and run
```bash
# Ubuntu / Debian / WSL
sudo apt install build-essential libncurses-dev

make              # build
./space_defender  # or: make run
make test         # run the unit tests
make clean        # remove build files
```
The terminal must be at least **70x24**.

## Project structure
```
space-defender/
├── Makefile
├── README.md
├── docs/dokumen_progres1.md      # progress document
├── include/                      # headers
│   ├── config.h                  # every tunable constant
│   ├── types.h                   # structs and enums
│   └── game.h player.h enemy.h boss.h bullet.h powerup.h base.h
│       effects.h stars.h difficulty.h highscore.h ui.h ui_internal.h
├── src/
│   ├── main.c         # entry point, menu flow
│   ├── game.c         # game loop, input, collisions, levels, scoring
│   ├── player.c       # movement, firing, damage
│   ├── enemy.c        # enemy types, spawning, behaviour
│   ├── boss.c         # boss movement, attack patterns, defeat
│   ├── bullet.c       # bullet movement (player and enemy)
│   ├── powerup.c      # drops and effects
│   ├── base.c         # base health
│   ├── effects.c      # explosions and on-screen banner
│   ├── stars.c        # background
│   ├── difficulty.c   # difficulty table
│   ├── highscore.c    # file I/O for the Hall of Fame
│   ├── ui_core.c      # ncurses init, colours, helpers
│   ├── ui_menu.c      # logo and main menu
│   ├── ui_game.c      # in-game drawing and HUD
│   └── ui_screens.c   # help, scores, name prompt, game over
└── tests/             # unit tests (make test)
```

## How the game works
- `main.c` shows the menu and starts a mission; `game_run()` is the game loop:
  read input -> `game_update()` -> `ui_draw_game()` -> wait one frame.
- All state lives in one `Game` struct (`types.h`), allocated with `calloc` and passed by pointer to every module.
- Gameplay numbers (speeds, damage, timers, chances) are constants in `config.h` and `difficulty.c`, so balancing never needs logic changes.

## Course topics used
| Topic | Where |
|---|---|
| Variables, data types | everywhere |
| Conditionals, loops | `game.c`, `enemy.c`, `boss.c` |
| Arrays | bullets, enemies, power-ups, stars, explosions (`types.h`) |
| Functions, modular code | every `.c` file |
| Structs and enums | `types.h` |
| Pointers | modules receive `Game *g`, `Player *p`, `Enemy *e` |
| Dynamic memory | `calloc` / `free` in `game_create` / `game_destroy` |
| File handling | `highscore.c` |
| Random numbers | spawning, drops, zigzag movement |
| Multi-file programs, Makefile | `include/`, `src/`, `Makefile` |

## Suggested work split (3 members)
| Member | Files |
|---|---|
| 1. Game logic | `game.c`, `player.c`, `bullet.c`, `base.c`, `difficulty.c` |
| 2. Interface | `ui_*.c`, `effects.c`, `stars.c` |
| 3. Content | `enemy.c`, `boss.c`, `powerup.c`, `highscore.c`, tests, docs |

Each member commits their own work to GitHub (one branch per feature, merged with pull requests) so the commit history shows every contribution.

## Team
| Name | Student ID | Role |
|---|---|---|
| ... | ... | ... |
