#include <stdio.h>
#include <string.h>

#include "../header/Command.h"
#include "../header/CLI.h"

static void removeQuotes(char* str)
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

void cmd_add(char* args)
{
    int a, b;

    if (sscanf(args, "%d %d", &a, &b) != 2)
    {
        printf("사용법: add 숫자 숫자\n");
        return;
    }

    printf("결과: %d\n", a + b);
}
