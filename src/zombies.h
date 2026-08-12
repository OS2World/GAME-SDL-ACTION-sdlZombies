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


#ifndef __ZOMBIS_H
#define __ZOMBIS_H


#include "sdl_util.h"
#include "hole.h"


typedef enum { ZOMBIS_DEAD, ZOMBIS_HIDE, 
			ZOMBIS_START, ZOMBIS_RUN } Zombies_State_t;

typedef struct
{
	int x, y;					/* Coords */
	int dx, dy;					/* -1,0,1 : left . right or up . down */
	int mv;						/* if zombies have moved */
	
	/* intern but can be change */
	int step, dir;				/* direction */
	int speed;				/* speed (in pixel) */

	int w, h;					/* half width and height for collision */

	Zombies_State_t state;		/* Zombies State : dead, hide, start, run  */ 

	Uint32 time_now, time_show;	/* time before zombies is show */
	Uint32 time_start;			/* time before zombies start */
	
	SDL_Rect * coord_rect;		/* background coord rectangle */
} Zombies_t;

extern void InitZombies(Zombies_t * zombies, SDL_Rect * coord_rect, Uint32 time,
		Uint32 time_start, int pspeed_min, int pspeed_max);
		
extern void SetTimeStartZombies(Zombies_t * zombies);

extern void PutZombies(Zombies_t * zombies);

extern void MoveZombies(Zombies_t * zombies);

extern int ZombiesFallInHole(Zombies_t * zomb, Hole_t * hole);

#endif /* __ZOMBIS_H */
