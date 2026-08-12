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
#include "option.h"
#include "draw_text.h"
#include <string.h>

static int SetParamLinear(int min, int max, float coeff);
static Uint32 SetParamLinearUint(Uint32 min, Uint32 max, float coeff);


void InitStartStopOptions(Option_t * opt_beg, Option_t * opt_end)
{
	/* Option of the begining */
	opt_beg->status.nb_life = 10;
	opt_beg->status.level = 0;
	opt_beg->status.nb_level = 20;

	opt_beg->bgnd_w = 1.5*MAXX;
	opt_beg->bgnd_h = 1.5*MAXY;
	opt_beg->nb_hole = 20;
	opt_beg->nb_zombies = 5;
	opt_beg->status.nb_zombies_ND = opt_beg->nb_zombies;

	opt_beg->peng_speed = 5;
	opt_beg->zomb_speed_min = 1; opt_beg->zomb_speed_max = 2;

	opt_beg->lim_scroll_x = 100; opt_beg->lim_scroll_y = 100;

	opt_beg->time_show = 100;
	opt_beg->time_inter = 1000;
	opt_beg->time_start = 3000;

	opt_beg->game_speed = 8;
	
	
	/* Option of the end */
	opt_end->bgnd_w = 4*MAXX;
	opt_end->bgnd_h = 4*MAXY;
	opt_end->nb_hole = 5;
	opt_end->nb_zombies = 30;
	opt_end->status.nb_zombies_ND = opt_end->nb_zombies;

	opt_end->peng_speed = 6;
	opt_end->zomb_speed_min = 3; opt_end->zomb_speed_max = 5;

	opt_end->lim_scroll_x = 100; opt_end->lim_scroll_y = 100;

	opt_end->time_show = 100;
	opt_end->time_inter = 100;
	opt_end->time_start = 300;

	opt_end->game_speed = 8;
}



void SetOptions(Option_t *opt_beg, Option_t *opt_end, Option_t *opt)
{
	float coeff = (float) opt->status.level / (float) (opt->status.nb_level-1);

	opt->bgnd_w = SetParamLinear(opt_beg->bgnd_w, opt_end->bgnd_w, coeff);
	opt->bgnd_h = SetParamLinear(opt_beg->bgnd_h, opt_end->bgnd_h, coeff);
	opt->nb_hole = SetParamLinear(opt_beg->nb_hole, opt_end->nb_hole, coeff);
	opt->nb_zombies = SetParamLinear(opt_beg->nb_zombies, 
						opt_end->nb_zombies, coeff);
	opt->status.nb_zombies_ND = opt->nb_zombies;

	opt->peng_speed = SetParamLinear(opt_beg->peng_speed, 
						opt_end->peng_speed, coeff);
	opt->zomb_speed_min = SetParamLinear(opt_beg->zomb_speed_min,
						opt_end->zomb_speed_min, coeff);
	opt->zomb_speed_max = SetParamLinear(opt_beg->zomb_speed_max, 
						opt_end->zomb_speed_max, coeff);

	opt->lim_scroll_x = SetParamLinear(opt_beg->lim_scroll_x, 
						opt_end->lim_scroll_x, coeff);
	opt->lim_scroll_y = SetParamLinear(opt_beg->lim_scroll_y, 
						opt_end->lim_scroll_y, coeff);

	opt->time_show = SetParamLinearUint(opt_beg->time_show, 
						opt_end->time_show, coeff);
	opt->time_inter = SetParamLinearUint(opt_beg->time_inter, 
						opt_end->time_inter, coeff);
	opt->time_start = SetParamLinearUint(opt_beg->time_start, 
						opt_end->time_start, coeff);

	opt->game_speed = SetParamLinearUint(opt_beg->game_speed, 
						opt_end->game_speed, coeff);
}



static int SetParamLinear(int min, int max, float coeff)
{
	return min + (max - min) * coeff;
}


