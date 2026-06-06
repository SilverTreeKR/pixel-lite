#include <stdio.h>
#include <string.h>

#include "../header/Command.h"
#include "../header/CLI.h"
#include "../header/FileCore.h"
#include "../header/ImgCore.h"

// ===================================
// 유틸리티
// ===================================

static void removeQuotes(char* str)
{
    int len = strlen(str);

    if (len >= 2 &&
        str[0] == '"' &&
        str[len - 1] == '"')
    {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

// 이미지 처리 공통 흐름:
// 로드 → 처리 → 저장 → 해제
// processAndSave(이미지 처리 함수, 접두사)
static void processAndSave(Image* (*applyFunc)(Image*), const char* prefix)
{
    // 원본 이미지 로드
    Image* src = loadImage();

    // 없으면 종료
    if (src == NULL) return;

    // 이미지 처리 함수 실행
    Image* dst = applyFunc(src);
   
    // 처리 실패시
    // PS. 여기서 오류나면 진짜 모르겠다. 때려치고 싶어질거 같음. 2026-06-06 태웅
    if (dst == NULL)
    {
        printf("오류: 이미지 처리 실패\n");
        freeImage(src);
        return;
    }

    // 처리된 이미지 저장
    saveImage(dst, prefix);

    // 메모리 해제
    freeImage(src);
    freeImage(dst);
}

// info — 현재 설정된 경로 출력
void cmd_info(char* args)
{
    printf("===== 현재 경로 정보 =====\n");

    if (isImagePathSet())
        printf("이미지  경로: %s\n", getImagePath());
    else
        printf("이미지  경로: (설정 안 됨)\n");

    if (isResultPathSet())
        printf("결과물 폴더: %s\n", getResultPath());
    else
        printf("결과물 폴더: (설정 안 됨)\n");

    printf("==========================\n");
}

// 도움말 명령어
void cmd_help(char* args)
{
    printf("===== 사용 가능한 명령어 =====\n");
    printf("  [경로 설정]\n");
    printf("  setImage  \"경로\"      - 이미지 파일 경로 설정\n");
    printf("  setResult \"경로\"      - 결과물 폴더 경로 설정\n");
    printf("  info                  - 현재 경로 정보 출력\n");
    printf("\n");
    printf("  [이미지 처리]\n");
    printf("  blur                  - 평균 블러 (2x2)\n");
    printf("  grayscale             - 흑백 변환\n");
    printf("  invert                - 색상 반전\n");
    printf("  chromakey [threshold] - 크로마키 (기본 threshold: 100)\n");
    printf("  enhance R G B         - 색상 강조 (각 0~255)\n");
    printf("\n");
    printf("  [기타]\n");
    printf("  help                  - 명령어 목록 출력\n");
    printf("  exit                  - CLI 종료\n");
    printf("==============================\n");
}

// ===================================
// FileCore.c
// ===================================

// setImage "경로" — 이미지 경로 직접 지정
void cmd_setImage(char* args)
{
    if (strlen(args) == 0) // 인자값이 안들어온 경우
    {
        printf("사용법: setImage \"경로\"\n");
        printf("예시:   setImage \"C:\\photos\\input.png\"\n");
        return;
    }

    removeQuotes(args); // 따옴표 제거하시고
	setImagePath(args); // FileCore의 imagePath에 저장
    printf("이미지 경로 설정 완료: %s\n", args);
}

// setResult "경로" — 결과물 폴더 경로 직접 지정
void cmd_setResult(char* args)
{
    if (strlen(args) == 0) // 인자값이 안들어온 경우
    {
        printf("사용법: setResult \"경로\"\n");
        printf("예시:   setResult \"C:\\photos\\output\\\"\n");
        return;
    }

	removeQuotes(args); // 따옴표 제거하시고

    // 끝에 '\\' 없으면 추가
	// PS. 폴더 경로는 항상 '\\'로 끝나야 나중에 파일명 붙일 때 편하다.. 
    //     이걸 3시간 박고 나서야 깨달을 줄은 몰랐지. 씨부럴거 2026-06-06 태웅
    int len = strlen(args);
    if (len > 0 && args[len - 1] != '\\')
    {
        args[len] = '\\';
        args[len + 1] = '\0';
    }

	setResultPath(args); // FileCore의 resultPath에 저장
    printf("결과물 폴더 설정 완료: %s\n", args);
}

// ===================================
// ImgCore.c
// ===================================

void cmd_blur(char* args)
{
    processAndSave(applyBlur, "blur_");
}

void cmd_grayscale(char* args)
{
    processAndSave(applyGrayscale, "bw_");
}

void cmd_invert(char* args)
{
    processAndSave(applyInvert, "invert_");
}

// chromakey [threshold]
// threshold 기본값: 100
// threshold = 어디까지 초록색으로 볼 것인지에 대한 값 80~100 권장
// 비고 : 개발력 한계로 processAndSave 함수 적용 불가 /  threshold 인자 필요함..
void cmd_chromakey(char* args)
{
    int threshold = 100; // 기본값
    if (strlen(args) > 0)
        sscanf(args, "%d", &threshold);

    // 원본 로드 하시고
    Image* src = loadImage();
    // 없으면 ㅈ까
    if (src == NULL) return;

    // 크로마키 입힐거임 (원본, 임계값)
    Image* dst = applyChromaKey(src, threshold);
    // 이거 작동하면 나도 이젠 모른다 시부럴
    if (dst == NULL)
    {
        printf("오류: 크로마키 처리 실패\n");
        freeImage(src);
        return;
    }

    // 저장
    saveImage(dst, "chromakey_");
    // 메모리 해제를 습관화 하자.
    freeImage(src);
    freeImage(dst);
}

// enhance R G B  (0~255)
// 비고 : 개발력 한계로 processAndSave 함수 적용 불가 / 수정할 r g b 값 인자 필요함..
void cmd_enhance(char* args)
{
    // 강조할 RGB 값 저장 변수
    int r = 0, g = 0, b = 0;

    // 사용자 입력에서 RGB값 가져오시고
    if (sscanf(args, "%d %d %d", &r, &g, &b) != 3)
    {
        printf("사용법: enhance R G B\n");
        printf("예시:   enhance 100 0 50\n");
        printf("        R G B 범위: 0~255\n");
        return;
    }

    // 범위 검사 (입력 받은 거에서)
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
    {
        printf("오류: R G B 값은 0~255 범위여야 합니다.\n");
        return;
    }

    // 원본 가져오시고
    Image* src = loadImage();

    // 없으면 ㅈ까
    if (src == NULL) return;

    // 색상 강조 하셔야지
    Image* dst = applyEnhance(src, r, g, b);

    // 아 제발요
    if (dst == NULL)
    {
        printf("오류: 색상 강조 처리 실패\n");
        freeImage(src);
        return;
    }

    // 저장
    saveImage(dst, "enhance_");

    // 메모리 해제
    freeImage(src);
    freeImage(dst);
}
