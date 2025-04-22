#include "renderer.h"
#include "game.h"
#include <iostream>
#include <string>


Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const &snake, 
                      Snake const &comp_snake, 
                      Food const &food, 
                      std::vector<Obstacle> const &obstacles) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  // Set color based on food type
  switch (food.type) {
    case FoodType::Regular:
      SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);  // white
      break;
    case FoodType::Bonus:
      SDL_SetRenderDrawColor(sdl_renderer, 255, 215, 0, 255);    // gold
      break;
    case FoodType::SpeedUp:
      SDL_SetRenderDrawColor(sdl_renderer, 0, 255, 0, 255);      // green
      break;
  }
  block.x = food.position.x * block.w;
  block.y = food.position.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render obstacles
  SDL_SetRenderDrawColor(sdl_renderer, 0x88, 0x88, 0x00, 0xFF);
  for(auto const &obs : obstacles)
  {
    block.x = obs.GetPosition().x * block.w;
    block.y = obs.GetPosition().y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render player snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Draw AI snake (blue)
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0xFF, 0xFF);
  for (SDL_Point const &point : comp_snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
  // Render comp_snake's head
  block.x = static_cast<int>(comp_snake.head_x) * block.w;
  block.y = static_cast<int>(comp_snake.head_y) * block.h;
  if (comp_snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
