#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <shlobj.h>     // SHBrowseForFolder
#include <commdlg.h>    // GetOpenFileName

#include "../header/FileCore.h"

// FileCore 구조체의 정적 인스턴스 정의
static FileCore instance = {
    .imagePath = "",
    .resultPath = ""
};

// 정적 인스턴스 반환 함수
FileCore* getFileCore(void)
{
    return &instance;
}

// ================================================
// Setter 
// ================================================
// 비고: strncpy(복사본 저장될 대상 주소, 복사할 문자열, 복사할 최대 바이트 수)
// ================================================


// 영상 처리 효과 입힐 이미지 경로 설정 함수
void setImagePath(const char* path)
{
    strncpy(instance.imagePath, path, MAX_PATH_LEN - 1); 
    instance.imagePath[MAX_PATH_LEN - 1] = '\0';
}

// 영상 처리 효과 결과물 이미지 경로 설정 함수
void setResultPath(const char* path)
{
    strncpy(instance.resultPath, path, MAX_PATH_LEN - 1);
    instance.resultPath[MAX_PATH_LEN - 1] = '\0';
}

// ================================================
// Getter
// ================================================
// 비고: 단순히 반환만 할 것. 가공 X
// ================================================

// 영상 처리 효과 입힐 이미지 경로 반환 함수
const char* getImagePath(void)
{
    return instance.imagePath;
}

// 영상 처리 효과 결과물 이미지 경로 반환 함수
const char* getResultPath(void)
{
    return instance.resultPath;
}

