#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define V 10
#define E 20
#define INF 1000000

typedef struct
{
    int u, v, w;
} Edge;

int main()
{
    int graph[V][V];
    int dist[V][V];
    Edge edges[E];
    srand(time(NULL));

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }

    // 무작위 간선 생성
    for (int i = 0; i < E; i++)
    {
        int u, v, w;
        do
        {
            u = rand() % V;
            v = rand() % V;
        } while (u == v || graph[u][v] != INF); // 중복/자기자신 금지
        w = rand() % 20 + 1; // 1~20 사이 가중치
        graph[u][v] = w;
        graph[v][u] = w; // 무방향 그래프
        edges[i].u = u;
        edges[i].v = v;
        edges[i].w = w;
    }

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    for (int k = 0; k < V; k++)
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];

    // 그래프 간선 출력
    printf("=== Random Graph Edges (Total %d) ===\n", E);
    for (int i = 0; i < E; i++)
        printf("Edge %2d: %d --(%2d)--> %d\n", i + 1, edges[i].u, edges[i].w, edges[i].v);
    printf("\n");

    // 모든 쌍 최단거리 출력
    printf("=== All-Pairs Shortest Paths (V=%d) ===\n", V);
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (dist[i][j] == INF)
                printf("(%d -> %d) : No path\n", i, j);
            else
                printf("(%d -> %d) : %d\n", i, j, dist[i][j]);
        }
    }

    return 0;
}
