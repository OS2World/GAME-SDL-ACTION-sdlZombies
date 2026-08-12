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
#include "draw_text.h"
#include "global.h"
#include <stdlib.h>
#include <string.h>

/* Avoid ctype.h's MY_TOUPPER() macro which pulls in __ctype[] (unresolved
   under WLINK on OS/2).  ASCII-only uppercase is all the game needs. */
#define MY_TOUPPER(c) ((c) >= 'a' && (c) <= 'z' ? (c) - 32 : (c))


/* Draw text: */

void Sdl_DrawText(int x, int y, char * str) 
{
	int i;
	SDL_Rect src, dest;
 
	for (i = 0; i < strlen(str); i++)
	{
		if (MY_TOUPPER(str[i]) >= 'A' && MY_TOUPPER(str[i]) <= 'Z')
		{
			src.x = (MY_TOUPPER(str[i]) - 'A') * 32; src.y = 0;
			src.w = 32; src.h = 32;

			dest.x = x + (i * 32); dest.y = y;
			dest.w = 32; dest.h = 32;

			SDL_BlitSurface(images[IMG_LETTER], &src, screen->vscreen, &dest);
		}
	}
}



/* Draw a number on the screen: */

void Sdl_DrawNumber(int x, int y, int v) 
{
	char str[16];
	int i;
	SDL_Rect src, dest;

	sprintf(str, "%d", v);

	for (i = 0; i < strlen(str); i++)
	{
		src.x = (str[i] - '0') * 32; src.y = 0;
		src.w = 32; src.h = 32;

		dest.x = x + (i * 32); dest.y = y;
		dest.w = 32; dest.h = 32;

		SDL_BlitSurface(images[IMG_NUMBER_0], &src, screen->vscreen, &dest);
	}
}

void Sdl_DrawTextNumber(int x, int y, char *str)
{
	int i;
	SDL_Rect src, dest;
 
	for (i = 0; i < strlen(str); i++)
	{
		if (MY_TOUPPER(str[i]) >= 'A' && MY_TOUPPER(str[i]) <= 'Z')
		{
			src.x = (MY_TOUPPER(str[i]) - 'A') * 32; src.y = 0;
			src.w = 32; src.h = 32;

			dest.x = x + (i * 32); dest.y = y;
			dest.w = 32; dest.h = 32;

			SDL_BlitSurface(images[IMG_LETTER], &src, screen->vscreen, &dest);
		}

		if (str[i] >= '0' && str[i] <= '9')
		{
			src.x = (str[i] - '0') * 32; src.y = 0;
			src.w = 32; src.h = 32;

			dest.x = x + (i * 32); dest.y = y;
			dest.w = 32; dest.h = 32;

			SDL_BlitSurface(images[IMG_NUMBER_0], &src, screen->vscreen, &dest);
		}
	}
}


void Sdl_DrawTextNumberSmall(int x, int y, char *str, int font)
{
	int i;
	SDL_Rect src, dest;
	SDL_Surface * img_letter;
	SDL_Surface * img_number;

	if (font == 1) {
		img_letter = images[IMG_LETTER_SMALL_2];
		img_number = images[IMG_NUMBER_SMALL_2];
	} else {
		img_letter = images[IMG_LETTER_SMALL];
		img_number = images[IMG_NUMBER_SMALL];
	}
 
	for (i = 0; i < strlen(str); i++)
	{
		if (MY_TOUPPER(str[i]) >= 'A' && MY_TOUPPER(str[i]) <= 'Z')
		{
			src.x = (MY_TOUPPER(str[i]) - 'A') * 16; src.y = 0;
			src.w = 16; src.h = 16;

			dest.x = x + (i * 16); dest.y = y;
			dest.w = 16; dest.h = 16;

			SDL_BlitSurface(img_letter, &src, screen->vscreen, &dest);
		}

		if (str[i] >= '0' && str[i] <= '9')
		{
			src.x = (str[i] - '0') * 16; src.y = 0;
			src.w = 16; src.h = 16;

			dest.x = x + (i * 16); dest.y = y;
			dest.w = 16; dest.h = 16;

			SDL_BlitSurface(img_number, &src, screen->vscreen, &dest);
		}
	}
}




/*=========================================================*
 *		Get String and Number Functions
 *=========================================================*/
static void DrawGetString(char * msg, char * str)
{
	Sdl_PutSurface(screen, (MAXX - images[IMG_CARRE]->w) / 2, 
		(MAXY - images[IMG_CARRE]->h) / 2, images[IMG_CARRE]);

	Sdl_DrawTextNumberSmall( (MAXX - strlen(msg)*16) / 2,
		MAXY/2 - 20, msg, 0);
	
	Sdl_DrawTextNumberSmall( (MAXX - strlen(str)*16) / 2,
		MAXY/2 + 5, str, 0);

	Sdl_Flip(screen);
}

int Sdl_GetString(char * msg, char * str, int maxchar, int enbl_letter)
{
	int done = SDL_EVT_NULL;
	SDL_Event event;
	char * strbis;

	strbis = malloc(maxchar + 1);
	if (strbis == NULL) return SDL_EVT_VALID;

	strcpy(strbis, str);

	if (maxchar > 28) maxchar = 28;
	if (strlen(strbis) < maxchar) strcat(strbis, "i");

	DrawGetString(msg, strbis);

	while(done == SDL_EVT_NULL)
	{
		while ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_QUIT : done = SDL_EVT_EXIT; break;

				case SDL_KEYDOWN :
					if (event.key.keysym.sym == key_quit) done = SDL_EVT_QUIT;

					if (event.key.keysym.sym == SDLK_RETURN ||
						event.key.keysym.sym == SDLK_KP_ENTER) 
						done = SDL_EVT_VALID;

					if (event.key.keysym.sym >= SDLK_a &&
						event.key.keysym.sym <= SDLK_z &&
						enbl_letter == 1 &&
						strlen(strbis) < maxchar)
					{
						strbis[ strlen(strbis) - 1 ] = event.key.keysym.sym;
						strcat(strbis, "i");
					}
					if (event.key.keysym.sym >= SDLK_KP_0 &&
						event.key.keysym.sym <= SDLK_KP_9 &&
						strlen(strbis) < maxchar)
					{
						strbis[ strlen(strbis) - 1 ] = '0' +
							event.key.keysym.sym - SDLK_KP_0;
						strcat(strbis, "i");
					}
					if (event.key.keysym.sym == SDLK_SPACE &&
						strlen(strbis) < maxchar)
					{
						strbis[ strlen(strbis) - 1 ] = ' ';
						strcat(strbis, "i");
					}

					if ((event.key.keysym.sym == SDLK_LEFT ||
						event.key.keysym.sym == SDLK_BACKSPACE ||
						event.key.keysym.sym == SDLK_DELETE) &&
						strlen(strbis) > 1)
					{
						strbis[ strlen(strbis) - 2 ] = 0;
						strcat(strbis, "i");
					}
				break;
			}
		}
		
		DrawGetString(msg, strbis);

		SDL_Delay(10);
	}

	strbis[ strlen(strbis) - 1 ] = 0;

	if (done == SDL_EVT_VALID) {
		strcpy(str, strbis);
		PlaySound(SND_MENU_VAL);
	} else PlaySound(SND_MENU);

	return done;
}

int Sdl_GetNumber(char * msg, int * val, int min, int max)
{
	char str[255];

	sprintf(str, "%d", *val);

	Sdl_GetString(msg, str, 254, 0);

	*val = atoi(str);
	if (*val < min) *val = min;
	if (*val > max) *val = max;

	return 0;
}
