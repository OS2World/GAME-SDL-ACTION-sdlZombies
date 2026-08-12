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



#include "penguin.h"
#include "global.h"
#include "sdl_util.h"




static struct
{
	SDL_Rect rect;
	SDL_Surface * img_src;
} tab_dir[DIR_MAXDIR][MAX_STEP];




static void init_tab_dir(int dir, int step, 
				int x1, int y1, int x2, int y2, int num_img)
{
	tab_dir[dir][step].rect.x = x1; tab_dir[dir][step].rect.y = y1;
	tab_dir[dir][step].rect.w = x2-x1+1; tab_dir[dir][step].rect.h = y2-y1+1;
	tab_dir[dir][step].img_src = images[num_img];
}



void InitPenguin(Penguin_t * peng, SDL_Rect * coord_rect, int pspeed,
		int plim_scroll_x, int plim_scroll_y)
{
	peng->x = 30; peng->y = 30;

	peng->dx = 0; peng->dy = 0;

	peng->speed = pspeed;
	peng->step = Random(0, MAX_STEP - 1);
	peng->dir = Random(0, DIR_START - 1);

	peng->w = 10; peng->h = 10;

	peng->lim_scroll_x = plim_scroll_x;
	peng->lim_scroll_y = plim_scroll_y;

	peng->coord_rect = coord_rect;

	/* Right */
	init_tab_dir(DIR_R, 0, 0, 0, 25, 29, IMG_PINGUS_STOP);
	init_tab_dir(DIR_R, 1, 0, 0, 32, 29, IMG_PINGUS);
	init_tab_dir(DIR_R, 2, 0, 0, 25, 29, IMG_PINGUS_STOP);
	init_tab_dir(DIR_R, 3, 0, 33, 32, 62, IMG_PINGUS);

	/* Left */
	init_tab_dir(DIR_L, 0, 31, 0, 56, 29, IMG_PINGUS_STOP);
	init_tab_dir(DIR_L, 1, 75, 0, 107, 29, IMG_PINGUS);
	init_tab_dir(DIR_L, 2, 31, 0, 56, 29, IMG_PINGUS_STOP);
	init_tab_dir(DIR_L, 3, 76, 32, 109, 61, IMG_PINGUS);
	
	/* Up */
	init_tab_dir(DIR_U, 0, 63, 2, 92, 27, IMG_PINGUS_STOP);
	init_tab_dir(DIR_U, 1, 115, 0, 144, 33, IMG_PINGUS);
	init_tab_dir(DIR_U, 2, 63, 2, 92, 27, IMG_PINGUS_STOP);
	init_tab_dir(DIR_U, 3, 115, 36, 144, 68, IMG_PINGUS);
	
	/* Down */
	init_tab_dir(DIR_D, 0, 99, 2, 128, 27, IMG_PINGUS_STOP);
	init_tab_dir(DIR_D, 1, 40, 0, 69, 32, IMG_PINGUS);
	init_tab_dir(DIR_D, 2, 99, 2, 128, 27, IMG_PINGUS_STOP);
	init_tab_dir(DIR_D, 3, 40, 34, 69, 66, IMG_PINGUS);

	/* Right Up */
	init_tab_dir(DIR_RU, 0, 0, 0, 26, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_RU, 1, 0, 0, 39, 35, IMG_PINGUS_45);
	init_tab_dir(DIR_RU, 2, 0, 0, 26, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_RU, 3, 0, 39, 35, 78, IMG_PINGUS_45);
	
	/* Right Down */
	init_tab_dir(DIR_RD, 0, 32, 0, 57, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_RD, 1, 44, 0, 79, 39, IMG_PINGUS_45);
	init_tab_dir(DIR_RD, 2, 32, 0, 57, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_RD, 3, 43, 42, 82, 77, IMG_PINGUS_45);
	
	/* Left Up */
	init_tab_dir(DIR_LU, 0, 95, 0, 121, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_LU, 1, 130, 0, 165, 39, IMG_PINGUS_45);
	init_tab_dir(DIR_LU, 2, 95, 0, 121, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_LU, 3, 134, 45, 173, 80, IMG_PINGUS_45);
	
	/* Left Down */
	init_tab_dir(DIR_LD, 0, 65, 0, 91, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_LD, 1, 83, 0, 122, 35, IMG_PINGUS_45);
	init_tab_dir(DIR_LD, 2, 65, 0, 91, 26, IMG_PINGUS_45_STOP);
	init_tab_dir(DIR_LD, 3, 89, 38, 124, 77, IMG_PINGUS_45);
}


