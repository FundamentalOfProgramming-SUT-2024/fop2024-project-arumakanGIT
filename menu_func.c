#include "menu_func.h"
// default
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
// direction
#include <dirent.h>
#include <sys/stat.h>
// Time
#include <time.h>
#include <unistd.h>
// emoji
#include <locale.h>

#define MAX_NAMES 256
#define MAX_LINE 1024
#define LIMIT_INPUT 25


void remove_dir(const char *path)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (!dir)
    {
        perror("Error opening directory");
        return;
    }

    char filepath[1024];

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
        struct stat st;
        if (stat(filepath, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                remove_dir(filepath);
            }
            else
            {
                remove(filepath);
            }
        }
    }

    closedir(dir);
    rmdir(path);
}

char *setare(char buffer[MAX_NAMES])
{
    char *setareh = (char *)malloc(MAX_NAMES);
    memset(setareh, 0, sizeof(setareh));
    for (int i = 0; i < strlen(buffer); i++)
        setareh[i] = '*';
    setareh[strlen(setareh)] = '\0';
    return setareh;
}

void delay(int milli_seconds)
{
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;
}

int random_num(int min, int max)
{
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
    encMsg[0] = '\0';
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
            int str_len = strlen(encMsg);
            encMsg[str_len] = a[0];
            encMsg[str_len + 1] = a[1];
            encMsg[str_len + 2] = '\0';
        }
    }
    char ccc[5];
    ccc[4] = '\0';
    if (checksum(buffer) == 0)
        strcpy(ccc, "0000");
    else
        sprintf(ccc, "%04X", checksum(buffer));

    int str_len = strlen(encMsg);
    encMsg[str_len] = ccc[0];
    encMsg[str_len + 1] = ccc[1];
    encMsg[str_len + 2] = ccc[2];
    encMsg[str_len + 3] = ccc[3];
    encMsg[str_len + 4] = '\0';
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

int email_exist(char email[MAX_NAMES])
{
    /*
    0 = not exist
    1 = exist
    */
    FILE *r;
    r = fopen("./.users/Login.txt", "r");
    int users_count;
    fscanf(r, "%d\n", &users_count);
    for (int i = 0; i < users_count; i++)
    {
        char currentEmail[MAX_NAMES];
        fscanf(r, "%*S %*S %s\n", currentEmail);
        if (strcmp(currentEmail, email) == 0)
            return 1;
    }
    fclose(r);
    return 0;
}

int username_exist(char username[MAX_NAMES])
{
    /*
    0 = not exist
    1 = exist
    */
    FILE *r;
    r = fopen("./.users/Login.txt", "r");
    int users_count;
    fscanf(r, "%d\n", &users_count);
    for (int i = 0; i < users_count; i++)
    {
        char currentUsername[MAX_NAMES];
        fscanf(r, "%s %*S %*S\n", currentUsername);
        if (strcmp(currentUsername, username) == 0)
            return 1;
    }
    fclose(r);
    return 0;
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

void printBoard(int Ypadding, int Xpadding, int h, int w)
{
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    int x = scrX / 2, y = scrY / 2;

    char line[w + 1], row[w + 1];
    line[w] = '\0';
    row[w] = '\0';
    memset(line, '-', w);
    memset(row, ' ', w);

    row[0] = '|';
    row[w - 1] = '|';

    mvprintw(y - (h / 2) + Ypadding, x - (w / 2) + Xpadding, "%s", line);
    mvprintw(y - (h / 2) + Ypadding + h - 1, x - (w / 2) + Xpadding, "%s", line);
    int yy = y - (h / 2) + Ypadding + 1;
    for (int i = 0; i < h - 2; i++)
        mvprintw(yy + i, x - (w / 2) + Xpadding, "%s", row);
}

void EditUserInfo(char username[MAX_NAMES])
{
    FILE *read;
    read = fopen("./.users/Login.txt", "r");
    int users_count;
    fscanf(read, "%d\n", &users_count);
    int target_index;
    char names[users_count][MAX_NAMES], password[users_count][MAX_NAMES], email[users_count][MAX_NAMES];
    for (int i = 0; i < users_count; i++)
    {
        fscanf(read, "%s %s %s\n", names[i], password[i], email[i]);
        if (strcmp(names[i], username) == 0)
            target_index = i;
    }
    fclose(read);

    FILE *write;
    write = fopen("./.users/Login.txt", "w");
    fprintf(write, "%d\n", users_count);
    fprintf(write, "%s %s %s\n", names[target_index], password[target_index], email[target_index]);
    for (int i = 0; i < users_count; i++)
        if (i != target_index)
            fprintf(write, "%s %s %s\n", names[i], password[i], email[i]);
    fclose(write);
}

void addToLogin(char username[MAX_NAMES], char password[MAX_NAMES], char email[MAX_NAMES])
{
    FILE *read;
    read = fopen("./.users/Login.txt", "r");
    int users_count;
    fscanf(read, "%d\n", &users_count);
    char un[users_count][MAX_NAMES], pw[users_count][MAX_NAMES], em[users_count][MAX_NAMES];
    for (int i = 0; i < users_count; i++)
        fscanf(read, "%s %s %s\n", un[i], pw[i], em[i]);
    fclose(read);

    FILE *write;
    write = fopen("./.users/Login.txt", "w");
    fprintf(write, "%d\n", users_count + 1);
    fprintf(write, "%s %s %s\n", username, encrypt(password, username), email);
    for (int i = 0; i < users_count; i++)
        fprintf(write, "%s %s %s\n", un[i], pw[i], em[i]);
    fclose(write);
}

void addToLeaderBoard(char username[MAX_NAMES])
{
    FILE *r = fopen("./.users/LeaderBoard.txt", "r");
    int users_count, target = -1;
    fscanf(r, "%d", &users_count);
    int big = 0, golds[users_count], score[users_count], games[users_count], ex[users_count];
    char names[users_count][MAX_NAMES];

    for (int i = 0; i < users_count; i++)
    {
        fscanf(r, "%s %d %d %d %d\n", names[i], &golds[i], &score[i], &games[i], &ex[i]);
        if (strlen(names[i]) > big)
            big = strlen(names[i]);
        if (strcmp(username, names[i]) == 0)
            target = i;
    }
    fclose(r);

    for (int i = 0; i < users_count - 1; i++)
        for (int j = i + 1; j < users_count; j++)
            if (golds[j] > golds[i])
            {
                char n[MAX_NAMES];
                int t;
                strcpy(n, names[j]);
                strcpy(names[j], names[i]);
                strcpy(names[i], n);

                t = golds[j];
                golds[i] = golds[j];
                golds[i] = t;

                t = score[j];
                score[i] = score[j];
                score[j] = t;

                t = games[j];
                games[i] = games[j];
                games[j] = t;

                t = ex[j];
                ex[i] = ex[j];
                ex[j] = ex[i];
            }

    if (target == -1)
    {
        FILE *w = fopen("./.users/LeaderBoard.txt", "w");
        fprintf(w, "%d\n", users_count + 1);
        for (int i = 0; i < users_count; i++)
            fprintf(w, "%s %d %d %d %d\n", names[i], golds[i], score[i], games[i], ex[i]);
        fprintf(w, "%s %d %d %d %d\n", username, 0, 0, 0, 0);
        fclose(w);
    }
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
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    cbreak();
    start_color();
    if (has_colors() && can_change_color())
    {
        init_color(COLOR_BLACK, 0, 0, 0);
        init_color(8, 128, 128, 128);

        init_color(9, 1000, 843, 0);
        init_color(10, 750, 750, 750);
        init_color(11, 804, 498, 196);
        init_color(12, 381, 201, 30);
        init_color(13, 347, 112, 419);
        init_color(14, 0, 387, 0);
    }

    // inverse
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    // default
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));
    // red inverse
    init_pair(3, COLOR_WHITE, COLOR_RED);
    // red
    init_pair(4, COLOR_RED, COLOR_BLACK);
    // gray
    init_pair(8, 8, COLOR_BLACK);
    // gold
    init_pair(9, 9, COLOR_BLACK);
    // silver
    init_pair(10, 10, COLOR_BLACK);
    // bronze
    init_pair(11, 11, COLOR_BLACK);
    // cyan inverse
    init_pair(12, COLOR_BLACK, COLOR_CYAN);
    // cyan
    init_pair(13, COLOR_CYAN, COLOR_BLACK);

    // gold revers
    init_pair(5, COLOR_BLACK, 9);
    // Blue
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    // Green inverse
    init_pair(7, COLOR_BLACK, COLOR_GREEN);

    // Red
    init_pair(20, COLOR_RED, COLOR_BLACK);
    // Green
    init_pair(21, COLOR_GREEN, COLOR_BLACK);
    // Blue
    init_pair(22, COLOR_BLUE, COLOR_BLACK);
    // Yellow
    init_pair(23, COLOR_YELLOW, COLOR_BLACK);
    // Brown
    init_pair(24, 12, COLOR_BLACK);
    // purple
    init_pair(25, 13, COLOR_BLACK);
}

