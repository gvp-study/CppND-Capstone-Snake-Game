# Snake Game Modifications

This document outlines the significant changes made to the original Snake Game implementation.

# Snake Game Demo

<video width="640" height="480" controls>
  <source src="snakegame.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

## Major New Features

1. **Computer Opponent Snake**
   - Added a second computer-controlled snake that competes with the player
   - Uses A* pathfinding algorithm to navigate towards food
   - A* uses the Eucleidian distance as the heuristic
   - Rendered computer snake in cyan to distinguish from player snake in blue
   
2. **Enhanced Food System**
   - Three food types with different effects:
     - `Regular`: +1 score, grows snake by 1 segment. Colored white.
     - `Bonus`: +5 score, grows snake by 2 segments. Colored yellow.
     - `SpeedUp`: +2 score, increases snake speed. Colored green.

3. **Obstacles**
   - Added stationary and moving obstacles
   - Both player and Computer snakes bounce when hitting obstacles and reverse their movement
   - Moving obstacles bounce back when hitting the sides

4. **Game Over Dialog**
   - New dialog box appears when game ends
   - Shows final score and snake size
   - Allows player to enter their name
   - Uses SDL_ttf for text rendering

5. **Improved Rendering**
   - Different colors for different food types
   - Visual distinction between player and Computer snakes
   - Obstacles rendered in distinct color

## Code Changes Breakdown

### Game Class
- Added `comp_snake` member variable
- Added `obstacles` vector and `PlaceObstacles()` method
- Enhanced `PlaceFood()` to create different food types
- Modified `Update()` to handle:
  - Computer snake movement via pathfinding
  - Food type effects
  - Obstacle collisions
  - New scoring system

### Snake Class

- Added Computer-specific functionality:
  - `SetPath()` for pathfinding directions
  - `MoveTowardsTarget()` for Computer movement
  - `is_Computer` flag to distinguish Computer snakes
- Added `ChangeDirection()` helper method

### Renderer Class
- Updated to render:
  - Computer snake (blue)
  - Different food types (white, gold, green)
  - Obstacles (yellow-brown)
- Added support for SDL_ttf text rendering

### New Classes
- `Obstacle`: Handles obstacle properties and movement
- `Food`: Enhanced food system with types
- `Dialog`: Game over dialog with text input
- AStar: Smart search from current snake head location to the food using astar algorithm in 2D grid.

## Technical Improvements
- Added A* pathfinding algorithm for Computer snake
- Better collision detection system
- More sophisticated game state management
- Improved memory management with smart pointers
- Added error handling for font loading

## Possible Gameplay Changes
- Competitive element with Computer opponent
- Strategic decisions about which food to target
- Need to navigate around obstacles
- Higher risk/reward with different food types