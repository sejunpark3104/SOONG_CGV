#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "movie.h"

/**
 * 표준입력장치로부터 입력된 버퍼 비우기 위함
 */
void flushing() {
	char c;
	while((c=getchar()) != '\n' && c != EOF);
}

/**
 * 신호제어를 하기 위함 (ctrl+c)
 */
void int_handler(int dummy) {
	char input;
	printf("\n\nGet Interrupt Signal.");
	printf("\nDo you want to exit myMOVIE program? (y/N) ");
	input = getchar();
	if (input == 'Y' || input == 'y') {
		exit(0);
	}
	printf("\nPress enter key to contniue\n");
	return;
}

/**
 * save 와 update 옵션 체크하기 위함
 */
int is_option_existed(char* options, int numOption, char an_option) {
	int i;
	for (i=0; i<numOption; i++) {
		if (options[i] == an_option) {
			return 1;
		}
	}
	return 0;
}

/**
 * colon(':') 을 "??;"로 바꿈
 */
char* change_colon (char* str) {
	char* newstr;
	int len = 0;
	int i = 0, j = 0;

	while (str[len] != '\0') len++;
	newstr = (char*) malloc(len * 2);

	while (str[i] != '\0') {
		if (str[i] == ':') {
			newstr[j] = '?';
			newstr[j+1] = '?';
			newstr[j+2] = ';';
			j += 3;
		} else {
			newstr[j] = str[i];
			j++;
		}
		i++;
	}

	newstr[j] = '\0';
	return newstr;
}

/**
 *  "??;" 을 colon(':')로 바꿈
 */
char* recover_colon (char* str) {
	char* newstr;
	int len = 0;
	int i = 0, j = 0;

	while (str[len] != '\0') len++;
	newstr = (char*) malloc(len);

	while (str[i] != '\0') {
		if (str[i  ] == '?'
		 && str[i+1] == '?'
 		 && str[i+2] == ';') {

			newstr[j] = ':';
			i += 3;
		} else {
			newstr[j] = str[i];
			i++;
		}
		j++;
	}

	newstr[j] = '\0';
	return newstr;
}

/**
 * 파일 존재하는지 체크
 * 존재하면 return 1, 안하면 return 0.
 */
int is_file_existed (char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp) {
		fclose(fp);
		return 1;
	}
	return 0;
}

/**
 * 현 시간부로 파일 rename
 */
int rename_file (char* filename) {
	time_t timer;
	struct tm* tm_info;
	char buffer[100];
  char new_filename[100];

	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer, 26, "%Y%m%d%H%M", tm_info);
	sprintf(new_filename, "%s.%s", filename, buffer);

	return rename(filename, new_filename);;
}