int search_login(char username[MAX_NAMES], char password[MAX_NAMES])
{
    /*
    0 = not found
    1 = correct password
    2 = wrong password
    */
    if (checkMail(username))
    {
        FILE *file = fopen("./.users/Login.txt", "r");
        int user;
        fscanf(file, "%d\n", &user);
        for (int i = 0; i < user; i++)
        {
            char un[MAX_NAMES], email[MAX_NAMES];
            fscanf(file, "%s %*S %s", un, email);
            if (strcmp(email, username) == 0)
            {
                strcpy(username, un);
                break;
            }
        }
        fclose(file);
    }

    FILE *read;
    read = fopen("./.users/Login.txt", "r");
    int target = -1, users_count;
    fscanf(read, "%d\n", &users_count);
    char usernames[users_count][MAX_NAMES], passwords[users_count][MAX_NAMES], emails[users_count][MAX_NAMES];
    for (int i = 0; i < users_count; i++)
    {
        fscanf(read, "%s %s %s\n", usernames[i], passwords[i], emails[i]);
        if (strcmp(username, usernames[i]) == 0)
        {
            target = i;
            break;
        }
    }
    fclose(read);

    if (target == -1)
        return 0;
    else
    {
        char encrypted_pass[MAX_NAMES];
        strcpy(encrypted_pass, encrypt(password, username));
        if (strcmp(encrypted_pass, passwords[target]) == 0)
            return 1;
        else
            return 2;
    }
}

void new_pass(char username[MAX_NAMES])
{
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    mvprintw((scrY / 2) - 3, (scrX / 2) - 10, "Enter New Password : ");
    int ch, p = 0;
    char password[MAX_NAMES];
    memset(password, 0, sizeof(password));
    password[0] = '\0';
    while (1)
    {
        curs_set(0);
        mvprintw((scrY / 2) + 5, (scrX / 2) - 3, "Cancel");
        mvprintw((scrY / 2) + 1, (scrX / 2) - 8, "random password");
        mvprintw((scrY / 2) + 3, (scrX / 2) - 2, "Done");

        // password
        if (p == 0)
        {
            move((scrY / 2) + 7, 0);
            clrtoeol();

            curs_set(1);
            move((scrY / 2) - 1, (scrX / 2) + (strlen(password) / 2));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(password))
                    password[strlen(password) - 1] = '\0';
                else if (ch >= '!' && ch <= '~' && strlen(password) < LIMIT_INPUT)
                {
                    password[strlen(password)] = (char)ch;
                    password[strlen(password)] = '\0';
                }
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2) - 1, 0);
                clrtoeol();
                attron(COLOR_PAIR(3));
                mvprintw((scrY / 2) - 1, (scrX / 2) - (strlen(password) / 2), "%s", setare(password));
                attroff(COLOR_PAIR(3));
                refresh();
            }
        }

        // random password
        else if (p == 1)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 1, (scrX / 2) - 8, "random password");
            attroff(COLOR_PAIR(1) | A_BLINK);

            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    strcpy(password, random_pass());

                    move((scrY / 2) - 1, 0);
                    clrtoeol();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) - 1, (scrX / 2) - (strlen(password) / 2), "%s", password);
                    attroff(COLOR_PAIR(3));
                    refresh();
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // done
        else if (p == 2)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 3, (scrX / 2) - 2, "Done");
            attroff(COLOR_PAIR(1) | A_BLINK);

            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    if (search_login(username, password) == 1)
                    {
                        move((scrY / 2) + 7, 0);
                        clrtoeol();
                        mvprintw((scrY / 2) + 7, (scrX / 2) - 15, "The password is already taken.");
                    }
                    else if (strlen(password) >= 7)
                    {
                        FILE *read;
                        read = fopen("./.users/Login.txt", "r");
                        int users_count, target_index = -1;
                        fscanf(read, "%d\n", &users_count);
                        char names[users_count][MAX_NAMES], emails[users_count][MAX_NAMES], passwords[users_count][MAX_NAMES];
                        for (int i = 0; i < users_count; i++)
                        {
                            fscanf(read, "%s %s %s\n", names[i], passwords[i], emails[i]);
                            if (strcmp(names[i], username) == 0)
                                target_index = i;
                        }
                        fclose(read);

                        if (target_index != -1)
                        {
                            strcpy(passwords[target_index], encrypt(password, username));
                            FILE *write;
                            write = fopen("./.users/Login.txt", "w");
                            fprintf(write, "%d\n", users_count);
                            for (int i = 0; i < users_count; i++)
                                fprintf(write, "%s %s %s\n", names[i], passwords[i], emails[i]);
                            fclose(write);

                            char path[MAX_LINE];
                            sprintf(path, "./.users/%s/UserInfo.txt", username);
                            FILE *read2;
                            read2 = fopen(path, "r");
                            char data[3][MAX_NAMES];
                            for (int i = 0; i < 3; i++)
                                fscanf(read2, "%s\n", data[i]);
                            fclose(read2);

                            strcpy(data[1], encrypt(password, username));

                            FILE *write2;
                            write2 = fopen(path, "w");
                            for (int i = 0; i < 3; i++)
                                fprintf(write2, "%s\n", data[i]);
                            fclose(write2);

                            clear();
                            refresh();
                            return;
                        }
                        return;
                    }
                    else
                    {
                        move((scrY / 2) + 7, 0);
                        clrtoeol();
                        mvprintw((scrY / 2) + 7, (scrX / 2) - 25, "Please enter a password with at least 7 characters.");
                    }
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Cancel
        else if (p == 3)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 5, (scrX / 2) - 3, "Cancel");
            attroff(COLOR_PAIR(1) | A_BLINK);

            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    refresh();
                    clear();
                    return;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        if (ch == KEY_UP)
            p = (p + 3) % 4;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 1) % 4;
    }
}

void securityQ(char username[MAX_NAMES])
{
    char path[MAX_LINE + 16];
    if (checkMail(username))
        sprintf(path, "./.users/%s/sq.txt", username);
    else
    {
        FILE *file = fopen("./.users/Login.txt", "r");
        int user;
        fscanf(file, "%d\n", &user);
        for (int i = 0; i < user; i++)
        {
            char un[MAX_NAMES], email[MAX_NAMES];
            fscanf(file, "%s %*S %s", un, email);
            if (strcmp(email, username) == 0)
            {
                strcpy(username, un);
                break;
            }
        }
        fclose(file);
        sprintf(path, "./.users/%s/sq.txt", username);
    }

    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    mvprintw((scrY / 2) - 4, (scrX / 2) - 10, "Security questions :");
    mvprintw((scrY / 2) - 3, (scrX / 2) - 25, "What is the name of your pet?");
    mvprintw((scrY / 2) - 2, (scrX / 2) - 25, "What is the name of your best friend?");
    mvprintw((scrY / 2) - 1, (scrX / 2) - 25, "What was the name of your school?");
    mvprintw((scrY / 2), (scrX / 2) - 25, "What is your favorite movie?");
    mvprintw((scrY / 2) + 2, (scrX / 2) - 2, "Done");

    int ch, p = 0;
    char pet[MAX_NAMES], bf[MAX_NAMES], school[MAX_NAMES], movie[MAX_NAMES];

    memset(pet, 0, sizeof(pet));
    memset(bf, 0, sizeof(bf));
    memset(school, 0, sizeof(school));
    memset(movie, 0, sizeof(movie));

    while (1)
    {
        mvprintw((scrY / 2) + 4, (scrX / 2) - 3, "Cancel");
        mvprintw((scrY / 2) + 2, (scrX / 2) - 2, "Done");
        curs_set(1);
        // pet
        if (p == 0)
        {
            move((scrY / 2) - 3, (scrX / 2) + 16 + strlen(pet));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~' && strlen(pet) < LIMIT_INPUT)
                    pet[strlen(pet)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    pet[strlen(pet) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2) - 3, (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2) - 3, (scrX / 2) + 16, "%s", pet);
                refresh();
            }
        }

        // bf
        else if (p == 1)
        {
            move((scrY / 2) - 2, (scrX / 2) + 16 + strlen(bf));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~' && strlen(bf) < LIMIT_INPUT)
                    bf[strlen(bf)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    bf[strlen(bf) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2) - 2, (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2) - 2, (scrX / 2) + 16, "%s", bf);
                refresh();
            }
        }

        // school
        else if (p == 2)
        {
            move((scrY / 2) - 1, (scrX / 2) + 16 + strlen(school));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~' && strlen(school) < LIMIT_INPUT)
                    school[strlen(school)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    school[strlen(school) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2) - 1, (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2) - 1, (scrX / 2) + 16, "%s", school);
                refresh();
            }
        }

        // movie
        else if (p == 3)
        {
            move((scrY / 2), (scrX / 2) + 16 + strlen(movie));
            while (1)
            {
                ch = getch();
                if (ch >= '!' && ch <= '~' && strlen(movie) < LIMIT_INPUT)
                    movie[strlen(movie)] = (char)ch;
                else if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(pet) != 0)
                    movie[strlen(movie) - 1] = '\0';
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;

                move((scrY / 2), (scrX / 2) + 16);
                clrtoeol();
                mvprintw((scrY / 2), (scrX / 2) + 16, "%s", movie);
                refresh();
            }
        }

        // done
        else if (p == 4)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 2, (scrX / 2) - 2, "Done");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    if (access(path, F_OK) == 0)
                    {
                        char answers[4][MAX_NAMES];
                        FILE *r = fopen(path, "r");
                        for (int i = 0; i < 4; i++)
                            fscanf(r, "%s\n", answers[i]);
                        fclose(r);
                        if (!strcmp(pet, answers[0]) && !strcmp(bf, answers[1]) && !strcmp(school, answers[2]) && !strcmp(movie, answers[3]))
                        {
                            clear();
                            refresh();
                            new_pass(username);
                            clear();
                            refresh();
                            return;
                        }
                        else
                        {
                            move((scrY / 2) + 6, 0);
                            clrtoeol();
                            mvprintw((scrY / 2) + 6, (scrX / 2) - 9, "Something is wrong");
                        }
                    }
                    else
                    {
                        if (strlen(pet) && strlen(movie) && strlen(bf) && strlen(school))
                        {
                            FILE *write = fopen(path, "w");
                            fprintf(write, "%s\n%s\n%s\n%s", pet, bf, school, movie);
                            fclose(write);
                            clear();
                            refresh();
                            return;
                        }
                        else
                        {
                            move((scrY / 2) + 6, 0);
                            clrtoeol();
                            mvprintw((scrY / 2) + 6, (scrX / 2) - 15, "Please answer all the questions.");
                        }
                    }
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Cancel
        else if (p == 5)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 4, (scrX / 2) - 3, "Cancel");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n' && access(path, F_OK) == 0)
                {
                    refresh();
                    clear();
                    return;
                }
                else if (ch == '\n' && access(path, F_OK) != 0)
                {
                    move((scrY / 2) + 6, 0);
                    clrtoeol();
                    mvprintw((scrY / 2) + 6, (scrX / 2) - 15, "You must answer the questions.");
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        if (ch == KEY_UP)
            p = (p + 5) % 6;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 1) % 6;
    }

    clear();
    refresh();
}