void SetCoordPenguin(Penguin_t * peng)
{
	peng->x = Random(0, peng->coord_rect->w);
	peng->y = Random(0, peng->coord_rect->h);

	peng->coord_rect->x = peng->x - screen->width / 2;
	peng->coord_rect->y = peng->y - screen->height / 2;

	if (peng->coord_rect->x < 0) peng->coord_rect->x = 0;
	
	if (peng->coord_rect->x + screen->width > peng->coord_rect->w)
		peng->coord_rect->x = peng->coord_rect->w - screen->width;
		
	if (peng->coord_rect->y < 0) peng->coord_rect->y = 0;
	
	if (peng->coord_rect->y + screen->height > peng->coord_rect->h)
		peng->coord_rect->y = peng->coord_rect->h - screen->height;
}


void PutPenguin(Penguin_t * peng)
{
	SDL_Rect drect;
	SDL_Rect tb = tab_dir[peng->dir][peng->step].rect;
	SDL_Surface * img = tab_dir[peng->dir][peng->step].img_src;
	
	drect.x = peng->x - tb.w / 2 - peng->coord_rect->x; 
	drect.y = peng->y - tb.h / 2 - peng->coord_rect->y;
	drect.w = tb.w; drect.h = tb.h;

	SDL_BlitSurface(img, &tb, screen->vscreen, &drect);
}

void MovePenguin(Penguin_t * peng)
{
	if (peng->dx == 0 && peng->dy == 0 && peng->mv == 0) return;

	peng->step += 1; if (peng->step >= MAX_STEP) peng->step = 0;

	if (peng->dx > 0) {	/* Right */
		peng->dir = DIR_R;
		peng->x += peng->speed;

		if (peng->x > peng->coord_rect->w) peng->x = peng->coord_rect->w;

		if (peng->x - peng->coord_rect->x > screen->width - peng->lim_scroll_x) {
			peng->coord_rect->x += peng->speed;
			if (peng->coord_rect->x > peng->coord_rect->w - screen->width)
				peng->coord_rect->x = peng->coord_rect->w - screen->width;
		}
	}

	if (peng->dx < 0) {	/* Left */
		peng->dir = DIR_L;
		peng->x -= peng->speed;

		if (peng->x < 0) peng->x = 0;

		if (peng->x - peng->coord_rect->x < peng->lim_scroll_x) {
			peng->coord_rect->x -= peng->speed;
			if (peng->coord_rect->x < 0) peng->coord_rect->x = 0;
		}
	}

	if (peng->dy < 0) {	/* Up */
		peng->dir = DIR_U;
		peng->y -= peng->speed;

		if (peng->y < 0) peng->y = 0;

		if (peng->y - peng->coord_rect->y < peng->lim_scroll_y) {
			peng->coord_rect->y -= peng->speed;
			if (peng->coord_rect->y < 0) peng->coord_rect->y = 0;
		}
	}

	if (peng->dy > 0) {	/* Down */
		peng->dir = DIR_D;
		peng->y += peng->speed;

		if (peng->y > peng->coord_rect->h) peng->y = peng->coord_rect->h;

		if (peng->y - peng->coord_rect->y > screen->height - peng->lim_scroll_y) {
			peng->coord_rect->y += peng->speed;
			if (peng->coord_rect->y > peng->coord_rect->h - screen->height)
				peng->coord_rect->y = peng->coord_rect->h - screen->height;
		}
	}

	if (peng->dx > 0 && peng->dy > 0) peng->dir = DIR_RD;	/* Right Down */
	if (peng->dx > 0 && peng->dy < 0) peng->dir = DIR_RU;	/* Right Up */
	if (peng->dx < 0 && peng->dy > 0) peng->dir = DIR_LD;	/* Left Down */
	if (peng->dx < 0 && peng->dy < 0) peng->dir = DIR_LU;	/* Left Up */
}


/* return 1 if penguin fall in hole */
int PenguinFallInHole(Penguin_t * peng, Hole_t * hole)
{
	if (peng->x - peng->w <= hole->x + hole->w &&
		peng->x + peng->w >= hole->x - hole->w &&
		peng->y - peng->h <= hole->y + hole->h &&
		peng->y + peng->h >= hole->y - hole->h ) return 1;

	return 0;
}


/* return 1 if penguin is eat by a zombies */
int ZombiesEatPenguin(Penguin_t * peng, Zombies_t * zomb)
{
	if ( zomb->state != ZOMBIS_RUN ) return 0;

	if (peng->x - peng->w <= zomb->x + zomb->w &&
		peng->x + peng->w >= zomb->x - zomb->w &&
		peng->y - peng->h <= zomb->y + zomb->h &&
		peng->y + peng->h >= zomb->y - zomb->h ) return 1;

	return 0;
}
