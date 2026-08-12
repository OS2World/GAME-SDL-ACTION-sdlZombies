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

#ifndef __DRAW_TEXT_H
#define __DRAW_TEXT_H


#include "sdl_util.h"

extern void Sdl_DrawText(int x, int y, char * str); 
extern void Sdl_DrawNumber(int x, int y, int v);

extern void Sdl_DrawTextNumber(int x, int y, char *str);
extern void Sdl_DrawTextNumberSmall(int x, int y, char *str, int font);

/* Get a string : if enbl_letter != 1 : get only numbers */
extern int Sdl_GetString(char * msg, char * str, int maxchar, int enbl_letter);
extern int Sdl_GetNumber(char * msg, int * val, int min, int max);

#endif /* __DRAW_TEXT_H */
































