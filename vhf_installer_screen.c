#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL.h"
#include "SDL_ttf.h"

#define TEXT_MARGIN 40
#define FONT "res/sans.ttf"
#define FONTSIZE 24

void get_text(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Color textColor,
        SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

int main(int argc, char *argv[])
{
    SDL_DisplayMode displaymode;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Texture *texture1, *texture2;
    SDL_Rect textbox1, textbox2;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (SDL_GetCurrentDisplayMode(0, &displaymode) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get displaymode: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer(displaymode.w, displaymode.h, 0, &window, &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_ShowCursor(SDL_DISABLE);

    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF library: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    TTF_Font* font = TTF_OpenFont(FONT, FONTSIZE);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load font: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Color textColorHeader = {255, 0, 0, 0};
    SDL_Color textColor = {255, 255, 255, 0};

    get_text(renderer, TEXT_MARGIN, TEXT_MARGIN, "VirtualHereFree installer by milaq", font, textColorHeader, &texture1, &textbox1);
    get_text(renderer, TEXT_MARGIN, textbox1.y + 2 * textbox1.h, "Installing, please wait...", font, textColor, &texture2, &textbox2);

    SDL_FreeSurface(surface);

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture1, NULL, &textbox1);
        SDL_RenderCopy(renderer, texture2, NULL, &textbox2);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
