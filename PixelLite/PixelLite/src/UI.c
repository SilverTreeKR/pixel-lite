#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define MENU_SIZE 7

char* menu[MENU_SIZE] = {
    "사진 선택",
    "크로마키",
    "흑백",
    "색상반전",
    "색상 강조",
    "블러",
    "종료"
};

void displayMenu() {
	system("cls");

    printf(
        " ______   __     __  __     ______     __            __         __     ______   ______    \n"
        "/\\  == \\ /\\ \\   /\\_\\_\\_\\   /\\  ___\\   /\\ \\          /\\ \\       /\\ \\   /\\__  _\\ /\\  ___\\   \n"
        "\\ \\  _-/ \\ \\ \\  \\/_/\\_\\/_  \\ \\  __\\   \\ \\ \\____     \\ \\ \\____  \\ \\ \\  \\/_/\\ \\/ \\ \\  __\\   \n"
        " \\ \\_\\    \\ \\_\\   /\\_\\/\\_\\  \\ \\_____\\  \\ \\_____\\     \\ \\_____\\  \\ \\_\\    \\ \\_\\  \\ \\_____\\ \n"
        "  \\/_/     \\/_/   \\/_/\\/_/   \\/_____/   \\/_____/      \\/_____/   \\/_/     \\/_/   \\/_____/ \n"
    );

    printf("******* 메뉴 선택 *******\n");
    for (int i = 0; i < MENU_SIZE; i++) {
		printf("%d. %s\n", i + 1, menu[i]);
	}
	printf("선택: ");
}

int main() {
    int select = 0;
    int key;

    while (1) {
        displayMenu(select);

        key = _getch();

        // 방향키 입력 처리
        if (key == 224) {
            key = _getch();

            // 위 방향키
            if (key == 72) {
                select--;

                if (select < 0)
                    select = MENU_SIZE - 1;
            }

            // 아래 방향키
            else if (key == 80) {
                select++;

                if (select >= MENU_SIZE)
                    select = 0;
            }
        }

        // Enter 키
        else if (key == 13) {
            system("cls");
            printf("선택한 메뉴: %s\n", menu[select]);

            if (select == 3) {
                printf("프로그램 종료\n");
                break;
            }

            printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
            _getch();
        }
    }

    return 0;
}