#include <stdio.h>
#include "../header/grayscale.h"

void applyGrayscale(Pixel** pixels, int width, int height) {
    int row, col;
    int gray;
    Pixel* p = NULL;

    if (pixels == NULL) {
        printf("Error: no image data.\n");
        return;
    }

    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            p = &pixels[row][col];
            gray = (int)(p->r * 0.299f + p->g * 0.587f + p->b * 0.114f);
            p->r = gray;
            p->g = gray;
            p->b = gray;
        }
    }

    printf("Grayscale done.\n");
}