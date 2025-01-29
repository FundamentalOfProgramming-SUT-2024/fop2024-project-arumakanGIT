#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAMES 256
#define MAX_LINE 1024

/*
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
                free(username);
                free(password);
                free(SHOWpassword);
                free(email);
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
*/

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

int main()
{

    return 0;
}