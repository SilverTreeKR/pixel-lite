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

// 이미지를 로드 하여 Image 구조체로 반환하는 함수
Image* loadImage(void)
{
    const char* path = getImagePath(); // 현재 설정된 이미지 경로 가져오기


	// 경로기 비어있을 경우  오류 메시지 출력 후 NULL 반환
    if (strlen(path) == 0)
    {
        printf("오류: 이미지 경로가 설정되지 않았습니다.\n");
        printf("      UI에서 '사진 선택' 또는 CLI에서 setImage \"경로\" 를 사용하세요.\n");
        return NULL;
    }

	// Image 구조체를 위한 메모리 할당
    Image* img = (Image*)malloc(sizeof(Image));

    // 메모리 할당을 실패할 경우
    if (img == NULL)
    {
        printf("오류: 메모리 할당 실패\n");
        return NULL;
    }

    // stb_image로 로드 (채널 4로 강제: RGBA)
    img->data = stbi_load(path,
        &img->width, // 이미지 너비 저장
		&img->height, // 이미지 높이 저장
		&img->channels, // 원본 채널 수 저장
        4);  // 4 = RGBA로 강제 로드 (크로마키에서 투명도 필요)

	// 이미지 로드 실패 시 오류 메시지 출력 후 메모리 해제 및 NULL 반환
    if (img->data == NULL)
    {
        printf("오류: 이미지 로드 실패 → %s\n", path);
        printf("      지원 형식: PNG, JPG, JPEG, BMP\n");
        free(img);
        return NULL;
    }

    // 실제 사용 채널 수를 RGBA(4)로 설정 
    img->channels = 4; 
    printf("이미지 로드 성공: %s (%dx%d)\n",
        path, img->width, img->height);

    // 로드된 이미지 반환
    return img;
}