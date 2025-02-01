#ifndef INIT_FUNC_H
#define INIT_FUNC_H

#define MAX_NAMES 256
#define MAX_LINE 1024

void remove_dir(const char *path);
char *setare(char buffer[MAX_NAMES]);
void delay(int milli_seconds);
int random_num(int min, int max);
char *random_pass();
void toLowerCase(char str[]);
int checksum(char buffer[MAX_LINE]);
char *encrypt(char buffer[MAX_NAMES], char key[MAX_NAMES]);
int checkKEY(int sum, char key[5]);
char *decrypt(char buffer[1024], char key[512]);

#endif