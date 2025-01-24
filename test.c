#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE *file;
    file = fopen("./.users/Users1.txt", "r");
    int n, m;
    fscanf(file, "%d %d", &n, &m);
    printf("n = %d, m = %d\n", n, m);
    int h[n], g[n];
    char names[n][256];
    for (int i = 0; i < n; i++)
    {
        fscanf(file, "%d %s %d", &h[i], names[i], &g[i]);
        printf("%d %s %d\n", h[i], names[i], g[i]);
    }
    fclose(file);
    return 0;
}
