#include <stdio.h>
#include <stdlib.h>
#include "movie.h"

int sort_usage() { //sort 사용법
	printf("sort m|d|a option [-f filename]\n");
	return 1;
}

int sort_movie   (Context* context, FILE* fp, char classToSort, char option);
int sort_director(Context* context, FILE* fp, char classToSort, char option);
int sort_actor   (Context* context, FILE* fp, char classToSort, char option);
int isEarlier(char* a, char* b);

int sort(Context* context, int cmdCnt, char* cmdArg[]) {
	int i, j, result = 0;
	int numArg = 0;
	int filenameFlag = 0;
	char filename[100]={'\0', };
	FILE* fp = NULL;

	// 정렬 위한 매개 변수
	char classToSort;
	char option;

	for (i=1; i<cmdCnt; i++) {
		if (*(*(cmdArg+i)+0) == '-') {
			if (filenameFlag == 0 && *(*(cmdArg+i)+1) == 'f') {
				filenameFlag = 1;
			}
		} else {

			// 파일 이름 설정
			if (filenameFlag == 1) {
				sprintf(filename, "%s", *(cmdArg+i));
				filenameFlag = 2;

			} else {
				numArg++;

				// 정렬하기 위한 클래스 (m|d|a) 설정
				if (numArg == 1) {
					classToSort = *(*(cmdArg+i)+0);

					// 기본 필드를 정렬하도록 설정
					if      (classToSort == 'm') option = 't';
					else if (classToSort == 'd') option = 'n';
					else if (classToSort == 'a') option = 'n';
				}

				// 정렬 위한 필드 설정 (t|g|d|y|r|a|n|s|b|m)
				if (numArg == 2) {
					option = *(*(cmdArg+i)+0);
				}
			}
		}
	}

	if (numArg < 1) {
		return sort_usage();
	}

	if (filenameFlag == 2 && *(filename+0) != '\0') {
		fp = fopen(filename, "w");
	}

	// 옵션에 따른 분기
	switch (classToSort) {
	case 'm':
		result = sort_movie   (context, fp, classToSort, option);
		break;
	case 'd':
		result = sort_director(context, fp, classToSort, option);
		break;
	case 'a':
		result = sort_actor   (context, fp, classToSort, option);
		break;
	default:
		result = sort_usage();
		break;
	}

	if (filenameFlag == 2 && *(filename+0) != '\0') {
		fclose(fp);
	}

	return result;
}

int sort_movie   (Context* context, FILE* fp, char classToSort, char option) {
	int result = 0;
	LinkedList* sort_list = NULL;
	Movie* m_ptr = context->movie;
	LinkedList* l_ptr, * ll_ptr;
	LinkedList* prev_ptr;
	LinkedList* new_node;
	char *line=(char *)malloc(sizeof(char)*500);

	while (m_ptr != NULL) {
		new_node = (LinkedList*) malloc(sizeof(LinkedList));
		new_node->ref = m_ptr;
		new_node->next = NULL;

		// value 설정
		switch (option) {
		case 't': sprintf(new_node->value, "%s",    m_ptr->title);           break;
		case 'g': sprintf(new_node->value, "%s",    m_ptr->genre);           break;
		case 'd': sprintf(new_node->value, "%s",    m_ptr->director->value); break;
		case 'y': sprintf(new_node->value, "%010d", m_ptr->year);            break;
		case 'r': sprintf(new_node->value, "%010d", m_ptr->time);            break;
		case 'a': sprintf(new_node->value, "%s",    m_ptr->actor->value);    break;
		default : printf("Unknonw option: %c\n", option); free(new_node); return 1;
		}

		if (sort_list == NULL) {
			sort_list = new_node;
			sort_list->next = NULL;

		} else {
			prev_ptr = sort_list;
			l_ptr = sort_list;

			// 노드 삽입 위치 찾기
			while (l_ptr != NULL) {
				if (isEarlier(l_ptr->value, new_node->value)) {
					prev_ptr = l_ptr;
					l_ptr = l_ptr->next;
				} else {
					break;
				}
			}

			// 새 노드 삽입
			if (l_ptr == sort_list) {
				new_node->next = sort_list;
				sort_list = new_node;
			} else {
				new_node->next = prev_ptr->next;
				prev_ptr->next = new_node;
			}
		}
		m_ptr = m_ptr->next;
	}

	// 결과 출력
	l_ptr = sort_list;
	while (l_ptr != NULL) {
		m_ptr = (Movie*)l_ptr->ref;

		// 출력하기 위한 데이터 라인 설정
		sprintf(line, "%d:%s:%s:%s:%d:%d:%s",
				m_ptr->serial_number, m_ptr->title, m_ptr->genre,
				m_ptr->director->value, m_ptr->year, m_ptr->time,
				m_ptr->actor->value);

		// 출력하기 위한 배우 라인 설정
		ll_ptr = m_ptr->actor->next;
		while (ll_ptr != NULL) {
			sprintf(line, "%s,%s", line, ll_ptr->value);
			ll_ptr = ll_ptr->next;
		}

		// 표준출력하기 위함
		printf("%s\n", line);

		// 파일에 입력하기 위함
		if (fp != NULL) {
			fprintf(fp, "%s\n", line);
		}

		l_ptr = l_ptr->next;
	}


	deleteList(&sort_list);
	return result;
}

