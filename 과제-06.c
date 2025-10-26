#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTEX 100
#define SPARSE_EDGES 100
#define DENSE_EDGES 4000

typedef struct
{
    int n;
    int **matrix;
} AdjMatrixGraph;

typedef struct Node
{
    int vertex;
    struct Node *next;
} Node;

typedef struct
{
    int n;
    Node **list;
} AdjListGraph;

// 그래프 생성
AdjMatrixGraph *createMatrixGraph(int n)
{
    AdjMatrixGraph *g = (AdjMatrixGraph *)malloc(sizeof(AdjMatrixGraph));
    g->n = n;
    g->matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        g->matrix[i] = (int *)calloc(n, sizeof(int));
    }
    return g;
}

AdjListGraph *createListGraph(int n)
{
    AdjListGraph *g = (AdjListGraph *)malloc(sizeof(AdjListGraph));
    g->n = n;
    g->list = (Node **)malloc(n * sizeof(Node *));
    for (int i = 0; i < n; i++)
        g->list[i] = NULL;
    return g;
}

void addEdgeMatrix(AdjMatrixGraph *g, int u, int v)
{
    g->matrix[u][v] = 1;
    g->matrix[v][u] = 1;
}

void addEdgeList(AdjListGraph *g, int u, int v)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = g->list[u];
    g->list[u] = newNode;

    newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = u;
    newNode->next = g->list[v];
    g->list[v] = newNode;
}

// 열결확인
int isConnectedMatrix(AdjMatrixGraph *g, int u, int v, int *count)
{
    (*count)++;
    return g->matrix[u][v];
}

int isConnectedList(AdjListGraph *g, int u, int v, int *count)
{
    Node *p = g->list[u];
    while (p)
    {
        (*count)++;
        if (p->vertex == v)
            return 1;
        p = p->next;
    }
    return 0;
}

// 노드 출력
void printAdjacentMatrix(AdjMatrixGraph *g, int u, int *count)
{
    printf("노드 %d의 인접 노드(행렬): ", u);
    for (int i = 0; i < g->n; i++)
    {
        (*count)++;
        if (g->matrix[u][i])
            printf("%d ", i);
    }
    printf("\n");
}

void printAdjacentList(AdjListGraph *g, int u, int *count)
{
    printf("노드 %d의 인접 노드(리스트): ", u);
    Node *p = g->list[u];
    while (p)
    {
        (*count)++;
        printf("%d ", p->vertex);
        p = p->next;
    }
    printf("\n");
}

// 메모리 추정
size_t getMemoryUsageMatrix(AdjMatrixGraph *g)
{
    return sizeof(AdjMatrixGraph) + sizeof(int *) * g->n + sizeof(int) * g->n * g->n;
}

size_t getMemoryUsageList(AdjListGraph *g, int edges)
{
    return sizeof(AdjListGraph) + sizeof(Node *) * g->n + edges * sizeof(Node) * 2;
}

// 무작위 그래프 생성
void generateRandomEdges(int edges, int *uArr, int *vArr)
{
    srand(time(NULL));
    for (int i = 0; i < edges; i++)
    {
        uArr[i] = rand() % MAX_VERTEX;
        vArr[i] = rand() % MAX_VERTEX;
        if (uArr[i] == vArr[i])
            i--;
    }
}

int main()
{
    int uArr[4000], vArr[4000];
    generateRandomEdges(SPARSE_EDGES, uArr, vArr);
    generateRandomEdges(DENSE_EDGES, uArr, vArr);

    AdjMatrixGraph *sparseM = createMatrixGraph(MAX_VERTEX);
    AdjListGraph *sparseL = createListGraph(MAX_VERTEX);
    AdjMatrixGraph *denseM = createMatrixGraph(MAX_VERTEX);
    AdjListGraph *denseL = createListGraph(MAX_VERTEX);

    clock_t start, end;
    double time_used;
    int count;

    // 희소 그래프 삽입
    start = clock();
    for (int i = 0; i < SPARSE_EDGES; i++)
        addEdgeMatrix(sparseM, uArr[i], vArr[i]);
    end = clock();
    printf("[희소 그래프-행렬] 간선 삽입 시간: %.6f초\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < SPARSE_EDGES; i++)
        addEdgeList(sparseL, uArr[i], vArr[i]);
    end = clock();
    printf("[희소 그래프-리스트] 간선 삽입 시간: %.6f초\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 연결 여부 테스트
    count = 0;
    isConnectedMatrix(sparseM, 1, 2, &count);
    printf("[행렬] 연결 여부 비교 횟수: %d\n", count);

    count = 0;
    isConnectedList(sparseL, 1, 2, &count);
    printf("[리스트] 연결 여부 비교 횟수: %d\n", count);

    // 인접 노드 출력
    count = 0;
    printAdjacentMatrix(sparseM, 1, &count);
    printf("[행렬] 인접 노드 탐색 비교 횟수: %d\n", count);

    count = 0;
    printAdjacentList(sparseL, 1, &count);
    printf("[리스트] 인접 노드 탐색 비교 횟수: %d\n", count);

    // 메모리 사용량 출력
    printf("[행렬 메모리 사용량] %zu Bytes\n", getMemoryUsageMatrix(sparseM));
    printf("[리스트 메모리 사용량] %zu Bytes\n", getMemoryUsageList(sparseL, SPARSE_EDGES));

    return 0;
}
