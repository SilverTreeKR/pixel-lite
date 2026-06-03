#include <stdio.h>
#include <string.h>

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