int sort_director(Context* context, FILE* fp, char classToSort, char option) {
	int result = 0;
	LinkedList* sort_list = NULL;
	Director* d_ptr = context->director;
	LinkedList* l_ptr, * ll_ptr;
	LinkedList* prev_ptr;
	LinkedList* new_node;
	char *line=(char *)malloc(sizeof(char)*500);

	while (d_ptr != NULL) {
		new_node = (LinkedList*) malloc(sizeof(LinkedList));
		new_node->ref = d_ptr;
		new_node->next = NULL;

		// value 설정
		switch (option) {
		case 'n': sprintf(new_node->value, "%s", d_ptr->name);               break;
		case 's': sprintf(new_node->value, "%c", d_ptr->sex);                break;
		case 'b': sprintf(new_node->value, "%s", d_ptr->birth);              break;
		case 'm': sprintf(new_node->value, "%s", d_ptr->title->value);       break;
		default : printf("Unknonw option: %c\n", option); free(new_node); return 1;
		}

		if (sort_list == NULL) {
			sort_list = new_node;
			sort_list->next = NULL;

		} else {
			prev_ptr = sort_list;
			l_ptr = sort_list;

			// 노드 삽입 위치 찾기
			while (l_ptr != NULL) {
				if (isEarlier(l_ptr->value, new_node->value)) {
					prev_ptr = l_ptr;
					l_ptr = l_ptr->next;
				} else {
					break;
				}
			}

			// 새 노드 삽입
			if (l_ptr == sort_list) {
				new_node->next = sort_list;
				sort_list = new_node;
			} else {
				new_node->next = prev_ptr->next;
				prev_ptr->next = new_node;
			}
		}
		d_ptr = d_ptr->next;
	}

	// 결과 출력
	l_ptr = sort_list;
	while (l_ptr != NULL) {
		d_ptr = (Director*)l_ptr->ref;

		// 출력하기 위한 데이터 라인 설정
		sprintf(line, "%d:%s:%c:%s:%s",
				d_ptr->serial_number, d_ptr->name, d_ptr->sex,
				d_ptr->birth, d_ptr->title->value);

		// 출력하기 위한 감독 라인 설정
		ll_ptr = d_ptr->title->next;
		while (ll_ptr != NULL) {
			sprintf(line, "%s,%s", line, ll_ptr->value);
			ll_ptr = ll_ptr->next;
		}

		// 표준출력하기 위함
		printf("%s\n", line);

		// 파일 입력하기 위함
		if (fp != NULL) {
			fprintf(fp, "%s\n", line);
		}

		l_ptr = l_ptr->next;
	}

	deleteList(&sort_list);
	return result;
}

int sort_actor   (Context* context, FILE* fp, char classToSort, char option) {
	int result = 0;
	LinkedList* sort_list = NULL;
	Actor* a_ptr = context->actor;
	LinkedList* l_ptr, * ll_ptr;
	LinkedList* prev_ptr;
	LinkedList* new_node;
	char *line=(char *)malloc(sizeof(char)*500);

	while (a_ptr != NULL) {
		new_node = (LinkedList*) malloc(sizeof(LinkedList));
		new_node->ref = a_ptr;
		new_node->next = NULL;

		// value 설정
		switch (option) {
		case 'n': sprintf(new_node->value, "%s", a_ptr->name);               break;
		case 's': sprintf(new_node->value, "%c", a_ptr->sex);                break;
		case 'b': sprintf(new_node->value, "%s", a_ptr->birth);              break;
		case 'm': sprintf(new_node->value, "%s", a_ptr->title->value);       break;
		default : printf("Unknonw option: %c\n", option); free(new_node); return 1;
		}

		if (sort_list == NULL) {
			sort_list = new_node;
			sort_list->next = NULL;

		} else {
			prev_ptr = sort_list;
			l_ptr = sort_list;

			// 노드 삽입 위치 찾기
			while (l_ptr != NULL) {
				if (isEarlier(l_ptr->value, new_node->value)) {
					prev_ptr = l_ptr;
					l_ptr = l_ptr->next;
				} else {
					break;
				}
			}

			// 새 노드 삽입
			if (l_ptr == sort_list) {
				new_node->next = sort_list;
				sort_list = new_node;
			} else {
				new_node->next = prev_ptr->next;
				prev_ptr->next = new_node;
			}
		}
		a_ptr = a_ptr->next;
	}

	// 결과 출력
	l_ptr = sort_list;
	while (l_ptr != NULL) {
		a_ptr = (Actor*)l_ptr->ref;

		// 출력 위한 데이터라인 설정
		sprintf(line, "%d:%s:%c:%s:%s",
				a_ptr->serial_number, a_ptr->name, a_ptr->sex,
				a_ptr->birth, a_ptr->title->value);

		// 출력 위한 배우 데이터라인 설정
		ll_ptr = a_ptr->title->next;
		while (ll_ptr != NULL) {
			sprintf(line, "%s,%s", line, ll_ptr->value);
			ll_ptr = ll_ptr->next;
		}

		// 표준 출력
		printf("%s\n", line);

		// 파일에 입력
		if (fp != NULL) {
			fprintf(fp, "%s\n", line);
		}

		l_ptr = l_ptr->next;
	}

	deleteList(&sort_list);
	return result;
}

//정렬 함수
int isEarlier(char* a, char* b) {
	int i = 0;
	while(*(a+i) == *(b+i) && *(a+i) != '\0') i++;
	return (*(a+i) <= *(b+i))? 1:0;
}
