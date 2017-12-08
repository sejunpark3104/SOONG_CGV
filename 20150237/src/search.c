#include <stdio.h>
#include <fnmatch.h> //패턴 매칭 헤더파일
#include "movie.h"

#define FNMATCH_FLAG 0

int search_usage() { //search 사용법
	printf("search [option] string\n");
	return 1;
}

int search_movie   (Context* context, char* pattern);
int search_director(Context* context, char* pattern);
int search_actor   (Context* context, char* pattern);

int search(Context* context, int cmdCnt, char* cmdArg[]) {
	int i, j;
	int numOption = 0, numArg = 0;
	char option[100];
	int m = 0, d = 0, a = 0;
	int result = 0;

	// search 하기 위한 매개변수
	char strSearch[1000];

	for (i=1; i<cmdCnt; i++) {
		if (cmdArg[i][0] == '-') {
			// 옵션들 설정
			for (j=1; *(cmdArg[i]+j) != '\0'; j++) {
				option[numOption] = *(cmdArg[i]+j);
				numOption++;
			}
		} else {
			numArg++;

			// search하기 위한 매개변수 설정
			if (numArg == 1) {
				sprintf(strSearch, "%s", cmdArg[i]);
			}
		}
	}

	if (numArg < 1) {
		return search_usage();;
	}

	// search 하기 위한 클래스 설정
	if (numOption == 0) {
		m = d = a =  1;
	} else {
		for (i=0; i<numOption; i++) {
			switch (option[i]) {
			case 'm': m = 1; break;
			case 'd': d = 1; break;
			case 'a': a = 1; break;
			}
		}
	}

	// 옵션에 따른 분기
	if (m) result += search_movie   (context, strSearch);
	if (d) result += search_director(context, strSearch);
	if (a) result += search_actor   (context, strSearch);

	if (result == 0) {
		printf("@@ No such record");
	}
	printf("\n");

	return 0;
}

int search_movie   (Context* context, char* pattern) {
	int result = 0;
	char line[500];
	LinkedList* l_ptr;
	Movie* m_ptr = context->movie;

	while (m_ptr != NULL) {
		int bSearched = 0;

		//영화 검색기준: 제목, 해당영화 감독명, 해당영화 배우들 이름
		if (!fnmatch(pattern, m_ptr->title, FNMATCH_FLAG)) bSearched = 1;
		l_ptr = m_ptr->director;
		while (!bSearched && l_ptr != NULL) {
			if (!fnmatch(pattern, l_ptr->value, FNMATCH_FLAG)) bSearched = 1;
			l_ptr = l_ptr->next;
		}
		l_ptr = m_ptr->actor;
		while (!bSearched && l_ptr != NULL) {
			if (!fnmatch(pattern, l_ptr->value, FNMATCH_FLAG)) bSearched = 1;
			l_ptr = l_ptr->next;
		}

		//value==패턴이면 출력
		if (bSearched) {

			// 출력 위한 데이터라인 설정
			sprintf(line, "%d:%s:%s:%s:%d:%d:%s",
					m_ptr->serial_number, m_ptr->title, m_ptr->genre,
					m_ptr->director->value, m_ptr->year, m_ptr->time,
					m_ptr->actor->value);

			// 출력 위한 배우 라인 설정
			l_ptr = m_ptr->actor->next;
			while (l_ptr != NULL) {
				sprintf(line, "%s,%s", line, l_ptr->value);
				l_ptr = l_ptr->next;
			}

			//표준 출력
			printf("%s\n", line);

			result++;
		}
		m_ptr = m_ptr->next;
	}

	return result;
}

int search_director(Context* context, char* pattern) {
	int result = 0;
	char line[500];
	char searchTarget[500];
	LinkedList* l_ptr;
	Director* d_ptr = context->director;

	while (d_ptr != NULL) {
		int bSearched = 0;

		//영화 검색기준: 제목, 해당영화 감독명, 해당영화 배우들 이름
		if (!fnmatch(pattern, d_ptr->name, FNMATCH_FLAG)) bSearched = 1;
		l_ptr = d_ptr->title;
		while (!bSearched && l_ptr != NULL) {
			if (!fnmatch(pattern, l_ptr->value, FNMATCH_FLAG)) bSearched = 1;
			l_ptr = l_ptr->next;
		}

		//value==패턴이면 출력
		if (bSearched) {

			// 출력 위한 데이터라인 설정
			sprintf(line, "%d:%s:%c:%s:%s",
					d_ptr->serial_number, d_ptr->name, d_ptr->sex,
					d_ptr->birth, d_ptr->title->value);

			// 출력 위한 감독 라인 설정
			l_ptr = d_ptr->title->next;
			while (l_ptr != NULL) {
				sprintf(line, "%s,%s", line, l_ptr->value);
				l_ptr = l_ptr->next;
			}

			//표준 출력
			printf("%s\n", line);

			result++;
		}
		d_ptr = d_ptr->next;
	}

	return result;
}

int search_actor   (Context* context, char* pattern) {
	int result = 0;
	char line[500];
	char searchTarget[500];
	LinkedList* l_ptr;
	Actor* a_ptr = context->actor;

	while (a_ptr != NULL) {
		int bSearched = 0;

		//영화 검색기준: 제목, 해당영화 감독명, 해당영화 배우들 이름
		if (!fnmatch(pattern, a_ptr->name, FNMATCH_FLAG)) bSearched = 1;
		l_ptr = a_ptr->title;
		while (!bSearched && l_ptr != NULL) {
			if (!fnmatch(pattern, l_ptr->value, FNMATCH_FLAG)) bSearched = 1;
			l_ptr = l_ptr->next;
		}

		//value==패턴이면 출력
		if (bSearched) {

			// 출력 위한 데이터라인 설정
			sprintf(line, "%d:%s:%c:%s:%s",
					a_ptr->serial_number, a_ptr->name, a_ptr->sex,
					a_ptr->birth, a_ptr->title->value);

			// 출력 위한 배우 라인 설정
			l_ptr = a_ptr->title->next;
			while (l_ptr != NULL) {
				sprintf(line, "%s,%s", line, l_ptr->value);
				l_ptr = l_ptr->next;
			}

			//표준 출력
			printf("%s\n", line);

			result++;
		}
		a_ptr = a_ptr->next;
	}

	return result;
}
