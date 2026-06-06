#ifndef COMMAND_H
#define COMMAND_H

#include "CLI.h"

// FileCore 명령
void cmd_setImage(char* args);
void cmd_setResult(char* args);
void cmd_info(char* args);

// ImgCore 명령
void cmd_blur(char* args);
void cmd_grayscale(char* args);
void cmd_invert(char* args);
void cmd_chromakey(char* args);
void cmd_enhance(char* args);

// 공통
void cmd_help(char* args);

#endif