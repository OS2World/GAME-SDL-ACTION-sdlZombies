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

#include "menu.h"
#include <stdlib.h>
#include "global.h"
#include "sdl_util.h"


int InitMenu(Menu_t * menu, SDL_Surface * pscreen, int pnb_item,
		SDL_Surface * pimage_back)
{
	int i;

	menu->screen = pscreen;
	menu->image_back = pimage_back;
	menu->nb_item = pnb_item;
	menu->cur = 0;

	menu->item = (MenuItem_t *) malloc( sizeof(MenuItem_t) * menu->nb_item );

	if (menu->item == NULL) return -1;

	for(i = 0; i < menu->nb_item; i++)
	{
		menu->item[i].image = NULL;
		menu->item[i].image_select = NULL;
		menu->item[i].background = NULL;
	}

	return 0;
}

void CloseMenu(Menu_t * menu)
{
	int i;

	if (menu->item != NULL) {
		for(i = 0; i < menu->nb_item; i++)
		{
			if (menu->item[i].background != NULL)
				SDL_FreeSurface( menu->item[i].background );
		}
	
		free( menu->item );
		menu->item = NULL;
	}
}



void SetMenuItem(Menu_t * menu, int num, SDL_Surface * pimage,
		SDL_Surface * pimage_select, int x, int y)
{
	if (num < 0 || num >= menu->nb_item) return;

	menu->item[num].image = pimage;
	menu->item[num].image_select = pimage_select;
	
	menu->item[num].pos.x = x - pimage->w / 2;
	menu->item[num].pos.y = y - pimage->h / 2;
	menu->item[num].pos.w = pimage->w;
	menu->item[num].pos.h = pimage->h;

	if (menu->item[num].background != NULL)
		SDL_FreeSurface(menu->item[num].background);

	menu->item[num].background =
		SDL_CreateRGBSurface(0,
			pimage->w, pimage->h, menu->screen->format->BitsPerPixel,
			0, 0, 0, 0);
	if (menu->item[num].background == NULL) 
		ComplainAndExit("Allocation Background for Item");
}



static void DeSelectMenuItem(Menu_t * menu, int num)
{
	SDL_BlitSurface(menu->item[num].background, NULL,
		menu->screen, &menu->item[num].pos);
	SDL_BlitSurface(menu->item[num].image, NULL,
		menu->screen, &menu->item[num].pos);
	Sdl_Flip(screen);
}

static void SelectMenuItem(Menu_t * menu, int num)
{
	SDL_BlitSurface(menu->item[num].background, NULL,
		menu->screen, &menu->item[num].pos);
	SDL_BlitSurface(menu->item[num].image_select, NULL,
		menu->screen, &menu->item[num].pos);
	Sdl_Flip(screen);
}


static void IncrCurSelMenu(Menu_t * menu)
{
	DeSelectMenuItem(menu, menu->cur);

	menu->cur += 1;
	if (menu->cur >= menu->nb_item) menu->cur = 0;

	SelectMenuItem(menu, menu->cur);

	PlaySound(SND_MENU);
}


static void DecrCurSelMenu(Menu_t * menu)
{
	DeSelectMenuItem(menu, menu->cur);
	
	menu->cur -= 1;
	if (menu->cur < 0) menu->cur = menu->nb_item - 1;

	SelectMenuItem(menu, menu->cur);

	PlaySound(SND_MENU);
}



int ActiveMenu(Menu_t * menu)
{
	int done = SDL_EVT_NULL;
	SDL_Event event;
	int i;

	/* Tile the background image across the entire virtual screen */
	{
		int tx, ty;
		for (ty = 0; ty < menu->screen->h; ty += menu->image_back->h)
			for (tx = 0; tx < menu->screen->w; tx += menu->image_back->w) {
				SDL_Rect dst = { tx, ty, menu->image_back->w, menu->image_back->h };
				SDL_BlitSurface(menu->image_back, NULL, menu->screen, &dst);
			}
	}

	for(i = 0; i < menu->nb_item; i++)
	{
		SDL_BlitSurface(menu->screen, & menu->item[i].pos, 
			menu->item[i].background, NULL);

		if (i == menu->cur) SDL_BlitSurface(menu->item[i].image_select, NULL, 
								menu->screen, & menu->item[i].pos);
			else SDL_BlitSurface(menu->item[i].image, NULL, 
					menu->screen, & menu->item[i].pos);
	}
		
	Sdl_Flip(screen);

	menu->type_valid = MENU_NONE;

	while(done == SDL_EVT_NULL)
	{
		while ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_QUIT : done = SDL_EVT_EXIT; break;

				case SDL_MOUSEMOTION :
					for(i = 0; i < menu->nb_item; i++)
					{
						if (i != menu->cur &&
							Sdl_XYOnRect(event.motion.x, event.motion.y,
								& menu->item[i].pos) )
						{
							DeSelectMenuItem(menu, menu->cur);
							menu->cur = i;
							SelectMenuItem(menu, menu->cur);
							PlaySound(SND_MENU);
						}
					}
				break;

				case SDL_KEYDOWN :
					if (Sdl_HandleGlobalKey(screen, &event.key)) break;
					if (event.key.keysym.sym == key_quit) {
						done = SDL_EVT_QUIT;
						PlaySound(SND_MENU_VAL);
					}

					if (event.key.keysym.sym == SDLK_DOWN ||
						event.key.keysym.sym == SDLK_KP_2)
						IncrCurSelMenu(menu);
					if (event.key.keysym.sym == SDLK_UP ||
						event.key.keysym.sym == SDLK_KP_8) 
						DecrCurSelMenu(menu); 

					if (event.key.keysym.sym == SDLK_RETURN ||
						event.key.keysym.sym == SDLK_KP_ENTER) {
						menu->type_valid = MENU_KEYBOARD;
						done = SDL_EVT_VALID;
						PlaySound(SND_MENU_VAL);
					}
				break;

				case SDL_MOUSEBUTTONUP : done = SDL_EVT_VALID;
					menu->type_valid = MENU_MOUSE;
					PlaySound(SND_MENU_VAL);
				break;
			}
		}

		SDL_Delay(10);
	}

	if (done == SDL_EVT_VALID) return menu->cur;

	return done;
}