int log_in()
{
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);

    int users_count, ch, p = 0, big = 0;

    char players[3][MAX_NAMES], username[MAX_NAMES], password[MAX_NAMES], SHOWpassword[MAX_NAMES];
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    memset(SHOWpassword, 0, sizeof(SHOWpassword));
    strcpy(username, "\0");
    strcpy(password, "\0");
    SHOWpassword[0] = '\0';

    FILE *read;
    read = fopen("./.users/Login.txt", "r");
    fscanf(read, "%d\n", &users_count);

    for (int i = 0; i < 3; i++)
    {
        memset(players[i], 0, sizeof(players[i]));
        strcpy(players[i], "\0");
    }

    for (int i = 0; i < users_count && i < 3; i++)
    {
        fscanf(read, "%s %*S %*S\n", players[i]);
        if (strlen(players[i]) > big)
            big = strlen(players[i]);
    }
    fclose(read);

    attron(COLOR_PAIR(9) | A_BOLD);
    mvprintw((scrY / 2) - 12, (scrX / 2) - 7, "Last Players :");
    attroff(COLOR_PAIR(9) | A_BOLD);
    attron(COLOR_PAIR(8));
    printBoard(-7, 0, 7, 36);
    attroff(COLOR_PAIR(8));
    attron(COLOR_PAIR(13) | A_BOLD);
    mvprintw((scrY / 2) - 1, (scrX / 2) - 14, "Email or Name : ");
    mvprintw((scrY / 2) + 1, (scrX / 2) - 14, "Password : ");
    attroff(COLOR_PAIR(13) | A_BOLD);

    while (1)
    {
        attron(COLOR_PAIR(21));
        mvprintw((scrY / 2) - 8, (scrX / 2) - (big / 2) - 1, "1- %s", players[0]);
        mvprintw((scrY / 2) - 7, (scrX / 2) - (big / 2) - 1, "2- %s", players[1]);
        mvprintw((scrY / 2) - 6, (scrX / 2) - (big / 2) - 1, "3- %s", players[2]);
        attroff(COLOR_PAIR(21));

        mvprintw((scrY / 2) + 3, (scrX / 2) - 7, "forgot password");
        mvprintw((scrY / 2) + 6, (scrX / 2) - 3, "Log in");
        mvprintw((scrY / 2) + 8, (scrX / 2) - 3, "Cancel");

        // player 1
        if (p == 0)
        {
            curs_set(0);
            attron(COLOR_PAIR(7) | A_BLINK);
            mvprintw((scrY / 2) - 8, (scrX / 2) - (big / 2) - 1, "1- %s", players[0]);
            attroff(COLOR_PAIR(7) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n' && strlen(players[0]))
                {
                    strcpy(username, players[0]);
                    move((scrY / 2) - 1, (scrX / 2) + 2);
                    clrtoeol();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) - 1, (scrX / 2) + 2, "%s", username);
                    attroff(COLOR_PAIR(3));
                    refresh();
                    mvprintw((scrY / 2) - 8, (scrX / 2) - (big / 2) - 1, "1- %s", players[0]);
                    move((scrY / 2) + 1, (scrX / 2) + 2 + strlen(password));
                    p = 3;
                    curs_set(1);
                    break;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // player 2
        else if (p == 1)
        {
            curs_set(0);
            attron(COLOR_PAIR(7) | A_BLINK);
            mvprintw((scrY / 2) - 7, (scrX / 2) - (big / 2) - 1, "2- %s", players[1]);
            attroff(COLOR_PAIR(7) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n' && strlen(players[1]))
                {
                    strcpy(username, players[1]);
                    move((scrY / 2) - 1, (scrX / 2) + 2);
                    clrtoeol();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) - 1, (scrX / 2) + 2, "%s", username);
                    attroff(COLOR_PAIR(3));
                    refresh();
                    mvprintw((scrY / 2) - 7, (scrX / 2) - (big / 2) - 1, "2- %s", players[1]);
                    move((scrY / 2) + 1, (scrX / 2) + 2 + strlen(password));
                    p = 3;
                    curs_set(1);
                    break;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // player 3
        else if (p == 2)
        {
            curs_set(0);
            attron(COLOR_PAIR(7) | A_BLINK);
            mvprintw((scrY / 2) - 6, (scrX / 2) - (big / 2) - 1, "3- %s", players[2]);
            attroff(COLOR_PAIR(7) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n' && strlen(players[2]))
                {
                    strcpy(username, players[2]);
                    move((scrY / 2) - 1, (scrX / 2) + 2);
                    clrtoeol();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) - 1, (scrX / 2) + 2, "%s", username);
                    attroff(COLOR_PAIR(3));
                    refresh();
                    mvprintw((scrY / 2) - 6, (scrX / 2) - (big / 2) - 1, "3- %s", players[2]);
                    move((scrY / 2) + 1, (scrX / 2) + 2 + strlen(password));
                    p = 3;
                    curs_set(1);
                    break;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // username
        else if (p == 3)
        {
            curs_set(1);
            move((scrY / 2) - 1, (scrX / 2) + 2 + strlen(username));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(username))
                    username[strlen(username) - 1] = '\0';
                else if (ch >= '!' && ch <= '~' && strlen(username) < LIMIT_INPUT)
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

        // password
        else if (p == 4)
        {
            curs_set(1);
            move((scrY / 2) + 1, (scrX / 2) + 2 + strlen(password));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(password))
                {
                    password[strlen(password) - 1] = '\0';
                    SHOWpassword[strlen(SHOWpassword) - 1] = '\0';
                }
                else if (ch > 32 && ch < 127 && strlen(username) < LIMIT_INPUT)
                {
                    password[strlen(password)] = (char)ch;
                    SHOWpassword[strlen(SHOWpassword)] = '*';
                    SHOWpassword[strlen(SHOWpassword)] = '\0';
                }
                else if (ch == KEY_DOWN || ch == KEY_UP || ch == '\n')
                    break;

                move((scrY / 2) + 1, (scrX / 2) + 2);
                clrtoeol();
                attron(COLOR_PAIR(4));
                mvprintw((scrY / 2) + 1, (scrX / 2) + 2, "%s", SHOWpassword);
                attroff(COLOR_PAIR(4));
                refresh();
            }
            curs_set(0);
        }

        // forgot password
        else if (p == 5)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 3, (scrX / 2) - 7, "forgot password");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();

                if (ch == '\n' && (strlen(username) == 0 || username_exist(username) == 0 || email_exist(username)))
                {
                    move((scrY / 2) + 11, 0);
                    clrtoeol();
                    attron(COLOR_PAIR(4));
                    mvprintw((scrY / 2) + 10, (scrX / 2) - 23, "There is no user with this username or email");
                    attron(COLOR_PAIR(3));
                }
                else if (ch == '\n' && (username_exist(username) || email_exist(username)))
                {
                    clear();
                    refresh();
                    flushinp();
                    securityQ(username);
                    if (log_in())
                        return 1;
                    else
                        return 0;
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
        }

        // login
        else if (p == 6)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 6, (scrX / 2) - 3, "Log in");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();

                if (ch == '\n' && (strlen(username) == 0 || (username_exist(username) == 0 && email_exist(username) == 0)))
                {
                    move((scrY / 2) + 10, 0);
                    clrtoeol();
                    attron(COLOR_PAIR(4));
                    mvprintw((scrY / 2) + 10, (scrX / 2) - 23, "There is no user with this username or email");
                    attroff(COLOR_PAIR(4));
                }
                else if (ch == '\n' && (username_exist(username) || email_exist(username)))
                {
                    int check = search_login(username, password);
                    if (check == 1)
                    {
                        EditUserInfo(username);
                        clear();
                        refresh();
                        return 1;
                    }
                    else if (check == 2)
                    {
                        move((scrY / 2) + 10, 0);
                        clrtoeol();
                        attron(COLOR_PAIR(4));
                        mvprintw((scrY / 2) + 10, (scrX / 2) - 7, "Wrong password!");
                        attroff(COLOR_PAIR(4));
                    }
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
        }

        // Cancel
        else if (p == 7)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 8, (scrX / 2) - 3, "Cancel");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    return 0;
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
        }

        if (ch == KEY_UP)
            p = (p + 7) % 8;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 1) % 8;
    }
}

