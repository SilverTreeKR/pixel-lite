#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define MENU_SIZE 7
#define UP 1
#define DOWN 2
#define ENTER 3

char* menu[MENU_SIZE] = {
    "사진 선택",
    "크로마키",
    "흑백",
    "색상 반전",
    "색상 강조",
    "블러",
    "종료"
};

void displayMenu(int select) {
    system("cls");
    printf(
        " ______   __     __  __     ______     __            __         __     ______   ______    \n"
        "/\\  == \\ /\\ \\   /\\_\\_\\_\\   /\\  ___\\   /\\ \\          /\\ \\       /\\ \\   /\\__  _\\ /\\  ___\\   \n"
        "\\ \\  _-/ \\ \\ \\  \\/_/\\_\\/_  \\ \\  __\\   \\ \\ \\____     \\ \\ \\____  \\ \\ \\  \\/_/\\ \\/ \\ \\  __\\   \n"
        " \\ \\_\\    \\ \\_\\   /\\_\\/\\_\\  \\ \\_____\\  \\ \\_____\\     \\ \\_____\\  \\ \\_\\    \\ \\_\\  \\ \\_____\\ \n"
        "  \\/_/     \\/_/   \\/_/\\/_/   \\/_____/   \\/_____/      \\/_____/   \\/_/     \\/_/   \\/_____/ \n\n"
    );
    
    printf("=== 메뉴 선택 ===\n\n");

    for (int i = 0; i < MENU_SIZE; i++) {
        if (i == select) {
            printf(" > %s <  \n", menu[i]);
        }
        else {
            printf("  %s   \n", menu[i]);
        }
    }
    printf("\n(↑ ↓ 이동 / ENTER 선택)\n\n");

}

int keyControl()
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

int main()
{
    int select = 0;
    int key;

    while (1)
    {
        displayMenu(select);

        key = keyControl();

        switch (key)
        {
        case UP:
            select--;

            if (select < 0)
                select = MENU_SIZE - 1;
            break;

        case DOWN:
            select++;

            if (select >= MENU_SIZE)
                select = 0;
            break;

        case ENTER:
            system("cls");
            printf("선택한 메뉴: %s\n", menu[select]);

            if (select == 6)
            {
                printf("\n아무 키나 누르면 종료됩니다.\n");
                return 0;
                _getch();
            }

            printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
            _getch();
            break;
        }
    }
    return 0;
}