/***************************************************************************
                        SdlZombies - Simple zombies game
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdl_util.h"
#include "global.h"

#ifndef VERSION
#define VERSION "1.0.0"
#endif


static void Usage(char *name)
{
    fprintf(stderr, "SdlZombies %s\n", VERSION);
    fprintf(stderr, "Usage: %s [Options]\n", name);
    fprintf(stderr, "  -f,  --fullscreen \t enable fullscreen\n");
    fprintf(stderr, "  -w,  --window \t enable window\n");
    fprintf(stderr, "  -ns, --nosound \t disable sounds\n");
    fprintf(stderr, "  -s,  --sound \t\t enable sounds\n");
    exit(1);
}


SdlScreen_t* Sdl_Init(int argc, char **argv)
{
    SdlScreen_t *screen;
    Uint32 win_flags = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    while (argc > 1) {
        --argc;
        if (strcmp(argv[argc], "--nosound") == 0 ||
                strcmp(argv[argc], "-ns") == 0) {
            printf("* Disable sound\n");
        } else
        if (strcmp(argv[argc], "--sound") == 0 ||
                strcmp(argv[argc], "-s") == 0) {
            printf("* Enable sound\n");
        } else
        if (strcmp(argv[argc], "-f") == 0 ||
                strcmp(argv[argc], "--fullscreen") == 0) {
            printf("* Enable fullscreen\n");
            fullscreen = 1;
        } else
        if (strcmp(argv[argc], "-w") == 0 ||
                strcmp(argv[argc], "--window") == 0) {
            printf("* Enable window\n");
            fullscreen = 0;
        } else {
            Usage(argv[0]);
        }
    }

    if (fullscreen)
        win_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;

    screen = (SdlScreen_t*)malloc(sizeof(SdlScreen_t));
    if (!screen) {
        fprintf(stderr, "Failed to allocate screen\n");
        exit(1);
    }

    /* Window is 1.5× the logical game size; renderer scales to fill it */
    screen->window = SDL_CreateWindow("SdlZombies",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        MAXX * 3 / 2, MAXY * 3 / 2, win_flags);
    if (!screen->window) {
        fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
        free(screen);
        exit(2);
    }

    screen->renderer = SDL_CreateRenderer(screen->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!screen->renderer) {
        /* Fall back to software renderer */
        screen->renderer = SDL_CreateRenderer(screen->window, -1, 0);
    }
    if (!screen->renderer) {
        fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(screen->window);
        free(screen);
        exit(2);
    }

    /* All game logic targets MAXX×MAXY; renderer scales to the larger window */
    SDL_RenderSetLogicalSize(screen->renderer, MAXX, MAXY);

    /* Virtual 640x480 drawing surface — all game blits go here */
    screen->vscreen = SDL_CreateRGBSurface(0, MAXX, MAXY, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!screen->vscreen) {
        fprintf(stderr, "Couldn't create vscreen: %s\n", SDL_GetError());
        SDL_DestroyRenderer(screen->renderer);
        SDL_DestroyWindow(screen->window);
        free(screen);
        exit(2);
    }

    screen->width  = MAXX;
    screen->height = MAXY;

    SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
    SDL_RenderClear(screen->renderer);
    SDL_RenderPresent(screen->renderer);

#ifndef NOSOUND
    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) < 0)
        fprintf(stderr, "Warning: couldn't open audio: %s\n", SDL_GetError());
#endif

    return screen;
}


void Sdl_Close(SdlScreen_t *screen)
{
    if (screen) {
        if (screen->vscreen)
            SDL_FreeSurface(screen->vscreen);
        SDL_DestroyRenderer(screen->renderer);
        SDL_DestroyWindow(screen->window);
        free(screen);
    }

#ifndef NOSOUND
    Mix_CloseAudio();
#endif

    SDL_Quit();
}


/* Upload vscreen to renderer and present — replaces SDL_Flip */
void Sdl_Flip(SdlScreen_t *screen)
{
    SDL_Texture *tex;

    tex = SDL_CreateTextureFromSurface(screen->renderer, screen->vscreen);
    if (tex) {
        SDL_RenderCopy(screen->renderer, tex, NULL, NULL);
        SDL_RenderPresent(screen->renderer);
        SDL_DestroyTexture(tex);
    }
}


void Sdl_SwapFullScreen(SdlScreen_t *screen)
{
    fullscreen = !fullscreen;
    SDL_SetWindowFullscreen(screen->window,
        fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}


void Sdl_WaitNoEvent(void)
{
    SDL_Event event;
    int done;

    do {
        done = 0;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:         done = 1; break;
                case SDL_MOUSEBUTTONDOWN: done = 1; break;
            }
        }
    } while (done == 1);
}


int Sdl_Pause(void)
{
    int done = SDL_EVT_NULL;
    SDL_Event event;

    while (done == SDL_EVT_NULL) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    done = SDL_EVT_EXIT;
                    break;
                case SDL_KEYDOWN:
                    if (Sdl_HandleGlobalKey(screen, &event.key)) break;
                    done = SDL_EVT_VALID;
                    if (event.key.keysym.sym == key_quit)
                        done = SDL_EVT_QUIT;
                    break;
                case SDL_MOUSEBUTTONUP:
                    done = SDL_EVT_VALID;
                    break;
            }
        }
        SDL_Delay(10);
    }

    return done;
}


/* Draw a single pixel onto vscreen */
void Sdl_PutPixel(SdlScreen_t *screen, int x, int y,
        Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_Rect rect = { x, y, 1, 1 };
    SDL_FillRect(screen->vscreen, &rect,
        SDL_MapRGB(screen->vscreen->format, red, green, blue));
}


/* Blit source surface onto vscreen at (x, y) */
void Sdl_PutSurface(SdlScreen_t *screen, int x, int y,
        SDL_Surface *source)
{
    SDL_Rect dstrect = { x, y, source->w, source->h };
    SDL_BlitSurface(source, NULL, screen->vscreen, &dstrect);
}


int Sdl_XYOnRect(int x, int y, SDL_Rect *rect)
{
    if (x >= rect->x && y >= rect->y &&
            x <= rect->x + rect->w && y <= rect->y + rect->h)
        return 1;
    return 0;
}


int Sdl_HandleGlobalKey(SdlScreen_t *screen, SDL_KeyboardEvent *key)
{
    if (key->keysym.sym == SDLK_RETURN && (key->keysym.mod & KMOD_ALT)) {
        Sdl_SwapFullScreen(screen);
        return 1;
    }
    return 0;
}


void Sdl_WarpMouse(SdlScreen_t *screen, int x, int y)
{
    int win_w, win_h;
    SDL_GetWindowSize(screen->window, &win_w, &win_h);
    SDL_WarpMouseInWindow(screen->window,
        x * win_w / screen->width,
        y * win_h / screen->height);
}


void ComplainAndExit(char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}


void InitRandom(void)
{
    int i;

    srand((unsigned int)SDL_GetTicks());

    for (i = 0; i < Random(0, 1000); i++)
        Random(0, 1000);
}

int Random(int rmin, int rmax)
{
    return rmin + (int)(1.0 * (rmax - rmin + 1) * rand() / (RAND_MAX + 1.0));
}
