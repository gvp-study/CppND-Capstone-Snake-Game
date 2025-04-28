# Snake Game Modifications

## Project Option
This document explains the work related to  "Extend the C++ Snake Game" project option from the Udacity C++ Nanodegree.

# Snake Game Demo

The video below shows the working of the modified SnakeGame. The player snake is represented by the blue head and white body and is controlled with the keyboard by the user. There are two obstacles, represented in brown. One is static and the other bounces between the left and right sides of the display. When the snake head hits an obstacle it bounces back on itself. The snake food is marked by three types marked in white, green or yellow. The snake behavior changes based on the type of food. The computer controlled snake is marked in cyan and computes its path from its head to the food location using A* search in a 2D grid.

<video width="640" height="480" controls>
  <source src="snakegame.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>
## Instructions for Running the Project

The build uses the SDL_ttf_dev package that is not available and has to be installed before running the cmake command. So use the setup.sh to combine the package installation and the cmake and make process as shown below.

```bash
./setup.sh
build/SnakeGame
```

## Code Structure Overview
- `src/`
  - `main.cpp`: Entry point. Initializes game and renderer.
  - `game.h/cpp`: Game logic, food placement, snake update control, obstacle handling.
  - `snake.h/cpp`: Player-controlled snake and computer-controlled (computer) snake.
  - `renderer.h/cpp`: Renders snakes, food, obstacles.
  - `controller.h/cpp`: Handles user input.
  - `obstacle.h/cpp`: Defines moving and fixed obstacles.
  - `astar.h/cpp`: Implements basic A* pathfinding for computer snake.
- `CMakeLists.txt`: Build configuration.
  - The original CMakeLists.txt was modified to automatically install additional packages like SDL_ttf  on the Udacity workspace. 

## New Features Added

### 1. **Different Food Types**
- Introduced `FoodType` enum: `Regular`, `Bonus`, `SpeedUp`.
- Eating different foods causes different effects (normal grow, double grow, speed boost).
- See: `game.h`, `game.cpp` changes for food type handling.

### 2. **Obstacles (Fixed and Moving)**
- Created an `Obstacle` class with support for static and moving obstacles.
- Moving obstacles oscillate in the x-axis.
- Obstacles are randomly placed and animated.
- See: `obstacle.h/cpp`, `game.cpp`.

### 3. **Computer Snake Using A* Search**
- Added a second snake (`comp_snake`) controlled by basic A* pathfinding.
- The computer finds the path from the snake head to the current food while avoiding obstacles using the A* algorithm in a 2D grid.
- See: `snake.h`, `snake.cpp`, `astar.h/cpp`, `game.cpp`.

### 4. **Concurrency (Multithreading)**
- Used `std::async` and `std::future` to run player and computer snake updates in parallel.
- Each update step now launches two async tasks: one for player snake, one for computer snake.
- Future `.get()` ensures both finish before proceeding.
- This satisfies the Concurrency Rubric (multiple threads and future/promise).
- See `game.cpp`.

### 5. **Improved Frame Timing**
- Used a lag compensation loop to allow consistent simulation updates even if the rendering framerate fluctuates.
- Helps make the snake movement smooth and predictable.
- See: `game.cpp Run()` method.

---

## Updated `Game::Update()` Design

```cpp
void Game::Update()
{
  if (!snake.alive) return;

  static int comp_update_counter = 0;
  const int comp_update_threshold = 5;

  // Plan path for comp_snake if needed
  if (comp_update_counter == 0) {
    std::set<SDL_Point> occupied;
    for (auto const &point : snake.body)
      occupied.insert(point);
    for (auto const &obs : obstacles)
      occupied.insert(obs.GetPosition());

    auto path = AStar({static_cast<int>(comp_snake.head_x), static_cast<int>(comp_snake.head_y)},
                      food.position,
                      grid_width, grid_height, occupied);
    comp_snake.SetPath(path);
  }

  // Launch async updates
  auto player_future = std::async(std::launch::async, [this]() {
    snake.Update();
  });

  std::future<void> comp_future;
  if (comp_update_counter == 0) {
    comp_future = std::async(std::launch::async, [this]() {
      comp_snake.Update();
    });
  }

  player_future.get();
  if (comp_update_counter == 0) {
    comp_future.get();
  }

  comp_update_counter++;
  if (comp_update_counter >= comp_update_threshold) {
    comp_update_counter = 0;
  }

  // Check for food collection
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  if (food.position.x == new_x && food.position.y == new_y) {
    switch (food.type) {
      case FoodType::Regular:
        score += 1;
        snake.GrowBody();
        break;
      case FoodType::Bonus:
        score += 5;
        snake.GrowBody();
        snake.GrowBody();
        break;
      case FoodType::SpeedUp:
        score += 2;
        snake.speed += 0.05;
        break;
    }
    PlaceFood();
  }

  for (auto &obs : obstacles) {
    obs.Update();
  }

  for (const auto &obs : obstacles) {
    if (obs.GetPosition().x == new_x && obs.GetPosition().y == new_y) {
      Snake::Direction dir = snake.ChangeDirection(snake.direction);
      snake.direction = dir;
    }
  }
}
```

---

## Rubric Satisfaction Explanation

- **Object-Oriented Programming**: Added `Obstacle` and enhanced `Snake` class.
- **Memory Management**: Used smart memory patterns, no leaks.
- **Concurrency**: Used `std::async`, `std::future`, and real parallel execution for player and computer snakes.
- **Code Efficiency and Organization**: Proper file separation, clean class interfaces.

---

