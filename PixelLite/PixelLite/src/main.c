#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "../header/UI.h"
#include "../header/CLI.h"
#include "../header/Command.h"
#include "../header/FileCore.h"

int main()
{
    // =========================
    // UI 메뉴 선택창
    // =========================
    int select = 0;
    int key;

    while (1)
    {
        // select 값에 따라 메뉴 출력
        displayMenu(select);

        key = getKey();

        // 키 입력 가져오는 부분
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

            // =========================
            // 0. 사진 선택 -> 파일 탐색기 열어서 이미지 선택
            // =========================
            if (select == 0)
            {
                openImageDialog();
                printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                _getch();
                break;
            }

            // =========================
			// 6. CLI 메뉴 선택 -> runCLI()
            // =========================
			if (select == 6)
			{
                // 커맨드 목록 정의
                Command commands[] = {
                    {"setImage", cmd_setImage},
                    {"setResult", cmd_setResult},
                    {"info", cmd_info}
                };

                // 배열 개수 계산
                int n = sizeof(commands) / sizeof(commands[0]);

				// 커맨드 등록
                registerAll(commands, n);

				// CLI 실행
                system("cls");
                runCLI();
			}
            
            // =========================
			// 7. 종료 메뉴 선택 -> 프로그램 종료
            // =========================
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
