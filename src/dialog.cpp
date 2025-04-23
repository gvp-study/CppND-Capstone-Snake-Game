#include "dialog.h"
#include <iostream>
#include <fstream>

std::string GetSystemFontPath() {
    #ifdef __linux__
        return "/usr/share/fonts/truetype/arial.ttf";
    #elif __APPLE__
        return "../assets/fonts/Arial.ttf";
    #else
        return "";  // Fallback
    #endif
}
Dialog::Dialog(Renderer* renderer, int score, int ss)
    : renderer(renderer->GetRenderer()), score(score), size(ss) {
    //std::string fontPath = "../assets/fonts/Arial.ttf";
    std::string fontPath = GetSystemFontPath();
    if (!std::ifstream(fontPath)) {
        std::cerr << "Font not found at: " << fontPath << std::endl;
    }
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
    }
    
    font = TTF_OpenFont(fontPath.c_str(), 24);
    if (!font) {
        std::cerr << "Failed to load font! " << fontPath << std::endl;
    }
}

Dialog::~Dialog() {
    if (font) {
        TTF_CloseFont(font);
    }
}

void Dialog::show() {
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

std::string Dialog::GetName() {
    return name;
}