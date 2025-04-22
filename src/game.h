#ifndef GAME_H
#define GAME_H

#include <random>
#include <set>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "astar.h"

class Renderer;

class Obstacle {
  public:
      Obstacle(int x, int y, bool moving = false) : moving(moving) {
        position.x = x;
        position.y = y;
      }

      // Move if it's a moving obstacle
      void Update() {
        if (moving) {
            position.x += direction;
            if (position.x < 0 || position.x >= 32) direction *= -1;
        }
      }
      
      SDL_Point GetPosition() const {
        return position;
      }
      bool IsMoving() const {
        return moving;
      }
  
  private:
      SDL_Point position;
      bool moving;
      int direction = 1;
  };

enum class FoodType { Regular, Bonus, SpeedUp };

struct Food
{
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

  // Obstacle list
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

#endif