# climusic
**climusic** is a basic music player for the terminal, written in C.

# Build
## Requirements

Make sure the following dependencies are installed on your system:

- `ncurses`
- `SDL2`
- `SDL2_mixer`
- `Ruby` *Ruby is used to run the build script.*

### On Debian/Ubuntu-based systems:

```bash
sudo apt update
sudo apt install cmake libncurses-dev libsdl2-dev libsdl2-mixer-dev ruby-full
```

### On Termux

```bash
pkg update && pkg upgrade
pkg install cmake sdl2 sdl2-mixer ruby # no need ncurses because its built-in
```

# Run
## Any Linux

```bash
ruby build.rb
```

## Termux

```bash
ruby build.rb --termux
```