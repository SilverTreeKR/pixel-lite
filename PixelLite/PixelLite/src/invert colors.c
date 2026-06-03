#include <stdio.h>
#include "../header/invert.h"

void applyInvert(Pixel** pixels, int width, int height) {
    int total = 0, i = 0;
    Pixel* p = NULL;

    if (pixels == NULL) {
        printf("오류 메세지 1: 이미지 데이터가 없습니다.\n");
        return;
    }

    total = width * height;

    while (i < total) {
        int col = i % width;
        int row = i / width;
        p = &pixels[row][col];

        p->r = 255 - p->r;
        p->g = 255 - p->g;
        p->b = 255 - p->b;

        i++;
    }

    printf("색상 반전 완료: %d 픽셀 처리됨\n", total);
}