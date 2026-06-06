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

// 확장자 추출 후 소문자로 변환
// 예) "C:\photo\input.PNG" → "png"
void getExtension(const char* path, char* ext)
{
    // 경로 문자열에서 마지막 "." 위치 찾기
    const char* dot = strrchr(path, '.');
    // "." 이 없으면 확장자가 없는 파일
    if (dot == NULL)
    {
        // 빈 문자열 저장
        ext[0] = '\0';
        return;
    }

    // 확장자 저장용 인덱스
    int i = 0;

    dot++; // '.' 이후의 문자 읽기 시작
   
	// 문자열 끝까지 또는 최대 15자까지 확장자 추출
    while (dot[i] != '\0' && i < 15)
    {
        // 대문자를 소문자로 변환하여 저장
        ext[i] = (char)tolower((unsigned char)dot[i]);
        // 다음 문자 이동
        i++;
    }
    // 문자열 종료 문자 추가
    ext[i] = '\0';
}