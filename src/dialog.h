#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "renderer.h"

class Dialog {
public:
    Dialog(Renderer* renderer, int score, int ss);
    ~Dialog();
    
    void show();
    std::string GetName();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    int score;
    int size;
    std::string name;

    // Constants (could also be made static class members)
    static const int WINDOW_WIDTH = 400;
    static const int WINDOW_HEIGHT = 200;
    static const int TEXTBOX_X = 50;
    static const int TEXTBOX_Y = 80;
    static const int TEXTBOX_WIDTH = 300;
    static const int TEXTBOX_HEIGHT = 40;
    static const int SCORE_Y = 140;
};

#endif // DIALOG_H