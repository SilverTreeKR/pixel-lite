#ifndef FILECORE_H
#define FILECORE_H

#define MAX_PATH_LEN 512 // 최대 경로의 길이

typedef struct {
    char imagePath[MAX_PATH_LEN];   // 입력 이미지 전체 경로 (파일명 포함)
    char resultPath[MAX_PATH_LEN];  // 결과물 출력 폴더 경로
} FileCore;

FileCore* getFileCore(void); // 싱글톤 인스턴스 반환

// ------------------------------------------------
// 경로 설정
// ------------------------------------------------
void setImagePath(const char* path);   // 이미지 경로 설정
void setResultPath(const char* path);  // 결과물 폴더 경로 설정

// ------------------------------------------------
// 경로 조회
// ------------------------------------------------
const char* getImagePath(void);   // 현재 이미지 경로 반환
const char* getResultPath(void);  // 현재 결과물 폴더 경로 반환

// ------------------------------------------------
// 파일 탐색기 (Windows API)
// ------------------------------------------------
int openImageDialog(void);   // 파일 선택 다이얼로그 → imagePath 저장
int openFolderDialog(void);  // 폴더 선택 다이얼로그 → resultPath 저장

// ------------------------------------------------
// 유틸리티
// ------------------------------------------------
int isImagePathSet(void);   // imagePath 설정 여부 확인
int isResultPathSet(void);  // resultPath 설정 여부 확인

// 이미지 경로에서 파일명만 추출 (확장자 포함)
// 예) "C:\photos\input.png" → "input.png"
const char* getImageFileName(void);

// 결과물 전체 경로 생성
// 예) resultPath + "\" + prefix + 원본파일명
// prefix format: "bw_", "blur_" 등 효과 이름
void buildResultFilePath(char* dest, const char* prefix);

#endif