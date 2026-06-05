#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <shlobj.h>     // SHBrowseForFolder
#include <commdlg.h>    // GetOpenFileName

#include "../header/FileCore.h"

// ================================================
// 싱글톤 인스턴스
// ================================================

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
