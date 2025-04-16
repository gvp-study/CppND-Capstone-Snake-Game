#include <iostream>
#include <fstream>
#include "controller.h"
#include "renderer.h"
#include "game.h"
#include <SDL_ttf.h>

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 200;
const int TEXTBOX_X = 50;
const int TEXTBOX_Y = 80;
const int TEXTBOX_WIDTH = 300;
const int TEXTBOX_HEIGHT = 40;
const int SCORE_Y = 140;

class Dialog {
public:
    Dialog(Renderer* renderer, int score, int ss)
        : renderer(renderer->GetRenderer()), score(score), size(ss)
        {
            std::string fontPath = "../assets/fonts/Arial.ttf";
            if (!std::ifstream(fontPath)) {
              std::cerr << "Font not found at: " << fontPath << std::endl;
            }
            if (TTF_Init() == -1) {
              std::cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
            }
          
            font = TTF_OpenFont(fontPath.c_str(), 24);
            if (!font) {
                std::cerr << "Failed to load font!" << " ../assets/fonts/Arial.ttf" << std::endl;
            }
        }
    
    void show() {
        SDL_StartTextInput();
        std::string inputText = "";
        SDL_Event e;
        bool quit = false;
        
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_TEXTINPUT) {
                    inputText += e.text.text;
                } else if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_BACKSPACE && !inputText.empty()) {
                        inputText.pop_back();
                    } else if (e.key.keysym.sym == SDLK_RETURN) {
                        quit = true;
                    }
                }
            }
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            
            // Draw textbox background
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_Rect textboxRect = {TEXTBOX_X, TEXTBOX_Y, TEXTBOX_WIDTH, TEXTBOX_HEIGHT};
            SDL_RenderFillRect(renderer, &textboxRect);
            
            // Draw textbox border
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &textboxRect);
            
            // Render text inside textbox
            SDL_Color textColor = {0, 0, 0, 255};
            std::string displayText = inputText.empty() ? " " : inputText;
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, displayText.c_str(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            
            SDL_Rect textRect = {TEXTBOX_X + 5, TEXTBOX_Y + 10, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            
            // Render score below the textbox
            std::string scoreText = "Score: " + std::to_string(score);
            scoreText += " Size: " + std::to_string(size);
            SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            
            SDL_Rect scoreRect = {TEXTBOX_X, SCORE_Y, scoreSurface->w, scoreSurface->h};
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
            
            SDL_FreeSurface(scoreSurface);
            SDL_DestroyTexture(scoreTexture);
            
            SDL_RenderPresent(renderer);
        }
        SDL_StopTextInput();
        std::cout << "Name: " << inputText << " | Score: " << score << " | Size: " << size << std::endl;
        name = std::string(inputText);
    }
    std::string GetName() {return name;}
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    int score;
    int size;
    std::string name;
};


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
  //
  // Get the user name from a dialog box.
  //
  Dialog dialog(&renderer, game.GetScore(), game.GetSize());
  dialog.show();
    
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Name: " << dialog.GetName() << "\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}