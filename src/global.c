/***************************************************************************
                        SdlZombies - Simple zombies game
                             -------------------
    begin                : Sun Apr 15 16:55:07 CEST 2001
    copyright            : (C) 2001 by Philippe Brochard
    email                : hocwp@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sdl_util.h"
#include "global.h"

SdlScreen_t* screen = NULL;    /* Changed from SDL_Surface* to SdlScreen_t* */

SDL_Keycode key_quit = SDLK_ESCAPE;

int use_sound = 1;
int sound_volume = 2;
int use_map = 1;
int use_keyboard = 1;
int fullscreen = 0;

static const char * images_names[NUM_IMAGES] = {
    /* Letters and Numbers */
    SRC_DIR "data/status/letters.png",
    SRC_DIR "data/status/letters_small.png",
    SRC_DIR "data/status/letters_small_2.png",
    SRC_DIR "data/status/numbers-0.png",
    SRC_DIR "data/status/numbers-1.png",
    SRC_DIR "data/status/numbers_small.png",
    SRC_DIR "data/status/numbers_small_2.png",

    /* Penguin_t */
    SRC_DIR "data/penguin/penguin.png",
    SRC_DIR "data/penguin/penguin_stop.png",
    SRC_DIR "data/penguin/penguin_45.png",
    SRC_DIR "data/penguin/penguin_45_stop.png",

    /* Zombies_t */
    SRC_DIR "data/zombies/zombies.png",
    SRC_DIR "data/zombies/zombies_stop.png",
    SRC_DIR "data/zombies/zombies_45.png",
    SRC_DIR "data/zombies/zombies_45_stop.png",
    SRC_DIR "data/zombies/zombies_start.png",

    /* Backgrounds */
    SRC_DIR "data/background/back_1.png",
    SRC_DIR "data/background/back_2.png",
    SRC_DIR "data/background/back_3.png",
    SRC_DIR "data/background/back_4.png",
    SRC_DIR "data/background/back_5.png",
    SRC_DIR "data/background/back_6.png",
    SRC_DIR "data/background/back_7.png",
    SRC_DIR "data/background/back_8.png",
    SRC_DIR "data/background/back_9.png",
    SRC_DIR "data/background/back_10.png",

    /* Environment */
    SRC_DIR "data/environ/hole.png",
    SRC_DIR "data/environ/map.png",
    SRC_DIR "data/environ/map_2.png",
    SRC_DIR "data/environ/hole_small.png",
    SRC_DIR "data/environ/penguin_small.png",
    SRC_DIR "data/environ/zombies_small.png",

    /* Main Menu */
    SRC_DIR "data/menu/back.png",
    SRC_DIR "data/menu/title.png",
    SRC_DIR "data/menu/play.png",
    SRC_DIR "data/menu/play_s.png",
    SRC_DIR "data/menu/option.png",
    SRC_DIR "data/menu/option_s.png",
    SRC_DIR "data/menu/quit.png",
    SRC_DIR "data/menu/quit_s.png",

    /* Menu Options */
    SRC_DIR "data/menu_opt/level.png",
    SRC_DIR "data/menu_opt/level_s.png",

    SRC_DIR "data/menu_opt/no_sound.png",
    SRC_DIR "data/menu_opt/no_sound_s.png",
    SRC_DIR "data/menu_opt/low_sound.png",
    SRC_DIR "data/menu_opt/low_sound_s.png",
    SRC_DIR "data/menu_opt/med_sound.png",
    SRC_DIR "data/menu_opt/med_sound_s.png",
    SRC_DIR "data/menu_opt/hig_sound.png",
    SRC_DIR "data/menu_opt/hig_sound_s.png",

    SRC_DIR "data/menu_opt/no_map.png",
    SRC_DIR "data/menu_opt/no_map_s.png",
    SRC_DIR "data/menu_opt/map_trans.png",
    SRC_DIR "data/menu_opt/map_trans_s.png",
    SRC_DIR "data/menu_opt/map_und.png",
    SRC_DIR "data/menu_opt/map_und_s.png",
    SRC_DIR "data/menu_opt/map_ov.png",
    SRC_DIR "data/menu_opt/map_ov_s.png",

    SRC_DIR "data/menu_opt/fullscreen.png",
    SRC_DIR "data/menu_opt/fullscreen_s.png",
    SRC_DIR "data/menu_opt/window.png",
    SRC_DIR "data/menu_opt/window_s.png",

    SRC_DIR "data/menu_opt/quit.png",
    SRC_DIR "data/menu_opt/quit_s.png",


    /* Nothing */
    SRC_DIR "data/background/carre.png"
};

