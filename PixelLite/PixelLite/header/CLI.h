#ifndef CLI_H
#define CLI_H

typedef void (*CommandFunc)(char*);

typedef struct {
    char name[20];
    CommandFunc func;
} Command;

// 등록
void registerCommand(char* name, CommandFunc func);
void registerAll(Command commands[], int n);

// 실행
void executeCommand(char* name, char* args);

// CLI 시작
void runCLI(void);

#endif