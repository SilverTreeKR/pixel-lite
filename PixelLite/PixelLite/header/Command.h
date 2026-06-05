#ifndef COMMAND_H
#define COMMAND_H

#include "CLI.h"

// FileCore 명령
void cmd_setImage(char* args);   // setImage "경로"
void cmd_setResult(char* args);  // setResult "경로"
void cmd_info(char* args);       // info - 현재 경로 출력

#endif