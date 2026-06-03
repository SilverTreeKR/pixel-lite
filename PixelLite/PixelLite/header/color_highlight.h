#ifndef COLOR_HIGHLIGHT_H
#define COLOR_HIGHLIGHT_H

#include "../header/chromakey.h"

// channel: 0=R, 1=G, 2=B
// strength: 0 ~ 255 (255 = 최대 강조)
void applyColorHighlight(Pixel** pixels, int width, int height, int channel, int strength);

#endif