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

/*
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

void printRoom(Room r)
{
    if (!r.visible)
        return;

    int kind = r.kind, y = r.y, x = r.x, h = r.h, w = r.w, td = r.top_door, dd = r.down_door, rd = r.right_door, ld = r.right_door;
    char line_down[7], line_top[7], right_down[7], row_right[7], right_top[7], left_top[7], left_down[7], dots[w - 1], hd[7], vd[7];
    dots[w - 1] = '\0';
    memset(dots, '.', w - 1);

    strcpy(hd, "\u25AC");
    strcpy(vd, "\u25AE");

    strcpy(right_top, "\u2596");
    strcpy(left_top, "\u2597");
    strcpy(right_down, "\u2598");
    strcpy(left_down, "\u259D");

    // strcpy(row_left, "\u2590");
    strcpy(row_right, "\u258C");

    strcpy(line_top, "\u2580");
    strcpy(line_down, "\u2584");

    init_pair(14, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(13 + kind));

    mvprintw(y, x + w, "%s", right_top);
    for (int j = 0; j < w; j++)
    {
        mvprintw(y, x + j, "%s", line_down);
        mvprintw(y + h, x + j, "%s", line_top);
    }
    mvprintw(y + h, x + w, "%s", right_down);

    for (int i = y + 1; i < y + h; i++)
    {
        mvprintw(i, x, "%s", row_right);
        mvprintw(i, x + w, "%s", row_right);
    }
    attroff(COLOR_PAIR(13 + kind));
    attron(COLOR_PAIR(8));
    for (int i = y + 1; i < y + h; i++)
        mvprintw(i, x + 1, "%s", dots);
    attroff(COLOR_PAIR(8));

    attron(COLOR_PAIR(2));
    if (r.tv)
        mvprintw(r.y, r.x + r.top_door, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y, r.x + r.top_door, ".");

    if (r.dv)
        mvprintw(r.y + r.h, r.x + r.down_door, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y + r.h, r.x + r.down_door, ".");

    if (r.rv)
        mvprintw(r.y + r.right_door, r.x + r.w, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y + r.right_door, r.x + r.w, ".");
    if (r.lv)
        mvprintw(r.y + r.left_door, r.x, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y + r.left_door, r.x, ".");

    attroff(COLOR_PAIR(2));
}

void wprintRoom(Room r, WINDOW *win)
{
    int kind = r.kind, y = r.y, x = r.x, h = r.h, w = r.w, td = r.top_door, dd = r.down_door, rd = r.right_door, ld = r.right_door;
    char line_down[7], line_top[7], right_down[7], row_right[7], right_top[7], left_top[7], left_down[7], dots[w - 1], hd[7], vd[7];
    dots[w - 1] = '\0';
    memset(dots, '.', w - 1);

    strcpy(hd, "\u25AC");
    strcpy(vd, "\u25AE");

    strcpy(right_top, "\u2596");
    strcpy(left_top, "\u2597");
    strcpy(right_down, "\u2598");
    strcpy(left_down, "\u259D");

    // strcpy(row_left, "\u2590");
    strcpy(row_right, "\u258C");

    strcpy(line_top, "\u2580");
    strcpy(line_down, "\u2584");

    init_pair(14, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(13 + kind));

    mvwprintw(win, y, x + w, "%s", right_top);
    for (int j = 0; j < w; j++)
    {
        mvwprintw(win, y, x + j, "%s", line_down);
        mvwprintw(win, y + h, x + j, "%s", line_top);
    }
    mvwprintw(win, y + h, x + w, "%s", right_down);

    for (int i = y + 1; i < y + h; i++)
    {
        mvwprintw(win, i, x, "%s", row_right);
        mvwprintw(win, i, x + w, "%s", row_right);
    }
    attroff(COLOR_PAIR(13 + kind));
    attron(COLOR_PAIR(8));
    for (int i = y + 1; i < y + h; i++)
        mvwprintw(win, i, x + 1, "%s", dots);
    attroff(COLOR_PAIR(8));

    attron(COLOR_PAIR(2));
    if (r.tv)
        mvwprintw(win, r.y, r.x + r.top_door, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y, r.x + r.top_door, ".");

    if (r.dv)
        mvwprintw(win, r.y + r.h, r.x + r.down_door, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y + r.h, r.x + r.down_door, ".");

    if (r.rv)
        mvwprintw(win, r.y + r.right_door, r.x + r.w, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y + r.right_door, r.x + r.w, ".");
    if (r.lv)
        mvwprintw(win, r.y + r.left_door, r.x, "\u2593");
    // else if (random_num(0, 1))
    // mvprintw(r.y + r.left_door, r.x, ".");

    attroff(COLOR_PAIR(2));
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

    Player *p = (Player *)malloc(sizeof(Player));
    p->x = x;
    p->y = y;
    int ch;
    while (1)
    {
        attron(A_BLINK);
        mvprintw(p->y, p->x, "\u25B6");
        attroff(A_BLINK);
        ch = getch();
        if ((ch == KEY_UP || ch == KEY_DOWN || ch == KEY_RIGHT || ch == KEY_LEFT) && (p->x > 0 && p->x < scrX && p->y > 0 && p->y < scrY - 1))
            mvprintw(p->y, p->x, " ");

        if (ch == KEY_UP && p->y > 1)
            p->y--;
        else if (ch == KEY_DOWN && p->y < scrY - 2)
            p->y++;
        else if (ch == KEY_LEFT && p->x > 1)
            p->x--;
        else if (ch == KEY_RIGHT && p->x < scrX - 2)
            p->x++;
        else if (ch == 'q')
            break;
    }
}

void printHeart(int health, int hungry)
{
    mvprintw(1, 28, "                             ");
    mvprintw(1, 95, "                             ");
    for (int i = 0; i < (health + 9) / 10; i++)
        mvprintw(1, 28 + (3 * i), "\U0001F9E1");
    for (int i = 0; i < (hungry + 9) / 10; i++)
        mvprintw(1, 95 + (3 * i), "\U0001F356");
}

void print_rooms(char username[MAX_NAMES], int p)
{
    char path[MAX_LINE];
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, p);
    FILE *read_floor = fopen(path, "r");
    int rooms_count;
    fscanf(read_floor, "%d\n", &rooms_count);
    Room *rooms = (Room *)malloc(sizeof(Room) * rooms_count);
    for (int i = 0; i < rooms_count; i++)
    {
        fscanf(read_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &rooms[i].x, &rooms[i].y, &rooms[i].h, &rooms[i].w, &rooms[i].visible, &rooms[i].kind, &rooms[i].top_door, &rooms[i].down_door, &rooms[i].right_door, &rooms[i].left_door, &rooms[i].tv, &rooms[i].dv, &rooms[i].rv, &rooms[i].lv);
        printRoom(rooms[i]);
        // mvprintw(rooms[i].y + (rooms[i].h / 2), rooms[i].x + (rooms[i].w / 2), "%d", i);
    }
    fclose(read_floor);
}

void show_corridor(char username[MAX_NAMES], int p)
{
    char path[MAX_LINE];
    sprintf(path, "./.users/%s/~NEW_GAME/cells_%d.txt", username, p);
    FILE *read_cells = fopen(path, "r");
    char line[MAX_LINE];
    attron(COLOR_PAIR(2));
    while (fgets(line, sizeof(line), read_cells))
    {
        int y, x, v;
        sscanf(line, "%d %d %d\n", &x, &y, &v);
        if (v)
            mvprintw(y, x, "\u2593");
    }
    attroff(COLOR_PAIR(2));
    fclose(read_cells);
}

int find_xy(cell **c, int y, int x, int v)
{
    cell *current = *c;
    while (current != NULL)
    {
        if ((current)->x == x && (current)->y == y)
        {
            current->v = 1;
            mvprintw(y, x, "\u2593");
            return 1;
        }
        current = (current)->next;
    }
    return 0;
}

int is_door(Room *rooms, int rooms_count, int y, int x)
{
    for (int i = 0; i < rooms_count; i++)
    {
        // right door
        if (rooms[i].rv && x == rooms[i].x + rooms[i].w && y == rooms[i].y + rooms[i].right_door)
            return i + 1;

        // left door
        else if (rooms[i].lv && x == rooms[i].x && y == rooms[i].y + rooms[i].left_door)
            return i + 1;

        // top door
        else if (rooms[i].tv && x == rooms[i].x + rooms[i].top_door && y == rooms[i].y)
            return i + 1;

        // down door
        else if (rooms[i].dv && x == rooms[i].x + rooms[i].down_door && y == rooms[i].y + rooms[i].h)
            return i + 1;
    }
    return 0;
}

void start_game(int user_mode)
{
    UseColor();
    int scrY, scrX;
    getmaxyx(stdscr, scrY, scrX);
    int x = scrX / 2, y = scrY / 2;

    int floors = 0;

    // Read UserName
    FILE *read_for_username = fopen("./.users/Login.txt", "r");
    char username[MAX_NAMES];
    fscanf(read_for_username, "%*d\n%s", username);
    fclose(read_for_username);

    char path[MAX_LINE];
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME", username);
    // If Not Exist = Create User and count floors
    if (access(path, F_OK) == -1)
    {

        clear();
        refresh();
        flushinp();
        New_Game(user_mode, username);
        start_game(user_mode);
        return;
    }
    else
    {
        DIR *readFLoor = opendir(path);
        readFLoor = opendir(path);

        struct dirent *input;

        while ((input = readdir(readFLoor)) != NULL)
        {
            if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
                continue;
            if (input->d_name[0] == 'f')
                floors++;
        }
    }

    Player *p = (Player *)malloc(sizeof(Player));
    // Current Status for spawn Player
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME/cs.txt", username);
    int stair_room, current_floor, current_room;
    FILE *read_cs = fopen(path, "r");
    fscanf(read_cs, "%d %d\n%d\n%d\n%d\n%d\n%d", &p->x, &p->y, &current_floor, &p->health, &p->hungry, &current_room, &stair_room);
    fclose(read_cs);

    // Read LeaderBoard
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/LeaderBoard.txt");
    int users_count;
    FILE *leaderBoard = fopen(path, "r");
    fscanf(leaderBoard, "%d\n", &users_count);
    char names[users_count][MAX_NAMES];
    int golds[users_count], score[users_count], games[users_count], XPs[users_count], user_index = -1;
    for (int i = 0; i < users_count; i++)
    {
        fscanf(leaderBoard, "%s %d %d %d %d\n", names[i], &golds[i], &score[i], &games[i], &XPs[i]);
        if (strcmp(username, names[i]) == 0)
            user_index = i;
    }
    fclose(leaderBoard);
    p->XP = XPs[user_index];
    p->score = score[user_index];
    p->golds = golds[user_index];

    // Read Game Setting
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/GameSetting.txt", username);
    int character, character_color, difficulty;
    FILE *read_gs = fopen(path, "r");
    fscanf(read_gs, "%d\n%d\n%d\n%*d\n", &difficulty, &character, &character_color);
    fclose(read_gs);

    char *characters[5] = {"A", "ᛟ", "ᛸ", "\u2B55", "\U0001F7E5"};
    char *u26AA[6] = {"\u2B55", "\U0001F7E2", "\U0001F535", "\U0001F7E0", "\U0001F7E4", "\U0001F7E3"};
    char *u2590[6] = {"\U0001F7E5", "\U0001F7E9", "\U0001F7E6", "\U0001F7E7", "\U0001F7EB", "\U0001F7EA"};

    if (character < 3)
        strcpy(p->c, characters[character]);
    else if (character == 3)
        strcpy(p->c, u26AA[character_color]);
    else if (character == 4)
        strcpy(p->c, u2590[character_color]);

    int b = 1;
    while (b)
    {
        // Read cells
        cell *c = (cell *)malloc(sizeof(cell));
        char line[MAX_LINE];
        memset(path, 0, sizeof(path));
        sprintf(path, "./.users/%s/~NEW_GAME/cells_%d.txt", username, current_floor);
        FILE *read_cells = fopen(path, "r");
        if (fgets(line, sizeof(line), read_cells))
        {
            int x1, y1, v1;
            sscanf(line, "%d %d %d\n", &x1, &y1, &v1);
            c->x = x1;
            c->y = y1;
            c->v = v1;
            c->next = NULL;
        }
        while (fgets(line, sizeof(line), read_cells))
        {
            int x1, y1, v1;
            sscanf(line, "%d %d %d\n", &x1, &y1, &v1);
            add_to_corridor(&c, x1, y1, v1);
        }

        // Read Rooms
        memset(path, 0, sizeof(path));
        sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, current_floor);
        int rooms_count;
        FILE *read_floor = fopen(path, "r");
        fscanf(read_floor, "%d\n", &rooms_count);
        Room *rooms = (Room *)malloc(sizeof(Room) * rooms_count);
        for (int i = 0; i < rooms_count; i++)
            fscanf(read_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &rooms[i].x, &rooms[i].y, &rooms[i].h, &rooms[i].w, &rooms[i].visible, &rooms[i].kind, &rooms[i].top_door, &rooms[i].down_door, &rooms[i].right_door, &rooms[i].left_door, &rooms[i].tv, &rooms[i].dv, &rooms[i].rv, &rooms[i].lv);
        fclose(read_floor);

        // ---------------------------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------------------------------------------

        int ch, load_floor = 1, status = current_room, walk = 0, f = 0;
        // status -1 = in corridor
        while (1)
        {

            if (walk > 20)
            {
                p->hungry -= (difficulty + 1) * (walk - (walk % 20)) / 10;
                printHeart(p->health, p->hungry);
                p->XP += (difficulty + 1) * (walk - (walk % 20)) / 10;
                walk = 0;
            }

            int new_room = is_door(rooms, rooms_count, p->y, p->x);

            if (new_room && !rooms[new_room - 1].visible)
            {
                rooms[new_room - 1].visible = 1;
                printRoom(rooms[new_room - 1]);
                p->XP += 15;
            }

            if (load_floor)
            {
                clear();
                refresh();
                printHeart(p->health, p->hungry);
                attron(COLOR_PAIR(8));
                mvprintw(0, x - 7, "Bl4ck H0l3 <%d>", current_floor + 1);
                attroff(COLOR_PAIR(8));
                show_corridor(username, current_floor);
                print_rooms(username, current_floor);
                load_floor = 0;
                attron(COLOR_PAIR(9) | A_BOLD);
                mvprintw(2, x - strlen(username) / 2, "%s", username);
                attroff(COLOR_PAIR(9) | A_BOLD);
            }

            if (rooms[stair_room].visible && current_floor != floors - 1)
            {
                attron(COLOR_PAIR(8));
                mvprintw(rooms[stair_room].y + rooms[stair_room].h / 2, rooms[stair_room].x + (rooms[stair_room].w / 2) - 2, "<..>");
                if (current_floor == 0)
                    mvprintw(rooms[stair_room].y + rooms[stair_room].h / 2, rooms[stair_room].x + (rooms[stair_room].w / 2) - 2, "<.");
                if (current_floor == floors - 1)
                    mvprintw(rooms[stair_room].y + rooms[stair_room].h / 2, rooms[stair_room].x + (rooms[stair_room].w / 2), ".>");
                attroff(COLOR_PAIR(8));
            }

            mvprintw(1, x - 3, "       ");
            mvprintw(1, x - 3, "%d %d", p->x, p->y);

            char previous_character = mvinch(p->y, p->x), Next_character;

            attron(COLOR_PAIR(character_color + 20));
            mvprintw(p->y, p->x, "%s", p->c);
            attroff(COLOR_PAIR(character_color + 20));

            // getch();
            ch = getch();
            // if ((ch == KEY_UP || ch == KEY_DOWN || ch == KEY_RIGHT || ch == KEY_LEFT) && (p->x > 0 && p->x < scrX && p->y > 0 && p->y < scrY - 1))
            // {
            if (previous_character == '.')
            {
                attron(COLOR_PAIR(8));
                mvprintw(p->y, p->x, "%c", previous_character);
                attroff(COLOR_PAIR(8));
            }
            else if (previous_character == '>' && ch == '>' && current_floor != floors - 1)
            {
                current_floor++;

                free(rooms);

                // Read Rooms
                memset(path, 0, sizeof(path));
                sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, current_floor);
                int rooms_count;
                read_floor = fopen(path, "r");
                fscanf(read_floor, "%d\n", &rooms_count);
                Room *rooms = (Room *)malloc(sizeof(Room) * rooms_count);
                for (int i = 0; i < rooms_count; i++)
                    fscanf(read_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &rooms[i].x, &rooms[i].y, &rooms[i].h, &rooms[i].w, &rooms[i].visible, &rooms[i].kind, &rooms[i].top_door, &rooms[i].down_door, &rooms[i].right_door, &rooms[i].left_door, &rooms[i].tv, &rooms[i].dv, &rooms[i].rv, &rooms[i].lv);
                fclose(read_floor);

                p->x = random_num(rooms[stair_room].x + 1, rooms[stair_room].x + rooms[stair_room].w - 1);
                p->y = random_num(rooms[stair_room].y + 1, rooms[stair_room].y + rooms[stair_room].h - 1);

                rooms[stair_room].visible = 1;

                // save rooms
                sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, current_floor);
                FILE *write_rooms = fopen(path, "w");
                fprintf(write_rooms, "%d\n", rooms_count);
                for (int i = 0; i < rooms_count; i++)
                    fprintf(read_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", rooms[i].x, rooms[i].y, rooms[i].h, rooms[i].w, rooms[i].visible, rooms[i].kind, rooms[i].top_door, rooms[i].down_door, rooms[i].right_door, rooms[i].left_door, rooms[i].tv, rooms[i].dv, rooms[i].rv, rooms[i].lv);
                fclose(write_rooms);
                break;
            }
            else if (previous_character == '<' && ch == '<' && current_floor != 0)
            {
                current_floor--;
                free(rooms);

                // Read Rooms
                memset(path, 0, sizeof(path));
                sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, current_floor);
                int rooms_count;
                read_floor = fopen(path, "r");
                fscanf(read_floor, "%d\n", &rooms_count);
                Room *rooms = (Room *)malloc(sizeof(Room) * rooms_count);
                for (int i = 0; i < rooms_count; i++)
                    fscanf(read_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &rooms[i].x, &rooms[i].y, &rooms[i].h, &rooms[i].w, &rooms[i].visible, &rooms[i].kind, &rooms[i].top_door, &rooms[i].down_door, &rooms[i].right_door, &rooms[i].left_door, &rooms[i].tv, &rooms[i].dv, &rooms[i].rv, &rooms[i].lv);
                fclose(read_floor);

                p->x = random_num(rooms[stair_room].x + 1, rooms[stair_room].x + rooms[stair_room].w - 1);
                p->y = random_num(rooms[stair_room].y + 1, rooms[stair_room].y + rooms[stair_room].h - 1);
                // save rooms
                sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, current_floor);
                FILE *write_rooms = fopen(path, "w");
                fprintf(write_rooms, "%d\n", rooms_count);
                for (int i = 0; i < rooms_count; i++)
                    fprintf(read_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", rooms[i].x, rooms[i].y, rooms[i].h, rooms[i].w, rooms[i].visible, rooms[i].kind, rooms[i].top_door, rooms[i].down_door, rooms[i].right_door, rooms[i].left_door, rooms[i].tv, rooms[i].dv, rooms[i].rv, rooms[i].lv);
                fclose(write_rooms);
                break;
            }
            else
            {
                mvprintw(p->y, p->x, "\u2593");
            }
            // }

            if (ch == KEY_UP && p->y > 4 && (is_door(rooms, rooms_count, p->y - 1, p->x) || ((char)mvinch(p->y - 1, p->x) >= '!' && (char)mvinch(p->y - 1, p->x) <= '~') || find_xy(&c, p->y - 1, p->x, 1)))
            {
                if (f)
                {
                    previous_character = mvinch(p->y, p->x);
                    if (previous_character == '.')
                    {
                        attron(COLOR_PAIR(8));
                        mvprintw(p->y, p->x, "%c", previous_character);
                        attroff(COLOR_PAIR(8));
                    }
                    else
                        mvprintw(p->y, p->x, "\u2593");
                    while (ch == KEY_UP && p->y > 4 && (is_door(rooms, rooms_count, p->y - 1, p->x) || ((char)mvinch(p->y - 1, p->x) >= '!' && (char)mvinch(p->y - 1, p->x) <= '~') || find_xy(&c, p->y - 1, p->x, 1)))
                    {
                        walk++;
                        p->y--;
                        find_xy(&c, p->y - 1, p->x, 1);
                        find_xy(&c, p->y - 2, p->x, 1);
                        find_xy(&c, p->y - 1, p->x + 1, 1);
                        find_xy(&c, p->y - 1, p->x - 1, 1);
                    }
                    f = 0;
                }
                else
                {
                    walk++;
                    p->y--;
                    find_xy(&c, p->y - 1, p->x, 1);
                    find_xy(&c, p->y - 2, p->x, 1);
                    find_xy(&c, p->y - 1, p->x + 1, 1);
                    find_xy(&c, p->y - 1, p->x - 1, 1);
                }
            }
            else if (ch == KEY_DOWN && p->y < scrY - 5 && (is_door(rooms, rooms_count, p->y + 1, p->x) || ((char)mvinch(p->y + 1, p->x) >= '!' && (char)mvinch(p->y + 1, p->x) <= '~') || find_xy(&c, p->y + 1, p->x, 1)))
            {
                if (f)
                {
                    previous_character = mvinch(p->y, p->x);
                    if (previous_character == '.')
                    {
                        attron(COLOR_PAIR(8));
                        mvprintw(p->y, p->x, "%c", previous_character);
                        attroff(COLOR_PAIR(8));
                    }
                    else
                        mvprintw(p->y, p->x, "\u2593");
                    while (ch == KEY_DOWN && p->y < scrY - 5 && (is_door(rooms, rooms_count, p->y + 1, p->x) || ((char)mvinch(p->y + 1, p->x) >= '!' && (char)mvinch(p->y + 1, p->x) <= '~') || find_xy(&c, p->y + 1, p->x, 1)))
                    {
                        walk++;
                        p->y++;
                        find_xy(&c, p->y + 1, p->x, 1);
                        find_xy(&c, p->y + 2, p->x, 1);
                        find_xy(&c, p->y + 1, p->x + 1, 1);
                        find_xy(&c, p->y + 1, p->x - 1, 1);
                    }
                    f = 0;
                }
                else
                {
                    walk++;
                    p->y++;
                    find_xy(&c, p->y + 1, p->x, 1);
                    find_xy(&c, p->y + 2, p->x, 1);
                    find_xy(&c, p->y + 1, p->x + 1, 1);
                    find_xy(&c, p->y + 1, p->x - 1, 1);
                }
            }
            else if (ch == KEY_LEFT && p->x > 1 && (is_door(rooms, rooms_count, p->y, p->x - 1) || ((char)mvinch(p->y, p->x - 1) >= '!' && (char)mvinch(p->y, p->x - 1) <= '~') || find_xy(&c, p->y, p->x - 1, 1)))
            {
                if (f)
                {
                    previous_character = mvinch(p->y, p->x);
                    if (previous_character == '.')
                    {
                        attron(COLOR_PAIR(8));
                        mvprintw(p->y, p->x, "%c", previous_character);
                        attroff(COLOR_PAIR(8));
                    }
                    else
                        mvprintw(p->y, p->x, "\u2593");
                    while (ch == KEY_LEFT && p->x > 1 && (is_door(rooms, rooms_count, p->y, p->x - 1) || ((char)mvinch(p->y, p->x - 1) >= '!' && (char)mvinch(p->y, p->x - 1) <= '~') || find_xy(&c, p->y, p->x - 1, 1)))
                    {
                        walk++;
                        p->x--;
                        find_xy(&c, p->y, p->x - 1, 1);
                        find_xy(&c, p->y, p->x - 2, 1);
                        find_xy(&c, p->y + 1, p->x - 1, 1);
                        find_xy(&c, p->y - 1, p->x - 1, 1);
                    }

                    f = 0;
                }
                else
                {
                    walk++;
                    p->x--;
                    find_xy(&c, p->y, p->x - 1, 1);
                    find_xy(&c, p->y, p->x - 2, 1);
                    find_xy(&c, p->y + 1, p->x - 1, 1);
                    find_xy(&c, p->y - 1, p->x - 1, 1);
                }
            }
            else if (ch == KEY_RIGHT && p->x < scrX - 2 && (is_door(rooms, rooms_count, p->y, p->x + 1) || ((char)mvinch(p->y, p->x + 1) >= '!' && (char)mvinch(p->y, p->x + 1) <= '~') || find_xy(&c, p->y, p->x + 1, 1)))
            {
                if (f)
                {
                    previous_character = mvinch(p->y, p->x);
                    if (previous_character == '.')
                    {
                        attron(COLOR_PAIR(8));
                        mvprintw(p->y, p->x, "%c", previous_character);
                        attroff(COLOR_PAIR(8));
                    }
                    else
                        mvprintw(p->y, p->x, "\u2593");
                    while (ch == KEY_RIGHT && p->x < scrX - 2 && (is_door(rooms, rooms_count, p->y, p->x + 1) || ((char)mvinch(p->y, p->x + 1) >= '!' && (char)mvinch(p->y, p->x + 1) <= '~') || find_xy(&c, p->y, p->x + 1, 1)))
                    {
                        walk++;
                        p->x++;
                        find_xy(&c, p->y, p->x + 1, 1);
                        find_xy(&c, p->y, p->x + 2, 1);
                        find_xy(&c, p->y + 1, p->x + 1, 1);
                        find_xy(&c, p->y - 1, p->x + 1, 1);
                    }
                    f = 0;
                }
                else
                {
                    walk++;
                    p->x++;
                    find_xy(&c, p->y, p->x + 1, 1);
                    find_xy(&c, p->y, p->x + 2, 1);
                    find_xy(&c, p->y + 1, p->x + 1, 1);
                    find_xy(&c, p->y - 1, p->x + 1, 1);
                }
            }
            else if (ch == 'q')
            {
                // WINDOW *win = newwin(scrY, scrX, 0, 0);
                // box(win, 0, 0);
                // mvwprintw(win, scrY / 2, scrX / 2, "");
#pragma region save

                // save cells
                memset(path, 0, sizeof(path));
                sprintf(path, "./.users/%s/~NEW_GAME/cells_%d.txt", username, current_floor);
                FILE *file = fopen(path, "w");
                while (c != NULL)
                {
                    fprintf(file, "%d %d %d\n", c->x, c->y, c->v);
                    c = c->next;
                }
                fclose(file);
                free(c);

                // save rooms
                sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, current_floor);
                FILE *write_rooms = fopen(path, "w");
                fprintf(write_rooms, "%d\n", rooms_count);
                for (int i = 0; i < rooms_count; i++)
                    fprintf(read_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", rooms[i].x, rooms[i].y, rooms[i].h, rooms[i].w, rooms[i].visible, rooms[i].kind, rooms[i].top_door, rooms[i].down_door, rooms[i].right_door, rooms[i].left_door, rooms[i].tv, rooms[i].dv, rooms[i].rv, rooms[i].lv);
                fclose(write_rooms);
                b = 0;
                break;
            }
            else if (ch == 'm')
            {
                WINDOW *win = newwin(scrY, scrX, 0, 0);
                box(win, 0, 0);
                for (int j = 0; j < rooms_count; j++)
                {
                    wprintRoom(rooms[j], win);
                    wrefresh(win);
                }
                getch();
                delwin(win);
                clear();
                refresh();
                load_floor = 1;
                continue;
            }
            else if (ch == 'e')
            {
#pragma region save
                // break;
            }
            else if (ch == 'f')
                f = !f;
            else if (ch == 'g')
            {
#pragma region save
                // break;
            }
            new_room = 0;
        }
    }

    // Read LeaderBoard
    XPs[user_index] = p->XP;
    score[user_index] = p->score;
    golds[user_index] = p->golds;
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/LeaderBoard.txt");
    leaderBoard = fopen(path, "w");
    fprintf(leaderBoard, "%d\n", users_count);
    for (int i = 0; i < users_count; i++)
    {
        fprintf(leaderBoard, "%s %d %d %d %d\n", names[i], golds[i], score[i], games[i], XPs[i]);
        if (strcmp(username, names[i]) == 0)
            user_index = i;
    }
    fclose(leaderBoard);

    // current status of user
    sprintf(path, "./.users/%s/~NEW_GAME/cs.txt", username);
    FILE *write_cs = fopen(path, "w");
    fprintf(write_cs, "%d %d\n%d\n%d\n%d\n%d\n%d\n", p->x, p->y, current_floor, p->health, p->hungry, current_room, stair_room);
    fclose(write_cs);

    clear();
    refresh();
    flushinp();
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

void setMusic(int user_mode)
{
    if (user_mode == 3)
        return;

    char path[MAX_LINE], username[MAX_NAMES];

    FILE *adsfsfdsa = fopen("./.users/Login.txt", "r");
    fscanf(adsfsfdsa, "%*d\n%s", username);
    fclose(adsfsfdsa);

    int target;
    sprintf(path, "./.users/%s/GameSetting.txt", username);
    FILE *gs = fopen(path, "r");
    fscanf(gs, "%*d\n%*d\n%*d\n%d", &target);
    fclose(gs);

    char music_names[MAX_NAMES][MAX_NAMES];
    DIR *dir_read = opendir("./.musics");
    int index = 0;
    struct dirent *input2;
    while ((input2 = readdir(dir_read)) != NULL)
    {
        if (!strcmp(input2->d_name, ".") || !strcmp(input2->d_name, ".."))
            continue;
        strcpy(music_names[index], input2->d_name);
        music_names[index][strlen(music_names[index])] = '\0';
        index++;
    }
    closedir(dir_read);
    sprintf(path, "./.musics/%s", music_names[target]);

    Mix_Quit();
    Mix_Music *music = Mix_LoadMUS(path);
    Mix_PlayMusic(music, -1);
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
        while (user_menu(user_mode))
        {
            // setMusic(user_mode);
            start_game(user_mode);
            // Mix_Quit();
        }
    }

    // SDL_Quit();
    remove_dir("./.users/.~GUEST");
    endwin();
    return 0;
}