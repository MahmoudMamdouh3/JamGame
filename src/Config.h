#pragma once

#include <string>

// Window Configuration
constexpr unsigned int BASE_WINDOW_WIDTH = 1920;
constexpr unsigned int BASE_WINDOW_HEIGHT = 1080;
inline unsigned int WINDOW_WIDTH = BASE_WINDOW_WIDTH;
inline unsigned int WINDOW_HEIGHT = BASE_WINDOW_HEIGHT;

// Tile Configuration
const float TILE_WIDTH = 64.f;
const float TILE_HEIGHT = 32.f;
const float BLOCK_HEIGHT = 40.f;
const int MAP_SIZE = 30; // 30x30 Grid

// --- PHYSICS ---
const float GRAVITY = 2000.0f;
const float JUMP_FORCE = 550.0f;
const float MOVE_SPEED = 6.0f;
const float COLLISION_BIAS = 0.3f;

// UI Configuration
const std::string GAME_TITLE = "Insert\tSome\tArabic\tSaying\tHere";
const std::string FONT_PATH = "C:\\Windows\\Fonts\\arial.ttf";

// Audio Configuration
extern float AUDIO_VOLUME;

// --- ANIMATION ---
const float ANIM_FRAME_TIME = 0.12f;
const int FRAME_WIDTH = 50;
const int FRAME_HEIGHT = 37;
const float SPRITE_SCALE = 2.5f;