static Uint32 SetParamLinearUint(Uint32 min, Uint32 max, float coeff)
{
	return min + ((float) max - (float) min) * coeff;
}



void ShowOptionsText(Option_t *opt)
{
	printf("\n_______________________________________\n");

	printf("Level : %d / %d\n", opt->status.level, opt->status.nb_level);
	printf("Nb life : %d\n", opt->status.nb_life);

	printf("\nBackground : %d, %d\n", opt->bgnd_w, opt->bgnd_h);
	printf("Nb Zombies : %d\n", opt->nb_zombies);
	printf("Nb Hole : %d\n", opt->nb_hole);

	printf("\nPenguin Speed : %d\n", opt->peng_speed);
	printf("Lim scroll : %d, %d\n", opt->lim_scroll_x, opt->lim_scroll_y);

	printf("\nZombies speed min/max : %d, %d\n", opt->zomb_speed_min,
			opt->zomb_speed_max);

	printf("\ntime_show : %u\n", opt->time_show);
	printf("time inter : %u\n", opt->time_inter);
	printf("time start : %u\n", opt->time_start);

	printf("\ngame speed : %u\n", opt->game_speed);
	printf("*** start ***\n");
}



static void DrawScreenOption(Option_t * opt)
{
	char str[255];

	sprintf(str, "Level %dI%d", opt->status.level + 1, opt->status.nb_level);
	Sdl_DrawTextNumber(MAXX / 2 - 16*strlen(str), 30, str);
	
	sprintf(str, "life %d", opt->status.nb_life);
	Sdl_DrawTextNumber(MAXX / 2 - 16*strlen(str), 80, str);
	
	sprintf(str, "Zombies %d", opt->nb_zombies);
	Sdl_DrawTextNumber(MAXX / 2 - 16*strlen(str), 130, str);
	
	sprintf(str, "Holes %d", opt->nb_hole);
	Sdl_DrawTextNumber(MAXX / 2 - 16*strlen(str), 180, str);

	sprintf(str, "Background %dx%d", opt->bgnd_w, opt->bgnd_h);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 230, str, 0);
	

	sprintf(str, "Penguin Speed %d", opt->peng_speed);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 255, str, 0);
	
	sprintf(str, "Zombies speed min %d max %d", opt->zomb_speed_min,
			opt->zomb_speed_max);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 280, str, 0);
			
	sprintf(str, "Lim scroll %d %d", opt->lim_scroll_x, opt->lim_scroll_y);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 305, str, 0);

	sprintf(str, "time show %u", opt->time_show);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 330, str, 0);
	
	sprintf(str, "time inter %u", opt->time_inter);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 355, str, 0);
	
	sprintf(str, "time start %u", opt->time_start);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 380, str, 0);

	sprintf(str, "game speed %u", opt->game_speed);
	Sdl_DrawTextNumberSmall(MAXX / 2 - 8*strlen(str), 405, str, 0);
}


int ShowOptions(Option_t *opt,	SDL_Rect * coord_rect,
		SDL_Surface * background)
{
	int done = SDL_EVT_NULL;

	PlaySound(SND_INTRO);

	SDL_BlitSurface(background, coord_rect, screen->vscreen, NULL);

	DrawScreenOption(opt);
	
	Sdl_Flip(screen);

	done = Sdl_Pause();

	return done;
}





/*=========================================================*
 *		Menu to Set Options
 *=========================================================*/
