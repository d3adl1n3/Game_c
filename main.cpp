#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <filesystem>
#include <random>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

using namespace std;

#include "SVGParser.hpp"
#include "Sprite.hpp"
#include "Timer.hpp"
#include "BasicTile.hpp"
#include "Background.hpp"
#include "PauseButton.hpp"
#include "GameOver.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "FieldMap.hpp"
#include "Game.hpp"

int main(int argc, const char * argv[]) {
    Game game;
    while (game.isRunning()){
        game.handleGestures();
        game.update();
        game.render();
    }
    game.close();
    return 0;
}
