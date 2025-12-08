#include "Game.h"
#include "Config.h"

// Define global audio volume
float AUDIO_VOLUME = 50.f;

int main()
{
    Game game;
    game.run();
    return 0;
}