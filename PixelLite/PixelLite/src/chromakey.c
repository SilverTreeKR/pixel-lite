#include <stdio.h>
#include "../header/chromakey.h"

void applyChromaKey(Pixel** pixels, int width, int height, int threshold) {
    int total = 0, i = 0, removed = 0;
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

        if (p->g > p->r + threshold && p->g > p->b + threshold) {
            p->a = 0;
            removed++;
        }
        i++;
    }

    printf("크로마키 완료: %d / %d 픽셀 제거됨\n", removed, total);
}

void applyChromaKeyDefault(Pixel** pixels, int width, int height) {
    applyChromaKey(pixels, width, height, 40);
}