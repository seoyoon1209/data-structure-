#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

int main()
{
    char input[1000];
    int stack[1000]; // 각 노드 자식 수를 저장하는 스택
    int top = -1;    // 스택 포인터
    int i, res = TRUE;

    // 입력 받기
    if (!fgets(input, sizeof(input), stdin))
    {
        printf("ERROR\n");
        return 0;
    }

    for (i = 0; input[i] != '\0'; i++)
    {
        char c = input[i];

        if (c == ' ' || c == '\n' || c == '\t')
            continue; // 공백 무시

        if (c == '(')
        {
            // 새 노드 시작 → 스택에 push
            top++;
            stack[top] = 0; // 자식 수 0으로 초기화
        }
        else if (c >= 'A' && c <= 'Z')
        {
            // 알파벳 노드
            if (top >= 0)
            {
                stack[top]++; // 부모 노드의 자식 수 증가
                if (stack[top] > 2)
                {
                    res = FALSE;
                }
            }
        }
        else if (c == ')')
        {
            if (top < 0)
            {
                res = ERROR;
                break;
            }
            top--; // 노드 종료
        }
        else
        {
            // 잘못된 문자
            res = ERROR;
            break;
        }
    }

    // 최종 검사
    if (res != ERROR && top != -1)
        res = ERROR; // 괄호 짝 안 맞음

    if (res == TRUE)
        printf("TRUE\n");
    else if (res == FALSE)
        printf("FALSE\n");
    else
        printf("ERROR\n");

    return 0;
}
