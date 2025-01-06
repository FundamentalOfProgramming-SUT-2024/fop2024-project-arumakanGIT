#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h>

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

void printAsciiArt(char mode, int scrX, int scrY, int x, int y)
{
    switch (mode)
    {
    case 'r':
        // 28
        mvprintw((scrY / 2) - y - 5, (scrX / 2) - x, "  _____  ______ _____ _____  _____ _______ ______ _____");
        mvprintw((scrY / 2) - y - 4, (scrX / 2) - x, " |  __ \\|  ____/ ____|_   _|/ ____|__   __|  ____|  __ \\ ");
        mvprintw((scrY / 2) - y - 3, (scrX / 2) - x, " | |__) | |__ | |  __  | | | (___    | |  | |__  | |__) |");
        mvprintw((scrY / 2) - y - 2, (scrX / 2) - x, " |  _  /|  __|| | |_ | | |  \\___ \\   | |  |  __| |  _  / ");
        mvprintw((scrY / 2) - y - 1, (scrX / 2) - x, " | | \\ \\| |___| |__| |_| |_ ____) |  | |  | |____| | \\ \\ ");
        mvprintw((scrY / 2) - y, (scrX / 2) - x, " |_|  \\_\\______\\_____|_____|_____/   |_|  |______|_|  \\_\\");
        break;
    case 'R':
        // 15
        mvprintw((scrY / 2) - y - 4, (scrX / 2) - x, " ____   ___   ____ _   _ _____");
        mvprintw((scrY / 2) - y - 3, (scrX / 2) - x, "|  _ \\ / _ \\ / ___| | | | ____|");
        mvprintw((scrY / 2) - y - 2, (scrX / 2) - x, "| |_) | | | | |  _| | | |  _|  ");
        mvprintw((scrY / 2) - y - 1, (scrX / 2) - x, "|  _ <| |_| | |_| | |_| | |___ ");
        mvprintw((scrY / 2) - y, (scrX / 2) - x, "|_| \\_\\____/ \\____|\\___/|_____|");
        break;
    }
}

void UseColor()
{
    start_color();
    if (has_colors() && can_change_color())
        init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
}

int checkMail(char *email)
{
    if (strchr(email, ' '))
        return 0;
    if (!strrchr(email, '@'))
        return 0;
    if (strchr(email, '@') != strrchr(email, '@'))
        return 0;
    if (!strrchr(email, '.'))
        return 0;
    if (strchr(email, '.') != strrchr(email, '.'))
        return 0;
    int a = strchr(email, '@') - email;
    int dot = strchr(email, '.') - email;
    if (!a)
        return 0;
    if (a > dot)
        return 0;
    if (!(dot - a - 1))
        return 0;
    if (!(strlen(email) - dot - 1))
        return 0;
    return 1;
}

