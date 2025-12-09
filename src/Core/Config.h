#pragma once
#include <string>

// --- ENUMS ---
// Moved here so Game.h, Menu.h, and PauseMenu.h can all see it
enum class GameState
{
    Menu,
    Playing,
    Paused,
    Exiting
};

// --- MUTABLE SETTINGS ---
// 'inline' prevents "redefinition" errors
inline int WINDOW_WIDTH = 1920;
inline int WINDOW_HEIGHT = 1080;
inline float AUDIO_VOLUME = 50.0f;

// --- CONSTANTS ---
const std::string GAME_TITLE = "Isometric Adventure";
const int BASE_WINDOW_WIDTH = 1920;
const int BASE_WINDOW_HEIGHT = 1080;
const std::string FONT_PATH = "assets/fonts/static/Cairo-Regular.ttf";

const float TILE_WIDTH = 128.f;
const float TILE_HEIGHT = 128.f;
const float BLOCK_HEIGHT = 40.f;
const int MAP_SIZE = 16;

const float GRAVITY = 2000.0f;
const float JUMP_FORCE = 550.0f;
const float MOVE_SPEED = 6.0f;
const float FOLLOWER_SPEED = 10.0f;
const float COLLISION_BIAS = 0.3f;

const float ANIM_FRAME_TIME = 0.12f;
const int FRAME_WIDTH = 50;
const int FRAME_HEIGHT = 37;
const float SPRITE_SCALE = 2.5f;