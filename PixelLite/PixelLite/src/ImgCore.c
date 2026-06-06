#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ================================================
// stb_image 설정
// stb_image.h / stb_image_write.h 는
// lib/ 폴더에 위치해야 함
// ================================================
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"

#include "../header/ImgCore.h"
#include "../header/FileCore.h"

// RGB 값을 강제로 0~255 범위로 맞추는 함수
// EX) clamp(300) → 255, clamp(-20) → 0
// 0~255 범위를 벗어날 경우 오류가 발생하기 때문
int clamp(int value)
{
    if (value < 0)   return 0;
    if (value > 255) return 255;
    return value;
}

