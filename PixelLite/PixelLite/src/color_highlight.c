#include <stdio.h>
#include "../header/color_highlight.h"

void applyColorHighlight(Pixel** pixels, int width, int height, int channel, int strength) {
    int row, col;
    Pixel* p = NULL;

    if (pixels == NULL) {
        printf("Error: no image data.\n");
        return;
    }

    if (strength < 0)   strength = 0;
    if (strength > 255) strength = 255;

    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            p = &pixels[row][col];
            if (channel == 0) {
                p->g = (unsigned char)(p->g * (255 - strength) / 255);
                p->b = (unsigned char)(p->b * (255 - strength) / 255);
            }
            else if (channel == 1) {
                p->r = (unsigned char)(p->r * (255 - strength) / 255);
                p->b = (unsigned char)(p->b * (255 - strength) / 255);
            }
            else if (channel == 2) {
                p->r = (unsigned char)(p->r * (255 - strength) / 255);
                p->g = (unsigned char)(p->g * (255 - strength) / 255);
            }
        }
    }

    printf("Color highlight done. channel: %s, strength: %d\n",
        channel == 0 ? "R" : channel == 1 ? "G" : "B", strength);
}