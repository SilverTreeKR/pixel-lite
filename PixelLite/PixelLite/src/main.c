#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "../header/UI.h"

int main()
{
    int select = 0;
    int key;

    while (1)
    {
        displayMenu(select);

        key = getKey();

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

            if (select == 7)
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
