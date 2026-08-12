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



#include "zombies.h"
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



void InitZombies(Zombies_t * zomb, SDL_Rect * coord_rect, Uint32 time,
		Uint32 time_start, int pspeed_min, int pspeed_max)
{
	zomb->x = Random(0, coord_rect->w); 
	zomb->y = Random(0, coord_rect->h);

	zomb->dx = 0; zomb->dy = 0;

	zomb->speed = Random(pspeed_min, pspeed_max); 
	zomb->step = Random(0, MAX_STEP - 1);
	zomb->dir = Random(0, DIR_START - 1);

	zomb->w = 10; zomb->h = 10;

	zomb->state = ZOMBIS_HIDE;

	zomb->time_now = SDL_GetTicks();
	zomb->time_show = time;
	zomb->time_start = time_start;

	zomb->coord_rect = coord_rect;

	/* Right */
	init_tab_dir(DIR_R, 0, 0, 0, 25, 29, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_R, 1, 0, 0, 32, 29, IMG_ZOMBIS);
	init_tab_dir(DIR_R, 2, 0, 0, 25, 29, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_R, 3, 0, 33, 32, 62, IMG_ZOMBIS);

	/* Left */
	init_tab_dir(DIR_L, 0, 31, 0, 56, 29, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_L, 1, 75, 0, 107, 29, IMG_ZOMBIS);
	init_tab_dir(DIR_L, 2, 31, 0, 56, 29, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_L, 3, 76, 32, 109, 61, IMG_ZOMBIS);
	
	/* Up */
	init_tab_dir(DIR_U, 0, 63, 2, 92, 27, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_U, 1, 115, 0, 144, 33, IMG_ZOMBIS);
	init_tab_dir(DIR_U, 2, 63, 2, 92, 27, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_U, 3, 115, 36, 144, 68, IMG_ZOMBIS);
	
	/* Down */
	init_tab_dir(DIR_D, 0, 99, 2, 128, 27, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_D, 1, 40, 0, 69, 32, IMG_ZOMBIS);
	init_tab_dir(DIR_D, 2, 99, 2, 128, 27, IMG_ZOMBIS_STOP);
	init_tab_dir(DIR_D, 3, 40, 34, 69, 66, IMG_ZOMBIS);

	/* Right Up */
	init_tab_dir(DIR_RU, 0, 0, 0, 26, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_RU, 1, 0, 0, 39, 35, IMG_ZOMBIS_45);
	init_tab_dir(DIR_RU, 2, 0, 0, 26, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_RU, 3, 0, 39, 35, 78, IMG_ZOMBIS_45);
	
	/* Right Down */
	init_tab_dir(DIR_RD, 0, 32, 0, 57, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_RD, 1, 44, 0, 79, 39, IMG_ZOMBIS_45);
	init_tab_dir(DIR_RD, 2, 32, 0, 57, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_RD, 3, 43, 42, 82, 77, IMG_ZOMBIS_45);
	
	/* Left Up */
	init_tab_dir(DIR_LU, 0, 95, 0, 121, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_LU, 1, 130, 0, 165, 39, IMG_ZOMBIS_45);
	init_tab_dir(DIR_LU, 2, 95, 0, 121, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_LU, 3, 134, 45, 173, 80, IMG_ZOMBIS_45);
	
	/* Left Down */
	init_tab_dir(DIR_LD, 0, 65, 0, 91, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_LD, 1, 83, 0, 122, 35, IMG_ZOMBIS_45);
	init_tab_dir(DIR_LD, 2, 65, 0, 91, 26, IMG_ZOMBIS_45_STOP);
	init_tab_dir(DIR_LD, 3, 89, 38, 124, 77, IMG_ZOMBIS_45);

	/* Start */
	init_tab_dir(DIR_START, 0, 0, 0, 11, 9, IMG_ZOMBIS_START);
	init_tab_dir(DIR_START, 1, 13, 0, 30, 14, IMG_ZOMBIS_START);
	init_tab_dir(DIR_START, 2, 35, 0, 59, 22, IMG_ZOMBIS_START);
	init_tab_dir(DIR_START, 3, 35, 0, 59, 22, IMG_ZOMBIS_START); /* not used */
}



