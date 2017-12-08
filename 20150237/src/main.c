#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>//방해 신호 제어위한 헤더파일
#include "movie.h"

/**
 * Main 함수
 */
int main(int argc, char* argv[]) {

	Context* context;
	char input[1000];
	char* cmdArg[100];
	int cmdCnt;
	int isOption[100];
	int result;

	cmdArg[0] = input;
	context = (Context*) malloc(sizeof(Context));
	context->movie = NULL;
	context->director = NULL;
	context->actor = NULL;

	// 방해신호 제어 설정
	signal(SIGINT, int_handler);

	//  log file 초기화
	init(context);

	printf(">> Welcome to My Movie <<\nFile Loading.....\nYou can use add, update, delete, search, sort, save, end commands.\n");

	// 사용자의 명령어 입력 받음
	do {
		int i = 0;
		result = 1;
		cmdCnt = 1;

		// 명령어 입력 받는 부분
		printf("(movie) ");

		do {
			input[i] = getchar();
			if (i==0 && input[i] == ' ') continue;

			if (input[i] == ' ') {
				input[i] = '\0';
				if (i > 0 && input[i-1] == '\0') {
					cmdArg[cmdCnt-1]++;
				} else {
					cmdArg[cmdCnt] = input+(i+1);
					cmdCnt++;
				}
			} else if (input[i] == '\n') {
				input[i] = '\0';
				if (i > 0 && input[i-1] == '\0') {
					cmdCnt--;
				}
				break;
			}
			i++;
		} while (1);

		// 프로세싱
		if (!strcmp(input, "search")) {
			result = search (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "print" )) {
			result = print  (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "add"   )) {
			result = add    (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "update")) {
			result = update (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "delete")) {
			result = delete (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "sort"  )) {
			result = sort   (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "save"  )) {
			result = save   (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "test"  )) {
			result = test   (context, cmdCnt, cmdArg);
		} else if (!strcmp(input, "end"   )) {

			// 종료전 모든 요소 저장
			cmdCnt = 2;
			cmdArg[1] = input;
			input[1] = '\0';
			input[0] = 'm'; save(context, cmdCnt, cmdArg);
			input[0] = 'd'; save(context, cmdCnt, cmdArg);
			input[0] = 'a'; save(context, cmdCnt, cmdArg);
			break;
		}

		if (result == 0) { 
			printf("@@ Done\n");
		}

	} while (1);

	free(context);

	return 0;
}
