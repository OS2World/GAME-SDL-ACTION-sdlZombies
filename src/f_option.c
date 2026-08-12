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

#include "f_option.h"
#include "global.h"
#include <string.h>
#include <stdlib.h>
#include "sdl_util.h"


static FILE * OpenOptionFile(char * mode)
{
	char * filename;
	FILE * fi;
#ifdef LINUX
	char * home;
#endif

#ifdef LINUX
	/* Get home directory (from $HOME variable)... if we can't determine it,
use the current directory ("."): */
	if (getenv("HOME") != NULL)
		home = getenv("HOME");
	else
		home = ".";

	/* Create the buffer for the filename: */
	filename = (char *) malloc(sizeof(char) * (strlen(home) +
							strlen("/.sdlzombiesrc") + 1));
	strcpy(filename, home);
	strcat(filename, "/.sdlzombiesrc");
#else
	filename = "sdlzombies.dat";
#endif

	/* Try opening the file: */

	fi = fopen(filename, mode);

	return(fi);
}



void LoadGameOptions(Option_t * opt_beg, Option_t * opt_end)
{
	FILE * fi;
	char temp[512];
	int vt;

	fi = OpenOptionFile("r");

	if (fi == NULL) return;

	do 
	{
		fgets(temp, sizeof(temp), fi);

		if( !feof(fi) )
		{
			temp[strlen(temp) - 1] = '\0';

			/* Global Options */
			if (strstr(temp, "sounds volume=") == temp) {
				sound_volume = atoi(temp + strlen("sounds volume="));
				if (sound_volume < 0 || sound_volume > 3) sound_volume = 2;
			}
			else if (strstr(temp, "use map=") == temp) {
				use_map = atoi(temp + strlen("use map="));
				if (use_map < 0 || use_map > 3) use_map = 1;
			}
			else if (strstr(temp, "full screen=") == temp) {
				fullscreen = atoi(temp + strlen("full screen="));
				if (fullscreen < 0 || fullscreen > 1) fullscreen = 0;
			}
			/* Start options */
			else if (strstr(temp, "Number of level=") == temp) {
				vt = atoi(temp + strlen("Number of level="));
				if (vt >= 0 && vt <= 1000)
					opt_beg->status.nb_level = vt;
			}
			else if (strstr(temp, "Number of life=") == temp) {
				vt = atoi(temp + strlen("Number of life="));
				if (vt >= 0 && vt <= 1000)
					opt_beg->status.nb_life = vt;
			}
			else if (strstr(temp, "Start number of zombies=") == temp) {
				vt = atoi(temp + strlen("Start number of zombies="));
				if (vt >= 0 && vt <= 1000)
					opt_beg->nb_zombies = vt;
			}
			else if (strstr(temp, "Start number of holes=") == temp) {
				vt = atoi(temp + strlen("Start number of holes="));
				if (vt >= 0 && vt <= 1000)
					opt_beg->nb_hole = vt;
			}
			else if (strstr(temp, "Start width of background=") == temp) {
				vt = atoi(temp + strlen("Start width of background="));
				if (vt >= MAXX && vt <= 10000)
					opt_beg->bgnd_w = vt;
			}
			else if (strstr(temp, "Start height of background=") == temp) {
				vt = atoi(temp + strlen("Start height of background="));
				if (vt >= MAXY && vt <= 10000)
					opt_beg->bgnd_h = vt;
			}
			else if (strstr(temp, "Start penguin speed=") == temp) {
				vt = atoi(temp + strlen("Start penguin speed="));
				if (vt >= 0 && vt <= 20)
					opt_beg->peng_speed = vt;
			}
			else if (strstr(temp, "Start minimum zombies speed=") == temp) {
				vt = atoi(temp + strlen("Start minimum zombies speed="));
				if (vt >= 0 && vt <= 20)
					opt_beg->zomb_speed_min = vt;
			}
			else if (strstr(temp, "Start maximum zombies speed=") == temp) {
				vt = atoi(temp + strlen("Start maximum zombies speed="));
				if (vt >= 0 && vt <= 20)
					opt_beg->zomb_speed_max = vt;
			}
			else if (strstr(temp, "Start scroll limite X=") == temp) {
				vt = atoi(temp + strlen("Start scroll limite X="));
				if (vt >= 0 && vt <= MAXX - 10)
					opt_beg->lim_scroll_x = vt;
			}
			else if (strstr(temp, "Start scroll limite Y=") == temp) {
				vt = atoi(temp + strlen("Start scroll limite Y="));
				if (vt >= 0 && vt <= MAXY - 10)
					opt_beg->lim_scroll_y = vt;
			}
			else if (strstr(temp, "Start show time=") == temp) {
				vt = atoi(temp + strlen("Start show time="));
				if (vt >= 0 && vt <= 100000)
					opt_beg->time_show = vt;
			}
			else if (strstr(temp, "Start interval time=") == temp) {
				vt = atoi(temp + strlen("Start interval time="));
				if (vt >= 0 && vt <= 100000)
					opt_beg->time_inter = vt;
			}
			else if (strstr(temp, "Start start time=") == temp) {
				vt = atoi(temp + strlen("Start start time="));
				if (vt >= 0 && vt <= 100000)
					opt_beg->time_start = vt;
			}
			else if (strstr(temp, "Start speed game=") == temp) {
				vt = atoi(temp + strlen("Start speed game="));
				if (vt >= 0 && vt <= 100000)
					opt_beg->game_speed = vt;
			}
			/* End Options */
			else if (strstr(temp, "End number of zombies=") == temp) {
				vt = atoi(temp + strlen("End number of zombies="));
				if (vt >= 0 && vt <= 1000)
					opt_end->nb_zombies = vt;
			}
			else if (strstr(temp, "End number of holes=") == temp) {
				vt = atoi(temp + strlen("End number of holes="));
				if (vt >= 0 && vt <= 1000)
					opt_end->nb_hole = vt;
			}
			else if (strstr(temp, "End width of background=") == temp) {
				vt = atoi(temp + strlen("End width of background="));
				if (vt >= MAXX && vt <= 10000)
					opt_end->bgnd_w = vt;
			}
			else if (strstr(temp, "End height of background=") == temp) {
				vt = atoi(temp + strlen("End height of background="));
				if (vt >= MAXY && vt <= 10000)
					opt_end->bgnd_h = vt;
			}
			else if (strstr(temp, "End penguin speed=") == temp) {
				vt = atoi(temp + strlen("End penguin speed="));
				if (vt >= 0 && vt <= 20)
					opt_end->peng_speed = vt;
			}
			else if (strstr(temp, "End minimum zombies speed=") == temp) {
				vt = atoi(temp + strlen("End minimum zombies speed="));
				if (vt >= 0 && vt <= 20)
					opt_end->zomb_speed_min = vt;
			}
			else if (strstr(temp, "End maximum zombies speed=") == temp) {
				vt = atoi(temp + strlen("End maximum zombies speed="));
				if (vt >= 0 && vt <= 20)
					opt_end->zomb_speed_max = vt;
			}
			else if (strstr(temp, "End scroll limite X=") == temp) {
				vt = atoi(temp + strlen("End scroll limite X="));
				if (vt >= 0 && vt <= MAXX - 10)
					opt_end->lim_scroll_x = vt;
			}
			else if (strstr(temp, "End scroll limite Y=") == temp) {
				vt = atoi(temp + strlen("End scroll limite Y="));
				if (vt >= 0 && vt <= MAXY - 10)
					opt_end->lim_scroll_y = vt;
			}
			else if (strstr(temp, "End show time=") == temp) {
				vt = atoi(temp + strlen("End show time="));
				if (vt >= 0 && vt <= 100000)
					opt_end->time_show = vt;
			}
			else if (strstr(temp, "End interval time=") == temp) {
				vt = atoi(temp + strlen("End interval time="));
				if (vt >= 0 && vt <= 100000)
					opt_end->time_inter = vt;
			}
			else if (strstr(temp, "End start time=") == temp) {
				vt = atoi(temp + strlen("End start time="));
				if (vt >= 0 && vt <= 100000)
					opt_end->time_start = vt;
			}
			else if (strstr(temp, "End speed game=") == temp) {
				vt = atoi(temp + strlen("End speed game="));
				if (vt >= 0 && vt <= 100000)
					opt_end->game_speed = vt;
			}
		}
	
	} while( ! feof(fi) );



	fclose(fi);
}