void SetTimeStartZombies(Zombies_t * zomb)
{
	zomb->time_now = SDL_GetTicks();
}



void PutZombies(Zombies_t * zomb)
{
	SDL_Rect drect;
	SDL_Rect tb = tab_dir[zomb->dir][zomb->step].rect;
	SDL_Surface * img = tab_dir[zomb->dir][zomb->step].img_src;

	if (zomb->state == ZOMBIS_HIDE || zomb->state == ZOMBIS_DEAD) return;
	
	drect.x = zomb->x - tb.w / 2 - zomb->coord_rect->x; 
	drect.y = zomb->y - tb.h / 2 - zomb->coord_rect->y;
	drect.w = tb.w; drect.h = tb.h;

	SDL_BlitSurface(img, &tb, screen->vscreen, &drect);
}

void MoveZombies(Zombies_t * zomb)
{
	if (zomb->state == ZOMBIS_DEAD) return;

	if (zomb->state == ZOMBIS_HIDE && 
		SDL_GetTicks() - zomb->time_now > zomb->time_show)
	{
		zomb->state = ZOMBIS_START;
		zomb->dir = DIR_START; zomb->step = 0;
		PlaySound(SND_ARISE);
	}
	if (zomb->state == ZOMBIS_START && 
		SDL_GetTicks() - zomb->time_now > zomb->time_show+zomb->time_start)
	{
		zomb->state = ZOMBIS_START;
		zomb->dir = DIR_START; zomb->step = 1;
	}
	if (zomb->state == ZOMBIS_START && 
		SDL_GetTicks() - zomb->time_now > zomb->time_show+zomb->time_start * 2)
	{
		zomb->state = ZOMBIS_START;
		zomb->dir = DIR_START; zomb->step = 2;
	}
	if (zomb->state == ZOMBIS_START && 
		SDL_GetTicks() - zomb->time_now > zomb->time_show+zomb->time_start * 3)
	{
		zomb->state = ZOMBIS_RUN;
	}

	if (zomb->state != ZOMBIS_RUN) return;

	if (zomb->dx == 0 && zomb->dy == 0 && zomb->mv == 0) return;

	zomb->step += 1;
	if (zomb->step >= MAX_STEP) zomb->step = 0;

	if (zomb->dx > 0) {	/* Right */
		zomb->dir = DIR_R;
		zomb->x += zomb->speed;

		if (zomb->x > zomb->coord_rect->w) zomb->x = zomb->coord_rect->w;
	}

	if (zomb->dx < 0) {	/* Left */
		zomb->dir = DIR_L;
		zomb->x -= zomb->speed;

		if (zomb->x < 0) zomb->x = 0;
	}

	if (zomb->dy < 0) {	/* Up */
		zomb->dir = DIR_U;
		zomb->y -= zomb->speed;

		if (zomb->y < 0) zomb->y = 0;
	}

	if (zomb->dy > 0) {	/* Down */
		zomb->dir = DIR_D;
		zomb->y += zomb->speed;

		if (zomb->y > zomb->coord_rect->h) zomb->y = zomb->coord_rect->h;
	}

	if (zomb->dx > 0 && zomb->dy > 0) zomb->dir = DIR_RD;	/* Right Down */
	if (zomb->dx > 0 && zomb->dy < 0) zomb->dir = DIR_RU;	/* Right Up */
	if (zomb->dx < 0 && zomb->dy > 0) zomb->dir = DIR_LD;	/* Left Down */
	if (zomb->dx < 0 && zomb->dy < 0) zomb->dir = DIR_LU;	/* Left Up */
}



/* return 1 if zombies fall in hole */
int ZombiesFallInHole(Zombies_t * zomb, Hole_t * hole)
{
	if (zomb->state == ZOMBIS_HIDE || zomb->state == ZOMBIS_DEAD) return 0;

	if (zomb->x - zomb->w <= hole->x + hole->w &&
		zomb->x + zomb->w >= hole->x - hole->w &&
		zomb->y - zomb->h <= hole->y + hole->h &&
		zomb->y + zomb->h >= hole->y - hole->h ) 
		return 1;

	return 0;
}
