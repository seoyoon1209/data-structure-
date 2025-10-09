#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100

// 이진탐색트리 구조체 정의
typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

// 노드 생성 함수
Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 트리에 노드 삽입
Node *insert(Node *root, int data)
{
    if (root == NULL)
        return createNode(data);

    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);

    // 중복 무시
    return root;
}

// 트리 탐색
int searchBST(Node *root, int target, int *count)
{
    (*count)++;
    if (root == NULL)
        return 0;
    if (root->data == target)
        return 1;
    else if (target < root->data)
        return searchBST(root->left, target, count);
    else
        return searchBST(root->right, target, count);
}

// 선형 탐색
int linearSearch(int arr[], int size, int target, int *count)
{
    for (int i = 0; i < size; i++)
    {
        (*count)++;
        if (arr[i] == target)
            return 1;
    }
    return 0;
}

int main()
{
    int arr[SIZE];
    Node *root = NULL;
    srand(time(NULL));

    // 배열에 무작위 수 저장
    printf("랜덤 데이터 생성 중...\n");
    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = rand() % 1001; // 0~1000 사이 수
        root = insert(root, arr[i]);
    }

    // 탐색할 숫자 선택
    int target = arr[rand() % SIZE]; // 배열 중 하나 선택
    printf("\n탐색할 숫자: %d\n", target);

    // 선형탐색
    int count_linear = 0;
    clock_t start1 = clock();
    int found1 = linearSearch(arr, SIZE, target, &count_linear);
    clock_t end1 = clock();
    double time_linear = (double)(end1 - start1) / CLOCKS_PER_SEC;

    // 이진탐색트리 탐색
    int count_bst = 0;
    clock_t start2 = clock();
    int found2 = searchBST(root, target, &count_bst);
    clock_t end2 = clock();
    double time_bst = (double)(end2 - start2) / CLOCKS_PER_SEC;

    // 결과 출력
    printf("\n===== 탐색 결과 비교 =====\n");
    printf("선형탐색:  %s (탐색 횟수: %d, 시간: %.8f초)\n",
           found1 ? "성공" : "실패", count_linear, time_linear);
    printf("이진탐색:  %s (탐색 횟수: %d, 시간: %.8f초)\n",
           found2 ? "성공" : "실패", count_bst, time_bst);

    printf("===========================\n");

    return 0;
}
