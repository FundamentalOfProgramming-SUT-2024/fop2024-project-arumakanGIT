#include <signal.h>
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

void add_New_User()
{
}

void RegisterMenu()
{
    curs_set(0);
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
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

    refresh();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    int choise = 1;
    int ch;
    while (1)
    {

        mvprintw((scrY / 2), scrX / 2 - 18, "° Create your hero for an epic quest.");
        mvprintw((scrY / 2) + 1, scrX / 2 - 9, "(Create New User)");
        mvprintw((scrY / 2) + 3, scrX / 2 - 33, "° Your champion awaits in the shadows. Return to claim your destiny.");
        mvprintw((scrY / 2) + 4, scrX / 2 - 6, "(User Login)");
        mvprintw((scrY / 2) + 6, scrX / 2 - 18, "° Enter the Realm as a Wandering Soul");
        mvprintw((scrY / 2) + 7, scrX / 2 - 8, "(Login As Guest)");
        mvprintw((scrY / 2) + 9, scrX / 2 - 2, "EXIT");

        refresh();

        switch (choise)
        {
        case 1:
            attron(A_BLINK | COLOR_PAIR(1));
            mvprintw((scrY / 2), scrX / 2 - 18, "° Create your hero for an epic quest.");
            attroff(A_BLINK | COLOR_PAIR(1));
            break;
        case 2:
            attron(A_BLINK | COLOR_PAIR(1));
            mvprintw((scrY / 2) + 3, scrX / 2 - 33, "° Your champion awaits in the shadows. Return to claim your destiny.");
            attroff(A_BLINK | COLOR_PAIR(1));
            break;
        case 3:
            attron(A_BLINK | COLOR_PAIR(1));
            mvprintw((scrY / 2) + 6, scrX / 2 - 18, "° Enter the Realm as a Wandering Soul");
            attroff(A_BLINK | COLOR_PAIR(1));
            break;
        case 0:
            attron(A_BLINK | COLOR_PAIR(1));
            mvprintw((scrY / 2) + 9, scrX / 2 - 2, "EXIT");
            attroff(A_BLINK | COLOR_PAIR(1));
            break;
        }

        refresh();

        ch = getch();
        switch (ch)
        {
        case KEY_UP:
        case 'w':
            choise = (choise + 3) % 4;
            break;
        case KEY_DOWN:
        case 's':
            choise = (choise + 1) % 4;
            break;
        case '\n':
            switch (choise)
            {
            case 1:
                add_New_User();
                break;
            case 2:
                /* code */
                break;
            case 3:
                /* code */
                break;
            case 0:
                ch = 'q';
                break;
            }
            break;
        }
        refresh();
        if (ch == 'q')
            break;
    }

    refresh();
    clear();
    // getch();
    curs_set(1);
}

void EnteroPage()
{
    start_color();
    if (has_colors() && can_change_color())
        init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));

    curs_set(0);
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
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
    // raw();
    noecho();
    keypad(stdscr, TRUE);

    EnteroPage(stdscr);
    RegisterMenu(stdscr);

    getch();
    endwin();
    return 0;
}