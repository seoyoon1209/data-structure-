#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

// 구조체 정의
typedef struct Node
{
    int key;
    struct Node *left, *right;
    int height;
} Node;

// 유틸 함수
int max(int a, int b) { return (a > b) ? a : b; }
int height(Node *n) { return n ? n->height : 0; }

Node *insertBST(Node *root, int key)
{
    if (root == NULL)
    {
        Node *node = (Node *)malloc(sizeof(Node));
        node->key = key;
        node->left = node->right = NULL;
        node->height = 1;
        return node;
    }
    if (key < root->key)
        root->left = insertBST(root->left, key);
    else
        root->right = insertBST(root->right, key);
    return root;
}

// AVL 트리 회전 함수
Node *rotateRight(Node *y)
{
    Node *x = y->left;
    Node *T = x->right;
    x->right = y;
    y->left = T;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}
Node *rotateLeft(Node *x)
{
    Node *y = x->right;
    Node *T = y->left;
    y->left = x;
    x->right = T;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

// AVL 삽입
Node *insertAVL(Node *root, int key)
{
    if (root == NULL)
    {
        Node *node = (Node *)malloc(sizeof(Node));
        node->key = key;
        node->left = node->right = NULL;
        node->height = 1;
        return node;
    }

    if (key < root->key)
        root->left = insertAVL(root->left, key);
    else if (key > root->key)
        root->right = insertAVL(root->right, key);
    else
        return root; // 중복 무시

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = height(root->left) - height(root->right);

    // 불균형 처리
    if (balance > 1 && key < root->left->key)
        return rotateRight(root);
    if (balance < -1 && key > root->right->key)
        return rotateLeft(root);
    if (balance > 1 && key > root->left->key)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && key < root->right->key)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// 탐색 횟수
int searchArray(int arr[], int key)
{
    int count = 0;
    for (int i = 0; i < SIZE; i++)
    {
        count++;
        if (arr[i] == key)
            break;
    }
    return count;
}

int searchBST(Node *root, int key)
{
    int count = 0;
    while (root != NULL)
    {
        count++;
        if (key == root->key)
            break;
        else if (key < root->key)
            root = root->left;
        else
            root = root->right;
    }
    return count;
}

// AVL 구조
int searchAVL(Node *root, int key)
{
    int count = 0;
    while (root != NULL)
    {
        count++;
        if (key == root->key)
            break;
        else if (key < root->key)
            root = root->left;
        else
            root = root->right;
    }
    return count;
}

// 평균
double averageSearchArray(int arr[], int test[], int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += searchArray(arr, test[i]);
    return sum / n;
}
double averageSearchBST(Node *root, int test[], int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += searchBST(root, test[i]);
    return sum / n;
}
double averageSearchAVL(Node *root, int test[], int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += searchAVL(root, test[i]);
    return sum / n;
}

// 메인 함수
int main()
{
    int data[SIZE];
    srand(time(NULL));

    // 작위 데이터 (0~10000)
    for (int i = 0; i < SIZE; i++)
        data[i] = rand() % 10001;

    Node *bst = NULL, *avl = NULL;
    for (int i = 0; i < SIZE; i++)
    {
        bst = insertBST(bst, data[i]);
        avl = insertAVL(avl, data[i]);
    }

    // 탐색 테스트용 난수 1000개 생성
    int test[SIZE];
    for (int i = 0; i < SIZE; i++)
        test[i] = rand() % 10001;

    printf("데이터 무작위 정수 1000개\n");
    printf("Array 평균 탐색 횟수: %.2f\n", averageSearchArray(data, test, SIZE));
    printf("BST 평균 탐색 횟수: %.2f\n", averageSearchBST(bst, test, SIZE));
    printf("AVL 평균 탐색 횟수: %.2f\n", averageSearchAVL(avl, test, SIZE));

    return 0;
}
