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
void EditGamesInfo(char username[MAX_NAMES])
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
*/

/*
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
*/

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

    int tv;
    int dv;
    int rv;
    int lv;
    int top_door;
    int down_door;
    int right_door;
    int left_door;
} Room;

typedef struct Floor
{
    int f;
    int rooms_count;
    Room *rooms;
} Floor;

typedef struct cell
{
    int x;
    int y;
    int v;
    struct cell *next;
} cell;

void add_to_corridor(cell **corridor, int x, int y, int visible, int *count)
{
    cell *newCell = (cell *)malloc(sizeof(cell));
    newCell->x = x;
    newCell->y = y;
    newCell->v = visible;
    newCell->next = *corridor;
    *corridor = newCell;
}

void printRoom(Room r)
{
    // if (!r.visible)
    // return;

    int kind = r.kind, y = r.y, x = r.x, h = r.h, w = r.w, td = r.top_door, dd = r.down_door, rd = r.right_door, ld = r.right_door;
    char line_down[7], line_top[7], right_down[7], row_left[7], row_right[7], right_top[7], left_top[7], left_down[7], dots[w - 1], hd[7], vd[7];
    dots[w - 2] = '\0';
    memset(dots, '.', w - 2);

    strcpy(hd, "\u25AC");
    strcpy(vd, "\u25AE");

    strcpy(right_top, "\u2596");
    strcpy(left_top, "\u2597");
    strcpy(right_down, "\u2598");
    strcpy(left_down, "\u259D");

    strcpy(row_left, "\u2590");
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
}

int nothing_in(int x, int y, Room *rooms, int rooms_count)
{
    int check = -1;
    for (int i = 0; i < rooms_count; i++)
        if ((x >= rooms[i].x - 1 && x <= rooms[i].x + rooms[i].w + 1) && (y >= rooms[i].y - 1 && y <= rooms[i].y + rooms[i].h + 1))
            check = i;

    return check;
}

