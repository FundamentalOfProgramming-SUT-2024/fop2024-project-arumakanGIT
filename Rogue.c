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

char *setare(char buffer[MAX_NAMES])
{
    char *setareh = (char *)malloc(MAX_NAMES);
    memset(setareh, 0, sizeof(setareh));
    for (int i = 0; i < strlen(buffer); i++)
        setareh[i] = '*';
    setareh[strlen(setareh)] = '\0';
    return setareh;
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

void addToUserInfo(char username[MAX_NAMES])
{
    FILE *read;
    read = fopen("./.users/Users.txt", "r");
    int users_count;
    fscanf(read, "%d\n", &users_count);
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
    fprintf(write, "%d\n", users_count + 1);
    fprintf(write, "%d %s %d\n", 1, username, 0);
    for (int i = 0; i < users_count; i++)
        fprintf(write, "%d %s %d\n", history[i], names[i], golds[i]);
    fclose(write);
}

void EditUserInfo(char username[MAX_NAMES])
{
    FILE *read;
    read = fopen("./.users/Users.txt", "r");
    int users_count;
    fscanf(read, "%d\n", &users_count);
    int history[users_count], golds[users_count], target_index;
    char names[users_count][MAX_NAMES];
    for (int i = 0; i < users_count; i++)
    {
        fscanf(read, "%d %s %d\n", &history[i], names[i], &golds[i]);
        if (strcmp(names[i], username) == 0)
            target_index = i;
        else
            history[i]++;
    }
    fclose(read);

    FILE *write;
    write = fopen("./.users/Users.txt", "w");
    fprintf(write, "%d\n", users_count);
    fprintf(write, "%d %s %d\n", 1, username, golds[target_index]);
    for (int i = 0; i < users_count; i++)
        if (i != target_index)
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
    noecho();
    keypad(stdscr, TRUE);
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
        mvprintw((scrY / 2) + 1, (scrX / 2) - 8, "random password");
        mvprintw((scrY / 2) + 3, (scrX / 2) - 2, "Done");

        // password
        if (p == 0)
        {
            move((scrY / 2) + 5, 0);
            clrtoeol();

            curs_set(1);
            move((scrY / 2) - 1, (scrX / 2) + (strlen(password) / 2));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(password))
                    password[strlen(password) - 1] = '\0';
                else if (ch >= '!' && ch <= '~')
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
                        move((scrY / 2) + 5, 0);
                        clrtoeol();
                        mvprintw((scrY / 2) + 5, (scrX / 2) - 15, "The password is already taken.");
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
                        }
                        clear();
                        refresh();
                        return;
                    }
                    else
                    {
                        move((scrY / 2) + 5, 0);
                        clrtoeol();
                        mvprintw((scrY / 2) + 5, (scrX / 2) - 25, "Please enter a password with at least 7 characters.");
                    }
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        if (ch == KEY_UP)
            p = (p + 2) % 3;
        else if (ch == KEY_DOWN || ch == '\n')
            p = (p + 1) % 3;
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

    int ch, p = 0;
    char pet[MAX_NAMES], bf[MAX_NAMES], school[MAX_NAMES], movie[MAX_NAMES];

    memset(pet, 0, sizeof(pet));
    memset(bf, 0, sizeof(bf));
    memset(school, 0, sizeof(school));
    memset(movie, 0, sizeof(movie));

    while (1)
    {
        mvprintw((scrY / 2) + 4, (scrX / 2) - 2, "Done");
        curs_set(1);
        // pet
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

        // bf
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

        // school
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

        // movie
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

        // done
        else if (p == 4)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 4, (scrX / 2) - 2, "Done");
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
                        FILE *write = fopen(path, "w");
                        fprintf(write, "%s\n%s\n%s\n%s", pet, bf, school, movie);
                        fclose(write);
                        clear();
                        refresh();
                        return;
                    }
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
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
    memset(username, 0, sizeof(username));
    memset(password, 0, sizeof(password));
    memset(SHOWpassword, 0, sizeof(SHOWpassword));
    strcpy(username, "\0");
    strcpy(password, "\0");
    SHOWpassword[0] = '\0';

    FILE *read;
    read = fopen("./.users/Users.txt", "r");
    fscanf(read, "%d\n", &users_count);

    for (int i = 0; i < 3; i++)
    {
        memset(players[i], 0, sizeof(players[i]));
        strcpy(players[i], "\0");
    }

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
        mvprintw((scrY / 2) + 4, (scrX / 2) - 7, "forgot password");
        mvprintw((scrY / 2) + 7, (scrX / 2) - 3, "Log in");

        // player 1
        if (p == 0)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) - 8, (scrX / 2) - (big / 2) - 1, "1- %s", players[0]);
            attroff(COLOR_PAIR(1) | A_BLINK);
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
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // player 2
        else if (p == 1)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) - 7, (scrX / 2) - (big / 2) - 1, "2- %s", players[1]);
            attroff(COLOR_PAIR(1) | A_BLINK);
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
                }
                else if (ch == KEY_UP || ch == KEY_DOWN)
                    break;
            }
        }

        // player 3
        else if (p == 2)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) - 6, (scrX / 2) - (big / 2) - 1, "3- %s", players[2]);
            attroff(COLOR_PAIR(1) | A_BLINK);
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

        // password
        else if (p == 4)
        {
            curs_set(1);
            move((scrY / 2) + 2, (scrX / 2) + 2 + strlen(password));
            while (1)
            {
                ch = getch();
                if ((ch == 7 || ch == KEY_BACKSPACE) && strlen(password))
                {
                    password[strlen(password) - 1] = '\0';
                    SHOWpassword[strlen(SHOWpassword) - 1] = '\0';
                }
                else if (ch > 32 && ch < 127)
                {
                    password[strlen(password)] = (char)ch;
                    SHOWpassword[strlen(SHOWpassword)] = '*';
                    SHOWpassword[strlen(SHOWpassword)] = '\0';
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

        // forgot password
        else if (p == 5)
        {
            curs_set(0);
            attron(COLOR_PAIR(1) | A_BLINK);
            mvprintw((scrY / 2) + 4, (scrX / 2) - 7, "forgot password");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();

                if (ch == '\n' && (strlen(username) == 0 || username_exist(username) == 0 || email_exist(username)))
                {
                    move((scrY / 2) + 9, 0);
                    clrtoeol();
                    mvprintw((scrY / 2) + 9, (scrX / 2) - 23, "There is no user with this username or email");
                }
                else if (ch == '\n' && (username_exist(username) || email_exist(username)))
                {
                    clear();
                    refresh();
                    flushinp();
                    securityQ(username);
                    return;
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
            mvprintw((scrY / 2) + 7, (scrX / 2) - 3, "Log in");
            attroff(COLOR_PAIR(1) | A_BLINK);
            while (1)
            {
                ch = getch();

                if (ch == '\n' && (strlen(username) == 0 || (username_exist(username) == 0 && email_exist(username) == 0)))
                {
                    move((scrY / 2) + 9, 0);
                    clrtoeol();
                    mvprintw((scrY / 2) + 9, (scrX / 2) - 23, "There is no user with this username or email");
                }
                else if (ch == '\n' && (username_exist(username) || email_exist(username)))
                {
                    int check = search_login(username, password);
                    if (check == 1)
                    {
                        EditUserInfo(username);
                        clear();
                        refresh();
                        return;
                    }
                    else if (check == 2)
                    {
                        move((scrY / 2) + 9, 0);
                        clrtoeol();
                        mvprintw((scrY / 2) + 9, (scrX / 2) - 7, "Wrong password!");
                    }
                }
                else if (ch == KEY_DOWN || ch == KEY_UP)
                    break;
            }
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
    cbreak();
    keypad(stdscr, TRUE);
    UseColor();
    int scrX, scrY;
    getmaxyx(stdscr, scrY, scrX);
    printAsciiArt('r', scrX, scrY, 28, 7);
    mvprintw((scrY / 2) - 4, (scrX / 2) - 17, "Create your hero for an epic quest.");
    mvprintw((scrY / 2) - 3, (scrX / 2) - 31, "--------------------------------------------------------------");
    mvprintw((scrY / 2) - 2, (scrX / 2) - 31, "|                                                            |");
    // mvprintw((scrY / 2) - 1, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2), (scrX / 2) - 31, "|                                                            |");
    // mvprintw((scrY / 2) + 1, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2) + 2, (scrX / 2) - 31, "|                                                            |");
    // mvprintw((scrY / 2) + 3, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2) + 4, (scrX / 2) - 31, "|                                                            |");
    mvprintw((scrY / 2) + 5, (scrX / 2) - 31, "--------------------------------------------------------------");
    mvprintw((scrY / 2) - 1, (scrX / 2) - 25, "Enter your name, hero : ");
    mvprintw((scrY / 2) + 1, (scrX / 2) - 25, "Enter your password : ");
    mvprintw((scrY / 2) + 3, (scrX / 2) - 25, "Enter your email : ");

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
                else if (ch >= '!' && ch <= '~')
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
                        mvprintw((scrY / 2) + 13, (scrX / 2) - 9, "Please enter a name.");
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
                else if (ch >= '!' && ch <= '~')
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
                        mvprintw((scrY / 2) + 13, (scrX / 2) - 22, "Minimum length for password is 7 characters.");
                    }
                    break;
                }

                move((scrY / 2) + 1, (scrX / 2) + 3);
                clrtoeol();
                refresh();
                attron(COLOR_PAIR(3));
                mvprintw((scrY / 2) + 1, (scrX / 2) + 3, "%s", setare(password));
                attroff(COLOR_PAIR(3));
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
                else if (ch >= '!' && ch <= '~')
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
                        mvprintw((scrY / 2) + 13, (scrX / 2) - 9, "Invalid email Format.");
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
                    return;
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

                        addToLogin(username, password, email);
                        addToUserInfo(username);

                        clear();
                        refresh();
                        flushinp();
                        securityQ(username);

                        return;
                    }
                    else
                    {
                        if (email_exist(email))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 19, "The email is already registered.");
                        }
                        else if (username_exist(username))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 20, "The username is already registered.");
                        }
                        else if (strlen(username))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 10, "Please enter a name.");
                        }
                        else if (strlen(password) < 7)
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 24, "Please enter a password with at least 7 characters.");
                        }
                        else if (checkMail(email))
                        {
                            move((scrY / 2) + 13, 0);
                            clrtoeol();
                            mvprintw((scrY / 2) + 13, (scrX / 2) - 13, "Please enter a valid email.");
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
        fprintf(Users, "0\n");
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
    flushinp();
    clear();
}

int main()
{
    srand(time(NULL));
    initscr();
    // raw();
    cbreak();
    noecho();
    curs_set(0);

    EnterPage();
    RegisterMenu();

    endwin();
    return 0;
}