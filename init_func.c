#include "init_func.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_NAMES 256
#define MAX_LINE 1024

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