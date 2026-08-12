
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


#ifndef __OPTION_H
#define __OPTION_H

#include "sdl_util.h"

typedef struct
{
	int nb_life;			/* life number of player */
	int nb_zombies_ND;		/* number of zombies Not Dead */
	int level, nb_level;	/* level and total level of the game */
} Status_t;

typedef struct
{
	Status_t status;

	int bgnd_w, bgnd_h;			/* size of the background (width and height) */
	int nb_zombies;				/* number of zombies */
	int nb_hole;				/* number of hole */

	int peng_speed;				/* Speed of the penguin */
	int lim_scroll_x, lim_scroll_y;		/* Limite for scrolling the screen */

	int zomb_speed_min, zomb_speed_max;	/* Speed min/max of zombies (x, y) */

	Uint32 time_show;			/* base time before zombies is show  */
	Uint32 time_inter;			/* interval before each zombies is show */
	Uint32 time_start;			/* time before zombies start */

	Uint32 game_speed;			/* Time to wait before the screen flip */
} Option_t;



extern void InitStartStopOptions(Option_t * opt_beg, Option_t * opt_end);

extern void SetOptions(Option_t *opt_beg, Option_t *opt_end, Option_t *opt);
extern void ShowOptionsText(Option_t *opt);
extern int ShowOptions(Option_t *opt,	SDL_Rect * coord_rect,
		SDL_Surface * background);

extern int MenuSetOptions(Option_t * opt_beg, Option_t * opt_end);


#endif /* __OPTION_H */