void add_New_User()
{
    curs_set(1);
    noecho();
    keypad(stdscr, TRUE);
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    char *username = (char *)malloc(256), *password = (char *)malloc(256), *SHOWpassword = (char *)malloc(256), *email = (char *)malloc(256);
    int checkUserName = 0, checkPassword = 0, checkEmail = 0, choice = 1, create = 0;
    username[0] = '\0';
    password[0] = '\0';
    email[0] = '\0';

    printAsciiArt('r', scrX, scrY, 28, 7);

    mvprintw((scrY / 2) - 4, scrX / 2 - 21, "---Create your hero for an epic quest.---");
    mvprintw((scrY / 2) - 1, scrX / 2 - 11, "Enter your name, hero:");
    mvprintw((scrY / 2) + 2, scrX / 2 - 10, "Enter your password:");
    mvprintw((scrY / 2) + 5, scrX / 2 - 9, "Enter your email:");
    // mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Create");
    // mvprintw((scrY / 2) + 9, scrX / 2 - 3, "Cancel");
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    char ch;
    while (1)
    {
        if (choice == 1)
        {
            // USER NAME

            attron(COLOR_PAIR(1));
            // ch = ' ';
            move((scrY / 2), scrX / 2 - 1);
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 8, 0);
                clrtoeol();
                move((scrY / 2) + 9, 0);
                clrtoeol();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(username) != 0)
                    username[strlen(username) - 1] = '\0';
                else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
                {
                    username[strlen(username)] = ch;
                    username[strlen(username)] = '\0';
                }
                else if (ch == '\n' || ch == KEY_UP || ch == KEY_DOWN)
                {
                    if (!strlen(username))
                    {
                        checkUserName = 0;
                        create = 0;
                        move((scrY / 2) + 8, 0);
                        clrtoeol();
                        move((scrY / 2) + 9, 0);
                        clrtoeol();
                        attroff(COLOR_PAIR(1));
                        mvprintw((scrY / 2) + 9, (scrX / 2) - 9, "---Enter your Name---");
                        attron(COLOR_PAIR(1));
                        ch = ' ';
                    }
                    else
                    {
                        checkUserName = 1;
                        if (checkEmail && checkPassword && checkUserName)
                            create = 1;
                        break;
                    }
                }
                else
                {
                    move((scrY / 2) + 8, 0);
                    clrtoeol();
                    move((scrY / 2) + 9, 0);
                    clrtoeol();
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 8, (scrX / 2) - 20, "---You can only use uppercase letters,---");
                    mvprintw((scrY / 2) + 9, (scrX / 2) - 19, "---lowercase letters, and UnderLine.---");
                    attron(COLOR_PAIR(1));
                }

                move((scrY / 2), 0);
                clrtoeol();
                mvprintw((scrY / 2), (scrX / 2) - 1 - (strlen(username) / 2), "%s", username);
                refresh();
            }
            move((scrY / 2) + 8, 0);
            clrtoeol();
            move((scrY / 2) + 9, 0);
            clrtoeol();
            attroff(COLOR_PAIR(1));
            if (create)
            {
                mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Create");
                mvprintw((scrY / 2) + 9, scrX / 2 - 3, "Cancel");
            }
        }
        else if (choice == 2)
        {

            // PASSWORD

            attron(COLOR_PAIR(1));
            move((scrY / 2) + 3, scrX / 2 - 1);
            // ch = ' ';
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 8, 0);
                clrtoeol();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(password) != 0)
                {
                    password[strlen(password) - 1] = '\0';
                    SHOWpassword[strlen(password) - 1] = '\0';
                }
                else if (ch >= '!' && ch <= '~')
                {
                    password[strlen(password)] = ch;
                    password[strlen(password)] = '\0';
                    SHOWpassword[strlen(SHOWpassword)] = '*';
                    SHOWpassword[strlen(SHOWpassword)] = '\0';
                }
                else if (ch == '\n' || ch == KEY_UP || ch == KEY_DOWN)
                {
                    if (strlen(password) >= 7)
                    {
                        checkPassword = 1;
                        if (checkPassword && checkEmail && checkUserName)
                            create = 1;
                        break;
                    }
                    else
                    {
                        checkPassword = 0;
                        create = 0;
                        move((scrY / 2) + 8, 0);
                        clrtoeol();
                        attroff(COLOR_PAIR(1));
                        mvprintw((scrY / 2) + 8, (scrX / 2) - 24, "The password is too short. (Minimum 7 characters)");
                        attron(COLOR_PAIR(1));
                        ch = ' ';
                    }
                }
                else
                {
                    move((scrY / 2) + 8, 0);
                    clrtoeol();
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 8, (scrX / 2) - 9, "---Invalid input---");
                    attron(COLOR_PAIR(1));
                }

                move((scrY / 2) + 3, 0);
                clrtoeol();
                mvprintw((scrY / 2) + 3, (scrX / 2) - 1 - (strlen(SHOWpassword) / 2), "%s", SHOWpassword);
                refresh();
            }
            move((scrY / 2) + 8, 0);
            clrtoeol();
            attroff(COLOR_PAIR(1));
            if (create)
            {
                mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Create");
                mvprintw((scrY / 2) + 9, scrX / 2 - 3, "Cancel");
            }
        }
        else if (choice == 3)
        {

            // EMAIL

            attron(COLOR_PAIR(1));
            // ch = ' ';
            move((scrY / 2) + 6, scrX / 2 - 1);
            while (ch != '\n')
            {
                ch = getch();

                move((scrY / 2) + 8, 0);
                clrtoeol();
                move((scrY / 2) + 9, 0);
                clrtoeol();
                move((scrY / 2) + 10, 0);
                clrtoeol();
                move((scrY / 2) + 11, 0);
                clrtoeol();

                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(email) != 0)
                    email[strlen(email) - 1] = '\0';
                else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || ch == '@' || ch == '.' || ch == '!' || ch == '#' || ch == '$' || ch == '%' || ch == '&' || ch == '*' || ch == '+' || ch == '-' || ch == '\'' || ch == '=' || ch == '?' || ch == '^' || ch == '`' || ch == '{' || ch == '}' || ch == '|')
                {
                    email[strlen(email)] = ch;
                    email[strlen(email)] = '\0';
                }
                else if (ch == '\n' || ch == KEY_UP || ch == KEY_DOWN)
                {
                    if (checkMail(email))
                    {
                        checkEmail = 1;
                        if (checkEmail && checkPassword && checkUserName)
                            create = 1;
                        break;
                    }
                    else
                    {
                        checkEmail = 0;
                        create = 0;
                        move((scrY / 2) + 8, 0);
                        clrtoeol();
                        move((scrY / 2) + 9, 0);
                        clrtoeol();
                        move((scrY / 2) + 10, 0);
                        clrtoeol();
                        move((scrY / 2) + 11, 0);
                        clrtoeol();
                        attroff(COLOR_PAIR(1));
                        mvprintw((scrY / 2) + 8, (scrX / 2) - 14, "The email you entered is incorrect.");
                        attron(COLOR_PAIR(1));
                        ch = ' ';
                    }
                }
                else
                {
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 8, (scrX / 2) - 14, "Email can only contain numbers,");
                    mvprintw((scrY / 2) + 9, (scrX / 2) - 14, "uppercase and lowercase letters,");
                    mvprintw((scrY / 2) + 10, (scrX / 2) - 12, "and the following characters:");
                    mvprintw((scrY / 2) + 11, (scrX / 2) - 17, " ـ ! # $ %% & \' * + - = ? ^ ` { } | ");
                    attron(COLOR_PAIR(1));
                }

                move((scrY / 2) + 6, 0);
                clrtoeol();
                mvprintw((scrY / 2) + 6, (scrX / 2) - 1 - (strlen(email) / 2), "%s", email);
                refresh();
            }
            move((scrY / 2) + 8, 0);
            clrtoeol();
            move((scrY / 2) + 9, 0);
            clrtoeol();
            move((scrY / 2) + 10, 0);
            clrtoeol();
            move((scrY / 2) + 11, 0);
            clrtoeol();
            attroff(COLOR_PAIR(1));
            if (create)
            {
                mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Create");
                mvprintw((scrY / 2) + 9, scrX / 2 - 3, "Cancel");
            }
        }
        else if (choice == 4)
        {
            // Create
        }
        else if (choice == 5)
        {
            // Cancel
        }

        if (ch == KEY_UP)
        {
            if (create)
                choice = (choice + 4) % 5;
            else
                choice = (choice + 2) % 3;
        }
        else if (ch == KEY_DOWN)
        {
            if (create)
                choice = (choice + 1) % 5;
            else
                choice = (choice + 1) % 3;
        }
        else if (ch == '\n')
        {
            if (choice == 1)
            {
                /* code */
            }
                }
    }

    getch();
    clear();
    curs_set(0);
}

int RegisterMenu()
{
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    UseColor();

    DIR *dir = opendir("./users");
    if (dir)
    {
    }

    // if ()
    // {
    //     /* code */
    // }

    printAsciiArt('r', scrX, scrY, 28, 4);
    refresh();
    keypad(stdscr, TRUE);

    int choice = 0;
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

        switch (choice)
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
            choice = (choice + 3) % 4;
            break;
        case KEY_DOWN:
        case 's':
            choice = (choice + 1) % 4;
            break;
        case '\n':
            switch (choice)
            {
            case 1:
                refresh();
                clear();
                add_New_User();
                return 1;
                break;
            case 2:
                /* code */
                return 2;
                break;
            case 3:
                /* code */
                return 3;
                break;
            case 0:
                ch = 'q';
                clear();
                return 0;
                break;
            }
            break;
        }
        refresh();
        if (ch == 'q')
            break;
    }
}

void EnteroPage()
{
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
    clear();
}

int main()
{
    initscr();
    // raw();
    noecho();
    curs_set(0);

    // EnteroPage(stdscr);
    // RegisterMenu(stdscr);
    add_New_User();

    getch();
    endwin();
    return 0;
}