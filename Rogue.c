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
    int health;
    int hungry;
    int golds;
    int XP;
    int score;
    int foods[5];
    int weapons[5];
} Node;

typedef struct Room
{
    int x;
    int y;
    int h;
    int w;
    int visible;
    int kind;
    int door;
    int secretDoor;
} Room;

typedef struct Floor
{
    int f;
    int rooms_count;
    Room *rooms;
} Floor;

void printRoom(int Y_padding, int X_padding, int h, int w)
{
    char line[w + 1], row[w + 1], dots[w - 1];
    line[w] = '\0';
    row[w] = '\0';
    dots[w - 2] = '\0';
    memset(line, '-', w);
    memset(row, ' ', w);
    memset(dots, '.', w - 2);
    row[0] = '|';
    row[w - 1] = '|';

    attron(COLOR_PAIR(13));
    mvprintw(Y_padding, X_padding, "%s", line);
    mvprintw(Y_padding + h, X_padding, "%s", line);
    for (int i = Y_padding + 1; i < Y_padding + h; i++)
        mvprintw(i, X_padding, "%s", row);
    attroff(COLOR_PAIR(13));
    // init_pair(14, 8, COLOR_WHITE);
    attron(COLOR_PAIR(8));
    for (int i = Y_padding + 1; i < Y_padding + h; i++)
        mvprintw(i, X_padding + 1, "%s", dots);
    attroff(COLOR_PAIR(8));
}

void generate_corridor(int rooms_count, Room *rooms, char username[MAX_NAMES], int floor)
{
    char path[MAX_LINE];
    sprintf(path, "./.users/%s/~NEW_GAME/corridor_%d.txt", username, floor);
    FILE *write = fopen(path, "w");
    fprintf(write, "%d\n", rooms_count);

    char neighbors[rooms_count][10];
    for (int i = 0; i < rooms_count; i++)
    {
        memset(neighbors[i], 0, 9);
        neighbors[i][0] = '\0';
    }

    for (int i = 0; i < rooms_count; i++)
    {
        int distance[rooms_count];
        int destination = 10, smaller = 200;
        int count = 0;
        for (int j = 0; j < rooms_count; j++)
        {
            if (j == i)
            {
                distance[j] = 0;
                continue;
            }

            distance[j] = abs((rooms[i].x + (rooms[i].w / 2)) - (rooms[j].x + (rooms[j].w / 2))) + 2 * abs((rooms[i].y + (rooms[i].h / 2)) - (rooms[j].y + (rooms[j].h / 2)));
            if (distance[j] < 60)
                count++;
            if (distance[j] < smaller)
            {
                smaller = distance[j];
                destination = j;
            }
        }

        if (count != 0)
        {
            int nei[count], min = count, temp = 0;
            if (rooms[i].door < count)
                min = rooms[i].door;

            for (int j = 0; j < rooms_count; j++)
                if (distance[j] < 80 && distance[j])
                    nei[temp++] = j;

            // sort

            // for (int k = 0; k < count - 1; k++)
            //     for (int l = k + 1; l < count; l++)
            //         if (strlen(neighbors[nei[l]]) < strlen(neighbors[nei[k]]))
            //         {
            //             int tttt = nei[k];
            //             nei[k] = nei[l];
            //             nei[l] = tttt;
            //         }

            for (int k = 0; k < count - 1; k++)
                for (int l = k + 1; l < count; l++)
                    if (distance[l] < distance[k])
                    {
                        int tttt = nei[k];
                        nei[k] = nei[l];
                        nei[l] = tttt;
                    }

            for (int j = strlen(neighbors[i]); j < min; j++)
            {
                if (strchr(neighbors[i], (char)(nei[j] + '0')) == NULL)
                {
                    neighbors[i][strlen(neighbors[i])] = (char)(nei[j] + '0');
                    neighbors[i][strlen(neighbors[i])] = '\0';
                }
                if (strchr(neighbors[nei[j]], (char)(i + '0')) == NULL)
                {
                    neighbors[nei[j]][strlen(neighbors[nei[j]])] = (char)(i + '0');
                    neighbors[nei[j]][strlen(neighbors[nei[j]])] = '\0';
                }
            }
        }
        else
        {
            if (strchr(neighbors[i], (char)(destination + '0')) == NULL)
            {
                neighbors[i][strlen(neighbors[i])] = (char)(destination + '0');
                neighbors[i][strlen(neighbors[i])] = '\0';
            }
            if (strchr(neighbors[destination], (char)(i + '0')) == NULL)
            {
                neighbors[destination][strlen(neighbors[destination])] = (char)(i + '0');
                neighbors[destination][strlen(neighbors[destination])] = '\0';
            }
        }
        fprintf(write, "%d = {%s}\n", i, neighbors[i]);
    }
    fclose(write);
}

