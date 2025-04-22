#include "astar.h"

struct Node
{
    SDL_Point point;
    int g, h;
    Node *parent;
    int f() const { return g + h; }
};

std::vector<SDL_Point> ReconstructPath(Node *node)
{
    std::vector<SDL_Point> path;
    while (node)
    {
        path.push_back(node->point);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}


std::vector<SDL_Point> AStar(SDL_Point start, SDL_Point goal,
                             int grid_width, int grid_height,
                             const std::set<SDL_Point> &obstacles)
{
    std::priority_queue<std::pair<int, SDL_Point>,
                        std::vector<std::pair<int, SDL_Point>>,
                        std::greater<>>
        frontier;
    frontier.push({0, start});

    std::map<SDL_Point, SDL_Point> came_from;
    std::map<SDL_Point, int> cost_so_far;
    came_from[start] = start;
    cost_so_far[start] = 0;

    const std::vector<SDL_Point> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!frontier.empty())
    {
        SDL_Point current = frontier.top().second;
        frontier.pop();

        if (current.x == goal.x && current.y == goal.y)
            break;

        for (const auto &dir : directions)
        {
            SDL_Point next{current.x + dir.x, current.y + dir.y};

            // Bounds check
            if (next.x < 0 || next.y < 0 || next.x >= grid_width || next.y >= grid_height)
                continue;

            // Obstacle check
            if (obstacles.find(next) != obstacles.end())
                continue;

            int new_cost = cost_so_far[current] + 1;

            if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next])
            {
                cost_so_far[next] = new_cost;
                int priority = new_cost + Heuristic(next, goal);
                frontier.push({priority, next});
                came_from[next] = current;
            }
        }
    }

    // Reconstruct path
    std::vector<SDL_Point> path;
    SDL_Point current = goal;
    while (current.x != start.x || current.y != start.y)
    {
        path.push_back(current);
        current = came_from[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
