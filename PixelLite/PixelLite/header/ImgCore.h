#ifndef IMGCORE_H
#define IMGCORE_H

// ================================================
// 사용 라이브러리: stb_image, stb_image_write
// ================================================

// stb_image로 로드한 픽셀 데이터를 담는 구조체
typedef struct {
    unsigned char* data;    // 픽셀 데이터 (RGBA 또는 RGB)
    int width;
    int height;
    int channels;           // 3 = RGB, 4 = RGBA
} Image;


// FileCore의 imagePath에서 이미지 로드
// 성공: Image 포인터 반환 / 실패: NULL
Image* loadImage(void);

// FileCore의 resultPath + prefix로 이미지 저장
// 성공: 1 / 실패: 0
int saveImage(Image* img, const char* prefix);

// 이미지 메모리 해제
void freeImage(Image* img);

// ------------------------------------------------
// 이미지 프로세싱 함수들
// 모두 새 Image를 반환 (원본 보존)
// 실패 시 NULL 반환
// ------------------------------------------------

// 평균 블러 (2x2 커널 고정)
Image* applyBlur(Image* src);

// 흑백 변환 (휘도 공식: 0.299R + 0.587G + 0.114B)
Image* applyGrayscale(Image* src);

// 크로마키 (초록색 제거)
// PNG → 투명 처리 / JPG → 흰색으로 대체
// threshold: 색거리 허용 범위 (권장: 80~120)
Image* applyChromaKey(Image* src, int threshold);

// 색상 반전 (255 - 각 채널)
Image* applyInvert(Image* src);

// 색상 강조 (R, G, B 각각 0~255 덧셈)
// 각 채널에 값을 더하고 강제로 0~255 범위로 잘라냄.
Image* applyEnhance(Image* src, int r, int g, int b);


// 값을 0~255 범위로 잘라냄
int clamp(int value);

// 두 색상 간의 색 거리 계산 (크로마키용)
double colorDistance(int r1, int g1, int b1,
    int r2, int g2, int b2);

// 확장자 추출 (소문자로 반환)
// 예) "input.PNG" → "png"
void getExtension(const char* path, char* ext);

#endif