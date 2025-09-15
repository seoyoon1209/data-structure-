#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 1000

typedef struct Node
{
    char data;
    struct Node *child[50];
    int childCount;
} Node;

Node *newNode(char c)
{
    Node *n = (Node *)malloc(sizeof(Node));
    n->data = c;
    n->childCount = 0;
    return n;
}

int getHeight(Node *r)
{
    if (r == NULL)
        return 0;
    if (r->childCount == 0)
        return 0;
    int max = 0;
    for (int i = 0; i < r->childCount; i++)
    {
        int h = getHeight(r->child[i]);
        if (h > max)
            max = h;
    }
    return max + 1;
}

int getNodeCount(Node *r)
{
    if (r == NULL)
        return 0;
    int sum = 1;
    for (int i = 0; i < r->childCount; i++)
    {
        sum += getNodeCount(r->child[i]);
    }
    return sum;
}

int getLeafCount(Node *r)
{
    if (r == NULL)
        return 0;
    if (r->childCount == 0)
        return 1;
    int sum = 0;
    for (int i = 0; i < r->childCount; i++)
    {
        sum += getLeafCount(r->child[i]);
    }
    return sum;
}

int main()
{
    printf("input을 입렵해주세요:");
    char input[MAX];
    scanf("%[^\n]", input);

    Node *stack[100];
    int top = -1;
    Node *root = NULL;
    Node *cur = NULL;

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (isalpha(input[i]))
        {
            Node *n = newNode(input[i]);
            if (root == NULL)
            {
                root = n;
            }
            else
            {
                // 스택  위 노드에 자식 연결
                stack[top]->child[stack[top]->childCount++] = n;
            }
            cur = n;
        }
        else if (input[i] == '(')
        {
            // 현재노드 부모로 저장
            stack[++top] = cur;
        }
        else if (input[i] == ')')
        {
            // 나오기
            top--;
        }
    }

    int h = getHeight(root);
    int nodes = getNodeCount(root);
    int leaves = getLeafCount(root);

    printf("결과: %d,%d,%d\n", h, nodes, leaves);

    return 0;
}
