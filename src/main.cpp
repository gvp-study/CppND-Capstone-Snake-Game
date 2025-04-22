#include <iostream>
#include "controller.h"
#include "renderer.h"
#include "game.h"
#include "dialog.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  
  // Get the user name from a dialog box
  Dialog dialog(&renderer, game.GetScore(), game.GetSize());
  dialog.show();
    
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Name: " << dialog.GetName() << "\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}
