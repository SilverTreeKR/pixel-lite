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
