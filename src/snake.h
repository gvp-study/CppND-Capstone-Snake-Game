#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake
{
public:
  enum class Direction
  {
    kUp,
    kDown,
    kLeft,
    kRight
  };

  Snake(int grid_width, int grid_height, bool is_ai = false);

  void Update();
  void GrowBody();
  bool SnakeCell(int x, int y);

  void SetPath(std::vector<SDL_Point> path);
  void MoveTowardsTarget();
  Direction ChangeDirection(Direction dir);

  Direction direction = Direction::kUp;
  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  bool is_ai{false};
  SDL_Point target;
  std::vector<SDL_Point> body;

private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;

  std::vector<SDL_Point> path;
};

#endif