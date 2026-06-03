#ifndef CHROMAKEY_H
#define CHROMAKEY_H

typedef struct {
    unsigned char r, g, b, a;
} Pixel;

void applyChromaKey(Pixel** pixels, int width, int height, int threshold);
void applyChromaKeyDefault(Pixel** pixels, int width, int height);

#endif