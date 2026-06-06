#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "../header/UI.h"
#include "../header/CLI.h"
#include "../header/Command.h"
#include "../header/FileCore.h"
#include "../header/ImgCore.h"

// ==========================================
// 색상 강조 UI
// 비고: 시간 관계상 main에 넣기로 했음.
// ==========================================

void runEnhanceUI(void)
{
    // 항목: R, G, B, 저장 (총 4개)
#define ENHANCE_ITEMS 4

    int values[3] = { 0, 0, 0 };  // R, G, B
    int select = 0;           // 현재 커서 위치
    int editing = 0;           // 값 조절 모드 여부
    int key;

    char* labels[3] = { "R", "G", "B" };

    while (1)
    {
        system("cls");
        printf("========== 색상 강조 ==========\n\n");

        for (int i = 0; i < 3; i++)
        {
            if (i == select)
            {
                if (editing)
                    printf("  >> [ %s: %3d ] <<  (↑↓ 조절 / ENTER 확정)\n",
                        labels[i], values[i]);
                else
                    printf("  >  %s: %3d       \n", labels[i], values[i]);
            }
            else
            {
                printf("     %s: %3d       \n", labels[i], values[i]);
            }
        }

        // 저장 항목
        if (select == 3)
            printf("\n  > [ 저장 ] <\n");
        else
            printf("\n    저장\n");

        printf("\n================================\n");
        printf("(↑↓ 이동 / ENTER 선택 / ESC 취소)\n");

        key = _getch();

        // 방향키 처리 (0xE0 + 방향키 코드)
        if (key == 224)
        {
            key = _getch();

            if (editing)
            {
                // 값 조절 모드
                if (key == 72) // UP
                    values[select] = (values[select] < 255) ? values[select] + 1 : 255;
                if (key == 80) // DOWN
                    values[select] = (values[select] > 0) ? values[select] - 1 : 0;
            }
            else
            {
                // 커서 이동 모드
                if (key == 72) // UP
                {
                    select--;
                    if (select < 0) select = ENHANCE_ITEMS - 1;
                }
                if (key == 80) // DOWN
                {
                    select++;
                    if (select >= ENHANCE_ITEMS) select = 0;
                }
            }
        }
        else if (key == 13) // ENTER
        {
            if (select == 3)
            {
                // 저장 선택
                system("cls");
                printf("색상 강조 저장 중...\n");
                printf("R: %d  G: %d  B: %d\n\n", values[0], values[1], values[2]);

                Image* src = loadImage();
                if (src != NULL)
                {
                    Image* dst = applyEnhance(src, values[0], values[1], values[2]);
                    if (dst != NULL)
                    {
                        saveImage(dst, "enhance_");
                        freeImage(dst);
                    }
                    freeImage(src);
                }

                printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                _getch();
                return;
            }
            else
            {
                // R/G/B 항목 선택: 조절 모드 토글
                editing = !editing;
            }
        }
        else if (key == 27) // ESC
        {
            if (editing)
                editing = 0;  // 조절 모드 해제
            else
                return;       // UI 종료
        }
    }
}
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
            // 1. 결과물 경로 -> 파일 탐색기 열어서 폴더 선택
            // =========================
            if (select == 1)
            {
                openFolderDialog();
                printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                _getch();
                break;
            }

            // =========================
            // 2. 크로마키
            // =========================
            if (select == 2)
            {
                if (!isImagePathSet())
                {
                    printf("오류: 먼저 사진을 선택해주세요.\n");
                }
                else if (!isResultPathSet())
                {
                    printf("오류: 먼저 결과물 경로를 설정해주세요.\n");
                }
                else
                {
                    Image* src = loadImage();
                    if (src)
                    {
                        Image* dst = applyChromaKey(src, 100);
                        if (dst) { saveImage(dst, "chromakey_"); freeImage(dst); }
                        freeImage(src);
                    }
                }
                printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                _getch();
                break;
            }

            // =========================
            // 3. 흑백화
            // =========================
            if (select == 3)
            {
                if (!isImagePathSet() || !isResultPathSet())
                {
                    printf("오류: 사진과 결과물 경로를 먼저 설정해주세요.\n");
                }
                else
                {
                    Image* src = loadImage();
                    if (src)
                    {
                        Image* dst = applyGrayscale(src);
                        if (dst) { saveImage(dst, "bw_"); freeImage(dst); }
                        freeImage(src);
                    }
                }
                printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                _getch();
                break;
            }

            // =========================
            // 4. 색상 반전
            // =========================
            if (select == 4)
            {
                if (!isImagePathSet() || !isResultPathSet())
                {
                    printf("오류: 사진과 결과물 경로를 먼저 설정해주세요.\n");
                }
                else
                {
                    Image* src = loadImage();
                    if (src)
                    {
                        Image* dst = applyInvert(src);
                        if (dst) { saveImage(dst, "invert_"); freeImage(dst); }
                        freeImage(src);
                    }
                }
                printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                _getch();
                break;
            }

            // =========================
            // 5. 색상 강조
            // =========================
            if (select == 5)
            {
                if (!isImagePathSet() || !isResultPathSet())
                {
                    printf("오류: 사진과 결과물 경로를 먼저 설정해주세요.\n");
                    printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                    _getch();
                }
                else
                {
                    runEnhanceUI();
                }
                break;
            }

            // =========================
            // 6. 블러 (평균 블러)
            // =========================
            if (select == 6)
            {
                if (!isImagePathSet() || !isResultPathSet())
                {
                    printf("오류: 사진과 결과물 경로를 먼저 설정해주세요.\n");
                }
                else
                {
                    Image* src = loadImage();
                    if (src)
                    {
                        Image* dst = applyBlur(src);
                        if (dst) { saveImage(dst, "blur_"); freeImage(dst); }
                        freeImage(src);
                    }
                }
                printf("\n아무 키나 누르면 메뉴로 돌아갑니다...");
                _getch();
                break;
            }
            
            // =========================
			// 7. CLI 메뉴 선택 -> runCLI()
            // =========================
			if (select == 7)
			{
                // 커맨드 목록 정의
                Command commands[] = {
                    {"setImage",   cmd_setImage},
                    {"setResult",  cmd_setResult},
                    {"info",       cmd_info},
                    {"blur",       cmd_blur},
                    {"grayscale",  cmd_grayscale},
                    {"invert",     cmd_invert},
                    {"chromakey",  cmd_chromakey},
                    {"enhance",    cmd_enhance},
                    {"help",       cmd_help}
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
			// 8. 종료 메뉴 선택 -> 프로그램 종료
            // =========================
            if (select == 8)
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
