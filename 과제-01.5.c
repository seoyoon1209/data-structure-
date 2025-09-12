// 공백 무시
// 이진 트리의 조건이 만족지만 검사
// 괄호 누락, 이상한 문자, 루트가 여러개 등은 없다고 가정

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 2

void trim(char *buf)
{
    char *src = buf;
    char *dst = buf;

    while (*src != '\0')
    {
        if (*dst != ' ')
        {
            *src = *dst;
            src++;
        }
        dst++;
    }
    *src = '\0'; // 문자열의 끝을 표시
}

int check_binary_tree(char *buf)
{
    int i = 0;
    int childCountStack[100]; // 각 노드 자식 수 기록
    int top = -1;

    while (buf[i] != '\0')
    {
        if (isalpha(buf[i]))
        {
            // 노드 발견
            if (top >= 0)
            {
                childCountStack[top]++; // 부모 자식수 증가
                if (childCountStack[top] > 2)
                    return FALSE; // 2명 초과면 실패
            }
        }
        else if (buf[i] == '(')
        {
            // 새로운 부모 시작
            top++;
            childCountStack[top] = 0;
        }
        else if (buf[i] == ')')
        {
            // 부모 끝
            top--;
            if (top < -1)
                return TRUE; // 스택 언더플로우 방지
        }
        else
        {
            return TRUE; // 허용되지 않은 문자
        }
        i++;
    }

    // 괄호가 모두 닫혔는지 확인
    if (top != -1)
        return 0;
}

int main()
{
    char buf[1000];
    fgets(buf, sizeof(buf), stdin);

    printf("%s", buf);

    trim(buf);

    printf("%s", buf);

    int result = check_binary_tree(buf);

    if (result == TRUE)
    {
        printf("TRUE");
        return 0;
    }
    else if (result == FALSE)
    {
        printf("FALSE");
    }
}