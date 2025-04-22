#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include <set>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "astar.h"
#include "obstacle.h"  // Include the new Obstacle header

class Renderer;

enum class FoodType { Regular, Bonus, SpeedUp };

struct Food {
    SDL_Point position;
    FoodType type;
};

class Game {
public:
    Game(std::size_t grid_width, std::size_t grid_height);
    void Run(Controller const &controller, Renderer &renderer,
             std::size_t target_frame_duration);
    int GetScore() const { return score; }
    int GetSize() const { return snake.size; }

private:
    Snake snake;
    Snake comp_snake;
    Food food;
    std::vector<Obstacle> obstacles;
    std::size_t grid_width;
    std::size_t grid_height;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;

    int score{0};

    void PlaceFood();
    void PlaceObstacles();
    void Update();
};

#endif // GAME_H
