#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// void dfs(int neighbors[][10], int visited[], int n, int node)
// {
//     visited[node] = 0;
//     for (int i = 0; i < n; i++)
//         if (neighbors[node][i] && !visited[i])
//             dfs(neighbors, visited, n, i);
// }

int dfs(int neighbors[][10], int *visited, int n, int node)
{
    visited[node] = 1;
    int cont = 0;
    for (int i = 0; i < n; i++)
        cont += visited[i];

    if (cont == n)
        return 1;

    for (int i = 0; i < n; i++)
        if (neighbors[node][i] && !visited[i])
            if (dfs(neighbors, visited, n, i))
                return 1;
    return 0;
}

int main()
{
    int n = 7;
    int graph[7][10] = {0}, *visited = (int *)malloc(sizeof(int) * n);

    graph[6][5] = 1;
    graph[5][4] = 1;
    graph[4][3] = 1;
    graph[3][2] = 1;
    graph[2][1] = 1;
    graph[1][0] = 1;

    int started_node = 0;
    int pattern[n], pat = 0;
    for (int i = 0; i < n; i++)
        pattern[i] = 0;

    int check = 0;

    while (1)
    {
        for (int i = 0; i < n; i++)
            visited[i] = 0;

        check += dfs(graph, visited, n, started_node);

        for (int i = 0; i < n; i++)
        {
            if (visited[i])
            {
                if (!pattern[i])
                    pat++;
                pattern[i] = 1;
                printf("%d ", i);
            }
        }
        printf("\n");

        if (pat == n)
            break;

        for (int i = 0; i < n; i++)
            if (!pattern[i])
                started_node = i;
    }

    if (check)
        printf("\n---YES---\n");
    else
        printf("\n---NO---\n");
    return 0;
}
