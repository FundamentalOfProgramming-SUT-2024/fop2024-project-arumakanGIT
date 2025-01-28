#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
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

/*
random password is ready
*/

int email_exist(char email[MAX_NAMES])
{
    FILE *read;
    read = fopen("./.users/Login.txt", "r");
    int users_count;
    fscanf(read, "%d\n", &users_count);
    for (int i = 0; i < users_count; i++)
    {
        char currentEmail[MAX_NAMES];
        fscanf(read, "%*S %*S %s\n", currentEmail);
        if (strcmp(currentEmail, email) == 0)
            return 1;
    }
    fclose(read);
    return 0;
}

void delay(int milli_seconds)
{
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;
}

int random_num(int min, int max)
{
    // srand(time(NULL)); اینو یادت نره استفاده کنی
    delay(35000);
    return rand() % (max - min + 1) + min;
}

char *random_pass()
{
    int pun, num, up, low, length;
    length = random_num(9, 12);
    pun = random_num(1, 2);
    num = random_num(1, 3);
    up = random_num(1, 3);
    low = length - pun - num - up;
    char *pass = (char *)malloc(length + 1);
    int ps[length];
    for (int i = 0; i < length; i++)
        ps[i] = 0;

    for (int i = 0; i < pun; i++)
    {
        char a = (char)random_num(33, 47);
        int p = random_num(0, length);
        while (ps[p])
            p = random_num(0, length);
        ps[p] = 1;
        pass[p] = a;
    }

    for (int i = 0; i < num; i++)
    {
        char a = (char)random_num(48, 57);
        int p = random_num(0, length);
        while (ps[p])
            p = random_num(0, length);
        ps[p] = 1;
        pass[p] = a;
    }

    for (int i = 0; i < up; i++)
    {
        char a = (char)random_num(65, 90);
        int p = random_num(0, length);
        while (ps[p])
            p = random_num(0, length);
        ps[p] = 1;
        pass[p] = a;
    }

    for (int i = 0; i < low; i++)
    {
        char a = (char)random_num(97, 122);
        int p = random_num(0, length);
        while (ps[p])
            p = random_num(0, length);
        ps[p] = 1;
        pass[p] = a;
    }

    pass[length] = '\0';
    return pass;
}

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
    // start color
    // set black color
    // back ground = black & foreground = white
    // code 1 = inverse background
    // code 3 = back text -> red
    start_color();
    if (has_colors() && can_change_color())
        init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_RED);
}

