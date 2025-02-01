// default
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
// Time
#include <time.h>
#include <unistd.h>
// direction
#include <dirent.h>
#include <sys/stat.h>
// sound
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
// emoji
#include <locale.h>
// my lib
#include "menu_func.h"
// IDK
// #include <sys/ioctl.h>
// #include <sys/types.h>

#define MAX_NAMES 256
#define MAX_LINE 1024
#define LIMIT_INPUT 25

// ʘ
// ۩
// ߷
// ౷
// ࿈
// ࿅
// ᛸ
// ࿋
// ࿕
// ᛟ
// ψ
// ░ ░ ░

/*
XP > level
1 = inverse
2 = default
3 = red inverse
4 = red
5 = gold inverse
6 = blue
7 = green inverse
8 = gray
9 = gold
10 = silver
11 = bronze
12 = cyan inverse
13 = cyan
20 = red
21 = green
22 = blue
23 = yellow
24 = brown
25 = purple
 */

typedef struct Node
{
    int x;
    int y;
} Node;

void New_Game(int user_mode)
{
#pragma region start game
    UseColor();
    int scrY, scrX;
    getmaxyx(stdscr, scrY, scrX);
    int x = scrX / 2, y = scrY / 2;
    // mvprintw(y, x, "");
}

void mv(int user_mode)
{
    UseColor();
    int scrY, scrX;
    getmaxyx(stdscr, scrY, scrX);
    printw(" \U0001F451 \U0001F947 \U0001F3C6 \U0001F511 \U0001F6AA \u2192 \U0001F47E \U0001F409 \u200D \U0001F6E1 \u2694 \u25B6 \u25C0 \u25B6 \u25CB \u25A0 \u2665 \u2663 \u2660 \u2620 \u231B \u231B \u2764 \u2744 \u2728 \u2714 \u2708 \u266A \u2665 \u263A \u2615 \u271D \u270C \u270B \u270A \u2705 \u267B \u2666 \u27BF \u27B0 \u27A1 \u2747 ");
    //             👑       🥇          🏆        🔑         🚪       →        👾        🐉         🛡      ⚔          ▶     ◀       ▶     ○      ■      ♥       ♣     ♠      ☠      ⌛     ⌛     ❤      ❄      ✨     ✔      ✈      ♪       ♥      ☺     ☕      ✝      ✌     ✋      ✊    ✅      ♻      ♦     ➿     ➰     ➡      ❇
    mvprintw(scrY - 1, 0, "scrY = %d", scrY);
    mvprintw(0, scrX - 9, "scrX = %d", scrX);
    int x = scrX / 2, y = scrY / 2;
    mvprintw(scrY - 1, x - 3, "y = %d", y);
    mvprintw(y, scrX - 6, "x = %d", x);
    printBoard(0, 0, 34, 140);
    Node *player = (Node *)malloc(sizeof(Node));
    player->x = x;
    player->y = y;
    int ch;
    while (1)
    {
        attron(A_BLINK);
        mvprintw(player->y, player->x, "\u25B6");
        attroff(A_BLINK);
        ch = getch();
        if ((ch == KEY_UP || ch == KEY_DOWN || ch == KEY_RIGHT || ch == KEY_LEFT) && (player->x > 0 && player->x < scrX && player->y > 0 && player->y < scrY - 1))
            mvprintw(player->y, player->x, " ");

        if (ch == KEY_UP && player->y > 1)
            player->y--;
        else if (ch == KEY_DOWN && player->y < scrY - 2)
            player->y++;
        else if (ch == KEY_LEFT && player->x > 1)
            player->x--;
        else if (ch == KEY_RIGHT && player->x < scrX - 2)
            player->x++;
        else if (ch == 'q')
            break;
    }
}

