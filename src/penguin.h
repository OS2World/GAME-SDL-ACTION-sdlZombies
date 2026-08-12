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


#ifndef __PINGUS_H
#define __PINGUS_H


#include "sdl_util.h"
#include "hole.h"
#include "zombies.h"

typedef struct
{
	int x, y;					/* Coords */
	int dx, dy;					/* -1,0,1 : left . right or up . down */
	int mv;						/* if penguin have moved */
	
	/* intern but can be change */
	int step, dir;				/* direction */
	int speed;				/* speed (in pixel) */

	int w, h;					/* half width and height for collision */

	int lim_scroll_x, lim_scroll_y;		/* Limite for scrolling the screen */
	
	SDL_Rect * coord_rect;		/* background coord rectangle */
} Penguin_t;

extern void InitPenguin(Penguin_t * peng, SDL_Rect * coord_rect, int pspeed, 
		int plim_scroll_x, int p_lim_scroll_y);

extern void SetCoordPenguin(Penguin_t * peng);

extern void PutPenguin(Penguin_t * penguin);

extern void MovePenguin(Penguin_t * penguin);

/* return 1 if penguin fall in hole */
extern int PenguinFallInHole(Penguin_t * peng, Hole_t * hole);

/* return 1 if penguin is eat by a zombies */
extern int ZombiesEatPenguin(Penguin_t * peng, Zombies_t * zomb);

#endif /* __PINGUS_H */
