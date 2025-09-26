#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

char node[MAX];
int left[MAX], right[MAX];
int nodeCount = 0;

// 트리 빌드
int buildTree(const char *str)
{
    int stack[MAX], top = -1;
    int i, cur = -1;

    for (i = 0; str[i]; i++)
    {
        if (isalpha(str[i]))
        {
            cur = ++nodeCount;
            node[cur] = str[i];
            left[cur] = right[cur] = 0;

            if (top >= 0)
            {
                int parent = stack[top];
                if (left[parent] == 0)
                    left[parent] = cur;
                else
                    right[parent] = cur;
            }
        }
        else if (str[i] == '(')
        {
            stack[++top] = cur;
        }
        else if (str[i] == ')')
        {
            top--;
        }
    }
    return 1;
}

// iterative preorder
void preorder(int root)
{
    int stack[MAX], top = -1, first = 1;
    stack[++top] = root;
    while (top >= 0)
    {
        int cur = stack[top--];
        if (first)
        {
            printf("%c", node[cur]);
            first = 0;
        }
        else
            printf(" %c", node[cur]);

        if (right[cur])
            stack[++top] = right[cur];
        if (left[cur])
            stack[++top] = left[cur];
    }
    printf("\n");
}

// iterative inorder
void inorder(int root)
{
    int stack[MAX], top = -1, cur = root, first = 1;
    while (cur || top >= 0)
    {
        while (cur)
        {
            stack[++top] = cur;
            cur = left[cur];
        }
        cur = stack[top--];
        if (first)
        {
            printf("%c", node[cur]);
            first = 0;
        }
        else
            printf(" %c", node[cur]);
        cur = right[cur];
    }
    printf("\n");
}

// iterative postorder
void postorder(int root)
{
    int stack1[MAX], stack2[MAX];
    int top1 = -1, top2 = -1, first = 1;
    stack1[++top1] = root;
    while (top1 >= 0)
    {
        int cur = stack1[top1--];
        stack2[++top2] = cur;
        if (left[cur])
            stack1[++top1] = left[cur];
        if (right[cur])
            stack1[++top1] = right[cur];
    }
    while (top2 >= 0)
    {
        int cur = stack2[top2--];
        if (first)
        {
            printf("%c", node[cur]);
            first = 0;
        }
        else
            printf(" %c", node[cur]);
    }
    printf("\n");
}

int main()
{
    char input[MAX];
    fgets(input, MAX, stdin);

    buildTree(input);

    printf("pre-order: ");
    preorder(1);

    printf("in-order: ");
    inorder(1);

    printf("post-order: ");
    postorder(1);

    return 0;
}