SDL_Surface * images[NUM_IMAGES];

#ifndef NOSOUND
Mix_Chunk * sounds[NUM_SOUNDS];
#endif

static const char * sounds_names[NUM_SOUNDS] = {
    SRC_DIR "data/sounds/menu_val.wav",
    SRC_DIR "data/sounds/menu.wav",

    SRC_DIR "data/sounds/intro.wav",
    SRC_DIR "data/sounds/final.wav",
    SRC_DIR "data/sounds/eat.wav",
    SRC_DIR "data/sounds/fall.wav",
    SRC_DIR "data/sounds/evil.wav",
    SRC_DIR "data/sounds/arise.wav"
};

static void LoadImages(void);
static void LoadSounds(void);

void InitGlobalVar(void)
{
    key_quit = SDLK_ESCAPE;

    LoadImages();
    LoadSounds();
}

void CloseGlobalVar(void)
{
    int i;

    for(i = 0; i < NUM_IMAGES; i++)
    {
        if (images[i] != NULL) SDL_FreeSurface(images[i]);
        images[i] = NULL;
    }

#ifndef NOSOUND
    for(i = 0; i < NUM_SOUNDS; i++)
    {
        if (sounds[i] != NULL) Mix_FreeChunk(sounds[i]);
    }
#endif
}

/* Sound section */

void PlaySound(int snd)
{
#ifndef NOSOUND
    if (use_sound == 1) {
        Mix_PlayChannel(-1, sounds[snd], 0);
    }
#endif
}

void HaltSound(void)
{
#ifndef NOSOUND
    if (use_sound == 1) {
        Mix_HaltChannel(-1);
    }
#endif
}

void SetSoundVolume(int volume)
{
#ifndef NOSOUND
    if (use_sound == 1) {
        Mix_Volume(-1, volume * MIX_MAX_VOLUME / 3);
    }
#endif
}

/* Load Section */

static SDL_Surface* LoadImageFromFile(const char *name)
{
    SDL_Surface *image;

    image = IMG_Load(name);
    if (image == NULL) {
        fprintf(stderr,
            "\nError: I couldn't load a graphics file:\n"
            "%s\n"
            "SDL error: %s\n\n", name, SDL_GetError());
        exit(1);
    }

    SDL_SetColorKey(image, SDL_TRUE,
        SDL_MapRGB(image->format, 0x80, 0x80, 0x80));

    return image;
}

static void LoadImages(void)
{
    int i;

    for(i = 0; i < NUM_IMAGES; i++)
        images[i] = LoadImageFromFile(images_names[i]);
}

static void LoadSounds(void)
{
    int i;

#ifndef NOSOUND
    if (use_sound == 1)
    {
        /* Load sounds: */
        for (i = 0; i < NUM_SOUNDS; i++)
        {
            sounds[i] = Mix_LoadWAV(sounds_names[i]);
            if (sounds[i] == NULL)
            {
                fprintf(stderr,
                    "\nError: I could not load the sound file:\n"
                    "%s\n"
                    "The Simple DirectMedia error that occured was:\n"
                    "%s\n\n", sounds_names[i], SDL_GetError());
                exit(1);
            }
        }
    }
#endif
}