#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

typedef struct TreeNode
{

    char key; // 대문자 한 글자

    struct TreeNode *left;

    struct TreeNode *right;

} TreeNode;

/* --------- 유틸리티 함수들 --------- */

TreeNode *new_node(char key)
{

    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));

    if (!n)
    {
        perror("malloc");
        exit(1);
    }

    n->key = (char)toupper((unsigned char)key);

    n->left = n->right = NULL;

    return n;
}

void free_subtree(TreeNode *node)
{

    if (!node)
        return;

    free_subtree(node->left);

    free_subtree(node->right);

    free(node);
}

/* 전위/중위/후위 순회 예시 */

void preorder(TreeNode *node)
{

    if (!node)
        return;

    printf("%c ", node->key);

    preorder(node->left);

    preorder(node->right);
}

void inorder(TreeNode *node)
{

    if (!node)
        return;

    inorder(node->left);

    printf("%c ", node->key);

    inorder(node->right);
}

void postorder(TreeNode *node)
{

    if (!node)
        return;

    postorder(node->left);

    postorder(node->right);

    printf("%c ", node->key);
}

/* 트리를 구조적으로 출력 (A(B( ),C( )) 형태) */

void print_tree(TreeNode *node)
{

    if (!node)
    {
        printf(" . ");
        return;
    }

    printf("%c(", node->key);

    print_tree(node->left);

    printf(",");

    print_tree(node->right);

    printf(")");
}

/* 키로 노드 찾기 (일반 이진트리: 선형 탐색) */

TreeNode *find_node(TreeNode *root, char key)
{

    if (!root)
        return NULL;

    if (root->key == (char)toupper((unsigned char)key))
        return root;

    TreeNode *p = find_node(root->left, key);

    if (p)
        return p;

    return find_node(root->right, key);
}

/* --------- 핵심 API --------- */

/*

 * insert_node:

 *  - root가 NULL이고 parent가 NULL이면 루트 생성

 *  - parent의 왼쪽(L) 또는 오른쪽(R)에 비어 있을 때만 삽입

 *  - 항상 (변경 가능성 있는) root를 반환

 */

TreeNode *insert_node(TreeNode *root, TreeNode *parent, char key, char child)
{

    char dir = (char)toupper((unsigned char)child);

    key = (char)toupper((unsigned char)key);

    if (root == NULL)
    {

        if (parent != NULL)
        {

            fprintf(stderr, "[insert_node] 초기 루트 삽입 시 parent는 NULL이어야 합니다.\n");

            return NULL;
        }

        // 초기 루트 생성

        return new_node(key);
    }

    if (parent == NULL)
    {

        fprintf(stderr, "[insert_node] 루트가 이미 있을 때 parent는 NULL일 수 없습니다.\n");

        return root;
    }

    if (dir == 'L')
    {

        if (parent->left != NULL)
        {

            fprintf(stderr, "[insert_node] 이미 왼쪽 자식이 존재합니다.\n");

            return root;
        }

        parent->left = new_node(key);
    }

    else if (dir == 'R')
    {

        if (parent->right != NULL)
        {

            fprintf(stderr, "[insert_node] 이미 오른쪽 자식이 존재합니다.\n");

            return root;
        }

        parent->right = new_node(key);
    }

    else
    {

        fprintf(stderr, "[insert_node] child는 'L' 또는 'R'이어야 합니다.\n");
    }

    return root;
}

/* 단말 여부 도우미(변경 없음) */

static int is_leaf(TreeNode *n)
{

    return n && (n->left == NULL) && (n->right == NULL);
}

/*

 * delete_node (이중포인터 없이 구현):

 *  - parent == NULL: 루트가 단말일 때만 삭제 -> NULL 반환, 아니면 유지

 *  - parent != NULL: parent의 L 또는 R 자식이 단말일 때만 삭제

 *  - 단말이 아니면 삭제하지 않고 경고 출력

 *  - 항상 (변경된) root를 반환

 */

TreeNode *delete_node(TreeNode *root, TreeNode *parent, char child)
{

    if (root == NULL)
        return NULL;

    // 루트 삭제 요청

    if (parent == NULL)
    {

        if (is_leaf(root))
        {

            free(root);

            return NULL;
        }

        else
        {

            fprintf(stderr, "[delete_node] 루트가 단말노드가 아니므로 삭제하지 않습니다.\n");

            return root;
        }
    }

    char dir = (char)toupper((unsigned char)child);

    if (dir != 'L' && dir != 'R')
    {

        fprintf(stderr, "[delete_node] child는 'L' 또는 'R'이어야 합니다.\n");

        return root;
    }

    if (dir == 'L')
    {

        if (parent->left == NULL)
        {

            fprintf(stderr, "[delete_node] parent의 왼쪽 자식이 없습니다.\n");

            return root;
        }

        if (is_leaf(parent->left))
        {

            free(parent->left);

            parent->left = NULL;
        }

        else
        {

            fprintf(stderr, "[delete_node] 왼쪽 자식이 단말노드가 아니므로 삭제하지 않습니다.\n");
        }
    }

    else
    { // dir == 'R'

        if (parent->right == NULL)
        {

            fprintf(stderr, "[delete_node] parent의 오른쪽 자식이 없습니다.\n");

            return root;
        }

        if (is_leaf(parent->right))
        {

            free(parent->right);

            parent->right = NULL;
        }

        else
        {

            fprintf(stderr, "[delete_node] 오른쪽 자식이 단말노드가 아니므로 삭제하지 않습니다.\n");
        }
    }

    return root;
}

/* --------- 사용 예제 (데모용 main) --------- */

int main(void)
{

    TreeNode *root = NULL;

    // 1) 루트 A 생성

    root = insert_node(root, NULL, 'A', 'L'); // parent==NULL => 루트 생성

    printf("트리(초기): ");

    print_tree(root);
    puts("");

    // 2) A의 왼쪽에 B, 오른쪽에 C 삽입

    TreeNode *A = find_node(root, 'A');

    root = insert_node(root, A, 'B', 'L');

    root = insert_node(root, A, 'C', 'R');

    // 3) B의 왼쪽에 D, 오른쪽에 E 삽입

    TreeNode *B = find_node(root, 'B');

    root = insert_node(root, B, 'D', 'L');

    root = insert_node(root, B, 'E', 'R');

    // 4) C의 오른쪽에 F 삽입

    TreeNode *C = find_node(root, 'C');

    root = insert_node(root, C, 'F', 'R');

    printf("트리(삽입 후): ");

    print_tree(root);
    puts("");

    printf("Preorder : ");
    preorder(root);
    puts("");

    printf("Inorder  : ");
    inorder(root);
    puts("");

    printf("Postorder: ");
    postorder(root);
    puts("");

    // 5) B의 오른쪽 자식(E) 서브트리 삭제

    root = delete_node(root, B, 'R');

    printf("트리(B의 오른쪽 E 삭제 후): ");

    print_tree(root);
    puts("");

    // 6) 루트 전체 삭제

    root = delete_node(root, NULL, 'L'); // parent==NULL => 루트 삭제

    printf("트리(루트 삭제 후): ");

    print_tree(root);
    puts("");

    return 0;
}
