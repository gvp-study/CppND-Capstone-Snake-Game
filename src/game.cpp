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
      random_w(1, static_cast<int>(grid_width - 2)),
      random_h(1, static_cast<int>(grid_height - 2))
{
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

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
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

void Game::PlaceObstacles()
{
  int x, y;
  int NObstacles = 2;
  int count = 0;
  static bool first = false;

  while (count++ < NObstacles && !first)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check if the location is occupied by a snake item before placing
    if (!snake.SnakeCell(x, y))
    {
      obstacles.emplace_back(x, y, count % 2 == 0 ? false : true);
    }
  }
  first = true;
}


#include <future>  // Make sure to include this

void Game::Update()
{
  if (!snake.alive) return;

  // Slow computer snake update only every 5 frames
  static int comp_update_counter = 0; 
  const int comp_update_threshold = 5;

  // Prepare path for computer snake first
  std::set<SDL_Point> occupied;
  for (auto const &point : snake.body)
    occupied.insert(point);
  for (auto const &obs : obstacles)
    occupied.insert(obs.GetPosition());
  // Wait for counter to reach threshold to update comp_snake
  if(comp_update_counter == 0) {
    // Finds the path as a vector of points from current location to the food
    auto path = AStar({static_cast<int>(comp_snake.head_x), static_cast<int>(comp_snake.head_y)},
                      food.position,
                      grid_width, grid_height, occupied);
    comp_snake.SetPath(path);
  }
     
  // Launch async task with the user snake.Update using lambda functions
  auto future = std::async(std::launch::async, [this]() {
    snake.Update();
  });
  // Launch async task with the computer snake.Update only if counter hits 0
  std::future<void> comp_future;
  if (comp_update_counter == 0) {
      comp_future = std::async(std::launch::async, [this]() {
        comp_snake.Update();
      });
  }

  // Wait for both Update()s to complete
  future.get();
  // Only Update() the computer snake when counter hits 0
  if (comp_update_counter == 0) {
    comp_future.get(); 
  }

  // Update counter
  comp_update_counter++;
  if (comp_update_counter >= comp_update_threshold) {
    comp_update_counter = 0;
  }

  // Put food only where the snake head is not
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  if (food.position.x == new_x && food.position.y == new_y) {
    switch (food.type) {
      // Grow at regular rate
      case FoodType::Regular:
        score += 1;
        snake.GrowBody();
        break;
      // Grow at double rate
      case FoodType::Bonus:
        score += 5;
        snake.GrowBody();
        snake.GrowBody();
        break;
      // Speed up
      case FoodType::SpeedUp:
        score += 2;
        snake.speed += 0.05;
        break;
    }
    // Place new food in random position    
    PlaceFood();
  }

  // Update any moving obstacles
  for (auto &obs : obstacles) {
    obs.Update();
  }

  // Check snake head with obstacles and reverse if they collide
  for (const auto &obs : obstacles) {
    if (obs.GetPosition().x == new_x && obs.GetPosition().y == new_y) {
      Snake::Direction dir = snake.ChangeDirection(snake.direction);
      snake.direction = dir;
    }
  }
}
/*
void Game::Update()
{
  if (!snake.alive)
    return;

  std::set<SDL_Point> occupied;
  for (auto const &point : snake.body)
    occupied.insert(point);
  for (auto const &obs : obstacles)
    occupied.insert(obs.GetPosition());
  // Finds the path as a vector of points from current location to the food
  auto path = AStar({static_cast<int>(comp_snake.head_x), static_cast<int>(comp_snake.head_y)},
                    food.position,
                    grid_width, grid_height, occupied);
  comp_snake.SetPath(path);
  // Updates the snake based on the user input
  snake.Update();
  // Updates the snakes head to the next point in the astar path
  comp_snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food at the current head position
  if (food.position.x == new_x && food.position.y == new_y)
  {
    switch (food.type)
    {
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
    // Place new food in random position
    PlaceFood(); 
  }

  // Update any moving obstacles
  for (auto &obs : obstacles)  {
    obs.Update();
  }  
  
  // Check snake head with obstacles and reverse if they collide
  for (const auto &obs : obstacles)
  {
    if (obs.GetPosition().x == new_x && obs.GetPosition().y == new_y)
    {
      Snake::Direction dir = snake.ChangeDirection(snake.direction);
      snake.direction = dir;
    }
  }
}
  */