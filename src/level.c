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



#include <stdio.h>
#include <stdlib.h>
#include "level.h"
#include "global.h"
#include "sdl_util.h"
#include "penguin.h"
#include "zombies.h"
#include "hole.h"
#include "option.h"
#include "draw_text.h"


static SDL_Surface * InitBackground(int w, int h);
static Zombies_t * InitTabZombies(int nb_zombies, SDL_Rect * coord_rect,
			Uint32 time_show, Uint32 time_inter, Uint32 time_start,
			int pspeed_min, int pspeed_max);
static Hole_t * InitTabHole(int nb_hole, SDL_Rect * coord_rect,
			SDL_Surface * background);


static void SetCoordPenguin_ZombiesHole(Penguin_t * penguin, Zombies_t * zombies,
				int nb_zombies, Hole_t * hole, int nb_hole);

static int AnalyseMainEvent(SDL_Rect * coord_rect, Penguin_t * penguin,
			Zombies_t * zombies, int nb_zombies);

static void AnalyseKeyBoard(Penguin_t *penguin);
static void MoveWithMouse(Penguin_t * penguin, SDL_Rect * coord_rect);

/* Move all zombies; return 1 if a zombies eat penguin */
static int MoveAllZombies(Zombies_t * zombies, int nb_zombies, Penguin_t *penguin);

/* Test if zombies or penguin fall in a hole; return 1 if penguin fall */
static int TestFallInHole(Penguin_t * penguin, Zombies_t * zombies, int nb_zombies, 
				Hole_t * hole, int nb_hole, Status_t *status);

static int TestZombiesAllDead(Zombies_t * zombies, int nb_zombies);

static void PutStatus(Status_t *status, int nb_zombies);
static void PutMap(SDL_Rect * coord_rect, Penguin_t * penguin, 
				Zombies_t * zombies, int nb_zombies, 
				Hole_t * hole, int nb_hole, int num_map);
static void DrawScreen(SDL_Surface *background, SDL_Rect * coord_rect,
				Penguin_t * penguin, Zombies_t * zombies, int nb_zombies,
				Hole_t * hole, int nb_hole, Status_t * status, int show_peng);




/*	Main function	*/
int RunLevel(Option_t * opt)
{
	int done;
	SDL_Surface * background;
	SDL_Rect coord_rect;
	Penguin_t penguin;
	Zombies_t * zombies;
	Hole_t * hole;
	int i;

	coord_rect.x = 0; coord_rect.y = 0;
	coord_rect.w = opt->bgnd_w; coord_rect.h = opt->bgnd_h;

	background = InitBackground(opt->bgnd_w, opt->bgnd_h);

	InitPenguin(& penguin, &coord_rect, opt->peng_speed,
		opt->lim_scroll_x, opt->lim_scroll_y);

	zombies = InitTabZombies(opt->nb_zombies, & coord_rect, opt->time_show,
				opt->time_inter, opt->time_start, opt->zomb_speed_min,
				opt->zomb_speed_max);

	hole = InitTabHole(opt->nb_hole, & coord_rect, background);
	
	SetCoordPenguin_ZombiesHole(&penguin, zombies, opt->nb_zombies, hole,
		opt->nb_hole);

	done = ShowOptions(opt, &coord_rect, background);
	if (done != SDL_EVT_VALID) return done;

	for(i = 0; i < opt->nb_zombies; i++)
		SetTimeStartZombies(& zombies[i]);

	/* hide mouse cursor if in keyboard mode */
	switch(use_keyboard)
	{
		case 0 : SDL_ShowCursor(1);
			Sdl_WarpMouse(screen,
				penguin.x - coord_rect.x, penguin.y - coord_rect.y);
		break;
		case 1 : SDL_ShowCursor(0); break;
	}

	/* main loop */
	done = SDL_EVT_NULL;
	while ( done == SDL_EVT_NULL )
	{
		done = AnalyseMainEvent(& coord_rect, &penguin, zombies, opt->nb_zombies);

		if (use_keyboard == 1) AnalyseKeyBoard(&penguin);
			else MoveWithMouse(&penguin, &coord_rect);

		if ( MoveAllZombies(zombies, opt->nb_zombies, &penguin) == 1 )
		{
			PlaySound(SND_EAT);
			SDL_Delay(300);
			DrawScreen(background, &coord_rect, &penguin, 
				zombies,	opt->nb_zombies, hole, opt->nb_hole,
				&opt->status, 0);
			SDL_Delay(700);
			SetCoordPenguin_ZombiesHole(&penguin, zombies, opt->nb_zombies, 
				hole, opt->nb_hole);
			if (use_keyboard == 0) Sdl_WarpMouse(screen,
									penguin.x - coord_rect.x,
									penguin.y - coord_rect.y);
			opt->status.nb_life -= 1;
			if (opt->status.nb_life <= 0) done = SDL_EVT_VALID;
		}

		if ( TestFallInHole(&penguin, zombies, opt->nb_zombies, 
				hole, opt->nb_hole, & opt->status) == 1)
		{
			PlaySound(SND_FALL);
			SDL_Delay(50);
			DrawScreen(background, &coord_rect, &penguin, 
				zombies,	opt->nb_zombies, hole, opt->nb_hole,
				&opt->status, 0);
			SDL_Delay(950);
			SetCoordPenguin_ZombiesHole(&penguin, zombies, opt->nb_zombies, 
				hole, opt->nb_hole);
			if (use_keyboard == 0) Sdl_WarpMouse(screen,
									penguin.x - coord_rect.x,
									penguin.y - coord_rect.y);
			opt->status.nb_life -= 1;
			if (opt->status.nb_life <= 0) done = SDL_EVT_VALID;
		}	

		if ( TestZombiesAllDead(zombies, opt->nb_zombies) == 1) 
			done = SDL_EVT_VALID;

		DrawScreen(background, &coord_rect, &penguin, 
			zombies,	opt->nb_zombies, hole, opt->nb_hole,
			&opt->status, 1);

		SDL_Delay(opt->game_speed);
	}

	SDL_FreeSurface(background);

	if (zombies != NULL) free(zombies);
	if (hole != NULL) free(hole);

	SDL_ShowCursor(1);

	return done;
}



