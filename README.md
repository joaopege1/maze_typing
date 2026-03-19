# Speed Typing Maze

This is my first complete project written in C++. It is a small game built to study the language and get hands-on experience with 2D graphics rendering using SFML.

## About

Speed Typing Maze is a 2D game where you navigate a player through a maze by typing directions into a text input. Instead of pressing arrow keys, you type `up`, `down`, `left`, or `right` and press Enter to move. The player slides in the chosen direction until hitting a wall. The goal is to reach the exit before the timer runs out.

## How to play

- Press **Enter** on the menu screen to start.
- Type a direction (`up`, `down`, `left`, `right`) and press **Enter** to move.
- Reach the exit tile before time runs out.
- If you run out of time, press **Enter** to restart or **Escape** to quit.

## Requirements

- C++17 or later
- [SFML 3](https://www.sfml-dev.org/)

## Building

Compile all `.cpp` files and link against SFML:

```bash
g++ -std=c++17 main.cpp Game.cpp Maze.cpp Player.cpp -lsfml-graphics -lsfml-window -lsfml-system -o SpeedMaze
```

Then run:

```bash
./SpeedMaze
```