int add_New_User()
{
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    printAsciiArt('r', scrX, scrY, 28, 7);
    attron(COLOR_PAIR(9) | A_BOLD);
    mvprintw((scrY / 2) - 4, (scrX / 2) - 17, "Create your hero for an epic quest.");
    attroff(COLOR_PAIR(9) | A_BOLD);
    attron(COLOR_PAIR(8));
    mvprintw((scrY / 2) - 3, (scrX / 2) - 31, "--------------------------------------------------------------");
    mvprintw((scrY / 2) - 2, (scrX / 2) - 31, "|                                                            |");
    // mvprintw((scrY / 2) - 1, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2), (scrX / 2) - 31, "|                                                            |");
    // mvprintw((scrY / 2) + 1, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2) + 2, (scrX / 2) - 31, "|                                                            |");
    // mvprintw((scrY / 2) + 3, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2) + 4, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2) + 5, (scrX / 2) - 31, "--------------------------------------------------------------");
    attroff(COLOR_PAIR(8));
    attron(COLOR_PAIR(13) | A_BOLD);
    mvprintw((scrY / 2) - 1, (scrX / 2) - 25, "Enter your name, hero : ");
    mvprintw((scrY / 2) + 1, (scrX / 2) - 25, "Enter your password : ");
    mvprintw((scrY / 2) + 3, (scrX / 2) - 25, "Enter your email : ");
    attroff(COLOR_PAIR(13) | A_BOLD);

    int ch, p = 0;
    char username[MAX_NAMES], password[MAX_NAMES], email[MAX_NAMES], SHOWpassword[MAX_NAMES];
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    memset(email, 0, sizeof(email));
    memset(SHOWpassword, 0, sizeof(SHOWpassword));
    username[0] = '\0';
    email[0] = '\0';
    password[0] = '\0';
    SHOWpassword[0] = '\0';

    while (1)
    {
        mvprintw((scrY / 2) + 7, (scrX / 2) - 8, "Random Password");
        mvprintw((scrY / 2) + 9, (scrX / 2) - 3, "Cancel");
        mvprintw((scrY / 2) + 11, (scrX / 2) - 3, "Create");

        // User Name
        if (p == 0)
        {
            curs_set(1);
            move((scrY / 2) - 1, (scrX / 2) + 3 + strlen(username));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(username))
                    username[strlen(username) - 1] = '\0';
                else if (ch >= '!' && ch < '~' && strlen(username) < LIMIT_INPUT)
                {
                    username[strlen(username)] = (char)ch;
                    username[strlen(username)] = '\0';
                }
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                {
                    if (!strlen(username))
                    {
                        move((scrY / 2) + 13, 0);
                        clrtoeol();
                        attron(COLOR_PAIR(20));
                        mvprintw((scrY / 2) + 13, (scrX / 2) - 9, "Please enter a name.");
                        attroff(COLOR_PAIR(20));
                    }
                    break;
                }

                move((scrY / 2) - 1, (scrX / 2) + 3);
                clrtoeol();
                refresh();
                attron(COLOR_PAIR(3));
                mvprintw((scrY / 2) - 1, (scrX / 2) + 3, "%s", username);
                attroff(COLOR_PAIR(3));
                refresh();
            }
            curs_set(0);
        }

        // Password
        else if (p == 1)
        {
            curs_set(1);
            move((scrY / 2) + 1, (scrX / 2) + 3 + strlen(password));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(password))
                {
                    password[strlen(password) - 1] = '\0';
                    SHOWpassword[strlen(SHOWpassword) - 1] = '\0';
                }
                else if (ch >= '!' && ch <= '~' && strlen(password) < LIMIT_INPUT)
                {
                    password[strlen(password)] = (char)ch;
                    password[strlen(password)] = '\0';
                }
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                {
                    if (strlen(password) < 7)
                    {
                        move((scrY / 2) + 13, 0);
                        clrtoeol();
                        attron(COLOR_PAIR(20));
                        mvprintw((scrY / 2) + 13, (scrX / 2) - 22, "Minimum length for password is 7 characters.");
                        attroff(COLOR_PAIR(20));
                    }
                    break;
                }

                move((scrY / 2) + 1, (scrX / 2) + 3);
                clrtoeol();
                refresh();
                attron(COLOR_PAIR(4));
                mvprintw((scrY / 2) + 1, (scrX / 2) + 3, "%s", setare(password));
                attroff(COLOR_PAIR(4));
                refresh();
            }
            curs_set(0);
        }

        // Email
        else if (p == 2)
        {
            curs_set(1);
            move((scrY / 2) + 3, (scrX / 2) + 3 + strlen(email));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(email))
                    email[strlen(email) - 1] = '\0';
                else if (ch >= '!' && ch <= '~' && strlen(email) < LIMIT_INPUT)
                {
                    email[strlen(email)] = (char)ch;
                    email[strlen(email)] = '\0';
                }
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                {
                    if (!checkMail(email))
                    {
                        move((scrY / 2) + 13, 0);
                        clrtoeol();
                        attron(COLOR_PAIR(20));
                        mvprintw((scrY / 2) + 13, (scrX / 2) - 9, "Invalid email Format.");
                        attroff(COLOR_PAIR(20));
                    }
                    break;
                }

                move((scrY / 2) + 3, (scrX / 2) + 3);
                clrtoeol();
                refresh();
                attron(COLOR_PAIR(3));
                mvprintw((scrY / 2) + 3, (scrX / 2) + 3, "%s", email);
                attroff(COLOR_PAIR(3));
                refresh();
            }
        }

        // Random Password
        else if (p == 3)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 7, (scrX / 2) - 8, "Random Password");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    strcpy(password, random_pass());
                    move((scrY / 2) + 1, (scrX / 2) + 3);
                    clrtoeol();
                    refresh();
                    attron(COLOR_PAIR(3));
                    mvprintw((scrY / 2) + 1, (scrX / 2) + 3, "%s", password);
                    attroff(COLOR_PAIR(3));
                    refresh();
                    curs_set(1);
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
        }

        // Cancel
        else if (p == 4)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 9, (scrX / 2) - 3, "Cancel");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    return 0;
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
        }

        // Create
        else if (p == 5)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 11, (scrX / 2) - 3, "Create");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    if (strlen(username) && (strlen(password) >= 7) && checkMail(email) && !email_exist(email) && !username_exist(username))
                    {
                        char path[MAX_LINE];
                        sprintf(path, "./.users/%s", username);
                        mkdir(path, 0777);

                        char file_path[MAX_LINE];
                        sprintf(file_path, "./.users/%s/UserInfo.txt", username);
                        FILE *write;
                        write = fopen(file_path, "w");
                        fprintf(write, "%s\n%s\n%s", username, encrypt(password, username), email);
                        fclose(write);

                        char gs_path[MAX_LINE];
                        sprintf(gs_path, "./.users/%s/GameSetting.txt", username);
                        FILE *gs = fopen(gs_path, "w");
                        fprintf(gs, "1\n0\n3\n3");
                        fclose(gs);

                        addToLogin(username, password, email);
                        addToLeaderBoard(username);

                        clear();
                        refresh();
                        flushinp();
                        securityQ(username);

                        return 1;
                    }
                    else
                    {
                        if (email_exist(email))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            attron(COLOR_PAIR(20));
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 19, "The email is already registered.");
                            attroff(COLOR_PAIR(20));
                        }
                        else if (username_exist(username))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            attron(COLOR_PAIR(20));
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 20, "The username is already registered.");
                            attroff(COLOR_PAIR(20));
                        }
                        else if (strlen(username))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            attron(COLOR_PAIR(20));
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 10, "Please enter a name.");
                            attroff(COLOR_PAIR(20));
                        }
                        else if (strlen(password) < 7)
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            attron(COLOR_PAIR(20));
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 24, "Please enter a password with at least 7 characters.");
                            attroff(COLOR_PAIR(20));
                        }
                        else if (checkMail(email))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            attron(COLOR_PAIR(20));
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 13, "Please enter a valid email.");
                            attroff(COLOR_PAIR(20));
                        }
                        break;
                    }
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
        }

        if (ch == KEY_UP)
            p = (p + 5) % 6;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 1) % 6;
    }
}

int login_guest()
{
    DIR *dir = opendir("./.users/.~GUEST");
    if (!dir)
    {
        mkdir("./.users/.~GUEST", 0777);
        dir = opendir("./.users/.~GUEST");
    }
    FILE *file = fopen("./.users/.~GUEST/LeaderBoard.txt", "w");
    fprintf(file, "0 0 0 0");
    fclose(file);

    char gs_path[MAX_LINE];
    sprintf(gs_path, "./.users/.~GUEST/GameSetting.txt");
    FILE *gs = fopen(gs_path, "w");
    fprintf(gs, "1\n0\n3\n3");
    fclose(gs);

    return 1;
}

