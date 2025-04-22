#include "obstacle.h"

Obstacle::Obstacle(int x, int y, bool moving) : moving(moving) {
    position.x = x;
    position.y = y;
}

void Obstacle::Update() {
    if (moving) {
        position.x += direction;
        if (position.x < 0 || position.x >= 32) {
            direction *= -1;
        }
    }
}

SDL_Point Obstacle::GetPosition() const {
    return position;
}

bool Obstacle::IsMoving() const {
    return moving;
}