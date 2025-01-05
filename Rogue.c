#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h>
#include <unistd.h>

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

void RegisterMenu(WINDOW *win)
{
    curs_set(0);
    int scrX, scrY;
    getmaxyx(win, scrY, scrX);
    WINDOW *menuWin = newwin(scrY, scrX, 0, 0);

    start_color();
    if (has_colors() && can_change_color())
        init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));

    // if ()
    // {
    //     /* code */
    // }
    // mvprintw((scrY / 2) - 2, scrX / 2, "Create your hero for an epic quest.");
    // mvprintw((scrY / 2) - 1, scrX / 2, "(Create New User)");

    mvprintw((scrY / 2) - 9, (scrX / 2) - 28, "  _____  ______ _____ _____  _____ _______ ______ _____");
    mvprintw((scrY / 2) - 8, (scrX / 2) - 28, " |  __ \\|  ____/ ____|_   _|/ ____|__   __|  ____|  __ \\ ");
    mvprintw((scrY / 2) - 7, (scrX / 2) - 28, " | |__) | |__ | |  __  | | | (___    | |  | |__  | |__) |");
    mvprintw((scrY / 2) - 6, (scrX / 2) - 28, " |  _  /|  __|| | |_ | | |  \\___ \\   | |  |  __| |  _  / ");
    mvprintw((scrY / 2) - 5, (scrX / 2) - 28, " | | \\ \\| |___| |__| |_| |_ ____) |  | |  | |____| | \\ \\ ");
    mvprintw((scrY / 2) - 4, (scrX / 2) - 28, " |_|  \\_\\______\\_____|_____|_____/   |_|  |______|_|  \\_\\");

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    attron(A_BLINK | COLOR_PAIR(1));
    mvprintw((scrY / 2), scrX / 2 - 18, "° Create your hero for an epic quest.");
    attroff(A_BLINK | COLOR_PAIR(1));
    mvprintw((scrY / 2) + 1, scrX / 2 - 9, "(Create New User)");
    mvprintw((scrY / 2) + 3, scrX / 2 - 33, "° Your champion awaits in the shadows. Return to claim your destiny.");
    mvprintw((scrY / 2) + 4, scrX / 2 - 6, "(User Login)");
    mvprintw((scrY / 2) + 6, scrX / 2 - 18, "° Enter the Realm as a Wandering Soul");
    mvprintw((scrY / 2) + 7, scrX / 2 - 8, "(Login As Guest)");
    refresh();
    getch();
    clear();
    delwin(menuWin);
    curs_set(1);
}

void EnteroPage(WINDOW *win)
{

    start_color();
    if (has_colors() && can_change_color())
        init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));

    curs_set(0);
    int scrX, scrY;
    getmaxyx(win, scrY, scrX);
    mvprintw((scrY / 2) - 3, (scrX / 2) - 15, " ____   ___   ____ _   _ _____");
    mvprintw((scrY / 2) - 2, (scrX / 2) - 15, "|  _ \\ / _ \\ / ___| | | | ____|");
    mvprintw((scrY / 2) - 1, (scrX / 2) - 15, "| |_) | | | | |  _| | | |  _|  ");
    mvprintw((scrY / 2), (scrX / 2) - 15, "|  _ <| |_| | |_| | |_| | |___ ");
    mvprintw((scrY / 2) + 1, (scrX / 2) - 15, "|_| \\_\\____/ \\____|\\___/|_____|");
    attron(A_BLINK);
    mvprintw((scrY / 2) + 3, (scrX / 2) - 12, "press eny key to start..");
    printw(".");
    attroff(A_BLINK);
    refresh();
    getch();
    clear();
    curs_set(1);
}

// int scrX, scrY;
// getmaxyx(stdscr, scrY, scrX);

int main()
{
    initscr();
    raw();
    noecho();

    EnteroPage(stdscr);
    RegisterMenu(stdscr);
    getch();
    endwin();
    return 0;
}