void add_door_to_room(char username[MAX_NAMES], int floor, int room, int door)
{
    char path[MAX_LINE];
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME/floor_%d", username, floor);
    FILE *read = fopen(path, "r");
    int vd[4];
    int rooms_count;
    fscanf(read, "%d\n", &rooms_count);
    Room *rooms = (Room *)malloc(sizeof(Room) * rooms_count);
    for (int i = 0; i < rooms_count; i++)
    {
        fscanf(read, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &rooms[i].x, &rooms[i].y, &rooms[i].h, &rooms[i].w, &rooms[i].visible, &rooms[i].kind, &rooms[i].top_door, &rooms[i].down_door, &rooms[i].right_door, &rooms[i].left_door, &rooms[i].tv, &rooms[i].dv, &rooms[i].rv, &rooms[i].lv);
        if (i == room)
        {
            vd[0] = rooms[i].tv;
            vd[1] = rooms[i].dv;
            vd[2] = rooms[i].rv;
            vd[3] = rooms[i].lv;
        }
    }
    if (!vd[door])
        vd[door]++;
    fclose(read);
    vd[0] = rooms[room].tv;
    vd[1] = rooms[room].dv;
    vd[2] = rooms[room].rv;
    vd[3] = rooms[room].lv;

    FILE *write = fopen(path, "w");
    fprintf(write, "%d\n", rooms_count);
    for (int i = 0; i < rooms_count; i++)
        fprintf(read, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", rooms[i].x, rooms[i].y, rooms[i].h, rooms[i].w, rooms[i].visible, rooms[i].kind, rooms[i].top_door, rooms[i].down_door, rooms[i].right_door, rooms[i].left_door, rooms[i].tv, rooms[i].dv, rooms[i].rv, rooms[i].lv);
    fclose(write);
}

void print_c(cell **c)
{
    cell *current = *c;
    while (current != NULL)
        if (current->v)
            mvprintw(current->y, current->x, "\u2591");
}

void a_to_b(int **matrix, int org, int des, Room *rooms, int rooms_count, char username[MAX_NAMES], int floor)
{
    if (org == des)
        return;

    int visible = 1;
    // road length
    int *length;
    *length = 0;

    int org_ox = rooms[org].x + (rooms[org].w / 2);
    int org_oy = rooms[org].y + (rooms[org].h / 2);
    int des_ox = rooms[des].x + (rooms[des].w / 2);
    int des_oy = rooms[des].y + (rooms[des].h / 2);

    // quadrant 1
    if (rooms[org].x + rooms[org].w + 3 < des_ox && rooms[org].y - 3 > des_oy)
    {
        // default = right door origin
        int orgDoor_x = rooms[org].w + rooms[org].x + 1;
        int orgDoor_y = rooms[org].y + rooms[org].right_door;
        add_door_to_room(username, floor, org, 2);

        // default = down door destination
        int desDoor_x = rooms[des].x + rooms[des].down_door;
        int desDoor_y = rooms[des].y + rooms[des].h + 2;
        add_door_to_room(username, floor, des, 2);

        // start tracking to destination door from here
        cell *c = (cell *)malloc(sizeof(cell));
        c->x = orgDoor_x;
        c->y = orgDoor_y;
        c->v = visible;
        c->next = NULL;
        add_to_corridor(&c, c->x + 1, c->y, visible, length);

        int x_distance = desDoor_x - orgDoor_x;
        int y_distance = desDoor_y - orgDoor_y;

        while (c->x != desDoor_x && c->y != desDoor_y)
        {
            int l = *length;

            // should go right
            if (c->x < desDoor_x)
            {
                int check = nothing_in(c->x + 1, c->y, rooms, rooms_count);
                // go right
                if (check == -1)
                    add_to_corridor(&c, c->x + 1, c->y, visible, length);

                // go up until can go right
                else
                {
                    int obstacle = check;
                    if (!matrix[org][obstacle])
                    {
                        int obsDoor_y = rooms[obstacle].y + rooms[obstacle].left_door;
                        add_door_to_room(username, floor, obstacle, 3);
                        matrix[org][obstacle] = 1;
                        matrix[obstacle][org] = 1;
                        while (obsDoor_y > c->y)
                            add_to_corridor(&c, c->x, c->y + 1, visible, length);
                        while (obsDoor_y < c->y)
                            add_to_corridor(&c, c->x, c->y - 1, visible, length);
                        print_c(&c);
                    }
                    free(c);
                    a_to_b(matrix, obstacle, des, rooms, rooms_count, username, floor);
                    break;
                }
            }

            // should go up
            else if (c->y > desDoor_y)
            {
                int check = nothing_in(c->x, c->y - 1, rooms, rooms_count);
                // go up
                if (check == -1)
                    add_to_corridor(&c, c->x, c->y - 1, visible, length);

                // go up until can go right
                else
                {
                    int obstacle = check;
                    if (!matrix[org][obstacle])
                    {
                        int obsDoor_x = rooms[obstacle].x + rooms[obstacle].down_door;
                        add_door_to_room(username, floor, obstacle, 1);
                        matrix[org][obstacle] = 1;
                        matrix[obstacle][org] = 1;
                        while (obsDoor_x > c->x)
                            add_to_corridor(&c, c->x + 1, c->y, visible, length);
                        while (obsDoor_x < c->x)
                            add_to_corridor(&c, c->x - 1, c->y, visible, length);
                        print_c(&c);
                    }
                    free(c);
                    a_to_b(matrix, obstacle, des, rooms, rooms_count, username, floor);
                    break;
                }
            }

            if (*length > 150 || *length == l)
                break;
        }

        c->y - 1;
        print_c(&c);
        return;
    }

    // quadrant 2
    else if (rooms[org].x - 3 > des_ox && rooms[org].y - 3 > des_oy)
    {
        // default = left door origin
        int orgDoor_x = rooms[org].x - 1;
        int orgDoor_y = rooms[org].y + rooms[org].left_door;
        add_door_to_room(username, floor, org, 3);

        // default = down door destination
        int desDoor_x = rooms[des].x + rooms[des].down_door;
        int desDoor_y = rooms[des].y + rooms[des].h + 2;
        add_door_to_room(username, floor, des, 1);

        // start tracking to destination door from here
        cell *c = (cell *)malloc(sizeof(cell));
        c->x = orgDoor_x;
        c->y = orgDoor_y;
        c->v = visible;
        c->next = NULL;
        add_to_corridor(&c, c->x - 1, c->y, visible, length);

        int x_distance = desDoor_x - orgDoor_x;
        int y_distance = desDoor_y - orgDoor_y;

        while (c->x != desDoor_x && c->y != desDoor_y)
        {
            int l = *length;

            // should go left
            if (c->x > desDoor_x)
            {
                int check = nothing_in(c->x - 1, c->y, rooms, rooms_count);
                // go left
                if (check == -1)
                    add_to_corridor(&c, c->x - 1, c->y, visible, length);

                // go up until can go left
                else
                {
                    int obstacle = check;
                    if (!matrix[org][obstacle])
                    {
                        int obsDoor_y = rooms[obstacle].y + rooms[obstacle].right_door;
                        add_door_to_room(username, floor, obstacle, 2);
                        matrix[org][obstacle] = 1;
                        matrix[obstacle][org] = 1;
                        while (obsDoor_y > c->y)
                            add_to_corridor(&c, c->x, c->y + 1, visible, length);
                        while (obsDoor_y < c->y)
                            add_to_corridor(&c, c->x, c->y - 1, visible, length);
                        print_c(&c);
                    }
                    free(c);
                    a_to_b(matrix, obstacle, des, rooms, rooms_count, username, floor);
                    break;
                }
            }

            // should go up
            else if (c->y > desDoor_y)
            {
                int check = nothing_in(c->x, c->y - 1, rooms, rooms_count);
                // go up
                if (check == -1)
                    add_to_corridor(&c, c->x, c->y - 1, visible, length);

                // go up until can go right
                else
                {
                    int obstacle = check;
                    if (!matrix[org][obstacle])
                    {
                        int obsDoor_x = rooms[obstacle].x + rooms[obstacle].down_door;
                        add_door_to_room(username, floor, obstacle, 1);
                        matrix[org][obstacle] = 1;
                        matrix[obstacle][org] = 1;
                        while (obsDoor_x > c->x)
                            add_to_corridor(&c, c->x + 1, c->y, visible, length);
                        while (obsDoor_x < c->x)
                            add_to_corridor(&c, c->x - 1, c->y, visible, length);
                        print_c(&c);
                    }
                    free(c);
                    a_to_b(matrix, obstacle, des, rooms, rooms_count, username, floor);
                    break;
                }
            }

            if (*length > 150 || *length == l)
                break;
        }

        c->y - 1;
        print_c(&c);
        return;
    }

    // quadrant 3
    else if (rooms[org].x - 3 > des_ox && rooms[org].y + rooms[org].h + 3 < des_oy)
    {
        // default = left door origin
        int orgDoor_x = rooms[org].x - 1;
        int orgDoor_y = rooms[org].y + rooms[org].left_door;
        add_door_to_room(username, floor, org, 3);

        // default = top door destination
        int desDoor_x = rooms[des].x + rooms[des].top_door;
        int desDoor_y = rooms[des].y - 2;
        add_door_to_room(username, floor, des, 0);

        // start tracking to destination door from here
        cell *c = (cell *)malloc(sizeof(cell));
        c->x = orgDoor_x;
        c->y = orgDoor_y;
        c->v = visible;
        c->next = NULL;
        add_to_corridor(&c, c->x - 1, c->y, visible, length);

        int x_distance = desDoor_x - orgDoor_x;
        int y_distance = desDoor_y - orgDoor_y;

        while (c->x != desDoor_x && c->y != desDoor_y)
        {
            int l = *length;

            // should go left
            if (c->x > desDoor_x)
            {
                int check = nothing_in(c->x - 1, c->y, rooms, rooms_count);
                // go left
                if (check == -1)
                    add_to_corridor(&c, c->x - 1, c->y, visible, length);

                // go down until can go left
                else
                {
                    int obstacle = check;
                    if (!matrix[org][obstacle])
                    {
                        int obsDoor_y = rooms[obstacle].y + rooms[obstacle].right_door;
                        add_door_to_room(username, floor, obstacle, 2);
                        matrix[org][obstacle] = 1;
                        matrix[obstacle][org] = 1;
                        while (obsDoor_y > c->y)
                            add_to_corridor(&c, c->x, c->y + 1, visible, length);
                        while (obsDoor_y < c->y)
                            add_to_corridor(&c, c->x, c->y - 1, visible, length);
                        print_c(&c);
                    }
                    free(c);
                    a_to_b(matrix, obstacle, des, rooms, rooms_count, username, floor);
                    break;
                }
            }

            // should go up
            else if (c->y > desDoor_y)
            {
                int check = nothing_in(c->x, c->y - 1, rooms, rooms_count);
                // go up
                if (check == -1)
                    add_to_corridor(&c, c->x, c->y - 1, visible, length);

                // go up until can go right
                else
                {
                    int obstacle = check;
                    if (!matrix[org][obstacle])
                    {
                        int obsDoor_x = rooms[obstacle].x + rooms[obstacle].down_door;
                        add_door_to_room(username, floor, obstacle, 1);
                        matrix[org][obstacle] = 1;
                        matrix[obstacle][org] = 1;
                        while (obsDoor_x > c->x)
                            add_to_corridor(&c, c->x + 1, c->y, visible, length);
                        while (obsDoor_x < c->x)
                            add_to_corridor(&c, c->x - 1, c->y, visible, length);
                        print_c(&c);
                    }
                    free(c);
                    a_to_b(matrix, obstacle, des, rooms, rooms_count, username, floor);
                    break;
                }
            }

            if (*length > 150 || *length == l)
                break;
        }

        c->y - 1;
        print_c(&c);
        return;
    }

    // quadrant 4
    else if (rooms[org].x + (rooms[des].w / 2) < des_ox && rooms[org].y + (rooms[des].h / 2) > des_oy)
    {
    }

    // UP vertical line
    else if (rooms[org].y + (rooms[des].h / 2) < des_oy)
    {
    }

    // DOWN vertical line
    else if (rooms[org].y + (rooms[des].h / 2) > des_oy)
    {
    }

    // RIGHT Horizontal line
    else if (rooms[org].x + (rooms[des].w / 2) < des_ox)
    {
    }

    // LEFT Horizontal line
    else if (rooms[org].x + (rooms[des].w / 2) > des_ox)
    {
    }
}

void print_corridor(int rooms_count, Room *rooms, char username[MAX_NAMES], int floor)
{
    int *count;
    *count = 0;
    char path[MAX_NAMES];
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME/corridor_%d", username, floor);
    FILE *read_file = fopen(path, "r");
    char line[MAX_LINE];
    int **roads = (int **)malloc(sizeof(int *));
    for (int i = 0; i < rooms_count; i++)
    {
        roads[i] = (int *)malloc(sizeof(int) * rooms_count);
        for (int j = 0; j < rooms_count; j++)
            roads[i][j] = 0;
    }

    while (fgets(line, sizeof(line), read_file))
    {
        // read line by line
        // rooms index
        int org = (int)(line[0] - '0');
        int des = (int)(line[2] - '0');

        // road matrix
        if (roads[org][des])
            continue;
        else
        {
            roads[org][des] = 1;
            roads[des][org] = 1;
            a_to_b(roads, org, des, rooms, rooms_count, username, floor);
        }
    }
}

void dfs(int **neighbors, int *visited, int n, int node)
{
    visited[node] = 1;
    for (int i = 0; i < n; i++)
        if (neighbors[node][i] && !visited[i])
            dfs(neighbors, visited, n, i);
}

int generate_corridor(int rooms_count, Room *rooms, char username[MAX_NAMES], int floor)
{
    char path[MAX_LINE];
    sprintf(path, "./.users/%s/~NEW_GAME/corridor_%d.txt", username, floor);
    FILE *write = fopen(path, "w");
    // fprintf(write, "%d\n", rooms_count);

    // int neighbors[rooms_count][rooms_count];

    int **neighbors = (int **)malloc(rooms_count * sizeof(int *));
    for (int i = 0; i < rooms_count; i++)
        neighbors[i] = (int *)calloc(rooms_count, sizeof(int));

    for (int i = 0; i < rooms_count; i++)
        for (int j = 0; j < rooms_count; j++)
            neighbors[i][j] = 0;

    for (int i = 0; i < rooms_count; i++)
    {
        for (int k = 0; k < rooms_count; k++)
        {
            if (i < k && abs((rooms[i].x + (rooms[i].w / 2)) - (rooms[k].x + (rooms[k].w / 2))) + (3 * abs((rooms[i].y + (rooms[i].h / 2)) - (rooms[k].y + (rooms[k].h / 2)))) < 55)
            {
                neighbors[i][k] = 1;
                neighbors[k][i] = 1;
                fprintf(write, "%d %d\n", i, k);
            }
        }
    }
    fclose(write);

    int *visited = (int *)calloc(rooms_count, sizeof(int));

    dfs(neighbors, visited, rooms_count, 0);

    int is_connected = 1;
    for (int i = 0; i < rooms_count; i++)
        if (!visited[i])
        {
            is_connected = 0;
            break;
        }

    free(visited);
    for (int i = 0; i < rooms_count; i++)
        free(neighbors[i]);
    free(neighbors);

    return is_connected;
}

void generate_map(char username[MAX_NAMES], int p)
{
    int padding = 3, rooms_count;
    Room *rooms;

    while (1)
    {
        // Create Floor
        rooms_count = random_num(6, 8);
        rooms = (Room *)malloc(sizeof(Room) * rooms_count);

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
            rooms[i].top_door = random_num(0, rooms[i].w - 2);
            rooms[i].down_door = random_num(0, rooms[i].w - 2);
            rooms[i].right_door = random_num(0, rooms[i].h - 2);
            rooms[i].left_door = random_num(0, rooms[i].h - 2);
            rooms[i].tv = 0;
            rooms[i].dv = 0;
            rooms[i].rv = 0;
            rooms[i].lv = 0;
        }

        if (generate_corridor(rooms_count, rooms, username, p))
            break;
        free(rooms);
    }

    char path[MAX_LINE];
    if (p == 0)
    {
        int x_player, y_player, r = random_num(5, rooms_count - 1);
        x_player = random_num(rooms[r].x + 1, rooms[r].x + rooms[r].w - 1);
        y_player = random_num(rooms[r].y + 1, rooms[r].y + rooms[r].h - 1);
        memset(path, 0, sizeof(path));
        sprintf(path, "./.users/%s/~NEW_GAME/cs.txt", username);
        FILE *cs = fopen(path, "w");
        fprintf(cs, "%d %d\n%d\n%d", x_player, y_player, 0, random_num(0, 4));
        fclose(cs);
    }

    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME/floor_%d.txt", username, p);
    FILE *write_floor = fopen(path, "w");
    fprintf(write_floor, "%d\n", rooms_count);
    for (int k = 0; k < rooms_count; k++)
        fprintf(write_floor, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", rooms[k].x, rooms[k].y, rooms[k].h, rooms[k].w, rooms[k].visible, rooms[k].kind, rooms[k].top_door, rooms[k].down_door, rooms[k].right_door, rooms[k].left_door, rooms[k].tv, rooms[k].dv, rooms[k].rv, rooms[k].lv);
    fclose(write_floor);
    free(rooms);
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

    if (access(path, F_OK) != 0)
    {
        DIR *dir = opendir(path);
        mkdir(path, 0777);
        closedir(dir);

        int difficulty;
        memset(path, 0, sizeof(path));
        sprintf(path, "./.users/%s/GameSetting.txt", username);
        FILE *rgs = fopen(path, "r");
        fscanf(rgs, "%d\n%*d\n%*d\n%*d", &difficulty);
        fclose(rgs);

        int floor;
        if (difficulty == 0)
            floor = 4;
        else if (difficulty == 1)
            floor = random_num(5, 6);
        else if (difficulty == 2)
            floor = 7;
        else if (difficulty == 3)
            floor = 1;

        attron(COLOR_PAIR(9) | A_BOLD);
        mvprintw(dy - 3, dx - 22, "Wizards are writing the fate of this land..");
        attroff(COLOR_PAIR(9) | A_BOLD);

        attron((COLOR_PAIR(9) | A_BOLD | A_BLINK));
        mvprintw(dy - 3, dx + 21, ".");
        attroff((COLOR_PAIR(9) | A_BOLD | A_BLINK));

        attron(COLOR_PAIR(8));
        printBoard(0, 0, 3, 2 * (floor + 3));
        attroff(COLOR_PAIR(8));

        attron(COLOR_PAIR(13));
        mvprintw(dy + 3, dx - 13, "Please wait a few moments!");
        attroff(COLOR_PAIR(13));

        refresh();

        for (int i = 0; i < floor; i++)
        {
            mvprintw(dy, dx - floor + ((i - 1) * 2), "\U0001F7E7");
            refresh();
            generate_map(username, i);
        }
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

void start_game(int user_mode)
{
    UseColor();
    int scrY, scrX;
    getmaxyx(stdscr, scrY, scrX);
    int dx = scrX / 2, dy = scrY / 2;

    FILE *read_for_username = fopen("./.users/Login.txt", "r");
    char username[MAX_NAMES];
    fscanf(read_for_username, "%*d\n%s", username);
    fclose(read_for_username);

    int floors = 0;
    char path[MAX_LINE];
    memset(path, 0, sizeof(path));
    sprintf(path, "./.users/%s/~NEW_GAME", username);
    if (access(path, F_OK) == 0)
    {
        DIR *readFLoor = opendir(path);
        readFLoor = opendir(path);

        struct dirent *input;

        while ((input = readdir(readFLoor)) != NULL)
        {
            if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
                continue;
            floors++;
        }

        floors = floors / 2;
    }
    else
    {
        clear();
        refresh();
        flushinp();
        New_Game(user_mode, username);
        start_game(user_mode);
        return;
    }

    int ch, p = 0;

    while (1)
    {
        clear();
        refresh();
        mvprintw(0, dx, "%d", p + 1);
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
            mvprintw(rooms[i].y + (rooms[i].h / 2), rooms[i].x + (rooms[i].w / 2), "%d", i);
        }
        fclose(read_floor);

        ch = getch();

        if (ch == 'q')
            break;
        else if (ch == KEY_UP)
            p = (p + floors - 1) % floors;
        else if (ch == KEY_DOWN)
            p = (p + 1) % floors;
    }
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
            start_game(user_mode);
        }
    }

    // SDL_Quit();
    remove_dir("./.users/.~GUEST");
    endwin();
    return 0;
}