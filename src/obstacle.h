#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"

class Obstacle {
public:
    Obstacle(int x, int y, bool moving = false);
    
    // Update obstacle position (for moving obstacles)
    void Update();
    
    // Get current position
    SDL_Point GetPosition() const;
    
    // Check if obstacle is moving
    bool IsMoving() const;

private:
    SDL_Point position;
    bool moving;
    int direction = 1;  // 1 for right, -1 for left
};

#endif // OBSTACLE_H