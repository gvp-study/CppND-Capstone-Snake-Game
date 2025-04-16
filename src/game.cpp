#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
  PlaceObstacles();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceObstacles() {
  int x, y;
  int NObstacles = 2;
  int count = 0;
  static bool first = false;

  while (count++ < NObstacles && !first) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      obstacles.emplace_back(x, y, count % 2 == 0 ? false : true);
    }
  }
  first = true;
}


void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food at the current head position
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    snake.GrowBody();
    snake.speed += 0.02;
  }

  PlaceObstacles();  // Update any moving obstacles

  // Check collision with obstacles and choose a new direction if needed
  for (const auto &obs : obstacles) {
    if (obs.GetPosition().x == new_x && obs.GetPosition().y == new_y) {

      // Try all possible directions to find a valid one
      std::vector<Snake::Direction> directions = {
        Snake::Direction::kUp,
        Snake::Direction::kDown,
        Snake::Direction::kLeft,
        Snake::Direction::kRight
      };

      for (auto dir : directions) {
        // Skip reversing direction
        /*
        if ((snake.direction == Snake::Direction::kUp && dir == Snake::Direction::kDown) ||
            (snake.direction == Snake::Direction::kDown && dir == Snake::Direction::kUp) ||
            (snake.direction == Snake::Direction::kLeft && dir == Snake::Direction::kRight) ||
            (snake.direction == Snake::Direction::kRight && dir == Snake::Direction::kLeft)) {
          continue;
        }
        */
        int dx = 0, dy = 0;
        switch (dir) {
          case Snake::Direction::kUp:    dx = -1; break;
          case Snake::Direction::kDown:  dx =  1; break;
          case Snake::Direction::kLeft:  dy = -1; break;
          case Snake::Direction::kRight: dy =  1; break;
        }

        int test_x = new_x + dx;
        int test_y = new_y + dy;

        bool blocked = false;
        for (const auto &o : obstacles) {
          if (o.GetPosition().x == test_x && o.GetPosition().y == test_y) {
            blocked = true;
            break;
          }
        }

        if (!blocked) {
          snake.direction = dir;
          return;  // Early exit after changing direction
        }
      }
    }
  }
}
