#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void updateUserInfo()
{
    FILE *file;
    file = fopen("./.users/Users.txt", "r+");
    int users_count, last;
    fscanf(file, "%d %d\n", &users_count, &last);
    int history[users_count], golds[users_count];
    char names[users_count][256];

    for (int i = 0; i < users_count; i++)
    {
        int h, g;
        char name[256];
        fscanf(file, "%*S %d %s %d\n", &h, name, &g);
        strcpy(names[i], name);
        history[i] = h;
        golds[i] = g;
    }

    if (last)
    {
        last--;
        for (int i = 0; i < users_count; i++)
            if (history[i] < history[last])
                history[i] += 1;
        history[last] = 1;
    }

    for (int i = 0; i < users_count - 1; i++)
    {

        for (int j = i + 1; j < users_count; j++)
        {
            if (golds[i] < golds[j])
            {
                int th, tg;
                char t[256];
                strcpy(t, names[i]);
                th = history[i];
                tg = golds[i];
                strcpy(names[i], names[j]);
                golds[i] = golds[j];
                history[i] = history[j];
                strcpy(names[j], t);
                golds[j] = tg;
                history[j] = th;
            }
        }
    }

    rewind(file);
    fprintf(file, "%d %d\n", users_count, 0);
    for (int i = 1; i <= users_count; i++)
        fprintf(file, "%d %d %s %d\n", i, history[i - 1], names[i - 1], golds[i - 1]);

    fclose(file);
}

void printUserInfo()
{
    FILE *file;
    file = fopen("./.users/Users.txt", "r+");
    int user_count;
    fscanf(file, "%d %*S\n", &user_count);
    int history, golds;
    char name[256];
    for (int i = 0; i < user_count; i++)
    {
        fscanf(file, "%*S %d %s %d\n", &history, name, &golds);
        printf("%d %d %s %d\n", i + 1, history, name, golds);
    }
}

int main()
{
    printf("First : \n");
    printUserInfo();
    updateUserInfo();
    printf("Second : \n");
    printUserInfo();
    return 0;
}
