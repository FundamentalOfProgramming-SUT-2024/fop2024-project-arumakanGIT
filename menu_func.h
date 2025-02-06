#ifndef MENU_FUNC_H
#define MENU_FUNC_H

#define MAX_NAMES 256
#define MAX_LINE 1024

typedef struct Player
{
    char c[7];
    int x;
    int y;
    int health;
    int hungry;
    int golds;
    int XP;
    int score;
    int foods[5];
    int weapons[5];
} Player;

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

typedef struct cell
{
    int x;
    int y;
    int v;
    struct cell *next;
} cell;

void remove_dir(const char *path);
void delay(int milli_seconds);
int random_num(int min, int max);
char *random_pass();
void toLowerCase(char str[]);
int checksum(char buffer[MAX_LINE]);
char *encrypt(char buffer[MAX_NAMES], char key[MAX_NAMES]);
int checkKEY(int sum, char key[5]);
char *decrypt(char buffer[1024], char key[512]);
int email_exist(char email[MAX_NAMES]);
int username_exist(char username[MAX_NAMES]);
int checkMail(char *email);
void printBoard(int Ypadding, int Xpadding, int h, int w);
void EditUserInfo(char username[MAX_NAMES]);
void addToLogin(char username[MAX_NAMES], char password[MAX_NAMES], char email[MAX_NAMES]);
void addToLeaderBoard(char username[MAX_NAMES]);
void printAsciiArt(char mode, int scrX, int scrY, int x, int y);
void UseColor();
int search_login(char username[MAX_NAMES], char password[MAX_NAMES]);
void new_pass(char username[MAX_NAMES]);
void securityQ(char username[MAX_NAMES]);
int log_in();
int add_New_User();
int login_guest();
int RegisterMenu();
void printTable(int big, int x, int y, int users_count, int tabs[]);
void clearTable(int big, int x, int y, int tabs[], char spaces[][MAX_NAMES], int users_count);
void leaderBoard(char username[MAX_NAMES]);
void game_setting(int user_mode, char username[MAX_NAMES]);
void sortLB();
void profile_menu(int user_mode, char username[MAX_NAMES]);
void add_to_corridor(cell **corridor, int x, int y, int visible);
int nothing_in(int x, int y, Room *rooms, int rooms_count);
void add_door_to_room(char username[MAX_NAMES], int floor, int room, int door);
void print_c(cell *c, char username[MAX_NAMES], int floor);
void a_to_b(int **matrix, int org, int des, Room *rooms, int rooms_count, char username[MAX_NAMES], int floor);
void print_corridor(int rooms_count, Room *rooms, char username[MAX_NAMES], int floor);
int is_overlapping(Room a, Room b);
void generate_map(char username[MAX_NAMES], int p);
void New_Game(int user_mode, char username[MAX_NAMES]);

#endif