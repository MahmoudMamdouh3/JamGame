#pragma once
#include <string>

// --- ENUMS ---
enum class GameState
{
    Menu,
    Playing,
    Paused,
    Exiting
};

// --- MUTABLE SETTINGS (Can change during runtime) ---
inline int WINDOW_WIDTH = 1920;
inline int WINDOW_HEIGHT = 1080;
inline float AUDIO_VOLUME = 50.0f;

// --- UI & SYSTEM CONSTANTS (Missing lines restored here!) ---
const std::string GAME_TITLE = "Isometric Puzzle Game";
const std::string FONT_PATH = "assets/fonts/arial.ttf";

// These are used for calculating relative positions in the menu
const int BASE_WINDOW_WIDTH = 1920;
const int BASE_WINDOW_HEIGHT = 1080;

// --- ISOMETRIC TILE SETTINGS ---
const float TILE_WIDTH = 128.f;
const float TILE_HEIGHT = 128.f;
const float BLOCK_HEIGHT = 40.f;
const int MAP_SIZE = 16;

// --- PHYSICS ---
const float GRAVITY = 2000.0f;
const float JUMP_FORCE = 550.0f;
const float MOVE_SPEED = 6.0f;
const float COLLISION_BIAS = 0.3f;

// --- ANIMATION SETTINGS (The fixes we made previously) ---
const int TEXTURE_WIDTH = 2048;
const int TEXTURE_HEIGHT = 1024;

const int FRAMES_PER_ROW = 8;
const int ROWS_IN_SHEET = 4;

// Auto-calculate frame size
const int FRAME_WIDTH = TEXTURE_WIDTH / FRAMES_PER_ROW;
const int FRAME_HEIGHT = TEXTURE_HEIGHT / ROWS_IN_SHEET;

// Sprite scale (0.35f usually works well for 2048x1024 sheets on these tiles)
const float SPRITE_SCALE = 0.35f;

const float ANIM_FRAME_TIME = 0.12f;