#pragma once

enum GameState { MENU, PLAYING, GAME_OVER, LEVEL_COMPLETE, GAME_WON };
enum GameMode  { EASY_MODE, HARD_MODE };

const int MAX_LEVELS = 10;

const unsigned int SCREEN_WIDTH  = 850;
const unsigned int SCREEN_HEIGHT = 600;
const int SCREEN_CENTER_X = SCREEN_WIDTH  / 2;
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;
const int TILE_SIZE       = 40;
const float TIME_LIMIT    = 2000.f;
