#include "snake.h"
#include <cmath>
#include <iostream>

Snake::Snake(int grid_width, int grid_height, bool is_ai)
    : grid_width(grid_width),
      grid_height(grid_height),
      head_x(grid_width / 2),
      head_y(grid_height / 2),
      is_ai(is_ai) {}

void Snake::SetPath(std::vector<SDL_Point> new_path) {
  path = std::move(new_path);
}

void Snake::MoveTowardsTarget() {
  if (path.empty()) return;

  SDL_Point next = path.front();
  path.erase(path.begin());

  int dx = next.x - static_cast<int>(head_x);
  int dy = next.y - static_cast<int>(head_y);

  if (dx == 1) direction = Direction::kRight;
  else if (dx == -1) direction = Direction::kLeft;
  else if (dy == 1) direction = Direction::kDown;
  else if (dy == -1) direction = Direction::kUp;
}

void Snake::Update() {
  if (is_ai) {
    speed = 0.05f;
    MoveTowardsTarget();
  }

  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Previous head cell

  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Current Head cell

  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

Snake::Direction Snake::ChangeDirection(Snake::Direction dir) {
  switch (dir) {
    case Snake::Direction::kUp: return Snake::Direction::kRight;
    case Snake::Direction::kDown: return Snake::Direction::kLeft;
    case Snake::Direction::kLeft: return Snake::Direction::kUp;
    case Snake::Direction::kRight: return Snake::Direction::kDown;
  }
  // Optional: default return to avoid compiler warning
  return dir;
}
 /*
  switch (dir) {
    case Snake::Direction::kUp: return Snake::Direction::kDown;
    case Snake::Direction::kDown: return Snake::Direction::kUp;
    case Snake::Direction::kLeft: return Snake::Direction::kRight;
    case Snake::Direction::kRight: return Snake::Direction::kLeft;
  }
  */
 