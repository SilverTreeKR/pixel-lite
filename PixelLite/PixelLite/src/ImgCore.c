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

// 이미지를 파일로 저장하는 함수
int saveImage(Image* img, const char* prefix)
{
    // 저장할 이미지가 없을 경우
    if (img == NULL)
    {
        printf("오류: 저장할 이미지가 없습니다.\n");
        return 0;
    }

    // 결과물 저장 폴더가 설정되어있지 않은 경우
    if (!isResultPathSet())
    {
        printf("오류: 결과물 폴더가 설정되지 않았습니다.\n");
        printf("      UI에서 '결과물 경로' 또는 CLI에서 setResult \"경로\" 를 사용하세요.\n");
        return 0;
    }


	// 결과 파일 경로를 저장할 버퍼
    char resultPath[MAX_PATH_LEN];

    // 결과 파일 전체 경로 생성
    // 예) C:\out\ + bw_ + input.png → C:\out\bw_input.png
	// 자세한 내용은 FileCore.c / FileCore.h 의 buildResultFilePath 함수 참조
    buildResultFilePath(resultPath, prefix);

    // 확장자 저장용 버퍼
    char ext[16];

    // 저장 경로에서 확장자 추출
    getExtension(resultPath, ext);

    // 저장 성공 여부 변수
    int success = 0;

    // PNG 파일인 경우
    if (strcmp(ext, "png") == 0)
    {
		// PNG로 저장 (알파 채널 포함)
        success = stbi_write_png(resultPath,
            img->width,
            img->height,
            img->channels,
            img->data,
            img->width * img->channels);
    }
    // JPG/JPEG 파일인 경우
    else if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0)
    {
		// JPG로 저장 (알파 채널 없음)
        success = stbi_write_jpg(resultPath,
            img->width,
            img->height,
            img->channels,
            img->data,
            90); // 품질 90
    }
    // BMP 파일인 경우
    else if (strcmp(ext, "bmp") == 0)
    {
		// BMP로 저장 (알파 채널 없음)
        success = stbi_write_bmp(resultPath,
            img->width,
            img->height,
            img->channels,
            img->data);
    }
    // 지원하지 않는 확장자인 경우 (svg 등)
    else
    {
        // 확장자 불명확 시 PNG로 저장
        printf("알 수 없는 확장자, PNG로 저장합니다.\n");

        // PNG 저장 경로 생성
        char pngPath[MAX_PATH_LEN];

        snprintf(pngPath, MAX_PATH_LEN, "%s%s%s.png",
            getResultPath(), prefix,
            getImageFileName());

		// PNG로 저장 (알파 채널 포함)
        success = stbi_write_png(pngPath,
            img->width,
            img->height,
            img->channels,
            img->data,
            img->width * img->channels);
    }

	// 저장 성공 여부 확인
    if (success)
        printf("저장 완료: %s\n", resultPath);
    else
        printf("오류: 이미지 저장 실패 → %s\n", resultPath);

	// 성공 여부 반환
    return success;
}


// Image 구조체의 메모리를 해제하는 함수
// 메모리 누수 방지
void freeImage(Image* img)
{
	// NULL 체크 (이미 해제되었거나 잘못된 포인터일 수 있음)
    if (img == NULL) return;

	// 이미지 데이터가 존재할 경우 메모리 해제
	// stbi_image_free는 stb_image로 로드된 이미지 데이터를 해제하는 함수
    if (img->data != NULL)
        stbi_image_free(img->data);
    free(img);
}


// 새로운 Image 구조체와 픽셀 데이터를 생성하는 함수
static Image* allocImage(int width, int height, int channels)
{
	// Image 구조체를 위한 메모리 할당
    Image* img = (Image*)malloc(sizeof(Image));

	// 메모리 할당 실패 시 NULL 반환
    if (img == NULL) return NULL;

	img->width = width; // 이미지 너비 설정
	img->height = height; // 이미지 높이 설정
	img->channels = channels; // 이미지 채널 수 설정
	img->data = (unsigned char*)calloc(width * height * channels, 1); // 픽셀 데이터 메모리 할당 및 0으로 초기화

	// 픽셀 데이터 할당 실패 시 메모리 해제 및 NULL 반환
    if (img->data == NULL)
    {
        free(img);
        return NULL;
    }

    // 생성된 이미지 반환
    return img;
}

// 이미지에 2x2 블러 효과를 적용하는 함수
Image* applyBlur(Image* src)
{
	// 입력 이미지가 NULL인 경우
    if (src == NULL) return NULL;

	// 결과 이미지를 저장할 새로운 Image 구조체 생성
    Image* dst = allocImage(src->width, src->height, src->channels);

    // 생성 실패 시 NULL 반환
    if (dst == NULL) return NULL;

    // 이미지 크기 및 채널 수 저장
    int w = src->width;
    int h = src->height;
    int c = src->channels;

    // 모든 행(높이) 순회
    for (int y = 0; y < h; y++)
    {
        // 모든 열(너비) 순회
        for (int x = 0; x < w; x++)
        {
			// 모든 행과 열의 픽셀에 대한 색상 채널 순회
            for (int ch = 0; ch < c; ch++)
            {
                // 알파 채널은 블러 미적용 (그대로 복사)
                if (ch == 3)
                {
                    // 원본 값 복사
                    dst->data[(y * w + x) * c + ch] =
                        src->data[(y * w + x) * c + ch];
                    continue;
                }

                // 주변 픽셀 값의 합
                int sum = 0;
                // 실제 더해진 픽셀 개수
                int count = 0;

				// 사용한 커널의 크기는 2x2이므로,
				// 현재 픽셀의 오른쪽 (x+1)과 아래쪽 (y+1) 픽셀까지 포함하여 총 4개의 픽셀을 참조
                // 아래는 이를 위한 알고리즘
                for (int ky = 0; ky < 2; ky++)
                {
                    for (int kx = 0; kx < 2; kx++)
                    {
                        // 이웃 픽셀 좌표 계산
                        int nx = x + kx;
                        int ny = y + ky;

                        // 이미지 범위 안에 있는 경우만 처리
                        if (nx < w && ny < h)
                        {
                            // 픽셀 값 누적
                            sum += src->data[(ny * w + nx) * c + ch];
                            // 사용한 픽셀 개수 증가
                            count++;
                        }
                    }
                }

                // 평균 값 곘나 후 결과 이미지에 저장
                dst->data[(y * w + x) * c + ch] =
                    (unsigned char)(sum / count);
            }
        }
    }

    printf("블러 처리 완료\n");

    // 결과 이미지 반환
    return dst;
}

