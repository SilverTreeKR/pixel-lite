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
