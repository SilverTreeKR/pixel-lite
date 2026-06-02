#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "../header/UI.h"

char* menu[MENU_SIZE] = {
    "사진 선택",
    "크로마키",
    "흑백",
    "색상 반전",
    "색상 강조",
    "블러",
    "CLI",
    "종료"
};

void textcolor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

void displayMenu(int select) {
    system("cls");

    textcolor(11);
    printf(" ______   __     __  __     ______     __            __         __     ______   ______    \n");

    textcolor(10);
    printf("/\\  == \\ /\\ \\   /\\_\\_\\_\\   /\\  ___\\   /\\ \\          /\\ \\       /\\ \\   /\\__  _\\ /\\  ___\\   \n");

    textcolor(3);
    printf("\\ \\  _-/ \\ \\ \\  \\/_/\\_\\/_  \\ \\  __\\   \\ \\ \\____     \\ \\ \\____  \\ \\ \\  \\/_/\\ \\/ \\ \\  __\\   \n");

    textcolor(9);
    printf(" \\ \\_\\    \\ \\_\\   /\\_\\/\\_\\  \\ \\_____\\  \\ \\_____\\     \\ \\_____\\  \\ \\_\\    \\ \\_\\  \\ \\_____\\ \n");

    textcolor(1);
    printf("  \\/_/     \\/_/   \\/_/\\/_/   \\/_____/   \\/_____/      \\/_____/   \\/_/     \\/_/   \\/_____/ \n");

    printf("                                                                                          \n");
    textcolor(7);
    printf("=========================================== 메뉴 선택 ===========================================\n\n");
    for (int i = 0; i < MENU_SIZE; i++) {
        if (i == select) {
            printf("                                       >    %s    <                                      \n", menu[i]);
        }
        else {
            printf("                                            %s                                           \n", menu[i]);
        }
    }
    printf("\n================================================================================================\n\n");
    printf("\n                                     (↑ ↓ 이동 / ENTER 선택)\n\n");

}

int getKey()
{
    int key = _getch();

    if (key == 224)
    {
        key = getch();

        switch (key)
        {
        case 72:
            return UP;

        case 80:
            return DOWN;
        }
    }

    else if (key == 13)
    {
        return ENTER;
    }

    return 0;
}
