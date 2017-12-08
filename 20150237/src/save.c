#include <stdio.h>
#include <stdlib.h>
#include "movie.h"

int save_usage() { //save 사용법
	printf("save m|d|a [option] [-f filename]\n");
	return 1;
}

int save_movie   (Context* context, FILE* fp, char* option, int numOption);
int save_director(Context* context, FILE* fp, char* option, int numOption);
int save_actor   (Context* context, FILE* fp, char* option, int numOption);

int save(Context* context, int cmdCnt, char* cmdArg[]) {
	int i, j, result = 0;
	int numOption = 0, numArg = 0;
	char *option=(char *)malloc(sizeof(char)*100);
	int filenameFlag = 0;
	char *filename=(char *)malloc(sizeof(char)*100);
	FILE* fp;

	// save하기 위한 매개 변수
	char *strSave=(char *)malloc(sizeof(char)*1000);

	for (i=1; i<cmdCnt; i++) {
		if (*(*(cmdArg+i)+0) == '-') {
			// 옵션 설정
			for (j=1; *(*(cmdArg+i)+j) != '\0'; j++) {
				if (*(*(cmdArg+i)+j) == 'f') {
					filenameFlag = 1;
				} else {
					*(option+(numOption)) = *(*(cmdArg+i)+j);
					numOption++;
				}
			}
		} else {

			// save하기 위한 매개 변수
			if (filenameFlag == 1) {
				sprintf(filename, "%s", *(cmdArg+i));
				filenameFlag = 2;
			} else {
				sprintf(strSave, "%s", *(cmdArg+i));
				numArg++;
			}
		}
	}

	if (numArg < 1) {
		return save_usage();
	}
	// list 저장하기 위함
	switch(*(strSave+0)) {
	case 'm':
		// 파일 안들어가지면, default name 사용
		if (filenameFlag == 0) {
			sprintf(filename, "%s", FILE_MOVIE_LIST);
		}

		// 파일 존재하면, 현 시간부로 rename
		if (is_file_existed(filename)) rename_file(filename);

		// 파일 열기, 저장하기
		fp = fopen(filename, "w");
		result = save_movie   (context, fp, option, numOption);
		fclose(fp);
		break;

	case 'd':
		// 파일 안들어가지면, default name 사용
		if (filenameFlag == 0) {
			sprintf(filename, "%s", FILE_DIRECTOR_LIST);
		}

		// 파일 존재하면, 현 시간부로 rename
		if (is_file_existed(filename)) rename_file(filename);

		// 파일 열기, 저장하기
		fp = fopen(filename, "w");
		result = save_director(context, fp, option, numOption);
		fclose(fp);
		break;

	case 'a':
		// 파일 안들어가지면, default name 사용
		if (filenameFlag == 0) {
			sprintf(filename, "%s", FILE_ACTOR_LIST);
		}

		// 파일 존재하면, 현 시간부로 rename
		if (is_file_existed(filename)) rename_file(filename);

		// 파일 열기, 저장하기
		fp = fopen(filename, "w");
		result = save_actor   (context, fp, option, numOption);
		fclose(fp);
		break;

	default:
		result = save_usage();
		break;
	}

	return 0;
}

