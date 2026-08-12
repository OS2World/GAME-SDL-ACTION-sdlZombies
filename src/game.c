/***************************************************************************
                        SdlZombies - Simple zombies game
 ***************************************************************************/

#include "sdl_util.h"
#include "global.h"
#include "menu.h"
#include "option.h"
#include "level.h"

#define MENU_PLAY   0
#define MENU_OPTION 1
#define MENU_QUIT   2

void MainLoop(Option_t *opt_beg, Option_t *opt_end)
{
    Menu_t   menu;
    Option_t opt;
    int      done_menu;
    int      done_game;
    int      sel;

    done_menu = SDL_EVT_NULL;

    while (done_menu == SDL_EVT_NULL)
    {
        InitMenu(&menu, screen->vscreen, 3, images[IMG_MENU_BACK]);
        SetMenuItem(&menu, MENU_PLAY,
            images[IMG_MENU_PLAY], images[IMG_MENU_PLAY_S],
            MAXX / 2, MAXY * 5 / 8);
        SetMenuItem(&menu, MENU_OPTION,
            images[IMG_MENU_OPTION], images[IMG_MENU_OPTION_S],
            MAXX / 2, MAXY * 5 / 8 + 60);
        SetMenuItem(&menu, MENU_QUIT,
            images[IMG_MENU_QUIT], images[IMG_MENU_QUIT_S],
            MAXX / 2, MAXY * 5 / 8 + 120);

        PlaySound(SND_MENU);

        sel = ActiveMenu(&menu);
        CloseMenu(&menu);

        if (sel == SDL_EVT_EXIT || sel == SDL_EVT_QUIT)
        {
            done_menu = SDL_EVT_EXIT;
        }
        else if (sel == MENU_PLAY)
        {
            PlaySound(SND_MENU_VAL);

            opt = *opt_beg;
            opt.status.level = 0;

            done_game = SDL_EVT_VALID;

            while (done_game == SDL_EVT_VALID &&
                   opt.status.level < opt.status.nb_level &&
                   opt.status.nb_life > 0)
            {
                SetOptions(opt_beg, opt_end, &opt);
                PlaySound(SND_INTRO);
                done_game = RunLevel(&opt);

                if (done_game == SDL_EVT_VALID)
                    opt.status.level++;
            }

            if (done_game == SDL_EVT_EXIT)
            {
                done_menu = SDL_EVT_EXIT;
            }
            else
            {
                PlaySound(SND_FINAL);
                Sdl_Pause();
            }
        }
        else if (sel == MENU_OPTION)
        {
            PlaySound(SND_MENU_VAL);
            if (MenuSetOptions(opt_beg, opt_end) == SDL_EVT_EXIT)
                done_menu = SDL_EVT_EXIT;
        }
        else if (sel == MENU_QUIT)
        {
            done_menu = SDL_EVT_EXIT;
        }
    }
}