int RegisterMenu()
{
    /*
    1 = new user
    2 = log in
    3 = guest
    0 = exit
    */

    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    UseColor();
    DIR *dir = opendir("./.users");
    if (!dir)
    {
        mkdir("./.users", 0777);
        dir = opendir("./.users");
    }

    if (access("./.users/Login.txt", F_OK) != 0)
    {
        FILE *Login = fopen("./.users/Login.txt", "w");
        fprintf(Login, "0\n");
        fclose(Login);
    }

    if (access("./.users/LeaderBoard.txt", F_OK) != 0)
    {
        FILE *leaderboard = fopen("./.users/LeaderBoard.txt", "w");
        fprintf(leaderboard, "0\n");
        fclose(leaderboard);
    }

    struct dirent *input;
    int users = 0;

    while ((input = readdir(dir)) != NULL)
    {
        if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
            continue;
        users++;
    }

    int ch, t;

    if (users == 2)
    {
        refresh();
        clear();
        if (add_New_User())
            return 1;
    }

    attron(COLOR_PAIR(9));
    printAsciiArt('r', scrX, scrY, 28, 4);
    attroff(COLOR_PAIR(9));

    refresh();
    keypad(stdscr, TRUE);

    int choice = 0;

    attron(COLOR_PAIR(8));
    printBoard(4, 0, 12, 75);
    mvprintw((scrY / 2) + 1, scrX / 2 - 9, "(Create New User)");
    mvprintw((scrY / 2) + 7, scrX / 2 - 8, "(Login As Guest)");
    mvprintw((scrY / 2) + 4, scrX / 2 - 6, "(User Login)");
    attroff(COLOR_PAIR(8));

    while (1)
    {

        attron(COLOR_PAIR(13) | A_BOLD);
        mvprintw((scrY / 2), scrX / 2 - 18, "° Create your hero for an epic quest.");
        mvprintw((scrY / 2) + 3, scrX / 2 - 33, "° Your champion awaits in the shadows. Return to claim your destiny.");
        mvprintw((scrY / 2) + 6, scrX / 2 - 18, "° Enter the Realm as a Wandering Soul");
        attroff(COLOR_PAIR(13) | A_BOLD);
        mvprintw((scrY / 2) + 12, scrX / 2 - 2, "EXIT");

        refresh();

        switch (choice)
        {
        case 1:
            attron(A_BLINK | COLOR_PAIR(12));
            mvprintw((scrY / 2), scrX / 2 - 18, "° Create your hero for an epic quest.");
            attroff(A_BLINK | COLOR_PAIR(12));
            break;
        case 2:
            attron(A_BLINK | COLOR_PAIR(12));
            mvprintw((scrY / 2) + 3, scrX / 2 - 33, "° Your champion awaits in the shadows. Return to claim your destiny.");
            attroff(A_BLINK | COLOR_PAIR(12));
            break;
        case 3:
            attron(A_BLINK | COLOR_PAIR(12));
            mvprintw((scrY / 2) + 6, scrX / 2 - 18, "° Enter the Realm as a Wandering Soul");
            attroff(A_BLINK | COLOR_PAIR(12));
            break;
        case 0:
            attron(A_BLINK | COLOR_PAIR(1));
            mvprintw((scrY / 2) + 12, scrX / 2 - 2, "EXIT");
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
                if (add_New_User())
                    return 1;
                else
                {
                    if (t = RegisterMenu())
                        return t;
                    else
                        return 0;
                }
                break;
            case 2:
                refresh();
                clear();
                if (log_in())
                    return 2;
                else
                {
                    if (t = RegisterMenu())
                        return t;
                    else
                        return 0;
                }
                break;
            case 3:
                refresh();
                clear();
                if (login_guest())
                    return 3;
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

void printTable(int big, int x, int y, int users_count, char line[MAX_LINE], char row[MAX_LINE])
{
    int lineX = x - (strlen(line) / 2);

    attron(COLOR_PAIR(8));
    // header
    mvprintw(y - 16, lineX, "%s", line);
    mvprintw(y - 15, lineX, "%s", row);
    mvprintw(y - 14, lineX, "%s", line);
    mvprintw(y - 13, lineX, "%s", row);
    int rrrr = -12;
    for (int i = 0; i < (2 * users_count) && rrrr < 10; i++)
    {
        if (rrrr == -6)
        {
            mvprintw(y + (rrrr++), lineX, "%s", line);
            if (users_count > 3)
            {
                mvprintw(y + (rrrr++), lineX, "%s", row);
                mvprintw(y + (rrrr++), lineX, "%s", row);
            }
        }
        else
            mvprintw(y + (rrrr++), lineX, "%s", row);
    }
    mvprintw(y + rrrr, lineX, "%s", line);
    attroff(COLOR_PAIR(8));

    attron(COLOR_PAIR(13));
    mvprintw(y - 17, x - 6, "LeaderBoard :");
    if (users_count > 10)
    {
        mvprintw(y - 1, x + (strlen(line) / 2) + 6, "Next Page");
        mvprintw(y + 1, x + (strlen(line) / 2) + 9, "\u25B6");
        mvprintw(y - 1, lineX - 19, "Previous Page");
        mvprintw(y + 1, lineX - 14, "\u25C4");
    }
    attroff(COLOR_PAIR(13));

    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(y - 15, lineX + 2, "Rate");
    mvprintw(y - 15, lineX + 9, "Name");
    mvprintw(y - 15, lineX + 12 + big, "Score");
    mvprintw(y - 15, lineX + 21 + big, "Golds");
    mvprintw(y - 15, lineX + 30 + big, "Games");
    mvprintw(y - 15, lineX + 38 + big, "ex");
    attroff(COLOR_PAIR(2) | A_BOLD);
}

void clearTable(int big, int y, int x, char line[MAX_LINE], char row[MAX_LINE])
{
    for (int i = -4; i < 9; i += 2)
    {
        move(y + i, 0);
        clrtoeol();
    }
    int lineX = x - ((46 + big) / 2);

    attron(COLOR_PAIR(8));
    for (int rrrr = -4; rrrr < 10; rrrr += 2)
        mvprintw(y + (rrrr), lineX, "%s", row);
    attroff(COLOR_PAIR(8));
}

void leaderBoard(char username[MAX_NAMES])
{
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    int x = scrX / 2, y = scrY / 2;

    FILE *file = fopen("./.users/LeaderBoard.txt", "r");
    int users_count, big = 4, target = -1;
    fscanf(file, "%d\n", &users_count);
    char names[users_count][MAX_NAMES];
    int rate[users_count], score[users_count], golds[users_count], games[users_count], ex[users_count], tabs[6];
    for (int i = 0; i < users_count; i++)
    {
        rate[i] = i + 1;
        fscanf(file, "%s %d %d %d %d\n", names[i], &score[i], &golds[i], &games[i], &ex[i]);
        if (strlen(names[i]) > big)
            big = strlen(names[i]);
        if (strcmp(names[i], username) == 0)
            target = i;
    }
    fclose(file);

    for (int i = 0; i < users_count - 1; i++)
        for (int j = i + 1; j < users_count; j++)
            if (golds[j] > golds[i])
            {
                char n[MAX_NAMES];
                int t;
                strcpy(n, names[j]);
                strcpy(names[j], names[i]);
                strcpy(names[i], n);

                t = golds[i];
                golds[i] = golds[j];
                golds[j] = t;

                t = score[i];
                score[i] = score[j];
                score[j] = t;

                t = games[i];
                games[i] = games[j];
                games[j] = t;

                t = ex[i];
                ex[i] = ex[j];
                ex[j] = ex[i];
            }

    FILE *write = fopen("./.users/LeaderBoard.txt", "w");
    fprintf(write, "%d\n", users_count);
    for (int i = 0; i < users_count; i++)
        fprintf(write, "%s %d %d %d %d\n", names[i], score[i], golds[i], games[i], ex[i]);
    fclose(write);

    char line[MAX_LINE], row[MAX_LINE];
    memset(line, 0, sizeof(line));
    memset(row, 0, sizeof(row));
    for (int i = 0; i < 46 + big; i++)
        line[i] = '-';
    row[0] = '|';
    for (int i = 1; i < 7; i++)
        row[i] = ' ';
    row[7] = '|';
    for (int i = 8; i < 10 + big; i++)
        row[i] = ' ';
    row[10 + big] = '|';
    for (int i = 11 + big; i < 19 + big; i++)
        row[i] = ' ';
    row[19 + big] = '|';
    for (int i = 20 + big; i < 28 + big; i++)
        row[i] = ' ';
    row[28 + big] = '|';
    for (int i = 29 + big; i < 36 + big; i++)
        row[i] = ' ';
    row[36 + big] = '|';
    for (int i = 37 + big; i < 45 + big; i++)
        row[i] = ' ';
    row[45 + big] = '|';

    tabs[0] = x - ((46 + big) / 2) + 2;
    tabs[1] = x - ((46 + big) / 2) + 9;
    tabs[2] = x - ((46 + big) / 2) + 12 + big;
    tabs[3] = x - ((46 + big) / 2) + 21 + big;
    tabs[4] = x - ((46 + big) / 2) + 30 + big;
    tabs[5] = x - ((46 + big) / 2) + 38 + big;

    // best players

    char epithets[3][MAX_NAMES] = {"GOAT", "Elite", "Master"};
    char emojis[3][MAX_NAMES] = {"\U0001F451", "\U0001F3C6", "\U0001F948"};

    printTable(big, x, y, users_count, line, row);

    int rr = -12;
    for (int i = 0; i < users_count && i < 3; i++)
    {
        if (i != target)
            mvprintw(y + rr, x + (strlen(line) / 2) + 2, "%s", emojis[i]);
        attron(COLOR_PAIR(i + 9));
        mvprintw(y + rr, x - (strlen(line) / 2) - strlen(epithets[i]) - 2, "%s", epithets[i]);
        attroff(COLOR_PAIR(i + 9));
        attron(COLOR_PAIR(i + 9) | A_BOLD);
        mvprintw(y + rr, tabs[0], "#%d", rate[i]);
        mvprintw(y + rr, tabs[1], "%s", names[i]);
        mvprintw(y + rr, tabs[2], "%d", score[i]);
        mvprintw(y + rr, tabs[3], "%d", golds[i]);
        mvprintw(y + rr, tabs[4], "%d", games[i]);
        mvprintw(y + rr, tabs[5], "%d", ex[i]);
        attroff(COLOR_PAIR(i + 9) | A_BOLD);
        rr += 2;
    }

    if (target < 3 && target >= 0)
    {
        attron(COLOR_PAIR(target + 9));
        mvprintw(y + (target * 2) - 12, x + (strlen(line) / 2) + 2, "you %s", emojis[target]);
        attroff(COLOR_PAIR(target + 9));
    }

    attron(COLOR_PAIR(1) | A_BLINK);
    mvprintw(y + 13, x - 2, "Exit");
    attroff(COLOR_PAIR(1) | A_BLINK);

    int r = -4;
    for (int i = 3; i < users_count && i < 10; i++)
    {
        mvprintw(y + r, tabs[0], "%d", rate[i]);
        if (target == i)
        {
            attron(COLOR_PAIR(13));
            mvprintw(y + r, x - (strlen(line) / 2) - 2, ">");
            mvprintw(y + r, x + (strlen(line) / 2) + 1, "<");
            attroff(COLOR_PAIR(13));
            attron(A_BOLD | COLOR_PAIR(12));
            mvprintw(y + r, tabs[1], "%s", names[i]);
            attroff(A_BOLD | COLOR_PAIR(12));
        }
        else
            mvprintw(y + r, tabs[1], "%s", names[i]);
        mvprintw(y + r, tabs[2], "%d", score[i]);
        mvprintw(y + r, tabs[3], "%d", golds[i]);
        mvprintw(y + r, tabs[4], "%d", games[i]);
        mvprintw(y + r, tabs[5], "%d", ex[i]);
        r += 2;
    }

    // getch();
    int ch, start = 4;
    while (1)
    {
        ch = getch();

        if (ch == '\n')
        {
            clear();
            refresh();
            flushinp();

            FILE *w = fopen("./.users/LeaderBoard.txt", "w");
            fprintf(w, "%d\n", users_count);
            for (int i = 0; i < users_count; i++)
                fprintf(w, "%s %d %d %d %d\n", names[i], score[i], golds[i], games[i], ex[i]);
            fclose(w);

            return;
        }
        else if (users_count > 10)
        {
            if (ch == KEY_LEFT)
            {
                start -= 7;
                clearTable(big, y, x, line, row);
                if (users_count - start - 6 < 0)
                    start += users_count - start - 6;
                if (start < 4)
                    start = 4;

                int r1 = -4;
                for (int i = start - 1; i < start + 6; i++)
                {
                    mvprintw(y + r1, tabs[0], "%d", rate[i]);
                    if (target == i)
                    {
                        attron(COLOR_PAIR(13));
                        mvprintw(y + r1, x - (strlen(line) / 2) - 2, ">");
                        mvprintw(y + r1, x + (strlen(line) / 2) + 1, "<");
                        attroff(COLOR_PAIR(13));
                        attron(A_BOLD | COLOR_PAIR(12));
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                        attroff(A_BOLD | COLOR_PAIR(12));
                    }
                    else
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                    mvprintw(y + r1, tabs[2], "%d", score[i]);
                    mvprintw(y + r1, tabs[3], "%d", golds[i]);
                    mvprintw(y + r1, tabs[4], "%d", games[i]);
                    mvprintw(y + r1, tabs[5], "%d", ex[i]);
                    r1 += 2;
                }
            }
            else if (ch == KEY_RIGHT)
            {
                start += 7;
                clearTable(big, y, x, line, row);
                if (users_count - start - 6 < 0)
                    start += users_count - start - 6;
                if (start < 4)
                    start = 4;

                int r1 = -4;
                for (int i = start - 1; i < start + 6; i++)
                {
                    mvprintw(y + r1, tabs[0], "%d", rate[i]);
                    if (target == i)
                    {
                        attron(COLOR_PAIR(13));
                        mvprintw(y + r1, x - (strlen(line) / 2) - 2, ">");
                        mvprintw(y + r1, x + (strlen(line) / 2) + 1, "<");
                        attroff(COLOR_PAIR(13));
                        attron(A_BOLD | COLOR_PAIR(12));
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                        attroff(A_BOLD | COLOR_PAIR(12));
                    }
                    else
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                    mvprintw(y + r1, tabs[2], "%d", score[i]);
                    mvprintw(y + r1, tabs[3], "%d", golds[i]);
                    mvprintw(y + r1, tabs[4], "%d", games[i]);
                    mvprintw(y + r1, tabs[5], "%d", ex[i]);
                    r1 += 2;
                }
            }
            else if (ch == KEY_UP)
            {
                start--;
                clearTable(big, y, x, line, row);
                if (users_count - start - 6 < 0)
                    start += users_count - start - 6;
                if (start < 4)
                    start = 4;

                int r1 = -4;
                for (int i = start - 1; i < start + 6; i++)
                {
                    mvprintw(y + r1, tabs[0], "%d", rate[i]);
                    if (target == i)
                    {
                        attron(COLOR_PAIR(13));
                        mvprintw(y + r1, x - (strlen(line) / 2) - 2, ">");
                        mvprintw(y + r1, x + (strlen(line) / 2) + 1, "<");
                        attroff(COLOR_PAIR(13));
                        attron(A_BOLD | COLOR_PAIR(12));
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                        attroff(A_BOLD | COLOR_PAIR(12));
                    }
                    else
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                    mvprintw(y + r1, tabs[2], "%d", score[i]);
                    mvprintw(y + r1, tabs[3], "%d", golds[i]);
                    mvprintw(y + r1, tabs[4], "%d", games[i]);
                    mvprintw(y + r1, tabs[5], "%d", ex[i]);
                    r1 += 2;
                }
            }
            else if (ch == KEY_DOWN)
            {
                start++;
                clearTable(big, y, x, line, row);
                if (users_count - start - 6 < 0)
                    start += users_count - start - 6;
                if (start < 4)
                    start = 4;

                int r1 = -4;
                for (int i = start - 1; i < start + 6; i++)
                {
                    mvprintw(y + r1, tabs[0], "%d", rate[i]);
                    if (target == i)
                    {
                        attron(COLOR_PAIR(13));
                        mvprintw(y + r1, x - (strlen(line) / 2) - 2, ">");
                        mvprintw(y + r1, x + (strlen(line) / 2) + 1, "<");
                        attroff(COLOR_PAIR(13));
                        attron(A_BOLD | COLOR_PAIR(12));
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                        attroff(A_BOLD | COLOR_PAIR(12));
                    }
                    else
                        mvprintw(y + r1, tabs[1], "%s", names[i]);
                    mvprintw(y + r1, tabs[2], "%d", score[i]);
                    mvprintw(y + r1, tabs[3], "%d", golds[i]);
                    mvprintw(y + r1, tabs[4], "%d", games[i]);
                    mvprintw(y + r1, tabs[5], "%d", ex[i]);
                    r1 += 2;
                }
            }
        }
    }
}

void game_setting(int user_mode, char username[MAX_NAMES])
{
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    int x = scrX / 2, y = scrY / 2;

    int difficulty, color, character, choose_music;
    char path[MAX_LINE];
    char *characters[5] = {"A", "ᛟ", "ᛸ", "\u2B55", "\U0001F7E5"};
    char *u26AA[6] = {"\u2B55", "\U0001F7E2", "\U0001F535", "\U0001F7E0", "\U0001F7E4", "\U0001F7E3"};
    char *u2590[6] = {"\U0001F7E5", "\U0001F7E9", "\U0001F7E6", "\U0001F7E7", "\U0001F7EB", "\U0001F7EA"};

    if (user_mode != 3)
        sprintf(path, "./.users/%s/GameSetting.txt", username);
    else
        sprintf(path, "./.users/.~GUEST/GameSetting.txt");

    FILE *file = fopen(path, "r");
    fscanf(file, "%d\n%d\n%d\n%d", &difficulty, &character, &color, &choose_music);
    fclose(file);

    DIR *dir = opendir("./.musics");
    int musics_count = 0;
    struct dirent *input;
    while ((input = readdir(dir)) != NULL)
    {
        if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
            continue;
        musics_count++;
    }
    closedir(dir);
    char(*music_names)[MAX_NAMES] = malloc(musics_count * MAX_NAMES);
    DIR *dir_read = opendir("./.musics");
    int index = 0;
    struct dirent *input2;
    while ((input2 = readdir(dir_read)) != NULL)
    {
        if (!strcmp(input2->d_name, ".") || !strcmp(input2->d_name, ".."))
            continue;
        if (index < musics_count)
        {
            strncpy(music_names[index], input2->d_name, MAX_NAMES - 1);
            music_names[index][MAX_NAMES - 1] = '\0';
            index++;
        }
    }
    closedir(dir_read);
    attron(COLOR_PAIR(8));
    printBoard(0, 0, 20, 75);
    attroff(COLOR_PAIR(8));

    attron(COLOR_PAIR(9) | A_BOLD);
    mvprintw(y - 12, x - 3, "SETTING");
    attroff(COLOR_PAIR(9) | A_BOLD);

    attron(COLOR_PAIR(13) | A_BOLD);
    mvprintw(y - 8, x - 9, "Difficulty Level :");
    mvprintw(y - 3, x - 9, "Character Setting :");
    mvprintw(y + 5, x - 3, "Songs :");
    attroff(COLOR_PAIR(13) | A_BOLD);

    mvprintw(y - 6, x - 35, " ");
    mvprintw(y - 6, x + 35, " ");
    mvprintw(y - 6, x - 22, "|------------|---------|--------------------|");
    mvprintw(y - 5, x - 26, "Explorer    Adventurer  Legend           Mediterranean");
    mvprintw(y + 1, x - 4, "Character");
    mvprintw(y + 2, x - 2, "Color");
    attron(COLOR_PAIR(color + 20));
    mvprintw(y - 1, x, "%s", characters[character]);
    attroff(COLOR_PAIR(color + 20));
    mvprintw(y + 7, x - (strlen(music_names[choose_music]) / 2), "%s", music_names[choose_music]);

    int ch, p = 0;

    int tab_op[4] = {x - 22, x - 9, x + 1, x + 22}, tab_dif[4] = {x - 26, x - 14, x - 2, x + 15};
    char *dif[4] = {"Explorer", "Adventurer", "Legend", "Mediterranean"};

    while (1)
    {
        mvprintw(y - 6, x - 30, " ");
        mvprintw(y - 6, x + 30, " ");
        mvprintw(y + 1, x - 6, " ");
        mvprintw(y + 1, x + 6, " ");
        mvprintw(y + 2, x - 6, " ");
        mvprintw(y + 2, x + 6, " ");
        mvprintw(y + 7, x - 30, " ");
        mvprintw(y + 7, x + 30, " ");
        mvprintw(y + 13, x - 2, "SAVE");
        mvprintw(y + 15, x - 2, "EXIT");

        mvprintw(y + 7, x - (strlen(music_names[choose_music]) / 2), "%s", music_names[choose_music]);

        if (character < 3)
        {
            attron(COLOR_PAIR(color + 20));
            mvprintw(y - 1, x, "%s", characters[character]);
            attroff(COLOR_PAIR(color + 20));
        }
        else if (character == 3)
            mvprintw(y - 1, x, "%s", u26AA[color]);
        else if (character == 4)
            mvprintw(y - 1, x, "%s", u2590[color]);

        // difficulty
        if (p == 0)
        {
            while (1)
            {
                mvprintw(y - 6, x - 30, " ");
                mvprintw(y - 6, x + 30, " ");
                mvprintw(y - 6, x - 22, "|------------|---------|--------------------|");
                mvprintw(y - 5, x - 26, "Explorer    Adventurer  Legend           Mediterranean");
                attron(COLOR_PAIR(9));
                if (difficulty != 0)
                    mvprintw(y - 6, x - 30, "\u25C0");
                if (difficulty != 3)
                    mvprintw(y - 6, x + 30, "\u25B6");
                attroff(COLOR_PAIR(9));

                attron(COLOR_PAIR(11));
                mvprintw(y - 6, tab_op[difficulty], "░");
                mvprintw(y - 5, tab_dif[difficulty], "%s", dif[difficulty]);
                attroff(COLOR_PAIR(11));

                ch = getch();

                if (ch == KEY_RIGHT && difficulty != 3)
                    difficulty = (difficulty + 1) % 4;
                else if (ch == KEY_LEFT && difficulty != 0)
                    difficulty = (difficulty + 3) % 4;
                else if (ch == KEY_DOWN || ch == KEY_UP || ch == '\n')
                    break;
            }
        }

        // character
        else if (p == 1)
        {
            while (1)
            {
                if (character < 3)
                {
                    attron(COLOR_PAIR(color + 20) | A_BLINK);
                    mvprintw(y - 1, x, "%s", characters[character]);
                    attroff(COLOR_PAIR(color + 20) | A_BLINK);
                }
                else if (character == 3)
                    mvprintw(y - 1, x, "%s", u26AA[color]);
                else if (character == 4)
                    mvprintw(y - 1, x, "%s", u2590[color]);

                mvprintw(y + 1, x - 4, "Character");

                attron(COLOR_PAIR(9));
                mvprintw(y + 1, x - 6, "\u25C0");
                mvprintw(y + 1, x + 6, "\u25B6");
                attroff(COLOR_PAIR(9));
                ch = getch();
                if (ch == KEY_RIGHT)
                    character = (character + 1) % 5;
                else if (ch == KEY_LEFT)
                    character = (character + 4) % 5;
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;
            }
        }

        // color
        else if (p == 2)
        {
            while (1)
            {
                if (character < 3)
                {
                    attron(COLOR_PAIR(color + 20) | A_BLINK);
                    mvprintw(y - 1, x, "%s", characters[character]);
                    attroff(COLOR_PAIR(color + 20) | A_BLINK);
                }
                else if (character == 3)
                    mvprintw(y - 1, x, "%s", u26AA[color]);
                else if (character == 4)
                    mvprintw(y - 1, x, "%s", u2590[color]);

                mvprintw(y + 2, x - 2, "Color");

                attron(COLOR_PAIR(9));
                mvprintw(y + 2, x - 6, "\u25C0");
                mvprintw(y + 2, x + 6, "\u25B6");
                attroff(COLOR_PAIR(9));
                ch = getch();
                if (ch == KEY_RIGHT)
                    color = (color + 1) % 6;
                else if (ch == KEY_LEFT)
                    color = (color + 5) % 6;
                else if (ch == KEY_UP || ch == KEY_DOWN || ch == '\n')
                    break;
            }
        }

        // song
        else if (p == 3)
        {
            while (1)
            {
                mvprintw(y + 7, x - 30, "                                                                  ");
                attron(COLOR_PAIR(21) | A_BLINK);
                mvprintw(y + 7, x - (strlen(music_names[choose_music]) / 2), "%s", music_names[choose_music]);
                attroff(COLOR_PAIR(21) | A_BLINK);
                attron(COLOR_PAIR(9));
                if (choose_music != 0)
                    mvprintw(y + 7, x - 30, "\u25C0");
                if (choose_music != musics_count - 1)
                    mvprintw(y + 7, x + 30, "\u25B6");
                attroff(COLOR_PAIR(9));

                ch = getch();

                if (ch == KEY_RIGHT && (choose_music != musics_count - 1))
                    choose_music = (choose_music + 1) % musics_count;
                else if (ch == KEY_LEFT && choose_music != 0)
                    choose_music = (choose_music + musics_count - 1) % musics_count;
                else if (ch == KEY_DOWN || ch == KEY_UP || ch == '\n')
                    break;
            }
        }

        // save
        else if (p == 4)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw(y + 13, x - 2, "SAVE");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();

                if (ch == '\n')
                {
                    FILE *write = fopen(path, "w");
                    fprintf(write, "%d\n%d\n%d\n%d", difficulty, character, color, choose_music);
                    fclose(write);
                    clear();
                    refresh();
                    flushinp();
                    return;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // exit
        else if (p == 5)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw(y + 15, x - 2, "EXIT");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();

                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    return;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        if (ch == KEY_UP)
            p = (p + 5) % 6;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 1) % 6;
    }
}

void sortLB()
{
    FILE *file = fopen("./.users/LeaderBoard.txt", "r");
    int users_count;
    fscanf(file, "%d\n", &users_count);
    char names[users_count][MAX_NAMES];
    int score[users_count], golds[users_count], games[users_count], ex[users_count], tabs[6];
    for (int i = 0; i < users_count; i++)
        fscanf(file, "%s %d %d %d %d\n", names[i], &score[i], &golds[i], &games[i], &ex[i]);
    fclose(file);

    for (int i = 0; i < users_count - 1; i++)
        for (int j = i + 1; j < users_count; j++)
            if (golds[j] > golds[i])
            {
                char n[MAX_NAMES];
                int t;
                strcpy(n, names[j]);
                strcpy(names[j], names[i]);
                strcpy(names[i], n);

                t = golds[i];
                golds[i] = golds[j];
                golds[j] = t;

                t = score[i];
                score[i] = score[j];
                score[j] = t;

                t = games[i];
                games[i] = games[j];
                games[j] = t;

                t = ex[i];
                ex[i] = ex[j];
                ex[j] = ex[i];
            }

    FILE *write = fopen("./.users/LeaderBoard.txt", "w");
    fprintf(write, "%d\n", users_count);
    for (int i = 0; i < users_count; i++)
        fprintf(write, "%s %d %d %d %d\n", names[i], score[i], golds[i], games[i], ex[i]);
    fclose(write);
}

void profile_menu(int user_mode, char username[MAX_NAMES])
{
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    int x = scrX / 2, y = scrY / 2;

    sortLB();

    char line[71];
    line[70] = '\0';
    memset(line, '-', 70);

    char *characters[5] = {"A", "ᛟ", "ᛸ", "\u2B55", "\U0001F7E5"};
    char *u26AA[6] = {"\u2B55", "\U0001F7E2", "\U0001F535", "\U0001F7E0", "\U0001F7E4", "\U0001F7E3"};
    char *u2590[6] = {"\U0001F7E5", "\U0001F7E9", "\U0001F7E6", "\U0001F7E7", "\U0001F7EB", "\U0001F7EA"};

    FILE *file = fopen("./.users/LeaderBoard.txt", "r");
    int users_count;
    fscanf(file, "%d\n", &users_count);
    char names[users_count][MAX_NAMES], email[MAX_NAMES];
    int score[users_count], golds[users_count], games[users_count], ex[users_count];

    int difficulty, character, color, G_score, G_gold, G_xp, G_game;
    FILE *login = fopen("./.users/Login.txt", "r");
    int user_count, target;
    fscanf(login, "%d\n", &user_count);
    char nam[user_count][MAX_NAMES], password[user_count][MAX_NAMES], emails[user_count][MAX_NAMES];
    int rank = -1;
    if (user_mode != 3)
    {
        for (int i = 0; i < users_count; i++)
            fscanf(file, "%s %d %d %d %d\n", names[i], &score[i], &golds[i], &games[i], &ex[i]);
        fclose(file);

        for (int i = 0; i < users_count - 1; i++)
            for (int j = i + 1; j < users_count; j++)
                if (golds[j] > golds[i])
                {
                    char n[MAX_NAMES];
                    int t;
                    strcpy(n, names[j]);
                    strcpy(names[j], names[i]);
                    strcpy(names[i], n);

                    t = golds[i];
                    golds[i] = golds[j];
                    golds[j] = t;

                    t = score[i];
                    score[i] = score[j];
                    score[j] = t;

                    t = games[i];
                    games[i] = games[j];
                    games[j] = t;

                    t = ex[i];
                    ex[i] = ex[j];
                    ex[j] = ex[i];
                }

        FILE *write = fopen("./.users/LeaderBoard.txt", "w");
        fprintf(write, "%d\n", users_count);
        for (int i = 0; i < users_count; i++)
            fprintf(write, "%s %d %d %d %d\n", names[i], score[i], golds[i], games[i], ex[i]);
        fclose(write);

        for (int i = 0; i < user_count; i++)
        {
            fscanf(login, "%s %s %s", nam[i], password[i], emails[i]);
            if (strcmp(username, nam[i]) == 0)
            {
                target = i;
                strcpy(email, emails[i]);
                break;
            }
        }
        fclose(login);

        for (int i = 0; i < users_count; i++)
            if (strcmp(names[i], username) == 0)
                rank = i;

        char path[MAX_LINE];
        sprintf(path, "./.users/%s/GameSetting.txt", username);
        FILE *rgs = fopen(path, "r");
        fscanf(rgs, "%d\n%d\n%d\n%*d", &difficulty, &character, &color);
        fclose(rgs);
    }
    else
    {
        char path[MAX_LINE];
        FILE *rgs = fopen("./.users/.~GUEST/GameSetting.txt", "r");
        fscanf(rgs, "%d\n%d\n%d\n%*d", &difficulty, &character, &color);
        fclose(rgs);

        FILE *rgs2 = fopen("./.users/.~GUEST/LeaderBoard.txt", "r");
        fscanf(rgs2, "%d %d %d %d", &G_score, &G_gold, &G_game, &G_xp);
        fclose(rgs2);
    }

    attron(COLOR_PAIR(8));
    printBoard(-1, 0, 17, 70);
    mvprintw(y - 1, x - (strlen(line) / 2), "%s", line);
    printBoard(-5, -28, 7, 11);
    attroff(COLOR_PAIR(8));
    attron(COLOR_PAIR(9) | A_BOLD);
    mvprintw(y - 11, x - 3, "Profile");
    char name2[MAX_NAMES];
    strcpy(name2, username);
    if (user_mode == 3)
    {
        for (int i = 0; i < strlen(name2) - 1; i++)
            name2[i] = name2[i + 1];
        name2[strlen(name2) - 1] = '\0';
    }
    mvprintw(y - 7, x - 21, "%s", name2);
    attroff(COLOR_PAIR(9) | A_BOLD);
    attron(COLOR_PAIR(13) | A_BOLD);
    mvprintw(y - 3, x - 21, "Level ");
    mvprintw(y + 1, x - 33, "Player Rank : ");
    mvprintw(y + 1, x + 3, "Total Golds : ");
    mvprintw(y + 3, x - 33, "Total Score : ");
    mvprintw(y + 3, x + 3, "Games Played : ");
    mvprintw(y + 5, x + 3, "XP : ");
    attroff(COLOR_PAIR(13) | A_BOLD);

    int level = 0;
    if (user_mode != 3)
    {
        mvprintw(y - 5, x - 21, "%s", email);
        while (50 * (level * level) + 500 * level < ex[rank])
            level++;
        mvprintw(y - 3, x - 14, "%d", level);
        attron(COLOR_PAIR(21));
        mvprintw(y + 1, x - 18, "%d", rank + 1);
        mvprintw(y + 1, x + 19, "%d", golds[rank]);
        mvprintw(y + 3, x - 18, "%d", score[rank]);
        mvprintw(y + 3, x + 19, "%d", games[rank]);
        mvprintw(y + 5, x + 19, "%d", ex[rank]);
        attroff(COLOR_PAIR(21));
    }
    else
    {
        mvprintw(y - 5, x - 21, "---");
        while (50 * (level * level) + 500 * level < ex[rank])
            level++;
        mvprintw(y - 3, x - 14, "%d", level);
        attron(COLOR_PAIR(21));
        mvprintw(y + 1, x - 18, "---");
        mvprintw(y + 1, x + 19, "%d", G_gold);
        mvprintw(y + 3, x - 18, "%d", G_score);
        mvprintw(y + 3, x + 19, "%d", G_game);
        mvprintw(y + 5, x + 19, "%d", G_xp);
        attroff(COLOR_PAIR(21));
    }

    if (character < 3)
    {
        attron(COLOR_PAIR(color + 20));
        mvprintw(y - 5, x - 28, "%s", characters[character]);
        attroff(COLOR_PAIR(color + 20));
    }
    else if (character == 3)
        mvprintw(y - 5, x - 28, "%s", u26AA[color]);
    else if (character == 4)
        mvprintw(y - 5, x - 28, "%s", u2590[color]);

    int ch, p = 2;

    while (1)
    {
        if (user_mode == 3)
        {

            attron(COLOR_PAIR(8));
            mvprintw(y + 9, x - 2, "EDIT");
            mvprintw(y + 11, x - 7, "Change Password");
            attroff(COLOR_PAIR(8));
        }
        else
        {
            mvprintw(y + 9, x - 2, "EDIT");
            mvprintw(y + 11, x - 7, "Change Password");
        }
        mvprintw(y + 13, x - 2, "EXIT");

        // EDIT
        if (p == 0)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw(y + 9, x - 2, "EDIT");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    int pp = 0;
                    mvprintw(y + 11, x - 7, "               ");
                    mvprintw(y + 13, x - 2, "    ");
                    while (1)
                    {
                        mvprintw(y + 9, x - 2, "SAVE");
                        // User Name
                        if (pp == 0)
                        {
                            move(y - 7, x - 21 + strlen(username));
                            curs_set(1);
                            while (1)
                            {
                                ch = getch();
                                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(username))
                                    username[strlen(username) - 1] = '\0';
                                else if (ch >= '!' && ch < '~' && strlen(username) < LIMIT_INPUT)
                                {
                                    username[strlen(username)] = (char)ch;
                                    username[strlen(username)] = '\0';
                                }
                                else if ((ch == KEY_UP || ch == KEY_DOWN) && strlen(username))
                                    break;

                                mvprintw(y - 7, x - 21, "                                                      ");
                                attron(COLOR_PAIR(9) | A_BOLD);
                                mvprintw(y - 7, x - 21, "%s", username);
                                attroff(COLOR_PAIR(9) | A_BOLD);
                            }
                            curs_set(0);
                        }

                        // Email
                        else if (pp == 1)
                        {
                            move(y - 5, x - 21 + strlen(email));
                            curs_set(1);
                            while (1)
                            {
                                ch = getch();
                                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(email))
                                    email[strlen(email) - 1] = '\0';
                                else if (ch >= '!' && ch < '~' && strlen(email) < LIMIT_INPUT)
                                {
                                    email[strlen(email)] = (char)ch;
                                    email[strlen(email)] = '\0';
                                }
                                else if ((ch == KEY_UP || ch == KEY_DOWN) && checkMail(email))
                                    break;

                                mvprintw(y - 5, x - 21, "                                                      ");
                                mvprintw(y - 5, x - 21, "%s", email);
                            }
                            curs_set(0);
                        }

                        // SAVE
                        else if (pp == 2)
                        {
                            attron(COLOR_PAIR(1) | A_BLINK);
                            mvprintw(y + 9, x - 2, "SAVE");
                            attroff(COLOR_PAIR(1) | A_BLINK);
                            while (1)
                            {
                                ch = getch();
                                if (ch == '\n' && user_mode != 3)
                                {
                                    strcpy(password[target], encrypt(decrypt(password[target], nam[target]), username));

                                    // LeaderBoard Updated
                                    strcpy(names[rank], username);
                                    FILE *write2 = fopen("./.users/LeaderBoard.txt", "w");
                                    fprintf(write2, "%d\n", users_count);
                                    for (int i = 0; i < users_count; i++)
                                        fprintf(write2, "%s %d %d %d %d\n", names[i], score[i], golds[i], games[i], ex[i]);
                                    fclose(write2);

                                    // UserInfo.txt Updated
                                    char old_path[MAX_LINE];
                                    sprintf(old_path, "./.users/%s", nam[target]);
                                    char new_path[MAX_LINE];
                                    sprintf(new_path, "./.users/%s", username);
                                    rename(old_path, new_path);
                                    char new_path2[MAX_LINE];
                                    sprintf(new_path2, "./.users/%s/UserInfo.txt", username);
                                    FILE *write_info = fopen(new_path2, "w");
                                    fprintf(write_info, "%s\n%s\n%s", username, password[target], email);
                                    fclose(write_info);

                                    // Login.txt Updated
                                    strcpy(emails[target], email);
                                    strcpy(nam[target], username);
                                    FILE *login_w = fopen("./.users/Login.txt", "w");
                                    fprintf(login_w, "%d\n", user_count);
                                    for (int i = 0; i < user_count; i++)
                                        fprintf(login_w, "%s %s %s", nam[i], password[i], emails[i]);
                                    fclose(login_w);
                                    break;
                                }
                                else if (ch == KEY_UP || ch == KEY_DOWN)
                                    break;
                            }
                        }

                        if (ch == KEY_UP)
                            pp = (pp + 2) % 3;
                        else if (ch == KEY_DOWN)
                            pp = (pp + 1) % 3;
                        else if (ch == '\n')
                            break;
                    }
                    mvprintw(y + 11, x - 7, "Change Password");
                    mvprintw(y + 13, x - 2, "EXIT");
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // Change Password
        else if (p == 1)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw(y + 11, x - 7, "Change Password");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    new_pass(username);
                    profile_menu(user_mode, username);
                    return;
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // EXIT
        else if (p == 2)
        {
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw(y + 13, x - 2, "EXIT");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();
                if (ch == '\n')
                {
                    clear();
                    refresh();
                    flushinp();
                    return;
                }
                else if ((ch == KEY_UP || ch == KEY_DOWN) && user_mode != 3)
                    break;
            }
        }

        if (user_mode != 3)
            if (ch == KEY_UP)
                p = (p + 2) % 3;
            else if (ch == KEY_DOWN || ch == '\n')
                p = (p + 1) % 3;
    }
}
