#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

int delete_usage() { // delete 사용법
	printf("delete m|d|a serial_number\n");
	return 1;
}

int delete_movie    (Context* context, FILE* fp, int numToDelete);
int delete_director (Context* context, FILE* fp, int numToDelete);
int delete_actor    (Context* context, FILE* fp, int numToDelete);

int delete(Context* context, int cmdCnt, char* cmdArg[]) {
	int i, j, result = 0;
	int numOption = 0, numArg = 0;
	char *option=(char *)malloc(sizeof(char)*100);
	FILE* fp;

	// delete 위한 매개변수
	char *strDelete=(char *)malloc(sizeof(char)*1000);
	int numToDelete;

	for (i=1; i<cmdCnt; i++) {
		if (*(*(cmdArg+i)+0) == '-') {
			// set option
			for (j=1; *(*(cmdArg+i)+j) != '\0'; j++) {
				*(option+(numOption)) = *(*(cmdArg+i)+j);
				numOption++;
			}
		} else {
			numArg++;

			// 삭제하기 위해 Set class(m|d|a)
			if (numArg == 1) {
				sprintf(strDelete, "%s", *(cmdArg+i));
			}

			// 삭제하기 위해 Set number
			else if (numArg == 2) {
				numToDelete = atoi(*(cmdArg+i));
			}
		}
	}

	if (numArg < 2) {
		return delete_usage();
	}

	//옵션에 따른 삭제 지점 설정
	switch(*(strDelete+0)) {
	case 'm':
		fp = fopen(FILE_MOVIE_LOG,    "a");
		result = delete_movie   (context, fp, numToDelete);
		fclose(fp);
		break;
	case 'd':
		fp = fopen(FILE_DIRECTOR_LOG, "a");
		result = delete_director(context, fp, numToDelete);
		fclose(fp);
		break;
	case 'a':
		fp = fopen(FILE_ACTOR_LOG,    "a");
		result = delete_actor   (context, fp, numToDelete);
		fclose(fp);
		break;
	default:
		result = delete_usage();
		break;
	}
	return result;
}

int delete_movie    (Context* context, FILE* fp, int numToDelete) {
	Movie* m_ptr = context->movie;
	Movie* prev_m_ptr = m_ptr;
	LinkedList* l_ptr;
	LinkedList* la_ptr;
	while (m_ptr != NULL) {
		if (m_ptr->serial_number == numToDelete) {

			// 영화와 감독 대표작 링크 삭제
			if (m_ptr->director->ref != NULL) {
				l_ptr = ((Director*)m_ptr->director->ref)->title;
				while (l_ptr != NULL) {
					if (!strcmp(l_ptr->value, m_ptr->title)) {
						l_ptr->ref = NULL;
						break;
					}
					l_ptr = l_ptr->next;
				}
			}

			// 영화의 감독리스트 삭제
			deleteList(&(m_ptr->director));

			// 영화의 배우 & 배우 링크 삭제
			l_ptr = m_ptr->actor;
			while (l_ptr != NULL) {
				if (l_ptr->ref != NULL) {
					la_ptr = ((Actor*)l_ptr->ref)->title;

					// 영화의 데이터에 대해 이 영화의 제목을 찾기
					while (la_ptr != NULL) {
						if (!strcmp(la_ptr->value, m_ptr->title)) {
							la_ptr->ref = NULL;
							break;
						}
						la_ptr = la_ptr->next;
					}
				}
				l_ptr = l_ptr->next;
			}

			// 영화의 배우리스트 삭제
			deleteList(&(m_ptr->actor));

			// 영화 데이터 넘겨주기
			if (m_ptr == context->movie) {
				context->movie = m_ptr->next;
			} else {
				prev_m_ptr->next = m_ptr->next;
			}
			free(m_ptr);

			// Write to log file
			fprintf(fp, "delete:%d:=:=:=:=:=:=\n", numToDelete);

			printf("  movie %d is successfully deleted.\n", numToDelete);
			return 0;

		} else if (m_ptr->serial_number > numToDelete) {
			break;

		}
		prev_m_ptr = m_ptr;
		m_ptr = m_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}

int delete_director (Context* context, FILE* fp, int numToDelete) {
	Director* d_ptr = context->director;
	Director* prev_d_ptr = d_ptr;
	LinkedList* l_ptr;
	LinkedList* lm_ptr;
	while (d_ptr != NULL) {
		if (d_ptr->serial_number == numToDelete) {

			// 영화와 감독 링크 삭제
			l_ptr = d_ptr->title;
			while (l_ptr != NULL) {
				if (l_ptr->ref != NULL) {
					((Movie*)l_ptr->ref)->director->ref = NULL;
				}
				l_ptr = l_ptr->next;
			}

			// 감독의 영화리스트 삭제
			deleteList(&(d_ptr->title));

			// 감독데이터 넘겨주기
			if (d_ptr == context->director) {
				context->director = d_ptr->next;
			} else {
				prev_d_ptr->next = d_ptr->next;
			}
			free(d_ptr);

			// Write to log file
			fprintf(fp, "delete:%d:=:=:=:=:=:=\n", numToDelete);

			printf("  director %d is successfully deleted.\n", numToDelete);
			return 0;

		} else if (d_ptr->serial_number > numToDelete) {
			break;

		}
		prev_d_ptr = d_ptr;
		d_ptr = d_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}

int delete_actor    (Context* context, FILE* fp, int numToDelete) {
	Actor* a_ptr = context->actor;
	Actor* prev_a_ptr = a_ptr;
	LinkedList* l_ptr;
	LinkedList* lm_ptr;
	while (a_ptr != NULL) {
		if (a_ptr->serial_number == numToDelete) {

			// 영화와 배우 링크 삭제
			l_ptr = a_ptr->title;
			while (l_ptr != NULL) {
				if (l_ptr->ref != NULL) {
					lm_ptr = ((Movie*)l_ptr->ref)->actor;
					while (lm_ptr != NULL) {
						if (!strcmp(lm_ptr->value, a_ptr->name)) {
							lm_ptr->ref = NULL;
							break;
						}
					}
				}
				l_ptr = l_ptr->next;
			}

			// 배우의 영화리스트 삭제
			deleteList(&(a_ptr->title));

			// 배우리스트 넘겨주기
			if (a_ptr == context->actor) {
				context->actor = a_ptr->next;
			} else {
				prev_a_ptr->next = a_ptr->next;
			}
			free(a_ptr);

			// Write to log file
			fprintf(fp, "delete:%d:=:=:=:=:=:=\n", numToDelete);

			printf("  actor %d is successfully deleted.\n", numToDelete);
			return 0;

		} else if (a_ptr->serial_number > numToDelete) {
			break;

		}
		prev_a_ptr = a_ptr;
		a_ptr = a_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}