static int DrawMenuOption(Option_t * opt_beg, Option_t * opt_end, int sel)
{
	int i, j;
	char str[256];
	int num;

	for(i = 0; i <= MAXX; i+= images[IMG_BACK_1]->w)
	for(j = 0; j <= MAXY; j+= images[IMG_BACK_1]->h)
		Sdl_PutSurface(screen, i, j, images[IMG_BACK_1]);

	Sdl_DrawTextNumber(MAXX / 2 - strlen("Options")*16, 5, "Options");

	num = 0; sprintf(str, "Number of level %d", opt_beg->status.nb_level);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str, 
		sel == num ? 1 : 0);

	num = 1; sprintf(str, "Number of life %d", opt_beg->status.nb_life);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 2; sprintf(str, "Number of zombies %d to %d", 
				opt_beg->nb_zombies, opt_end->nb_zombies);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 3; sprintf(str, "Number of holes %d to %d", 
				opt_beg->nb_hole, opt_end->nb_hole);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 4; sprintf(str, "Size of background");
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		(sel == num || sel == num + 1) ? 1 : 0);
	num = 5; sprintf(str, "%dx%d to %dx%d", opt_beg->bgnd_w, opt_beg->bgnd_h, 
		opt_end->bgnd_w, opt_end->bgnd_h);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		(sel == num || sel == num - 1) ? 1 : 0);

	num = 6; sprintf(str, "Penguin speed %d to %d", 
				opt_beg->peng_speed, opt_end->peng_speed);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 7; sprintf(str, "Zombies speed %di%d to %di%d", 
				opt_beg->zomb_speed_min, opt_beg->zomb_speed_max,
				opt_end->zomb_speed_min, opt_end->zomb_speed_max);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 8; sprintf(str, "Scroll limites %di%d to %di%d", 
				opt_beg->lim_scroll_x, opt_beg->lim_scroll_y,
				opt_end->lim_scroll_x, opt_end->lim_scroll_y);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 9; sprintf(str, "Show time %d to %d", 
				opt_beg->time_show, opt_end->time_show);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 10; sprintf(str, "Interval time %d to %d", 
				opt_beg->time_inter, opt_end->time_inter);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 11; sprintf(str, "Start time %d to %d", 
				opt_beg->time_start, opt_end->time_start);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 12; sprintf(str, "Game speed %d to %d", 
				opt_beg->game_speed, opt_end->game_speed);
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);

	num = 13; sprintf(str, "Quit");
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16)/2, 60+num*30, str,
		sel == num ? 1 : 0);
		
	Sdl_Flip(screen);

	return num;
}



