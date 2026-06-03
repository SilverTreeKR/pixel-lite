#include <stdio.h>
#include <string.h>
#include "../header/CLI.h"
#include "../header/Command.h"

#define MAX_COMMANDS 20
#define MAX_INPUT 100

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

    while (1)
    {
        printf("> ");

        fgets(input, sizeof(input), stdin);

		// 개행 문자 제거
        input[strcspn(input, "\n")] = '\0';

        // 초기화
        cmd[0] = '\0';
        args[0] = '\0';

        // 명령 + 인자 분리(파싱)
        sscanf(input, "%s %[^\n]", cmd, args);

        // exit 명령어 입력 시 종료
        if (strcmp(cmd, "exit") == 0)
        {
            break;
        }

        if (strlen(cmd) == 0)
            continue;

        executeCommand(cmd, args);
    }
}