// ================================================
// 흑백 변환
// 휘도 공식: Y = 0.299R + 0.587G + 0.114B
// -> 사람 눈에 맞춰 밝기 기여도를 조절하는 방식
// 3차원 -> 1차원으로 채널 수 변경
// ================================================

Image * applyGrayscale(Image * src)
{
	// 입력 이미지가 NULL인 경우
    if (src == NULL) return NULL;

	// 결과 이미지를 저장할 새로운 Image 구조체 생성
    Image* dst = allocImage(src->width, src->height, src->channels);
    
	// 생성 실패 시 NULL 반환
    if (dst == NULL) return NULL;

	// 이미지 크기 및 채널 수 저장
    int w = src->width;
    int h = src->height;
    int c = src->channels;

    // 모든 행(높이) 순회
    for (int y = 0; y < h; y++)
    {
		// 모든 열(너비) 순회
        for (int x = 0; x < w; x++)
        {
			// 현재 픽셀의 시작 인덱스를 계산
            int idx = (y * w + x) * c;

			// 원본 RGB 값 추출
            unsigned char r = src->data[idx + 0];
            unsigned char g = src->data[idx + 1];
            unsigned char b = src->data[idx + 2];

            // 휘도 계산
            unsigned char gray = (unsigned char)(
                0.299 * r + 0.587 * g + 0.114 * b
                );

			dst->data[idx + 0] = gray; // R 채널에 그레이스케일 값 저장
			dst->data[idx + 1] = gray; // G 채널에 그레이스케일 값 저장
			dst->data[idx + 2] = gray; // B 채널에 그레이스케일 값 저장

            // 알파 채널은 건들지 말고 그대로 복사
            if (c == 4)
                dst->data[idx + 3] = src->data[idx + 3];
        }
    }

    printf("흑백 변환 완료\n");

    // 결과 이미지 반환
    return dst;
}

// ================================================
// 크로마키 (초록색 제거)
// PNG → 알파값 0 (투명)
// JPG → 흰색 (255, 255, 255)
// ================================================

Image* applyChromaKey(Image* src, int threshold)
{
	// 입력 이미지가 NULL인 경우
    if (src == NULL) return NULL;

	// 결과 이미지를 저장할 새로운 Image 구조체 생성
    Image* dst = allocImage(src->width, src->height, src->channels);
    
	// 생성 실패 시 NULL 반환
    if (dst == NULL) return NULL;


	// 이미지 크기 및 채널 수 저장
    int w = src->width;
    int h = src->height;
    int c = src->channels;

    // 입력 파일 확장자 확인 (PNG 여부)
    char ext[16];
    getExtension(getImagePath(), ext);
    int isPNG = (strcmp(ext, "png") == 0);

    // 키 컬러: 순수 초록 (0, 255, 0)
    int keyR = 0, keyG = 255, keyB = 0;

	// 모든 행(높이) 순회
    for (int y = 0; y < h; y++)
    {
		// 모든 열(너비) 순회
        for (int x = 0; x < w; x++)
        {
			// 현재 픽셀의 시작 인덱스를 계산
            int idx = (y * w + x) * c;

			// 원본 RGB 값 추출
            unsigned char r = src->data[idx + 0];
            unsigned char g = src->data[idx + 1];
            unsigned char b = src->data[idx + 2];

			// 현재 픽셀과 초록색 사이의 거리 계산 (유클리드 거리)
            double dist = colorDistance(r, g, b, keyR, keyG, keyB);


			// 거리가 임계값보다 작은 경우 (초록색과 유사한 경우)
            if (dist <= threshold)
            {
				// PNG 이고 알파 채널이 있는 경우
                if (isPNG && c == 4)
                {
                    // 모든 채널을 0으로 설정 (투명화)
                    dst->data[idx + 0] = 0;
                    dst->data[idx + 1] = 0;
                    dst->data[idx + 2] = 0;
                    dst->data[idx + 3] = 0;   // 알파 = 0 (완전 투명)
                }
                else
                {
                    // JPG: 흰색으로 대체
                    dst->data[idx + 0] = 255;
                    dst->data[idx + 1] = 255;
                    dst->data[idx + 2] = 255;

					// 알파 채널이 없는 경우는 건드리지 않음
                    if (c == 4) dst->data[idx + 3] = 255;
                }
            }
            else
            {
                // 초록색이 아니면 원본 그대로 복사
                dst->data[idx + 0] = r;
                dst->data[idx + 1] = g;
                dst->data[idx + 2] = b;
				// 알파 채널이 있는 경우 복사
                if (c == 4) dst->data[idx + 3] = src->data[idx + 3];
            }
        }
    }

    printf("크로마키 처리 완료 (threshold: %d)\n", threshold);
    return dst;
}