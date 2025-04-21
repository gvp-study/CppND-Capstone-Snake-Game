#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"
#include <vector>
#include "SDL.h"
#include "snake.h"

class Obstacle;
enum class FoodType;      // Forward declare FoodType
struct Food;              // Forward declare Food

class Renderer {
 public:
 Renderer(const std::size_t screen_width, const std::size_t screen_height,
  const std::size_t grid_width, const std::size_t grid_height);
~Renderer();

//void Render(Snake const snake, SDL_Point const &food);
void Render(Snake const &snake, Snake const &ai_snake, Food const &food, std::vector<Obstacle> const &obstacles);
void UpdateWindowTitle(int score, int fps);
  SDL_Renderer* GetRenderer(){ return sdl_renderer;}

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif