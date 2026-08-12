/***************************************************************************
                        SdlZombies - Simple zombies game
 ***************************************************************************/

#ifndef __SDLUTIL_H
#define __SDLUTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#ifndef NOSOUND
#include <SDL2/SDL_mixer.h>
#endif

#ifndef SRC_DIR
#define SRC_DIR ""
#endif

/* SDL2 screen: window + renderer + virtual surface (640x480 drawing target) */
typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Surface  *vscreen;   /* virtual drawing surface — SDL_Flip uploads this */
    int width;
    int height;
} SdlScreen_t;

#define MAXX	640
#define MAXY	480

#define SDL_EVT_QUIT	-1
#define SDL_EVT_EXIT	-2
#define SDL_EVT_NULL	-3
#define SDL_EVT_VALID	-4

extern SdlScreen_t* Sdl_Init(int argc, char **argv);
extern void Sdl_Close(SdlScreen_t *screen);
extern void Sdl_Flip(SdlScreen_t *screen);
extern void Sdl_SwapFullScreen(SdlScreen_t *screen);
extern void Sdl_WaitNoEvent(void);
extern int  Sdl_Pause(void);

extern void Sdl_PutPixel(SdlScreen_t *screen, int x, int y,
        Uint8 red, Uint8 green, Uint8 blue);

/* Blit SDL_Surface onto screen->vscreen at (x,y) */
extern void Sdl_PutSurface(SdlScreen_t *screen, int x, int y,
        SDL_Surface *source);

extern int  Sdl_XYOnRect(int x, int y, SDL_Rect *rect);

/* Warp mouse to logical (game) coordinates, scaling to physical window size */
extern void Sdl_WarpMouse(SdlScreen_t *screen, int x, int y);

/* Call from every SDL_KEYDOWN handler: intercepts global shortcuts (Alt+Enter
   toggles fullscreen).  Returns 1 if the key was consumed, 0 otherwise. */
extern int Sdl_HandleGlobalKey(SdlScreen_t *screen, SDL_KeyboardEvent *key);

extern void ComplainAndExit(char *msg);

extern void InitRandom(void);
extern int  Random(int min, int max);

#endif
