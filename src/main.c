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



#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <stdio.h>
#include "sdl_util.h"
#include "global.h"
#include "option.h"
#include "f_option.h"


extern void MainLoop(Option_t *opt_beg, Option_t * opt_end);


/* Main procedure */
int main(int argc, char **argv)
{
	Option_t opt_beg, opt_end;

	sound_volume = 2;
	use_map = 1;
	use_keyboard = 1;
	fullscreen = 0;
	
	InitStartStopOptions(&opt_beg, &opt_end);
	
	LoadGameOptions(& opt_beg, &opt_end);

	screen = Sdl_Init(argc, argv);

	SDL_SetWindowTitle(screen->window, "SdlZombies");

	InitGlobalVar();

	MainLoop(&opt_beg, &opt_end);

	CloseGlobalVar();

	Sdl_Close(screen);

	SaveGameOptions(& opt_beg, &opt_end);

	return 0;
}
