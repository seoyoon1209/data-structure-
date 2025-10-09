#include <stdio.h>
#include <stdlib.h>

// 노드 구조체 정의
typedef struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
} Node;

// 새 노드 생성
Node *createNode(int key)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 이진 탐색 트리에 삽입
Node *insert(Node *root, int key)
{
    if (root == NULL)
        return createNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);

    return root; // 변경된 루트 반환
}

// 탐색 함수
Node *search(Node *root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

// 트리 출력 (중위순회: 오름차순 출력)
void inorder(Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

// 트리 높이 계산
int height(Node *root)
{
    if (root == NULL)
        return -1; // 빈 트리 = -1
    int leftH = height(root->left);
    int rightH = height(root->right);
    return (leftH > rightH ? leftH : rightH) + 1;
}

int main()
{
    int data[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(data) / sizeof(data[0]);

    Node *root = NULL;

    // 트리에 데이터 삽입
    for (int i = 0; i < n; i++)
    {
        root = insert(root, data[i]);
    }

    printf("중위순회 결과(정렬된 값): ");
    inorder(root);
    printf("\n");

    printf("트리 높이: %d\n", height(root));

    // 탐색 테스트
    int key = 40;
    Node *found = search(root, key);
    if (found != NULL)
        printf("%d 값이 트리에 존재합니다.\n", key);
    else
        printf("%d 값이 트리에 없습니다.\n", key);

    return 0;
}