static SDL_Surface * InitBackground(int bgnd_w, int bgnd_h)
{
	int i, j;
	SDL_Surface * background;
	SDL_Surface * img;

	background = SDL_CreateRGBSurface(0,
		bgnd_w, bgnd_h, screen->vscreen->format->BitsPerPixel, 0,0,0,0);
	if (background == NULL) ComplainAndExit("Allocation Background");

	img = images[ Random(IMG_BACK_1, IMG_BACK_10) ];

	for(i = 0; i <= bgnd_w; i+= img->w)
	for(j = 0; j <= bgnd_h; j+= img->h) {
		SDL_Rect drect = { i, j, img->w, img->h };
		SDL_BlitSurface(img, NULL, background, &drect);
	}

	return background;
}


static Zombies_t * InitTabZombies(int nb_zombies, SDL_Rect * coord_rect,
			Uint32 time_show, Uint32 time_inter, Uint32 time_start,
			int pspeed_min, int pspeed_max)
{
	Zombies_t * zombies;
	int i;

	zombies = (Zombies_t *) malloc( nb_zombies * sizeof(Zombies_t) );
	if (zombies == NULL) ComplainAndExit("Alloc. zombies");

	for(i = 0; i < nb_zombies; i++)
	{
		InitZombies(& zombies[i], coord_rect, time_show+ time_inter * i,
			time_start, pspeed_min, pspeed_max);
	}

	return zombies;
}


static Hole_t * InitTabHole(int nb_hole, SDL_Rect * coord_rect,
			SDL_Surface * background)
{
	Hole_t * hole;
	int i;

	hole = (Hole_t *) malloc( nb_hole * sizeof(Hole_t) );
	if (hole == NULL) ComplainAndExit("Alloc. holes");

	for(i = 0; i < nb_hole; i++)
	{
		InitHole(& hole[i], coord_rect);

		PutHole(& hole[i], background);
	}

	return hole;
}


/* Set coord of peng -> not in a hole and not on a zombies */
static void SetCoordPenguin_ZombiesHole(Penguin_t * penguin, Zombies_t * zombies,
			int nb_zombies, Hole_t * hole, int nb_hole)
{
	int i;
	int nb;

	do {
		SetCoordPenguin(penguin);

		for(nb = 0, i = 0; i < nb_hole; i++)
			if (PenguinFallInHole(penguin, & hole[i]) == 1) nb = 1;

		for(i = 0; i < nb_zombies; i++)
			if (ZombiesEatPenguin(penguin, & zombies[i]) == 1) nb = 1;
	} while( nb == 1 );
}


