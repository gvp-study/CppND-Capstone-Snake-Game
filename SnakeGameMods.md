# Snake Game Modifications

This document outlines the significant changes made to the original Snake Game implementation.

## Major New Features

1. **Computer Opponent Snake**
   - Added a second Computer-controlled snake that competes with the player
   - Uses A* pathfinding algorithm to navigate towards food
   - Rendered in blue to distinguish from player snake

2. **Enhanced Food System**
   - Three food types with different effects:
     - `Regular`: +1 score, grows snake by 1 segment. Colored white.
     - `Bonus`: +5 score, grows snake by 2 segments. Colored yellow.
     - `SpeedUp`: +2 score, increases snake speed. Colored green.

3. **Obstacles**
   - Added stationary and moving obstacles
   - Obstacles block both player and Computer snakes
   - Moving obstacles bounce back when hitting the obstacles.

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

## Technical Improvements
- Added A* pathfinding algorithm for Computer snake
- Better collision detection system
- More sophisticated game state management
- Improved memory management with smart pointers
- Added error handling for font loading

## Gameplay Changes
- Competitive element with Computer opponent
- Strategic decisions about which food to target
- Need to navigate around obstacles
- Higher risk/reward with different food types