int checkMail(char *email)
{
    // if correct return 1 else return 0

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

void toLowerCase(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
}

int checksum(char buffer[MAX_LINE])
{
    int sum = 0;
    int len = strlen(buffer);
    for (int i = 0; i < len; i++)
        sum += (int)buffer[i];
    int temp = sum & 0xFF;
    sum = ((sum >> 8) & 0xFF) | (temp << 8);
    return sum & 0xFFFF;
}

char *encrypt(char buffer[MAX_NAMES], char key[MAX_NAMES])
{
    char *encMsg = (char *)malloc(strlen(buffer) * 2 + 50);
    int key_len = strlen(key);
    int buffer_len = strlen(buffer);
    for (int i = 0; i < (buffer_len / key_len) + 1; i++)
    {
        char part[key_len + 1];
        memcpy(part, &buffer[i * key_len], key_len);
        part[key_len] = '\0';
        for (int j = i * key_len; j < (i + 1) * key_len && j < buffer_len; j++)
        {
            char a[3] = "";
            int x = (int)key[j % key_len];
            int y = buffer[j];
            sprintf(a, "%X", (x ^ y));
            if (strlen(a) == 1)
            {
                a[1] = a[0];
                a[0] = '0';
            }
            a[2] = '\0';
            strcat(encMsg, a);
        }
    }
    char ccc[5];
    ccc[4] = '\0';
    if (checksum(buffer) == 0)
        strcpy(ccc, "0000");
    else
        sprintf(ccc, "%04X", checksum(buffer));
    strcat(encMsg, ccc);
    encMsg[strlen(encMsg)] = '\0';
    toLowerCase(encMsg);
    return encMsg;
}

int checkKEY(int sum, char key[5])
{
    int num = (int)strtol(key, NULL, 16);
    if (sum == num)
        return 0;
    return 1;
}

char *decrypt(char buffer[1024], char key[512])
{
    char *encMsg = (char *)malloc(strlen(buffer));
    char keyvalue[5];
    memcpy(keyvalue, &buffer[strlen(buffer) - 4], 4);
    keyvalue[5] = '\0';
    buffer[strlen(buffer) - 4] = '\0';

    int count = 0;
    for (int i = 0; i < (int)strlen(buffer); i += 2)
    {
        char a[] = {buffer[i], buffer[i + 1], '\0'};
        int decimal = (int)strtol(a, NULL, 16);
        decimal = decimal ^ key[(i / 2) % strlen(key)];
        encMsg[count++] = (char)decimal;
    }
    encMsg[count] = '\0';
    if (checkKEY(checksum(encMsg), keyvalue))
        return "Invalid key!";
    return encMsg;
}

void addToUserInfo(char username[MAX_NAMES])
{
    FILE *read;
    read = fopen("./.users/Users.txt", "r");
    int users_count, last;
    fscanf(read, "%d %d\n", &users_count, &last);
    int history[users_count], golds[users_count];
    char names[users_count][MAX_NAMES];
    for (int i = 0; i < users_count; i++)
    {
        fscanf(read, "%d %s %d\n", &history[i], names[i], &golds[i]);
        history[i]++;
    }
    fclose(read);

    FILE *write;
    write = fopen("./.users/Users.txt", "w");
    fprintf(write, "%d %d\n", users_count + 1, last);
    fprintf(write, "%d %s %d\n", 1, username, 0);
    for (int i = 0; i < users_count; i++)
        fprintf(write, "%d %s %d\n", history[i], names[i], golds[i]);
    fclose(write);
}

void addToLogin(char username[MAX_NAMES], char password[MAX_NAMES], char email[MAX_NAMES])
{
    FILE *read;
    int users_count;
    read = fopen("./.users/Login.txt", "r");
    fscanf(read, "%d\n", &users_count);
    char un[users_count][MAX_NAMES], pw[users_count][MAX_NAMES], em[users_count][MAX_NAMES];
    for (int i = 0; i < users_count; i++)
        fscanf(read, "%s %s %s\n", un[i], pw[i], em[i]);
    fclose(read);
    FILE *write;
    write = fopen("./.users/Login.txt", "w");
    fprintf(write, "%d\n", users_count + 1);
    for (int i = 0; i < users_count; i++)
        fprintf(write, "%s %s %s\n", un[i], pw[i], em[i]);
    fprintf(write, "%s %s %s\n", username, encrypt(password, username), email);
}

void securityQ(char path[MAX_LINE])
{
    noecho();
    keypad(stdscr, TRUE);
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    mvprintw((scrY / 2) - 2, (scrX / 2) - 10, "Security questions :");
    mvprintw((scrY / 2) - 1, (scrX / 2) - 25, "What is the name of your pet?");
    mvprintw((scrY / 2), (scrX / 2) - 25, "What is the name of your best friend?");
    mvprintw((scrY / 2) + 1, (scrX / 2) - 25, "What was the name of your school?");
    mvprintw((scrY / 2) + 2, (scrX / 2) - 25, "What is your favorite movie?");
    mvprintw((scrY / 2) + 4, (scrX / 2) - 2, "Done");
    int ch;
    int p = 0;
    char pet[MAX_NAMES] = "", bf[MAX_NAMES] = "", school[MAX_NAMES] = "", movie[MAX_NAMES] = "";
    while (1)
    {
        mvprintw((scrY / 2) + 4, (scrX / 2) - 2, "Done");
        curs_set(1);
        if (p == 0)
        {
            move((scrY / 2) - 1, (scrX / 2) + 16 + strlen(pet));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~')
                    pet[strlen(pet)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    pet[strlen(pet) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2) - 1, (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2) - 1, (scrX / 2) + 16, "%s", pet);
                refresh();
            }
        }
        else if (p == 1)
        {
            move((scrY / 2), (scrX / 2) + 16 + strlen(bf));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~')
                    bf[strlen(bf)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    bf[strlen(bf) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2), (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2), (scrX / 2) + 16, "%s", bf);
                refresh();
            }
        }
        else if (p == 2)
        {
            move((scrY / 2) + 1, (scrX / 2) + 16 + strlen(school));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~')
                    school[strlen(school)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    school[strlen(school) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2) + 1, (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2) + 1, (scrX / 2) + 16, "%s", school);
                refresh();
            }
        }
        else if (p == 3)
        {
            move((scrY / 2) + 2, (scrX / 2) + 16 + strlen(movie));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~')
                    movie[strlen(movie)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    movie[strlen(movie) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2) + 2, (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2) + 2, (scrX / 2) + 16, "%s", movie);
                refresh();
            }
        }
        else if (p == 4)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 4, (scrX / 2) - 2, "Done");
            attroff(COLOR_PAIR(1) | A_BLINK);
            ch = getch();
            if (ch == '\n' && strlen(pet) && strlen(bf) && strlen(movie) && strlen(school))
            {
                FILE *write;
                char newPath[MAX_LINE + 7];
                sprintf(newPath, "%s/sq.txt", path);
                write = fopen(newPath, "w");
                fprintf(write, "%s\n%s\n%s\n%s\n", pet, bf, school, movie);
                clear();
                refresh();
                break;
            }
            else
                mvprintw((scrY / 2) + 6, (scrX / 2) - 16, "Please answer all the questions.");
        }

        if (ch == KEY_UP)
            p = (p + 4) % 5;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 6) % 5;
    }

    clear();
    refresh();
}

