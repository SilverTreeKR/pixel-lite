// blur.c
#include <stdio.h>
#include <stdlib.h>
#include "../header/blur.h"

static void blurHorizontal(Pixel** src, Pixel** dst, int width, int height, int radius) {
    int row, col, i, c;
    int sumR, sumG, sumB, count;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            sumR = 0; sumG = 0; sumB = 0; count = 0;
            for (i = -radius; i <= radius; i++) {
                c = col + i;
                if (c >= 0 && c < width) {
                    sumR += src[row][c].r;
                    sumG += src[row][c].g;
                    sumB += src[row][c].b;
                    count++;
                }
            }
            dst[row][col].r = (unsigned char)(sumR / count);
            dst[row][col].g = (unsigned char)(sumG / count);
            dst[row][col].b = (unsigned char)(sumB / count);
            dst[row][col].a = src[row][col].a;
        }
    }
}

static void blurVertical(Pixel** src, Pixel** dst, int width, int height, int radius) {
    int row, col, i, r;
    int sumR, sumG, sumB, count;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            sumR = 0; sumG = 0; sumB = 0; count = 0;
            for (i = -radius; i <= radius; i++) {
                r = row + i;
                if (r >= 0 && r < height) {
                    sumR += src[r][col].r;
                    sumG += src[r][col].g;
                    sumB += src[r][col].b;
                    count++;
                }
            }
            dst[row][col].r = (unsigned char)(sumR / count);
            dst[row][col].g = (unsigned char)(sumG / count);
            dst[row][col].b = (unsigned char)(sumB / count);
            dst[row][col].a = src[row][col].a;
        }
    }
}

void applyBlur(Pixel** pixels, int width, int height) {
    int row, i;
    int radius = 50;
    Pixel** temp = NULL;

    if (pixels == NULL) {
        printf("Error: no image data.\n");
        return;
    }

    temp = (Pixel**)malloc(height * sizeof(Pixel*));
    if (!temp) {
        printf("Error: memory allocation failed.\n");
        return;
    }
    for (row = 0; row < height; row++) {
        temp[row] = (Pixel*)malloc(width * sizeof(Pixel));
        if (!temp[row]) {
            printf("Error: memory allocation failed.\n");
            for (i = 0; i < row; i++) free(temp[i]);
            free(temp);
            return;
        }
    }

    blurHorizontal(pixels, temp, width, height, radius);
    blurVertical(temp, pixels, width, height, radius);

    for (row = 0; row < height; row++) free(temp[row]);
    free(temp);

    printf("Blur done.\n");
}