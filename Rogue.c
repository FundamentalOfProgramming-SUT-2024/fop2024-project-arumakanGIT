#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_NAMES 256
#define MAX_LINE 1024

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
    init_pair(3, COLOR_BLACK, COLOR_RED);
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

void updateUserInfo(char username[MAX_NAMES], int newH, int newGold)
{
    FILE *file;
    file = fopen("./.users/Users.txt", "r");
    int users_count, last;
    fscanf(file, "%d %d\n", &users_count, &last);

    if (strcmp(username, "") || !username)
        users_count++;

    int history[users_count], golds[users_count];
    char names[users_count][MAX_NAMES];

    for (int i = 0; i < users_count - 1; i++)
    {
        int h, g;
        char name[MAX_NAMES];
        fscanf(file, "%*S %d %s %d\n", &h, name, &g);
        strcpy(names[i], name);
        history[i] = h;
        golds[i] = g;
    }

    if (strcmp(username, "") || !username)
    {
        last = users_count;
        history[users_count - 1] = newH;
        golds[users_count - 1] = newGold;
        strcpy(names[users_count - 1], username);
    }

    if (last)
    {
        last--;
        for (int i = 0; i < users_count; i++)
            if (history[i] < history[last])
                history[i] += 1;
        history[last] = 1;
    }

    for (int i = 0; i < users_count - 1; i++)
    {
        for (int j = i + 1; j < users_count; j++)
        {
            if (golds[i] < golds[j])
            {
                int th, tg;
                char t[MAX_NAMES];
                strcpy(t, names[i]);
                th = history[i];
                tg = golds[i];
                strcpy(names[i], names[j]);
                golds[i] = golds[j];
                history[i] = history[j];
                strcpy(names[j], t);
                golds[j] = tg;
                history[j] = th;
            }
        }
    }
    fclose(file);

    FILE *user;
    user = fopen("./.users/Users.txt", "w");
    fprintf(user, "%d %d\n", users_count, 0);
    for (int i = 1; i <= users_count; i++)
        fprintf(user, "%d %d %s %d\n", i, history[i - 1], names[i - 1], golds[i - 1]);
}

