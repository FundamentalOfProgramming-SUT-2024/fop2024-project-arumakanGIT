#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <ncurses.h>

int main()
{
    char username[] = "amin";
    char path[] = "./.users";
    char newPath[200];
    // strcpy(newPath, strcat(path, username));
    puts(strcat(path, username));
    return 0;
}
