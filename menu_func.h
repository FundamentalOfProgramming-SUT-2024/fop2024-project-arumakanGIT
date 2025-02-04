#ifndef MENU_FUNC_H
#define MENU_FUNC_H

#define MAX_NAMES 256
#define MAX_LINE 1024

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

#endif