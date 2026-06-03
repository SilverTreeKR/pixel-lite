#include <stdio.h>
#include <string.h>
#include "../header/CLI.h"

#define MAX_COMMANDS 20
#define MAX_INPUT 100

// 함수 포인터 타입
typedef void (*CommandFunc)(char*);

// 명령 구조체
typedef struct {
    char name[20];
    CommandFunc func;
} Command;

// Registry
Command registry[MAX_COMMANDS];
int count = 0;

// =========================
// Registry 관련 함수
// =========================

void registerCommand(char* name, CommandFunc func)
{
    if (count >= MAX_COMMANDS)
        return;

    strcpy(registry[count].name, name);
    registry[count].func = func;
    count++;
}

void registerAll(Command commands[], int n)
{
    for (int i = 0; i < n; i++)
    {
        registerCommand(
            commands[i].name,
            commands[i].func
        );
    }
}

// =========================
// 유틸 함수
// =========================

void removeQuotes(char* str)
{
    int len = strlen(str);

    if (len >= 2 &&
        str[0] == '"' &&
        str[len - 1] == '"')
    {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

// =========================
// 명령 실행
// =========================

void executeCommand(char* name, char* args)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(registry[i].name, name) == 0)
        {
            registry[i].func(args);
            return;
        }
    }

    printf("명령 없음\n");
}

// =========================
// CLI 루프
// =========================

void runCLI(void)
{
    char input[MAX_INPUT];
    char cmd[20];
    char args[80];

    printf("CLI 프로그램 (exit 입력 시 종료)\n");

    while (1)
    {
        printf(">> ");

        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        cmd[0] = '\0';
        args[0] = '\0';

        sscanf(input, "%s %[^\n]", cmd, args);

        if (strcmp(cmd, "exit") == 0)
        {
            printf("종료합니다.\n");
            break;
        }

        if (strlen(cmd) == 0)
            continue;

        executeCommand(cmd, args);
    }
}