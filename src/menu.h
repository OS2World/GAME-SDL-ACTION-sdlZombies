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

#ifndef __MENU_H
#define __MENU_H

#include "sdl_util.h"

typedef enum { MENU_NONE, MENU_KEYBOARD, MENU_MOUSE } MenuValid_t;

typedef struct {
	SDL_Surface * image;			/* Image when item not selected */
	SDL_Surface * image_select;		/* Image when item is selected */
									/* Both images must be the same size */
	SDL_Rect pos;					/* Coords of the item : set with (x,y) 
										in AddMenuItem */

	/* intern */
	SDL_Surface * background;		/* Background of the image */
} MenuItem_t;


typedef struct {
	SDL_Surface * screen;			/* Screen for drawing */
	SDL_Surface * image_back;		/* Background image */
	int nb_item;					/* Number of items */
	MenuItem_t * item;				/* Array of items */

	int cur;						/* Curent Item */
	MenuValid_t type_valid;			/* Type of validation keyboard/mouse */
} Menu_t;


extern int InitMenu(Menu_t * menu, SDL_Surface * pscreen, int pnb_item,
				SDL_Surface * pimage_back);
extern void CloseMenu(Menu_t * menu);

extern void SetMenuItem(Menu_t * menu, int num, SDL_Surface * pimage,
				SDL_Surface * pimage_select, int x, int y);

extern int ActiveMenu(Menu_t * menu);

#endif /* __MENU_H */
