#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#include "SDL.h"
#include "SDL_ttf.h"

#define TEXT_MARGIN 40
#define FONT "res/sans.ttf"
#define FONTSIZE 24

#define INSTALL_DOWNLOAD 1
#define INSTALL_COPY 2
#define INSTALL_DONE 3

static int installStep = 0;
static int installError = 0;
static int installAbortSignal = 0;

void getSDLText(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Color textColor,
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

void *installThread(void *vargp) {
    int ret;
    sleep(2);

    installStep = INSTALL_DOWNLOAD;
    ret = 0;
    ret += system("mkdir -p /mnt/config/overlay/usr/local/bin/");
    ret += system("wget https://virtualhere.com/sites/default/files/usbserver/vhusbdarm -O /mnt/config/overlay/usr/local/bin/vhusbdarm");
    ret += system("chmod 755 /mnt/config/overlay/usr/local/bin/vhusbdarm");
    if (ret > 0) {
        installError = INSTALL_DOWNLOAD;
        sleep(4);
        installAbortSignal = 1;
        return;
    }

    installStep = INSTALL_COPY;
    ret = 0;
    ret += system("mkdir -p /mnt/config/overlay/etc/init.d/startup/");
    ret += system("cp res/S98virtualherefree /mnt/config/overlay/etc/init.d/startup/S98virtualherefree");
    ret += system("mkdir -p /mnt/config/overlay/usr/local/bin/");
    ret += system("cp res/vhfwatchdog /mnt/config/overlay/usr/local/bin/vhfwatchdog");
    ret += system("[[ -f /mnt/config/system/virtualherefree_config.ini ]] || cp res/virtualherefree_config.ini /mnt/config/system/virtualherefree_config.ini");
    ret += system("touch /mnt/config/system/virtualherefree_enabled.txt");
    if (ret > 0) {
        installError = INSTALL_COPY;
        sleep(4);
        installAbortSignal = 1;
        return;
    }

    installStep = INSTALL_DONE;
    sleep(4);
    system("/etc/init.d/reboot.sh");
}

int main(int argc, char *argv[]) {
    SDL_DisplayMode displaymode;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Texture *texture_header, *texture_download, *texture_copy, *texture_done, *texture_dl_failed, *texture_copy_failed;
    SDL_Rect textbox_header, textbox_download, textbox_copy, textbox_done, textbox_dl_failed, textbox_copy_failed;
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

    SDL_Color textColorHeader = {120, 120, 120, 0};
    SDL_Color textColorError = {255, 0, 0, 0};
    SDL_Color textColor = {255, 255, 255, 0};

    getSDLText(renderer, TEXT_MARGIN, TEXT_MARGIN, "VirtualHereFree installer by milaq", font, textColorHeader, &texture_header, &textbox_header);
    getSDLText(renderer, TEXT_MARGIN, textbox_header.y + 2 * textbox_header.h, "Downloading VirtualHere binary...", font, textColor, &texture_download, &textbox_download);
    getSDLText(renderer, TEXT_MARGIN, textbox_download.y + textbox_download.h, "Installing service...", font, textColor, &texture_copy, &textbox_copy);

    getSDLText(renderer, TEXT_MARGIN, textbox_copy.y + 2 * textbox_copy.h, "All done. Rebooting...", font, textColor, &texture_done, &textbox_done);
    getSDLText(renderer, TEXT_MARGIN, textbox_copy.y + 2 * textbox_copy.h, "Download failed.", font, textColorError, &texture_dl_failed, &textbox_dl_failed);
    getSDLText(renderer, TEXT_MARGIN, textbox_copy.y + 2 * textbox_copy.h, "Service installation failed.", font, textColorError, &texture_copy_failed, &textbox_copy_failed);

    SDL_FreeSurface(surface);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, installThread, NULL);

    while (!installAbortSignal) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture_header, NULL, &textbox_header);

        if (installStep >= INSTALL_DOWNLOAD) {
            SDL_RenderCopy(renderer, texture_download, NULL, &textbox_download);
        }
        if (installStep >= INSTALL_COPY) {
            SDL_RenderCopy(renderer, texture_copy, NULL, &textbox_copy);
        }

        if (installError == INSTALL_DOWNLOAD) {
            SDL_RenderCopy(renderer, texture_dl_failed, NULL, &textbox_dl_failed);
        } else if (installError == INSTALL_COPY) {
            SDL_RenderCopy(renderer, texture_copy_failed, NULL, &textbox_copy_failed);
        } else if (installStep >= INSTALL_DONE) {
            SDL_RenderCopy(renderer, texture_done, NULL, &textbox_done);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture_header);
    SDL_DestroyTexture(texture_download);
    SDL_DestroyTexture(texture_copy);
    SDL_DestroyTexture(texture_done);
    SDL_DestroyTexture(texture_dl_failed);
    SDL_DestroyTexture(texture_copy_failed);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
