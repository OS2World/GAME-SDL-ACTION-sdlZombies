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

#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "sdl_util.h" /* Updated to include sdl_util.h instead of SDL.h directly */
#ifndef NOSOUND
#include <SDL2/SDL_mixer.h>
#endif

extern void InitGlobalVar(void);
extern void CloseGlobalVar(void);

extern void PlaySound(int num);
extern void HaltSound(void);
extern void SetSoundVolume(int use_sound);

/* External screen variable - updated to SdlScreen_t */
extern SdlScreen_t* screen;   /* Changed from SDL_Surface* to SdlScreen_t* */

extern SDL_Keycode key_quit;

extern int use_sound;
extern int sound_volume;
extern int use_map;
extern int use_keyboard;
extern int fullscreen;

/* Directions definitions */
enum {
    DIR_R, DIR_L, DIR_U, DIR_D,
    DIR_RU, DIR_RD, DIR_LU, DIR_LD,
    DIR_START,
    DIR_MAXDIR
};

#define MAX_STEP 4

/* Images definitions */
enum {
    /* Letters and Numbers */
    IMG_LETTER, IMG_LETTER_SMALL, IMG_LETTER_SMALL_2,
    IMG_NUMBER_0, IMG_NUMBER_1,
    IMG_NUMBER_SMALL, IMG_NUMBER_SMALL_2,

    /* Penguin_t */
    IMG_PINGUS, IMG_PINGUS_STOP,
    IMG_PINGUS_45, IMG_PINGUS_45_STOP,

    /* Zombies_t */
    IMG_ZOMBIS, IMG_ZOMBIS_STOP,
    IMG_ZOMBIS_45, IMG_ZOMBIS_45_STOP,
    IMG_ZOMBIS_START,

    /* Backgrounds */
    IMG_BACK_1, IMG_BACK_2, IMG_BACK_3,
    IMG_BACK_4, IMG_BACK_5, IMG_BACK_6,
    IMG_BACK_7, IMG_BACK_8, IMG_BACK_9,
    IMG_BACK_10,

    /* Environment */
    IMG_HOLE,
    IMG_MAP, IMG_MAP_2,
    IMG_HOLE_SMALL, IMG_PINGUS_SMALL, IMG_ZOMBIS_SMALL,

    /* Main Menu */
    IMG_MENU_BACK, IMG_MENU_TITLE,
    IMG_MENU_PLAY, IMG_MENU_PLAY_S,
    IMG_MENU_OPTION, IMG_MENU_OPTION_S,
    IMG_MENU_QUIT, IMG_MENU_QUIT_S,

    /* Menu options */
    IMG_MENU_OPT_LEVEL, IMG_MENU_OPT_LEVEL_S,

    IMG_MENU_OPT_NO_SOUND, IMG_MENU_OPT_NO_SOUND_S,
    IMG_MENU_OPT_LOW_SOUND, IMG_MENU_OPT_LOW_SOUND_S,
    IMG_MENU_OPT_MED_SOUND, IMG_MENU_OPT_MED_SOUND_S,
    IMG_MENU_OPT_HIG_SOUND, IMG_MENU_OPT_HIG_SOUND_S,

    IMG_MENU_OPT_NO_MAP, IMG_MENU_OPT_NO_MAP_S,
    IMG_MENU_OPT_TRANS_MAP, IMG_MENU_OPT_TRANS_MAP_S,
    IMG_MENU_OPT_UNDER_MAP, IMG_MENU_OPT_UNDER_MAP_S,
    IMG_MENU_OPT_OVER_MAP, IMG_MENU_OPT_OVER_MAP_S,

    IMG_MENU_OPT_FULL, IMG_MENU_OPT_FULL_S,
    IMG_MENU_OPT_WINDOW, IMG_MENU_OPT_WINDOW_S,

    IMG_MENU_OPT_QUIT, IMG_MENU_OPT_QUIT_S,

    IMG_CARRE,

    /* This must be the last definition */
    NUM_IMAGES
};

extern SDL_Surface* images[NUM_IMAGES];

/* Sounds definitions */
enum {
    SND_MENU_VAL, SND_MENU,

    SND_INTRO, SND_FINAL,

    SND_EAT,
    SND_FALL,
    SND_EVIL,
    SND_ARISE,

    /* This must be the last definition */
    NUM_SOUNDS
};

#ifndef NOSOUND
extern Mix_Chunk * sounds[NUM_SOUNDS];
#endif

#endif  /* __GLOBAL_H */