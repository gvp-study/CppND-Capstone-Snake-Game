#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <SDL.h>
#include <set>
#include <queue>
#include <map>
#include <cmath>

inline int Heuristic(SDL_Point a, SDL_Point b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

inline bool ComparePoints(SDL_Point a, SDL_Point b) {
  return (a.x == b.x) && (a.y == b.y);
}

inline bool operator<(const SDL_Point &a, const SDL_Point &b) {
    return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
}
  
std::vector<SDL_Point> AStar(SDL_Point start, SDL_Point goal,
                             int grid_width, int grid_height,
                             const std::set<SDL_Point> &obstacles);

#endif
