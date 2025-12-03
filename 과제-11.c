#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 동적 배열 구조체
typedef struct
{
    int *data;
    int size;
    int capacity;
} DynArray;

void initArray(DynArray *arr, int cap)
{
    arr->data = (int *)malloc(sizeof(int) * cap);
    if (!arr->data)
    {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    arr->size = 0;
    arr->capacity = cap;
}

void resizeArray(DynArray *arr)
{
    arr->capacity *= 2;
    int *newData = (int *)realloc(arr->data, sizeof(int) * arr->capacity);
    if (!newData)
    {
        fprintf(stderr, "realloc 실패\n");
        exit(1);
    }
    arr->data = newData;
}

long long cmp_unsorted = 0;
long long cmp_sorted = 0;
long long cmp_avl = 0;

// 비정렬 배열
void unsorted_insert(DynArray *arr, int x)
{
    if (arr->size >= arr->capacity)
        resizeArray(arr);
    arr->data[arr->size++] = x;
}

int unsorted_search(DynArray *arr, int x)
{
    for (int i = 0; i < arr->size; i++)
    {
        cmp_unsorted++;
        if (arr->data[i] == x)
            return i;
    }
    return -1;
}

void unsorted_delete(DynArray *arr, int x)
{
    int idx = unsorted_search(arr, x);
    if (idx == -1)
        return;
    arr->data[idx] = arr->data[arr->size - 1];
    arr->size--;
}

// 정렬 배열
void sorted_insert(DynArray *arr, int x)
{
    if (arr->size >= arr->capacity)
        resizeArray(arr);

    int i;
    for (i = arr->size - 1; i >= 0; i--)
    {
        cmp_sorted++;
        if (arr->data[i] > x)
            arr->data[i + 1] = arr->data[i];
        else
            break;
    }
    arr->data[i + 1] = x;
    arr->size++;
}

int sorted_search(DynArray *arr, int x)
{
    int l = 0, r = arr->size - 1;
    while (l <= r)
    {
        cmp_sorted++;
        int m = (l + r) / 2;
        if (arr->data[m] == x)
            return m;
        if (arr->data[m] < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}

void sorted_delete(DynArray *arr, int x)
{
    int pos = sorted_search(arr, x);
    if (pos == -1)
        return;

    for (int i = pos; i < arr->size - 1; i++)
        arr->data[i] = arr->data[i + 1];

    arr->size--;
}

typedef struct node
{
    int key;
    int height;
    struct node *left, *right;
} Node;

int height(Node *n)
{
    return n ? n->height : 0;
}

int getBalance(Node *n)
{
    return height(n->left) - height(n->right);
}

Node *newNode(int key)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n)
    {
        fprintf(stderr, "노드 malloc 실패\n");
        exit(1);
    }
    n->key = key;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y;
}

Node *avl_insert(Node *node, int key)
{
    if (!node)
        return newNode(key);

    cmp_avl++;

    if (key < node->key)
        node->left = avl_insert(node->left, key);
    else if (key > node->key)
        node->right = avl_insert(node->right, key);
    else
        return node; // 중복은 무시

    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node *minValueNode(Node *node)
{
    Node *cur = node;
    while (cur->left)
        cur = cur->left;
    return cur;
}

Node *avl_delete(Node *root, int key)
{
    if (!root)
        return root;

    cmp_avl++;

    if (key < root->key)
        root->left = avl_delete(root->left, key);
    else if (key > root->key)
        root->right = avl_delete(root->right, key);
    else
    {
        // 자식 0 또는 1개
        if (!root->left || !root->right)
        {
            Node *temp = root->left ? root->left : root->right;

            if (!temp)
            { // 자식 0개
                temp = root;
                root = NULL;
            }
            else
            { // 자식 1개
                *root = *temp;
            }
            free(temp);
        }
        else
        {
            Node *temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = avl_delete(root->right, temp->key);
        }
    }

    if (!root)
        return root;

    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

int avl_search(Node *root, int key)
{
    while (root)
    {
        cmp_avl++;
        if (key == root->key)
            return 1;
        if (key < root->key)
            root = root->left;
        else
            root = root->right;
    }
    return 0;
}

int main()
{
    FILE *fp = fopen("dataset_id_ascending.csv", "r");
    if (!fp)
    {
        printf("dataset_id_ascending.csv 열기 실패!\n");
        return 0;
    }

    DynArray arr_unsorted, arr_sorted;
    initArray(&arr_unsorted, 2000);
    initArray(&arr_sorted, 2000);
    Node *root = NULL;

    char line[512];

    if (!fgets(line, sizeof(line), fp))
    {
        printf("빈 파일입니다.\n");
        fclose(fp);
        return 0;
    }

    cmp_unsorted = cmp_sorted = cmp_avl = 0;

    while (fgets(line, sizeof(line), fp))
    {

        char *tok = strtok(line, " \t\r\n");
        if (!tok)
            continue;

        int id = atoi(tok);
        if (id == 0)
            continue;

        unsorted_insert(&arr_unsorted, id);
        sorted_insert(&arr_sorted, id);
        root = avl_insert(root, id);
    }

    fclose(fp);

    printf("총 데이터 개수: %d\n\n", arr_unsorted.size);

    printf("삽입 시 비교 횟수\n");
    printf("비정렬 배열 : 0 (비교 없음)\n");
    printf("정렬 배열   : %lld\n", cmp_sorted);
    printf("AVL Tree   : %lld\n\n", cmp_avl);

    int test = 100500;

    cmp_unsorted = cmp_sorted = cmp_avl = 0;
    unsorted_search(&arr_unsorted, test);
    sorted_search(&arr_sorted, test);
    avl_search(root, test);

    printf("검색 시 비교 횟수 (ID = %d)\n", test);
    printf("비정렬 배열(순차 탐색) : %lld\n", cmp_unsorted);
    printf("정렬 배열(이진 탐색)   : %lld\n", cmp_sorted);
    printf("AVL Tree              : %lld\n\n", cmp_avl);

    cmp_unsorted = cmp_sorted = cmp_avl = 0;
    unsorted_delete(&arr_unsorted, test);
    sorted_delete(&arr_sorted, test);
    root = avl_delete(root, test);

    printf("삭제 시 비교 횟수 (ID = %d)\n", test);
    printf("비정렬 배열 : %lld\n", cmp_unsorted);
    printf("정렬 배열   : %lld\n", cmp_sorted);
    printf("AVL Tree   : %lld\n", cmp_avl);

    return 0;
}
