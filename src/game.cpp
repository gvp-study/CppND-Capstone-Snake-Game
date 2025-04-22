#include "game.h"
#include "astar.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      comp_snake(grid_width, grid_height, true),
      grid_width(grid_width),
      grid_height(grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
  PlaceObstacles();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 last_time = SDL_GetTicks();
  float lag = 0.0f;
  const float MS_PER_UPDATE = static_cast<float>(target_frame_duration);

  int frame_count = 0;
  Uint32 title_timestamp = SDL_GetTicks();
  bool running = true;

  while (running)
  {
    Uint32 current = SDL_GetTicks();
    float elapsed = static_cast<float>(current - last_time);
    last_time = current;
    lag += elapsed;

    // Handle input once per frame
    controller.HandleInput(running, snake);

    // Update game logic in fixed-size steps
    while (lag >= MS_PER_UPDATE)
    {
      Update(); // Logic update
      lag -= MS_PER_UPDATE;
    }

    // Render the current state (can interpolate if desired)
    renderer.Render(snake, comp_snake, food, obstacles);
    frame_count++;

    // Update window title once per second
    if ((SDL_GetTicks() - title_timestamp) >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = SDL_GetTicks();
    }

    SDL_Delay(1); // avoid 100% CPU usage
  }
}

/*
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
    renderer.Render(snake, comp_snake, food, obstacles);

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
*/

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Make sure the location is not occupied by the snake
    if (!snake.SnakeCell(x, y)) 
      break;
  }
  // Random position
  food.position = {x, y};

  // Random type
  int type = rand() % 3;
  food.type = static_cast<FoodType>(type);
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

  std::set<SDL_Point> occupied;
  for (auto const &point : snake.body) occupied.insert(point);
  for (auto const &obs : obstacles) occupied.insert(obs.GetPosition());
  
  auto path = AStar({static_cast<int>(comp_snake.head_x), static_cast<int>(comp_snake.head_y)},
                    food.position,
                    grid_width, grid_height, occupied);
  comp_snake.SetPath(path);
  
  snake.Update();
  comp_snake.Update();
  
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food at the current head position
  if (food.position.x == new_x && food.position.y == new_y) {
    switch (food.type) {
      case FoodType::Regular:
        score += 1;
        snake.GrowBody();
        break;
      case FoodType::Bonus:
        score += 5;
        snake.GrowBody();
        snake.GrowBody(); // grow extra
        break;
      case FoodType::SpeedUp:
        score += 2;
        snake.speed += 0.05;
        break;
    }
    PlaceFood();  // Place new food
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