static int AnalyseMainEvent(SDL_Rect * coord_rect, Penguin_t * penguin, 
				Zombies_t * zombies, int nb_zombies)
{
	SDL_Event event;
	int done = SDL_EVT_NULL;
	
	while ( SDL_PollEvent( &event ) )
	{
		switch( event.type )
		{
			case SDL_QUIT : done = SDL_EVT_EXIT; break;

			case SDL_MOUSEMOTION :
				if (use_keyboard == 1)
				{
					use_keyboard = 0; SDL_ShowCursor(1);
					Sdl_WarpMouse(screen,
						penguin->x - coord_rect->x,
						penguin->y - coord_rect->y);
				}
			break;

			case SDL_MOUSEBUTTONDOWN : use_map += 1;
				if (use_map > 3) use_map = 0;
				PlaySound(SND_MENU_VAL);
			break;

			case SDL_KEYDOWN :
				if (Sdl_HandleGlobalKey(screen, &event.key)) break;
				if (event.key.keysym.sym == key_quit) done = SDL_EVT_QUIT;
				if (event.key.keysym.sym == SDLK_k) {
					switch(use_keyboard)
					{
						case 0 : use_keyboard = 1; SDL_ShowCursor(0); break;
						case 1 : use_keyboard = 0; SDL_ShowCursor(1);
							Sdl_WarpMouse(screen,
								penguin->x - coord_rect->x,
								penguin->y - coord_rect->y);
						break;
					}
				}
				if (event.key.keysym.sym == SDLK_m ||
					event.key.keysym.sym == SDLK_KP_5 ||
					event.key.keysym.sym == SDLK_RCTRL ||
					event.key.keysym.sym == SDLK_LCTRL) 
				{ 
					use_map += 1; 
					if (use_map > 3) use_map = 0;
					PlaySound(SND_MENU_VAL);
				}
				if (event.key.keysym.sym == SDLK_s || 
					event.key.keysym.sym == SDLK_LSHIFT || 
					event.key.keysym.sym == SDLK_RSHIFT) 
				{
					sound_volume += 1;
					if (sound_volume > 3) sound_volume = 0;
					SetSoundVolume(sound_volume);
					PlaySound(SND_MENU_VAL);
				}

				if (event.key.keysym.sym == SDLK_DOWN || 
						event.key.keysym.sym == SDLK_UP || 
						event.key.keysym.sym == SDLK_RIGHT || 
						event.key.keysym.sym == SDLK_LEFT ||
						event.key.keysym.sym == SDLK_KP_6 ||
						event.key.keysym.sym == SDLK_KP_4 ||
						event.key.keysym.sym == SDLK_KP_2 ||
						event.key.keysym.sym == SDLK_KP_8)
				{
					use_keyboard = 1; SDL_ShowCursor(0);
				}

				if (event.key.keysym.sym == SDLK_f)
				{
					fullscreen = 1 - fullscreen;
					Sdl_SwapFullScreen(screen);
				}

				if (event.key.keysym.sym == SDLK_p ||
					event.key.keysym.sym == SDLK_SPACE)
				{
					if (Sdl_Pause() == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
				}
			break;	
		}
	}

	return done;
}



static void AnalyseKeyBoard(Penguin_t *penguin)
{
	const Uint8 *keystate;

	keystate = SDL_GetKeyboardState(NULL);

	penguin->dx = 0; penguin->dy = 0; penguin->mv = 0;
	if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_KP_6]) {
		penguin->dx = 1; penguin->mv = 1;
	}
	if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_KP_4]) {
		penguin->dx = -1; penguin->mv = 1;
	}
	if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_KP_8]) {
		penguin->dy = -1; penguin->mv = 1;
	}
	if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_KP_2]) {
		penguin->dy = 1; penguin->mv = 1;
	}
		
	MovePenguin(penguin);
}


static void MoveWithMouse(Penguin_t * penguin, SDL_Rect * coord_rect)
{
	int xm, ym;

	SDL_GetMouseState(&xm, &ym);
	xm += coord_rect->x; ym+= coord_rect->y;

	penguin->dx = 0; penguin->dy = 0; penguin->mv = 0;

	if (xm != penguin->x && xm > penguin->x - penguin->speed && 
			xm < penguin->x + penguin->speed) {
		penguin->x = xm; penguin->mv = 1; }
	if (ym != penguin->y && ym > penguin->y - penguin->speed && 
			ym < penguin->y + penguin->speed) {
		penguin->y = ym; penguin->mv = 1; }
	
	if (xm < penguin->x) { penguin->dx = -1; penguin->mv = 1; }
	if (xm > penguin->x) { penguin->dx = 1; penguin->mv = 1; }
	if (ym < penguin->y) { penguin->dy = -1; penguin->mv = 1; }
	if (ym > penguin->y) { penguin->dy = 1; penguin->mv = 1; }
		
	MovePenguin(penguin);
}


static void AnalyseMoveZombies(Zombies_t * zombies, Penguin_t * penguin)
{
	int xm, ym;

	xm = penguin->x; ym = penguin->y;

	zombies->dx = 0; zombies->dy = 0; zombies->mv = 0;

	if (xm != zombies->x && xm > zombies->x - zombies->speed && 
			xm < zombies->x + zombies->speed) {
		zombies->x = xm; zombies->mv = 1; }
	if (ym != zombies->y && ym > zombies->y - zombies->speed && 
			ym < zombies->y + zombies->speed) {
		zombies->y = ym; zombies->mv = 1; }
	
	if (xm < zombies->x) { zombies->dx = -1; zombies->mv = 1; }
	if (xm > zombies->x) { zombies->dx = 1; zombies->mv = 1; }
	if (ym < zombies->y) { zombies->dy = -1; zombies->mv = 1; }
	if (ym > zombies->y) { zombies->dy = 1; zombies->mv = 1; }
}




