#include <stdio.h>
#include <string.h>

#include "../header/Command.h"
#include "../header/CLI.h"
#include "../header/FileCore.h"

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