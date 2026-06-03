#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "../header/UI.h"
// [추가] 각 기능 헤더 파일 포함
#include "../header/chromakey.h"
#include "../header/invert.h"
#include "../header/filecore.h"
#include "../header/blur.h"
#include "../header/color_highlight.h"
#include "../header/grayscale.h"

// [추가] 전역 픽셀 배열 및 이미지 크기 변수
static Pixel** g_pixels = NULL;
static int g_width = 0, g_height = 0;

int main() {
    int select = 0;
    int key;
    // [추가] 색상 강조 기능을 위한 변수
    int ch = 0;
    int threshold = 0;
    int channel = 0;    // 0=R, 1=G, 2=B
    int strength = 128; // 강도 0~255

    while (1) {
        displayMenu(select);
        key = getKey();

        switch (key) {
        case UP:
            select--;
            if (select < 0) select = MENU_SIZE - 1;
            break;

        case DOWN:
            select++;
            if (select >= MENU_SIZE) select = 0;
            break;

        case ENTER:
            system("cls");
            // [변경] "Selected: " (영문으로 변경)
            printf("Selected: %s\n", menu[select]);

            switch (select) {
                // [추가] case 0: 이미지 로드
            case 0:
                if (g_pixels) {
                    freeImage(g_pixels, g_height);
                    g_pixels = NULL;
                }
                g_pixels = loadImage(&g_width, &g_height);
                if (g_pixels)
                    printf("Image loaded: %d x %d\n", g_width, g_height);
                break;

                // [추가] case 1: 크로마키 적용
            case 1:
                if (!g_pixels) {
                    printf("Load image first.\n");
                }
                else {
                    applyChromaKeyDefault(g_pixels, g_width, g_height);
                    saveImage(g_pixels, g_width, g_height);
                }
                break;

                // [추가] case 2: 그레이스케일 적용
            case 2:
                if (!g_pixels) {
                    printf("Load image first.\n");
                }
                else {
                    applyGrayscale(g_pixels, g_width, g_height);
                    saveImage(g_pixels, g_width, g_height);
                }
                break;

                // [추가] case 3: 반전 적용
            case 3:
                if (!g_pixels) {
                    printf("Load image first.\n");
                }
                else {
                    applyInvert(g_pixels, g_width, g_height);
                    saveImage(g_pixels, g_width, g_height);
                }
                break;

                // [추가] case 4: 색상 강조 - 채널/강도 조절 후 적용
            case 4:
                if (!g_pixels) {
                    printf("Load image first.\n");
                }
                else {
                    channel = 0;
                    strength = 128;

                    while (1) {
                        system("cls");
                        printf("Color Highlight\n");
                        printf("Channel: %s  (Left/Right to change)\n",
                            channel == 0 ? "R" : channel == 1 ? "G" : "B");
                        printf("Strength: %d  (Up/Down to change, 0~255)\n", strength);
                        printf("Press ENTER to apply, ESC to cancel.\n");

                        key = _getch();
                        if (key == 224) {
                            key = _getch();
                            if (key == 75) {        // Left: 채널 감소
                                channel--;
                                if (channel < 0) channel = 2;
                            }
                            else if (key == 77) {   // Right: 채널 증가
                                channel++;
                                if (channel > 2) channel = 0;
                            }
                            else if (key == 72) {   // Up: 강도 증가
                                strength += 5;
                                if (strength > 255) strength = 255;
                            }
                            else if (key == 80) {   // Down: 강도 감소
                                strength -= 5;
                                if (strength < 0) strength = 0;
                            }
                        }
                        else if (key == 13) {       // ENTER: 적용
                            applyColorHighlight(g_pixels, g_width, g_height, channel, strength);
                            saveImage(g_pixels, g_width, g_height);
                            break;
                        }
                        else if (key == 27) {       // ESC: 취소
                            printf("Cancelled.\n");
                            break;
                        }
                    }
                }
                break;

                // [추가] case 5: 블러 적용
            case 5:
                if (!g_pixels) {
                    printf("Load image first.\n");
                }
                else {
                    applyBlur(g_pixels, g_width, g_height);
                    saveImage(g_pixels, g_width, g_height);
                }
                break;

            case 6:
                printf("Not implemented yet.\n");
                break;

                // [변경] case 7: 종료 시 g_pixels 메모리 해제 후 종료 (기존엔 바로 return)
            case 7:
                if (g_pixels) freeImage(g_pixels, g_height);
                printf("\nPress any key to exit.\n");
                _getch();
                return 0;

            default:
                printf("Not implemented yet.\n");
                break;
            }

            printf("\nPress any key to return to menu...");
            _getch();
            break;
        }
    }
    return 0;
}