static int MoveAllZombies(Zombies_t * zombies, int nb_zombies, Penguin_t *penguin)
{
	int i;
	int ret = 0;

	for(i = 0; i < nb_zombies; i++)
	{
		AnalyseMoveZombies(&zombies[i], penguin);

		MoveZombies(&zombies[i]);

		if (ZombiesEatPenguin(penguin, &zombies[i]) == 1) ret = 1;
	}

	return ret;
}


static int TestFallInHole(Penguin_t * penguin, Zombies_t * zombies, int nb_zombies, 
				Hole_t * hole, int nb_hole, Status_t *status)
{
	int i, j;
	int ret = 0;

	for(i = 0; i < nb_hole; i++)
	{
		if ( PenguinFallInHole(penguin, & hole[i]) == 1 ) ret = 1;

		for(j = 0; j < nb_zombies; j++)
		{
			if ( ZombiesFallInHole(& zombies[j], & hole[i]) == 1 )
			{
				PlaySound(SND_FALL);
				zombies[j].state = ZOMBIS_DEAD;
				status->nb_zombies_ND -= 1;
			}
		}
	}

	return ret;
}



static int TestZombiesAllDead(Zombies_t * zombies, int nb_zombies)
{
	int i, ret;

	for(i = 0, ret = 1; i < nb_zombies; i++)
	{
		if ( zombies[i].state != ZOMBIS_DEAD ) ret = 0;
	}

	return ret;
}



static void PutStatus(Status_t *status, int nb_zombies)
{
	char str[255];
	int x;

	sprintf(str, "%d %dI%d %dI%d", 
		status->nb_life, status->nb_zombies_ND, nb_zombies, 
		status->level + 1, status->nb_level);

	x = MAXX / 2 - strlen(str) * 32 / 2;

	Sdl_DrawTextNumber(x ,10, str);
}


static void PutMap(SDL_Rect * coord_rect, Penguin_t * penguin, 
				Zombies_t * zombies, int nb_zombies, 
				Hole_t * hole, int nb_hole, int num_map)
{
	int i;
	int x,y;

	Sdl_PutSurface(screen, 10, MAXY - 110, images[num_map]);

	/* Put hole in map */
	for(i = 0; i < nb_hole; i++)
	{
		x = 10 + hole[i].x * 100 / coord_rect->w - 1;
		y = MAXY - 110 + hole[i].y * 100 / coord_rect->h - 1;

		Sdl_PutSurface(screen, x, y, images[IMG_HOLE_SMALL]);
	}
	
	/* Put zombies in map */
	for(i = 0; i < nb_zombies; i++)
	{
		x = 10 + zombies[i].x * 100 / coord_rect->w;
		y = MAXY - 110 + zombies[i].y * 100 / coord_rect->h;

		if (zombies[i].state != ZOMBIS_DEAD &&
				zombies[i].state != ZOMBIS_HIDE)
			Sdl_PutSurface(screen, x, y, images[IMG_ZOMBIS_SMALL]);
	}

	/* Put penguin in map */
	x = 10 + penguin->x * 100 / coord_rect->w - 1;
	y = MAXY - 110 + penguin->y * 100 / coord_rect->h - 1;

	Sdl_PutSurface(screen, x, y, images[IMG_PINGUS_SMALL]);
}

/* Draw the screen */
static void DrawScreen(SDL_Surface *background, SDL_Rect * coord_rect,
				Penguin_t * penguin, Zombies_t * zombies, int nb_zombies,
				Hole_t * hole, int nb_hole, Status_t * status, int show_peng)
{
	int i;

	SDL_BlitSurface(background, coord_rect, screen->vscreen, NULL);
	
	if (use_map == 2) 
		PutMap(coord_rect, penguin, zombies, nb_zombies,
			hole, nb_hole, IMG_MAP_2);

	for(i = nb_zombies - 1; i >= 0; i--)
		PutZombies(& zombies[i]);

	if (show_peng == 1) PutPenguin(penguin);

	PutStatus(status, nb_zombies);

	if (use_map == 1) 
		PutMap(coord_rect, penguin, zombies, nb_zombies,
			hole, nb_hole, IMG_MAP);
			
	if (use_map == 3) 
		PutMap(coord_rect, penguin, zombies, nb_zombies,
			hole, nb_hole, IMG_MAP_2);

	Sdl_Flip(screen);
}