void SaveGameOptions(Option_t * opt_beg, Option_t * opt_end)
{
	FILE * fi;

	fi = OpenOptionFile("w");

	if (fi == NULL) return;

	/* Begin */
	fprintf(fi, "# SdlZombies options file\n\n");

	/* Global Variables */
	fprintf(fi, "#----------------------------------\n");
	fprintf(fi, "# Global variables\n");
	fprintf(fi, "#----------------------------------\n");
	fprintf(fi, "sounds volume=%d\n", sound_volume);
	fprintf(fi, "use map=%d\n", use_map);
	fprintf(fi, "full screen=%d\n", fullscreen);

	/* Start Options */
	fprintf(fi, "\n#----------------------------------\n");
	fprintf(fi, "# Start options\n");
	fprintf(fi, "#----------------------------------\n");
	fprintf(fi, "Number of level=%d\n", opt_beg->status.nb_level);
	fprintf(fi, "Number of life=%d\n", opt_beg->status.nb_life);

	fprintf(fi, "Start number of zombies=%d\n", opt_beg->nb_zombies);
	fprintf(fi, "Start number of holes=%d\n", opt_beg->nb_hole);

	fprintf(fi, "Start width of background=%d\n", opt_beg->bgnd_w);
	fprintf(fi, "Start height of background=%d\n", opt_beg->bgnd_h);

	fprintf(fi, "Start penguin speed=%d\n", opt_beg->peng_speed);
	fprintf(fi, "Start minimum zombies speed=%d\n", opt_beg->zomb_speed_min);
	fprintf(fi, "Start maximum zombies speed=%d\n", opt_beg->zomb_speed_max);

	fprintf(fi, "Start scroll limite X=%d\n", opt_beg->lim_scroll_x);
	fprintf(fi, "Start scroll limite Y=%d\n", opt_beg->lim_scroll_y);

	fprintf(fi, "Start show time=%d\n", opt_beg->time_show);
	fprintf(fi, "Start interval time=%d\n", opt_beg->time_inter);
	fprintf(fi, "Start start time=%d\n", opt_beg->time_start);

	fprintf(fi, "Start speed game=%d\n", opt_beg->game_speed);


	/* End Options */
	fprintf(fi, "\n#----------------------------------\n");
	fprintf(fi, "# End options\n");
	fprintf(fi, "#----------------------------------\n");
	fprintf(fi, "End number of zombies=%d\n", opt_end->nb_zombies);
	fprintf(fi, "End number of holes=%d\n", opt_end->nb_hole);

	fprintf(fi, "End width of background=%d\n", opt_end->bgnd_w);
	fprintf(fi, "End height of background=%d\n", opt_end->bgnd_h);

	fprintf(fi, "End penguin speed=%d\n", opt_end->peng_speed);
	fprintf(fi, "End minimum zombies speed=%d\n", opt_end->zomb_speed_min);
	fprintf(fi, "End maximum zombies speed=%d\n", opt_end->zomb_speed_max);

	fprintf(fi, "End scroll limite X=%d\n", opt_end->lim_scroll_x);
	fprintf(fi, "End scroll limite Y=%d\n", opt_end->lim_scroll_y);

	fprintf(fi, "End show time=%d\n", opt_end->time_show);
	fprintf(fi, "End interval time=%d\n", opt_end->time_inter);
	fprintf(fi, "End start time=%d\n", opt_end->time_start);

	fprintf(fi, "End speed game=%d\n", opt_end->game_speed);

	/* End */
	fprintf(fi, "\n\n# (File automatically created.)\n");

	fclose(fi);
}
