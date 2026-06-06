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

// 두 색상 간의 색 거리 계산 (크로마키용)
// 현재 픽셀의 색상과 제거할 배경색 사이의 유사도 계산
double colorDistance(int r1, int g1, int b1,
    int r2, int g2, int b2)
{
    int dr = r1 - r2; // R값 차이 계산
	int dg = g1 - g2; // G값 차이 계산
	int db = b1 - b2; // B값 차이 계산
    // RGB 공간에서 유클리드 거리 계산 후 반환
    // √(ΔR² + ΔG² + ΔB²)
    return sqrt((double)(dr * dr + dg * dg + db * db));
}