void log_in()
{
    noecho();
    keypad(stdscr, TRUE);
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);

    int users_count, ch, p = 0, big = 0;

    char players[3][MAX_NAMES], username[MAX_NAMES], password[MAX_NAMES], SHOWpassword[MAX_NAMES];
    strcpy(username, "\0");
    strcpy(password, "\0");
    strcpy(SHOWpassword, "\0");

    FILE *read;
    read = fopen("./.users/Users.txt", "r");
    fscanf(read, "%d %*S\n", &users_count);
    for (int i = 0; i < 3; i++)
        strcpy(players[i], "\0");

    for (int i = 0; i < users_count && i < 3; i++)
    {
        fscanf(read, "%*S %s %*S\n", players[i]);
        if (strlen(players[i]) > big)
            big = strlen(players[i]);
    }
    fclose(read);

    mvprintw((scrY / 2) - 10, (scrX / 2) - 17, "---------- Last Players: ----------");
    mvprintw((scrY / 2) - 9, (scrX / 2) - 17, "|                                 |");
    mvprintw((scrY / 2) - 8, (scrX / 2) - 17, "|                                 |");
    mvprintw((scrY / 2) - 7, (scrX / 2) - 17, "|                                 |");
    mvprintw((scrY / 2) - 6, (scrX / 2) - 17, "|                                 |");
    mvprintw((scrY / 2) - 5, (scrX / 2) - 17, "|                                 |");
    mvprintw((scrY / 2) - 4, (scrX / 2) - 17, "-----------------------------------");
    mvprintw((scrY / 2) - 1, (scrX / 2) - 14, "Email or Name : ");
    mvprintw((scrY / 2) + 2, (scrX / 2) - 14, "Password : ");

    while (1)
    {
        mvprintw((scrY / 2) - 8, (scrX / 2) - (big / 2) - 1, "1- %s", players[0]);
        mvprintw((scrY / 2) - 7, (scrX / 2) - (big / 2) - 1, "2- %s", players[1]);
        mvprintw((scrY / 2) - 6, (scrX / 2) - (big / 2) - 1, "3- %s", players[2]);
        mvprintw((scrY / 2) + 3, (scrX / 2) - 7, "forgot password");
        mvprintw((scrY / 2) + 7, (scrX / 2) - 3, "Log in");

        if (p == 0)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) - 8, (scrX / 2) - (big / 2) - 1, "1- %s", players[0]);
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    strcpy(username, players[0]);
                    move((scrY / 2) - 1, (scrX / 2) + 2);
                    clrtoeol();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) - 1, (scrX / 2) + 2, "%s", username);
                    attroff(COLOR_PAIR(3));
                    refresh();
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }
        else if (p == 1)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) - 7, (scrX / 2) - (big / 2) - 1, "2- %s", players[1]);
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    strcpy(username, players[1]);
                    move((scrY / 2) - 1, (scrX / 2) + 2);
                    clrtoeol();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) - 1, (scrX / 2) + 2, "%s", username);
                    attroff(COLOR_PAIR(3));
                    refresh();
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }
        else if (p == 2)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) - 6, (scrX / 2) - (big / 2) - 1, "3- %s", players[2]);
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    strcpy(username, players[2]);
                    move((scrY / 2) - 1, (scrX / 2) + 2);
                    clrtoeol();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) - 1, (scrX / 2) + 2, "%s", username);
                    attroff(COLOR_PAIR(3));
                    refresh();
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }
        else if (p == 3)
        {
            curs_set(1);
            move((scrY / 2) - 1, (scrX / 2) + 2 + strlen(username));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(username))
                    username[strlen(username) - 1] = '\0';
                else if (ch >= '!' && ch <= '~')
                    username[strlen(username)] = (char)ch;
                else if (ch == KEY_DOWN || ch == KEY_UP || ch == '\n')
                    break;

                move((scrY / 2) - 1, (scrX / 2) + 2);
                clrtoeol();
                attron(COLOR_PAIR(3));
                mvprintw((scrY / 2) - 1, (scrX / 2) + 2, "%s", username);
                attroff(COLOR_PAIR(3));
                refresh();
            }
            curs_set(0);
        }
        else if (p == 4)
        {
            curs_set(1);
            move((scrY / 2) + 2, (scrX / 2) + 2 + strlen(SHOWpassword));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(SHOWpassword))
                {
                    password[strlen(password) - 1] = '\0';
                    SHOWpassword[strlen(SHOWpassword) - 1] = '\0';
                }
                else if (ch >= '!' && ch <= '~')
                {
                    password[strlen(password)] = (char)ch;
                    SHOWpassword[strlen(SHOWpassword)] = '*';
                }
                else if (ch == KEY_DOWN || ch == KEY_UP || ch == '\n')
                    break;

                move((scrY / 2) + 2, (scrX / 2) + 2);
                clrtoeol();
                attron(COLOR_PAIR(3));
                mvprintw((scrY / 2) + 2, (scrX / 2) + 2, "%s", SHOWpassword);
                attroff(COLOR_PAIR(3));
                refresh();
            }
            curs_set(0);
        }
        else if (p == 5)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 3, (scrX / 2) - 7, "forgot password");
            attroff(COLOR_PAIR(1) | A_BLINK);
            ch = getch();
            if (ch == '\n')
            {
                clear();
                refresh();
                // securityQ();
                break;
            }
        }
        else if (p == 6)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 7, (scrX / 2) - 3, "Log in");
            attroff(COLOR_PAIR(1) | A_BLINK);
            getch();
        }

        if (ch == KEY_UP)
            p = (p + 6) % 7;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 1) % 7;
    }
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

        mvprintw((scrY / 2) + 8, (scrX / 2) - 7, "Random Password");
        mvprintw((scrY / 2) + 10, scrX / 2 - 3, "Cancel");
        mvprintw((scrY / 2) + 12, scrX / 2 - 3, "Create");

        if (choice == 0)
        {
            // USER NAME

            attron(COLOR_PAIR(1));
            move((scrY / 2), scrX / 2 + (strlen(username) / 2));
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 14, 0);
                clrtoeol();
                move((scrY / 2) + 15, 0);
                clrtoeol();
                move((scrY / 2) + 16, 0);
                clrtoeol();
                move((scrY / 2) + 17, 0);
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
                        mvprintw((scrY / 2) + 14, (scrX / 2) - 6, "Enter your Name");
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
                    mvprintw((scrY / 2) + 14, (scrX / 2) - 17, "You can only use uppercase letters,");
                    mvprintw((scrY / 2) + 15, (scrX / 2) - 16, "lowercase letters, and UnderLine.");
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

            attron(COLOR_PAIR(1));
            move((scrY / 2) + 3, scrX / 2 + (strlen(SHOWpassword) / 2));
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 14, 0);
                clrtoeol();
                move((scrY / 2) + 15, 0);
                clrtoeol();
                move((scrY / 2) + 16, 0);
                clrtoeol();
                move((scrY / 2) + 17, 0);
                clrtoeol();

                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(password) != 0)
                {
                    password[strlen(password) - 1] = '\0';
                    SHOWpassword[strlen(SHOWpassword) - 1] = '\0';
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
                        mvprintw((scrY / 2) + 14, (scrX / 2) - 24, "The password is too short. (Minimum 7 characters)");
                        attron(COLOR_PAIR(1));
                    }
                    break;
                }
                else
                {
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 10, (scrX / 2) - 6, "Invalid input");
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

            attron(COLOR_PAIR(1));
            move((scrY / 2) + 6, scrX / 2 + (strlen(email) / 2));
            while (1)
            {
                ch = getch();

                move((scrY / 2) + 14, 0);
                clrtoeol();
                move((scrY / 2) + 15, 0);
                clrtoeol();
                move((scrY / 2) + 16, 0);
                clrtoeol();
                move((scrY / 2) + 17, 0);
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
                        mvprintw((scrY / 2) + 14, (scrX / 2) - 17, "The email you entered is incorrect.");
                        attron(COLOR_PAIR(1));
                    }
                    break;
                }
                else
                {
                    attroff(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 14, (scrX / 2) - 14, "Email can only contain numbers,");
                    mvprintw((scrY / 2) + 15, (scrX / 2) - 14, "uppercase and lowercase letters,");
                    mvprintw((scrY / 2) + 16, (scrX / 2) - 12, "and the following characters:");
                    mvprintw((scrY / 2) + 17, (scrX / 2) - 17, " ـ ! # $ %% & \' * + - = ? ^ ` { } | ");
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
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 8, (scrX / 2) - 7, "Random Password");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    move((scrY / 2) + 14, 0);
                    clrtoeol();
                    move((scrY / 2) + 15, 0);
                    clrtoeol();
                    move((scrY / 2) + 16, 0);
                    clrtoeol();
                    move((scrY / 2) + 17, 0);
                    clrtoeol();
                    char rp[MAX_NAMES];
                    strcpy(rp, random_pass());
                    mvprintw((scrY / 2) + 14, (scrX / 2) - 5, "%s", rp);

                    strcpy(password, rp);
                    strcpy(SHOWpassword, "\0");
                    for (int i = 0; i < strlen(password); i++)
                        SHOWpassword[i] = '*';
                    SHOWpassword[strlen(SHOWpassword)] = '\0';

                    move((scrY / 2) + 3, 0);
                    clrtoeol();
                    attron(COLOR_PAIR(1));
                    mvprintw((scrY / 2) + 3, (scrX / 2) - 1 - (strlen(SHOWpassword) / 2), "%s", SHOWpassword);
                    attroff(COLOR_PAIR(1));
                    refresh();

                    checkPassword = 1;
                    if (checkPassword && checkUserName && checkEmail)
                        create = 1;
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
        }
        else if (choice == 4)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 10, scrX / 2 - 3, "Cancel");
            attroff(COLOR_PAIR(1) | A_BLINK);
            ch = getch();
            if (ch == '\n')
            {
                clear();
                break;
            }
        }
        else if (choice == 5)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 12, (scrX / 2) - 3, "Create");
            attroff(COLOR_PAIR(1) | A_BLINK);
            ch = getch();
            if (ch == '\n')
            {
                char path[MAX_LINE];
                snprintf(path, MAX_LINE, "./.users/%s", username);
                DIR *dir = opendir(path);
                if (email_exist(email))
                {
                    mvprintw((scrY / 2) + 14, (scrX / 2) - 17, "This email has already been created.");
                    ch = getch();
                }
                else if (!dir)
                {
                    mkdir(path, 0777);
                    dir = opendir(path);
                    FILE *user;
                    char filePathUserInfo[MAX_LINE + 14];
                    snprintf(filePathUserInfo, MAX_LINE + 14, "%s/UserInfo.txt", path);
                    user = fopen(filePathUserInfo, "w");
                    fprintf(user, "%s\n%s\n%s\n%s", username, password, encrypt(password, username), email);
                    fclose(user);
                    addToUserInfo(username);
                    addToLogin(username, password, email);
                    clear();
                    refresh();
                    securityQ(path);
                    break;
                }
                else
                {
                    mvprintw((scrY / 2) + 14, (scrX / 2) - 17, "This name has already been created.");
                    ch = getch();
                }
            }
        }

        if (ch == KEY_UP)
        {
            if (create)
                choice = (choice + 5) % 6;
            else
                choice = (choice + 4) % 5;
        }
        else if (ch == KEY_DOWN || ch == '\n')
        {
            if (create)
                choice = (choice + 1) % 6;
            else
                choice = (choice + 1) % 5;
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

    FILE *Users;
    Users = fopen("./.users/Users.txt", "r");
    if (!Users)
    {
        Users = fopen("./.users/Users.txt", "w");
        fprintf(Users, "0 0\n");
    }
    fclose(Users);

    FILE *Login;
    Login = fopen("./.users/Login.txt", "r");
    if (!Login)
    {
        Login = fopen("./.users/Login.txt", "w");
        fprintf(Login, "0\n");
    }
    fclose(Login);

    struct dirent *input;
    FILE *fileinput;
    int users = 0;

    while ((input = readdir(dir)) != NULL)
    {
        if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
            continue;
        users++;
    }

    if (users == 2)
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
                refresh();
                clear();
                log_in();
                return 2;
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
    clear();
    refresh();
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
    srand(time(NULL));
    initscr();
    // raw();
    noecho();
    curs_set(0);

    EnterPage();
    RegisterMenu();
    // securityQ();

    endwin();
    return 0;
}