int user_menu(int user_mode)
{
    /*
    0 = exit
    1 = else
    */

    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);

    attron(COLOR_PAIR(9));
    printAsciiArt('R', scrX, scrY, 15, 7);
    attroff(COLOR_PAIR(9));

    attron(COLOR_PAIR(8));
    printBoard(4, 0, 15, 55);
    attroff(COLOR_PAIR(8));

    char username[MAX_NAMES];
    memset(username, 0, sizeof(username));
    strcpy(username, "~GUEST");

    attron(COLOR_PAIR(21));
    if (user_mode != 3)
    {
        memset(username, 0, sizeof(username));
        FILE *file = fopen("./.users/Login.txt", "r");
        fscanf(file, "%*d\n%s", username);
        fclose(file);
        char w[MAX_LINE];
        sprintf(w, "Welcome %s!", username);
        mvprintw((scrY / 2) - 5, (scrX / 2) - (strlen(w) / 2), "%s", w);
    }
    else
        mvprintw((scrY / 2) - 5, (scrX / 2) - 4, "welcome!");
    attroff(COLOR_PAIR(21));

    int ch, p = 0;

    while (1)
    {
        attron(COLOR_PAIR(13));
        mvprintw((scrY / 2) + 1, (scrX / 2) - 6, "continue Game");
        mvprintw((scrY / 2) - 1, (scrX / 2) - 4, "New Game");
        mvprintw((scrY / 2) + 3, (scrX / 2) - 5, "Leaderboard");
        mvprintw((scrY / 2) + 5, (scrX / 2) - 3, "Setting");
        mvprintw((scrY / 2) + 7, (scrX / 2) - 3, "Profile");
        mvprintw((scrY / 2) + 9, (scrX / 2) - 2, "Exit");
        attroff(COLOR_PAIR(13));
        if (user_mode == 3)
        {
            attron(COLOR_PAIR(8));
            mvprintw((scrY / 2) + 1, (scrX / 2) - 6, "continue Game");
            attroff(COLOR_PAIR(8));
        }

        // New Game
        if (p == 0)
        {
            attron(COLOR_PAIR(12) | A_BLINK);
            mvprintw((scrY / 2) - 1, (scrX / 2) - 4, "New Game");
            attroff(COLOR_PAIR(12) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    // New_Game(user_mode);
                    mv(user_mode);
                    return 1;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Continue Game
        else if (p == 1)
        {
            attron(COLOR_PAIR(12) | A_BLINK);
            mvprintw((scrY / 2) + 1, (scrX / 2) - 6, "continue Game");
            attroff(COLOR_PAIR(12) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    /* code */
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Leaderboard
        else if (p == 2)
        {
            attron(COLOR_PAIR(12) | A_BLINK);
            mvprintw((scrY / 2) + 3, (scrX / 2) - 5, "Leaderboard");
            attroff(COLOR_PAIR(12) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    leaderBoard(username);
                    clear();
                    refresh();
                    flushinp();
                    if (!user_menu(user_mode))
                        return 0;
                    else
                        return 1;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Setting
        else if (p == 3)
        {
            attron(COLOR_PAIR(12) | A_BLINK);
            mvprintw((scrY / 2) + 5, (scrX / 2) - 3, "Setting");
            attroff(COLOR_PAIR(12) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    game_setting(user_mode, username);
                    clear();
                    refresh();
                    flushinp();
                    if (!user_menu(user_mode))
                        return 0;
                    else
                        return 1;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Profile
        else if (p == 4)
        {
            attron(COLOR_PAIR(12) | A_BLINK);
            mvprintw((scrY / 2) + 7, (scrX / 2) - 3, "Profile");
            attroff(COLOR_PAIR(12) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    profile_menu(user_mode, username);
                    if (!user_menu(user_mode))
                        return 0;
                    else
                        return 1;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Exit
        else if (p == 5)
        {
            attron(COLOR_PAIR(12) | A_BLINK);
            mvprintw((scrY / 2) + 9, (scrX / 2) - 2, "Exit");
            attroff(COLOR_PAIR(12) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    return 0;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        if (ch == KEY_UP)
        {
            p = (p + 5) % 6;
            if (user_mode == 3 && p == 1)
                p--;
        }
        else if (ch == KEY_DOWN || ch == '\n')
        {
            p = (p + 1) % 6;
            if (user_mode == 3 && p == 1)
                p++;
        }
    }
}

void EnterPage()
{
    printf("\e[8;%d;%dt", 40, 150);
    fflush(stdout);

    Mix_Quit();

    Mix_Music *music = Mix_LoadMUS("./.musics/Main Title.mp3");
    Mix_PlayMusic(music, -1);

    clear();
    refresh();
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    printAsciiArt('R', scrX, scrY, 15, 0);
    attron(A_BLINK);
    mvprintw((scrY / 2) + 3, (scrX / 2) - 12, "press eny key to start..");
    printw(".");
    attroff(A_BLINK);
    refresh();
    getch();
    flushinp();
    clear();
}

int main()
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    setlocale(LC_ALL, "");
    initscr();
    // raw();
    cbreak();
    noecho();
    curs_set(0);

    EnterPage();

    int user_mode;
    while (user_mode = RegisterMenu())
    {
        if (!user_menu(user_mode))
            continue;
    }

    SDL_Quit();
    remove_dir("./.users/.~GUEST");
    endwin();
    return 0;
}