static int ValidMenuOption(Option_t * opt_beg, Option_t * opt_end, int sel)
{
	int done = SDL_EVT_NULL;

	switch( sel )
	{
		case 0 : if (Sdl_GetNumber("Number of level",
						& opt_beg->status.nb_level,0, 1000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 1 : if (Sdl_GetNumber("Number of life",
						& opt_beg->status.nb_life,0, 1000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 2 : if (Sdl_GetNumber("Start number of zombies",
						& opt_beg->nb_zombies,0, 1000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End number of zombies",
						& opt_end->nb_zombies,0, 1000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 3 : if (Sdl_GetNumber("Start number of holes",
						& opt_beg->nb_hole,0, 1000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End number of holes",
						& opt_end->nb_hole,0, 1000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 4 : case 5 : 
				if (Sdl_GetNumber("Start width of background",
						& opt_beg->bgnd_w,MAXX, 10000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End width of background",
						& opt_end->bgnd_w,MAXX, 10000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("Start height of background",
						& opt_beg->bgnd_h,MAXY, 10000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End height of background",
						& opt_end->bgnd_h,MAXY, 10000) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 6 : if (Sdl_GetNumber("Start penguin speed",
						& opt_beg->peng_speed,0, 20) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End penguin speed",
						& opt_end->peng_speed,0, 20) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 7 : if (Sdl_GetNumber("Start min zombies speed",
						& opt_beg->zomb_speed_min,0, 20) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End min zombies speed",
						& opt_end->zomb_speed_min,0, 20) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("Start max zombies speed",
						& opt_beg->zomb_speed_max,0, 20) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End max zombies speed",
						& opt_end->zomb_speed_max,0, 20) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 8 : if (Sdl_GetNumber("Start scroll limite x",
						& opt_beg->lim_scroll_x,0, MAXX - 10) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End scroll limite x",
						& opt_end->lim_scroll_x,0, MAXX - 10) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("Start scroll limite y",
						& opt_beg->lim_scroll_y,0, MAXY - 10) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
				if (Sdl_GetNumber("End scroll limite y",
						& opt_end->lim_scroll_y,0, MAXY - 10) == SDL_EVT_EXIT)
					done = SDL_EVT_EXIT;
		break;
		case 9 : {
			int tmp;
			tmp = (int)opt_beg->time_show;
			if (Sdl_GetNumber("Start show time", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_beg->time_show = (Uint32)tmp;
			tmp = (int)opt_end->time_show;
			if (Sdl_GetNumber("End show time", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_end->time_show = (Uint32)tmp;
		} break;
		case 10 : {
			int tmp;
			tmp = (int)opt_beg->time_inter;
			if (Sdl_GetNumber("Start interval time", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_beg->time_inter = (Uint32)tmp;
			tmp = (int)opt_end->time_inter;
			if (Sdl_GetNumber("End interval time", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_end->time_inter = (Uint32)tmp;
		} break;
		case 11 : {
			int tmp;
			tmp = (int)opt_beg->time_start;
			if (Sdl_GetNumber("Start start time", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_beg->time_start = (Uint32)tmp;
			tmp = (int)opt_end->time_start;
			if (Sdl_GetNumber("End start time", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_end->time_start = (Uint32)tmp;
		} break;
		case 12 : {
			int tmp;
			tmp = (int)opt_beg->game_speed;
			if (Sdl_GetNumber("Start speed game", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_beg->game_speed = (Uint32)tmp;
			tmp = (int)opt_end->game_speed;
			if (Sdl_GetNumber("End speed game", &tmp, 0, 100000) == SDL_EVT_EXIT) done = SDL_EVT_EXIT;
			opt_end->game_speed = (Uint32)tmp;
		} break;
	
		case 13 : done = SDL_EVT_VALID;
	}

	return done;
}



int MenuSetOptions(Option_t * opt_beg, Option_t * opt_end)
{
	int done = SDL_EVT_NULL;
	SDL_Event event;
	int cur = 0, lcur;
	int nb_option;

	nb_option = DrawMenuOption(opt_beg, opt_end, cur);

	while(done == SDL_EVT_NULL)
	{
		while ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_QUIT : done = SDL_EVT_EXIT; break;

				case SDL_KEYDOWN :
					if (event.key.keysym.sym == key_quit) {
						done = SDL_EVT_QUIT;
						PlaySound(SND_MENU_VAL);
					}

					if (event.key.keysym.sym == SDLK_DOWN ||
						event.key.keysym.sym == SDLK_KP_2) {
						PlaySound(SND_MENU);
						cur += 1; if (cur == 4) cur += 1;
						if (cur > nb_option) cur = 0;
					}
					if (event.key.keysym.sym == SDLK_UP ||
						event.key.keysym.sym == SDLK_KP_8) {
						PlaySound(SND_MENU);
						cur -= 1; if (cur == 4) cur -= 1;
						if (cur < 0) cur = nb_option;
					}

					if (event.key.keysym.sym == SDLK_RETURN ||
						event.key.keysym.sym == SDLK_KP_ENTER) {
						PlaySound(SND_MENU_VAL);
						done = ValidMenuOption(opt_beg, opt_end, cur);
					}
				break;

				case SDL_MOUSEMOTION :
					lcur = cur;
					cur = (event.motion.y - 55 + 10) / 30;
					if (cur < 0) cur = 0;
					if (cur > nb_option) cur = nb_option;
					if (cur != lcur) PlaySound(SND_MENU);
				break;

				case SDL_MOUSEBUTTONUP :
					PlaySound(SND_MENU_VAL);
					done = ValidMenuOption(opt_beg, opt_end, cur);
				break;
			}
		}
		
		DrawMenuOption(opt_beg, opt_end, cur);

		SDL_Delay(10);
	}

	return done;
}