void add_New_User()
{
    noecho();
    keypad(stdscr, TRUE);
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    char *username = (char *)malloc(MAX_NAMES), *password = (char *)malloc(MAX_NAMES), *SHOWpassword = (char *)malloc(MAX_NAMES), *email = (char *)malloc(MAX_NAMES);

    int checkUserName = 0, checkPassword = 0, checkEmail = 0, choice = 0, create = 0;

    username[0] = '\0';
    password[0] = '\0';
    email[0] = '\0';

    printAsciiArt('r', scrX, scrY, 28, 7);

    mvprintw((scrY / 2) - 4, scrX / 2 - 21, "---Create your hero for an epic quest.---");
    mvprintw((scrY / 2) - 1, scrX / 2 - 11, "Enter your name, hero:");
    mvprintw((scrY / 2) + 2, scrX / 2 - 10, "Enter your password:");
    mvprintw((scrY / 2) + 5, scrX / 2 - 9, "Enter your email:");

    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    int ch;
    while (1)
    {
        curs_set(1);
        if (choice == 0)
        {
            // USER NAME

            move((scrY / 2) + 8, 0);
            clrtoeol();
            move((scrY / 2) + 10, 0);
            clrtoeol();
            mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Cancel");
            mvprintw((scrY / 2) + 10, scrX / 2 - 3, "Create");

            attron(COLOR_PAIR(1));
            move((scrY / 2), scrX / 2 + (strlen(username) / 2));
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 12, 0);
                clrtoeol();
                move((scrY / 2) + 13, 0);
                clrtoeol();
                move((scrY / 2) + 14, 0);
                clrtoeol();
                move((scrY / 2) + 15, 0);
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
                        attroff(COLOR_PAIR(1));
                        mvprintw((scrY / 2) + 12, (scrX / 2) - 6, "Enter your Name");
                        attron(COLOR_PAIR(1));
                    }
                    else
                    {
                        checkUserName = 1;
                        if (checkPassword && checkUserName && checkEmail)
                            create = 1;
                    }
                    break;
                }
                else
                {
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 12, (scrX / 2) - 17, "You can only use uppercase letters,");
                    mvprintw((scrY / 2) + 13, (scrX / 2) - 16, "lowercase letters, and UnderLine.");
                    attron(COLOR_PAIR(1));
                }

                if (checkPassword && checkUserName && checkEmail)
                    create = 1;

                move((scrY / 2), 0);
                clrtoeol();
                mvprintw((scrY / 2), (scrX / 2) - 1 - (strlen(username) / 2), "%s", username);
                refresh();
            }

            attroff(COLOR_PAIR(1));
        }
        else if (choice == 1)
        {

            // PASSWORD

            move((scrY / 2) + 8, 0);
            clrtoeol();
            move((scrY / 2) + 10, 0);
            clrtoeol();
            mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Cancel");
            mvprintw((scrY / 2) + 10, scrX / 2 - 3, "Create");

            attron(COLOR_PAIR(1));
            move((scrY / 2) + 3, scrX / 2 + (strlen(SHOWpassword) / 2));
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 12, 0);
                clrtoeol();
                move((scrY / 2) + 13, 0);
                clrtoeol();
                move((scrY / 2) + 14, 0);
                clrtoeol();
                move((scrY / 2) + 15, 0);
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
                        if (checkPassword && checkUserName && checkEmail)
                            create = 1;
                    }
                    else
                    {
                        checkPassword = 0;
                        create = 0;
                        attroff(COLOR_PAIR(1));
                        mvprintw((scrY / 2) + 12, (scrX / 2) - 24, "The password is too short. (Minimum 7 characters)");
                        attron(COLOR_PAIR(1));
                    }
                    break;
                }
                else
                {
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 8, (scrX / 2) - 6, "Invalid input");
                    attron(COLOR_PAIR(1));
                }

                if (checkPassword && checkUserName && checkEmail)
                    create = 1;

                move((scrY / 2) + 3, 0);
                clrtoeol();
                mvprintw((scrY / 2) + 3, (scrX / 2) - 1 - (strlen(SHOWpassword) / 2), "%s", SHOWpassword);
                refresh();
            }
            attroff(COLOR_PAIR(1));
        }
        else if (choice == 2)
        {

            // EMAIL

            move((scrY / 2) + 8, 0);
            clrtoeol();
            move((scrY / 2) + 10, 0);
            clrtoeol();
            mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Cancel");
            mvprintw((scrY / 2) + 10, scrX / 2 - 3, "Create");

            attron(COLOR_PAIR(1));
            move((scrY / 2) + 6, scrX / 2 + (strlen(email) / 2));
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 12, 0);
                clrtoeol();
                move((scrY / 2) + 13, 0);
                clrtoeol();
                move((scrY / 2) + 14, 0);
                clrtoeol();
                move((scrY / 2) + 15, 0);
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
                        if (checkPassword && checkUserName && checkEmail)
                            create = 1;
                    }
                    else
                    {
                        checkEmail = 0;
                        create = 0;
                        attroff(COLOR_PAIR(1));
                        mvprintw((scrY / 2) + 12, (scrX / 2) - 17, "The email you entered is incorrect.");
                        attron(COLOR_PAIR(1));
                    }
                    break;
                }
                else
                {
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 12, (scrX / 2) - 14, "Email can only contain numbers,");
                    mvprintw((scrY / 2) + 13, (scrX / 2) - 14, "uppercase and lowercase letters,");
                    mvprintw((scrY / 2) + 14, (scrX / 2) - 12, "and the following characters:");
                    mvprintw((scrY / 2) + 15, (scrX / 2) - 17, " ـ ! # $ %% & \' * + - = ? ^ ` { } | ");
                    attron(COLOR_PAIR(1));
                }

                if (checkPassword && checkUserName && checkEmail)
                    create = 1;

                move((scrY / 2) + 6, 0);
                clrtoeol();
                mvprintw((scrY / 2) + 6, (scrX / 2) - 1 - (strlen(email) / 2), "%s", email);
                refresh();
            }
            attroff(COLOR_PAIR(1));
        }
        else if (choice == 3)
        {
            curs_set(0);
            mvprintw((scrY / 2) + 10, scrX / 2 - 3, "Create");
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Cancel");
            attroff(COLOR_PAIR(1) | A_BLINK);
            ch = getch();
            if (ch == '\n')
            {
                clear();
                break;
            }
        }
        else if (choice == 4)
        {

#pragma region Editing here

            mvprintw((scrY / 2) + 8, scrX / 2 - 3, "Cancel");
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 10, scrX / 2 - 3, "Create");
            attroff(COLOR_PAIR(1) | A_BLINK);
            ch = getch();
            if (ch == '\n')
            {
                char path[MAX_LINE];
                snprintf(path, MAX_LINE, "./.users/%s", username);
                DIR *dir = opendir(path);
                if (!dir)
                {
                    mkdir(path, 0777);
                    dir = opendir(path);
                    FILE *user;
                    char filePathUserInfo[MAX_LINE + 14];
                    snprintf(filePathUserInfo, MAX_LINE + 14, "%s/UserInfo.txt", path);
                    user = fopen(filePathUserInfo, "w");
                    fprintf(user, "%s\n%s\n%s", username, password, email);
                    fclose(user);
                    // FILE *file = fopen("./.users/Users.txt", "r+");
                    // int users_count;
                    // fscanf(file, "%d %*S\n", &users_count);
                    // rewind(file);
                    // users_count++;
                    // fprintf(file, "%d %d\n", users_count, users_count);
                    // fseek(file, 0, SEEK_END);
                    // fprintf(file, "%d %d %s %d\n", users_count, 0, username, 0);
                    // fclose(file);
                    updateUserInfo(username, 0, 0);
                    clear();
                    break;
                }
                else
                {
                    mvprintw((scrY / 2) + 12, (scrX / 2) - 17, "This name has already been created.");
                    ch = getch();
                }
            }
        }

        if (ch == KEY_UP)
        {
            if (create)
                choice = (choice + 4) % 5;
            else
                choice = (choice + 3) % 4;
        }
        else if (ch == KEY_DOWN || ch == '\n')
        {
            if (create)
                choice = (choice + 1) % 5;
            else
                choice = (choice + 1) % 4;
        }
    }
}

int RegisterMenu()
{
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    UseColor();
    DIR *dir = opendir("./.users");
    if (!dir)
    {
        mkdir("./.users", 0777);
        dir = opendir("./.users");
    }

    FILE *file;
    file = fopen("./.users/Users.txt", "w");
    if (file != NULL)
        fprintf(file, "0 0\n");
    fclose(file);

    struct dirent *input;
    FILE *fileinput;
    int users = 0;
    while ((input = readdir(dir)) != NULL)
    {
        if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
            continue;
        users++;
    }
    printw("%d", users);

    if (users == 0)
    {
        refresh();
        clear();
        mvprintw((scrY / 2) - 5, (scrX / 2) - 4, "Welcome!");
        add_New_User();
        return 1;
    }

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
                // return 2;
                break;
            case 3:
                // return 3;
                break;
            case 0:
                clear();
                return 0;
                break;
            }
            break;
        }
        refresh();
    }
}

void EnterPage()
{
    UseColor();
    noecho();
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

    EnterPage();
    RegisterMenu();
    // add_New_User();

    endwin();
    return 0;
}