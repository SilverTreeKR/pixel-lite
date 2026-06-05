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

// ================================================
// 파일 탐색기 - 이미지 선택 (GetOpenFileName)
// ================================================
// 비고: Windows API 사용하여 파일 탐색기 띄우고, 선택된 파일 경로 → imagePath 저장
//      일부 AI 활용, 오류 발생 가능성 있으므로 QA 진행 시 확인 부탁드립니다.
// ================================================


// 파일 선택 다이얼로그 열고 → imagePath 저장
int openImageDialog(void)
{
    OPENFILENAME ofn; // 파일 선택 다이얼로그 구조체
    char filePath[MAX_PATH_LEN] = ""; // 선택된 파일 경로 저장 버퍼

    ZeroMemory(&ofn, sizeof(ofn)); // 구조체 초기화

	ofn.lStructSize = sizeof(ofn); // 구조체 크기 설정
	ofn.hwndOwner = NULL; // 소유자 윈도우 핸들 (NULL이면 현재 활성 윈도우)
    ofn.lpstrFile = filePath; // 선택된 파일 경로 저장 버퍼 연결
	ofn.nMaxFile = sizeof(filePath); // 파일 경로 저장버퍼 최대크기 설정
    ofn.lpstrFilter =
        "이미지 파일\0*.png;*.jpg;*.jpeg;*.bmp\0" // 기본 이미지 파일 필터
		"PNG 파일\0*.png\0" // PNG 파일 표시
		"JPEG 파일\0*.jpg;*.jpeg\0" // JPEG 파일 표시
		"BMP 파일\0*.bmp\0" // BMP 파일 표시
		"모든 파일\0*.*\0"; // 모든 파일 표시
	ofn.nFilterIndex = 1; // 기본 필터 인덱스 (1부터 시작)
    ofn.lpstrTitle = "이미지 파일 선택"; // 파일 탐색기 타이틀
    
    // 옵션 설정하는 부분
    // ======================================================
	// OFN_FILEMUSTEXIST: 존재하는 파일만 선택 가능
	// OFN_PATHMUSTEXIST: 존재하는 경로만 선택 가능
    // ======================================================
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  

	// 파일 탐색기 열기
    if (GetOpenFileName(&ofn))
    {
		setImagePath(filePath); // FileCore의 imagePath에 선택된 파일 경로 저장
        printf("이미지 경로 설정: %s\n", filePath); // 선택된 경로 출력
        return 1;  // 성공
    }

    printf("이미지 선택 취소됨.\n"); // 선택 취소 시 메시지 출력
    return 0;  // 사용자가 취소했거나 오류 발생
}

// ================================================
// 파일 탐색기 - 결과물 폴더 선택 (SHBrowseForFolder)
// ================================================
// 비고: Windows API 사용하여 파일 탐색기 띄우고, 선택된 폴더 경로 → resultPath 저장
//      일부 AI 활용, 오류 발생 가능성 있으므로 QA 진행 시 확인 부탁드립니다.
// ================================================


int openFolderDialog(void)
{
	BROWSEINFO bi; // 폴더 선택 다이얼로그 구조체
	char folderPath[MAX_PATH_LEN] = ""; // 선택된 폴더 경로 저장 버퍼
    LPITEMIDLIST pidl; // 선택된 폴더의 ITEM ID LIST 저장 포인터

    ZeroMemory(&bi, sizeof(bi)); // 구조체 초기화

	bi.hwndOwner = NULL; // 소유자 윈도우 핸들 (NULL이면 현재 활성 윈도우)
	bi.lpszTitle = "결과물을 저장할 폴더 선택"; // 폴더 탐색기 타이틀
   
    // 폴더 선택 옵션 설정
	// ======================================================
	// BIF_RETURNONLYFSDIRS: 파일 시스템 디렉터리만 반환
	// BIF_NEWDIALOGSTYLE: 새로운 스타일의 다이얼로그 사용
	// ======================================================
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE; 

    pidl = SHBrowseForFolder(&bi); // 폴더 탐색기 열기

    if (pidl != NULL) // 사용자가 폴더를 선택한 경우
    {
		SHGetPathFromIDList(pidl, folderPath); // pidl에서 실제 폴더 경로를 folderPath 버퍼로 변환
		CoTaskMemFree(pidl); // pidl 메모리 해제

        // 끝에 '\\' 없으면 추가
		int len = strlen(folderPath); // 폴더 경로 길이 계산
		if (len > 0 && folderPath[len - 1] != '\\') // 마지막 문자가 '\\'가 아닌 경우
		{
			folderPath[len] = '\\'; // '\\' 추가
			folderPath[len + 1] = '\0'; // 문자열 종료 문자 추가
		}
        {
			folderPath[len] = '\\'; // '\\' 추가
			folderPath[len + 1] = '\0'; // 문자열 종료 문자 추가
        }

		setResultPath(folderPath); // FileCore의 resultPath에 선택된 폴더 경로 저장
        printf("결과물 폴더 설정: %s\n", folderPath); // 선택 경로 출력
        return 1;  // 성공
    }

    printf("폴더 선택 취소됨.\n");
    return 0;  // 사용자가 취소했거나 오류 발생
}