void generate_map(char username[MAX_NAMES])
{
    int padding = 2;
    char path[MAX_LINE];
    sprintf(path, "./.users/%s/~NEW_GAME", username);
    mkdir(path, 0777);

    int difficulty;
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/GameSetting.txt", username);
    FILE *rgs = fopen(path, "r");
    fscanf(rgs, "%d\n%*d\n%*d\n%*d", &difficulty);

    int floor;
    if (difficulty == 0)
        floor = 4;
    else if (difficulty == 1)
        floor = random_num(5, 6);
    else if (difficulty == 2)
        floor = 7;
    else if (difficulty == 3)
        floor = 1;

    Floor *floors = (Floor *)malloc(sizeof(Floor) * floor);

    // Create Floor
    for (int i = 0; i < floor; i++)
    {
        memset(path, 0, sizeof(path));
        sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, i);
        FILE *write_floor = fopen(path, "w");

        int rooms_count = random_num(6, 8);
        Room *rooms = (Room *)malloc(sizeof(Room) * rooms_count);

        // Create Rooms
        for (int i = 0; i < rooms_count; i++)
        {
            int resize = 0;

            rooms[i].h = random_num(4 + (i / 2), 6 + i);
            rooms[i].w = random_num(12 + i, 25);

            while (1)
            {
                if (resize > 6)
                {
                    rooms[i].h = random_num(4 + (i / 2), 6 + i);
                    rooms[i].w = random_num(12 + i, 25);
                    resize = 0;
                }

                // باید هرچقد میتونم تعداد دورهاش رو کمتر کنم
                rooms[i].x = random_num(10, 144 - rooms[i].w);
                rooms[i].y = random_num(3, 36 - rooms[i].h - 1);
                int check = 1;
                for (int j = 0; j < i && check; j++)
                    if (rooms[i].x >= rooms[j].x - padding - rooms[i].w - 1 && rooms[i].x <= rooms[j].x + rooms[j].w + padding && rooms[i].y >= rooms[j].y - padding - rooms[i].h - 1 && rooms[i].y <= rooms[j].y + rooms[j].h + padding)
                        check = 0;

                if (check)
                    break;
                else
                    resize++;
            }

            rooms[i].visible = 0;
            rooms[i].kind = 0;
            rooms[i].door = random_num(1, 3);
            if (rooms[i].door == 1)
                rooms[i].secretDoor = random_num(0, 1);
            else
                rooms[i].secretDoor = 0;
        }

        fprintf(write_floor, "%d\n", rooms_count);
        for (int k = 0; k < rooms_count; k++)
            fprintf(write_floor, "%d %d %d %d %d %d %d %d\n", rooms[k].x, rooms[k].y, rooms[k].h, rooms[k].w, rooms[k].visible, rooms[k].kind, rooms[k].door, rooms[k].secretDoor);
        fclose(write_floor);

        floors[i].rooms_count = rooms_count;
        floors[i].rooms = rooms;
        floors[i].f = i + 1;

#pragma region generate corridor
        // generate_corridor(rooms_count, rooms, username);
        free(rooms);
    }
    free(floors);
}

void New_Game(int user_mode, char username[MAX_NAMES])
{
    UseColor();
    int scrY, scrX;
    getmaxyx(stdscr, scrY, scrX);
    int dx = scrX / 2, dy = scrY / 2;

    char path[MAX_LINE];
    int ch, p = 0;

    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME", username);
    DIR *dir = opendir(path);

    if (!dir)
    {
        mkdir(path, 0777);
        generate_map(username);
        dir = opendir(path);
    }

    struct dirent *input;
    int floor = 0;

    while ((input = readdir(dir)) != NULL)
    {
        if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
            continue;
        floor++;
    }

    while (1)
    {
        clear();
        refresh();
        memset(path, 0, sizeof(path));
        sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, p);
        FILE *read_floor = fopen(path, "r");
        int rooms_count;
        fscanf(read_floor, "%d\n", &rooms_count);

        Room *rooms = (Room *)malloc(sizeof(Room) * rooms_count);
        for (int i = 0; i < rooms_count; i++)
        {
            fscanf(read_floor, "%d %d %d %d %d %d %d %d\n", &rooms[i].x, &rooms[i].y, &rooms[i].h, &rooms[i].w, &rooms[i].visible, &rooms[i].kind, &rooms[i].door, &rooms[i].secretDoor);
            printRoom(rooms[i].y, rooms[i].x, rooms[i].h, rooms[i].w);
            mvprintw(rooms[i].y + (rooms[i].h / 2), rooms[i].x + (rooms[i].w / 2), "%d", i);
        }

        generate_corridor(rooms_count, rooms, username, p);

        ch = getch();

        if (ch == 'q')
            break;
        else if (ch == KEY_UP)
            p = (p + floor - 1) % floor;
        else if (ch == KEY_DOWN)
            p = (p + 1) % floor;
    }
    clear();
    refresh();
    flushinp();
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
    // 0 = exit
    // 1 = else

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
                    New_Game(user_mode, username);
                    // mv(user_mode);
                    if (user_menu(user_mode))
                        return 1;
                    else
                        return 0;
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

    // Mix_Quit();

    // Mix_Music *music = Mix_LoadMUS("./.musics/Main Title.mp3");
    // Mix_PlayMusic(music, -1);

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
    // SDL_Init(SDL_INIT_AUDIO);
    // Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
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

    // SDL_Quit();
    remove_dir("./.users/.~GUEST");
    endwin();
    return 0;
}