int save_movie   (Context* context, FILE* fp, char* option, int numOption) {
	Movie* m_ptr = context->movie;
	LinkedList* l_ptr = NULL;
	char t = 0, g = 0, d = 0, y = 0, r = 0, a = 0;
	char *line=(char *)malloc(sizeof(1000));
	char* cbuf;
	if (numOption == 0) {
		t = g = d = y = r = a = 1;
	} else {
		if (is_option_existed(option, numOption, 't')) t = 1;
		if (is_option_existed(option, numOption, 'g')) g = 1;
		if (is_option_existed(option, numOption, 'd')) d = 1;
		if (is_option_existed(option, numOption, 'y')) y = 1;
		if (is_option_existed(option, numOption, 'r')) r = 1;
		if (is_option_existed(option, numOption, 'a')) a = 1;
	}

	while (m_ptr != NULL) {
		sprintf(line, "%d", m_ptr->serial_number);
		if (t) {
			cbuf = change_colon(m_ptr->title); //콜론 바꿔서 line에 넣기
			sprintf(line, "%s:%s", line, cbuf);
			free(cbuf);
		}
		if (g) sprintf(line, "%s:%s", line, m_ptr->genre);
		if (d) sprintf(line, "%s:%s", line, m_ptr->director->value);
		if (y) sprintf(line, "%s:%d", line, m_ptr->year);
		if (r) sprintf(line, "%s:%d", line, m_ptr->time);
		if (a) {
			char first = 0;
			l_ptr = m_ptr->actor;
			while (l_ptr != NULL) {
				if (!(first++)) 
					sprintf(line, "%s:%s", line, l_ptr->value);
				else            
					sprintf(line, "%s,%s", line, l_ptr->value);
				l_ptr = l_ptr->next;
			}
		}
		fprintf(fp, "%s\n", line);
		m_ptr = m_ptr->next;
	}
	return 0;
}

int save_director(Context* context, FILE* fp, char* option, int numOption) {
	Director* d_ptr = context->director;
	LinkedList* l_ptr = NULL;
	char n = 0, s = 0, b = 0, m = 0;
	char *line=(char *)malloc(sizeof(char)*1000);
	if (numOption == 0) {
		n = s = b = m = 1;
	} else {
		if (is_option_existed(option, numOption, 'n')) n = 1;
		if (is_option_existed(option, numOption, 's')) s = 1;
		if (is_option_existed(option, numOption, 'b')) b = 1;
		if (is_option_existed(option, numOption, 'm')) m = 1;
	}

	while (d_ptr != NULL) {
		sprintf(line, "%d", d_ptr->serial_number);
		if (n) sprintf(line, "%s:%s", line, d_ptr->name);
		if (s) sprintf(line, "%s:%c", line, d_ptr->sex);
		if (b) sprintf(line, "%s:%s", line, d_ptr->birth);
		if (m) {
			char first = 0;
			l_ptr = d_ptr->title;
			while (l_ptr != NULL) {
				if (!(first++)) sprintf(line, "%s:%s", line, l_ptr->value);
				else            sprintf(line, "%s,%s", line, l_ptr->value);
				l_ptr = l_ptr->next;
			}
		}
		fprintf(fp, "%s\n", line);
		d_ptr = d_ptr->next;
	}
	return 0;
}

int save_actor   (Context* context, FILE* fp, char* option, int numOption) {
	Actor* a_ptr = context->actor;
	LinkedList* l_ptr = NULL;
	char n = 0, s = 0, b = 0, m = 0;
	char *line=(char *)malloc(sizeof(char)*1000);
	if (numOption == 0) {
		n = s = b = m = 1;
	} else {
		if (is_option_existed(option, numOption, 'n')) n = 1;
		if (is_option_existed(option, numOption, 's')) s = 1;
		if (is_option_existed(option, numOption, 'b')) b = 1;
		if (is_option_existed(option, numOption, 'm')) m = 1;
	}

	while (a_ptr != NULL) {
		sprintf(line, "%d", a_ptr->serial_number);
		if (n) sprintf(line, "%s:%s", line, a_ptr->name);
		if (s) sprintf(line, "%s:%c", line, a_ptr->sex);
		if (b) sprintf(line, "%s:%s", line, a_ptr->birth);
		if (m) {
			char first = 0;
			l_ptr = a_ptr->title;
			while (l_ptr != NULL) {
				if (!(first++)) sprintf(line, "%s:%s", line, l_ptr->value);
				else            sprintf(line, "%s,%s", line, l_ptr->value);
				l_ptr = l_ptr->next;
			}
		}
		fprintf(fp, "%s\n", line);
		a_ptr = a_ptr->next;
	}
	return 0;
}
