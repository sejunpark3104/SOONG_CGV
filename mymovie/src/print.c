#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

int print_usage() {
	printf("print m|d|a serial_number\n");
	return 1;
}

int print_movie    (Context* context, int numToSearch);
int print_director (Context* context, int numToSearch);
int print_actor    (Context* context, int numToSearch);

int print(Context* context, int cmdCnt, char* cmdArg[]) {
	int i, j, result = 0;
	int numOption = 0, numArg = 0;
	char *option=(char *)malloc(sizeof(char)*100);

	// print 하기 위한 매개 변수
	char *strPrint=(char *)malloc(sizeof(char)*1000);
	int numToSearch;

	for (i=1; i<cmdCnt; i++) {
		if (*(*(cmdArg+i)+0) == '-') {
			// 옵션 설정
			for (j=1; *(*(cmdArg+i)+j) != '\0'; j++) {
				option[numOption] = *(*(cmdArg+i)+j);
				numOption++;
			}
		} else {
			numArg++;

			// 클래스(m|d|a) 설정
			if (numArg == 1) {
				sprintf(strPrint, "%s", *(cmdArg+i));
			}

			// 프린트하기 위한 number 설정
			else if (numArg == 2) {
				numToSearch = atoi(*(cmdArg+i));
			}
		}
	}

	if (numArg < 2) {
		return print_usage();
	}

	// 옵션에 따른 분기
	switch(*(strPrint+0)) {
	case 'm':
		result = print_movie   (context, numToSearch);
		break;
	case 'd':
		result = print_director(context, numToSearch);
		break;
	case 'a':
		result = print_actor   (context, numToSearch);
		break;
	default:
		result = print_usage();
		break;
	}
	return result;
}

int print_movie    (Context* context, int numToSearch) {
	Movie* m_ptr = context->movie;
	Director* d_ptr;
	Actor* a_ptr;
	LinkedList* l_ptr;
	LinkedList* l2_ptr;

	int aIdx = 0;
	while (m_ptr != NULL) {
		if (m_ptr->serial_number == numToSearch) {

			// Print serial_number, title, genre, (year, time)
			printf("%d, %s, %s (%d, %d)\n",
					m_ptr->serial_number, m_ptr->title, m_ptr->genre,
					m_ptr->year, m_ptr->time);

			// Print director
			l_ptr = m_ptr->director;
			printf("D : %s", l_ptr->value);

			// 감독 정보 출력
			if (l_ptr->ref != NULL) {
				printf("(%s)\n", ((Director*)l_ptr->ref)->birth);
			} else {
				printf("(-)\n");
			}

			// 영화의 감독리스트에 없는 감독명 프린트
			// add m 했을 때 넣은 director만 나오는 것이 아니라,
			// add d 했을 때 넣은 영화와 이름이 같으면 출력하기 위함
			d_ptr = context->director;
			while (d_ptr != NULL) {
				int flag = 0;

				l_ptr = d_ptr->title;
				while (l_ptr != NULL) {
					if (!strcmp(l_ptr->value, m_ptr->title)) {
						flag = 1;

						l2_ptr = m_ptr->actor;
						while (l2_ptr != NULL) {
							if (!strcmp(d_ptr->name, l2_ptr->value)) {
								flag = 0;
								break;
							}
							l2_ptr = l2_ptr->next;
						}

						if (flag) {

							// 감독이름 출력
							printf("D : %s(%s)\n", d_ptr->name, d_ptr->birth);

							break;
						}
					}
					l_ptr = l_ptr->next;
				}

				d_ptr = d_ptr->next;
			}

			// 배우들 출력
			l_ptr = m_ptr->actor;
			while (l_ptr != NULL) {

				// 배우 이름 출력
				printf("A%d : %s", ++aIdx, l_ptr->value);

				// 배우 세부정보 출력
				if (l_ptr->ref != NULL) {
					printf("(%s)\n", ((Actor*)l_ptr->ref)->birth);
				} else {
					printf("(-)\n");
				}

				l_ptr = l_ptr->next;
			}

			// 영화 배우리스트에 없는 배우 출력
			a_ptr = context->actor;
			while (a_ptr != NULL) {
				int flag = 0;

				l_ptr = a_ptr->title;
				while (l_ptr != NULL) {
					if (!strcmp(l_ptr->value, m_ptr->title)) {
						flag = 1;

						l2_ptr = m_ptr->actor;
						while (l2_ptr != NULL) {
							if (!strcmp(a_ptr->name, l2_ptr->value)) {
								flag = 0;
								break;
							}
							l2_ptr = l2_ptr->next;
						}

						if (flag) {

							// 배우 이름 출력
							printf("A%d : %s(%s)\n", ++aIdx, a_ptr->name, a_ptr->birth);

							break;
						}
					}
					l_ptr = l_ptr->next;
				}

				a_ptr = a_ptr->next;
			}

			return 0;

		} else if (m_ptr->serial_number > numToSearch) {
			break;

		}
		m_ptr = m_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}

int print_director (Context* context, int numToSearch) {
	Director* d_ptr = context->director;
	Movie* m_ptr;
	LinkedList* l_ptr;
	LinkedList* l2_ptr;
	while (d_ptr != NULL) {
		if (d_ptr->serial_number == numToSearch) {

			// Print serial_number, name, sex, birth
			printf("%d, %s, %c, %s\n",
					d_ptr->serial_number, d_ptr->name, d_ptr->sex, d_ptr->birth);

			// Print movies
			l_ptr = d_ptr->title;
			while (l_ptr != NULL) {

				// Print title of this movie
				printf("%s, ", l_ptr->value);

				// 영화 세부정보 출력
				if (l_ptr->ref != NULL) {
					printf("%d, %d\n",
							((Movie*)l_ptr->ref)->year, ((Movie*)l_ptr->ref)->time);
				} else {
					printf("-, -\n");
				}
				l_ptr = l_ptr->next;
			}

			// 감독의 대표작 리스트에 없는 영화 출력
			m_ptr = context->movie;
			while (m_ptr != NULL) {
				int flag = 0;

				l_ptr = m_ptr->director;
				if (l_ptr != NULL) {
					if (!strcmp(l_ptr->value, d_ptr->name)) {
						flag = 1;

						l2_ptr = d_ptr->title;
						while (l2_ptr != NULL) {
							if (!strcmp(m_ptr->title, l2_ptr->value)) {
								flag = 0;
								break;
							}
							l2_ptr = l2_ptr->next;
						}

						if (flag) {

							// 이 영화 정보 출력
							printf("%s, %d, %d\n", m_ptr->title, m_ptr->year, m_ptr->time);

						}
					}
				}

				m_ptr = m_ptr->next;
			}

			return 0;

		} else if (d_ptr->serial_number > numToSearch) {
			break;

		}
		d_ptr = d_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}

int print_actor    (Context* context, int numToSearch) {
	Actor* a_ptr = context->actor;
	Movie* m_ptr;
	LinkedList* l_ptr;
	LinkedList* l2_ptr;
	while (a_ptr != NULL) {
		if (a_ptr->serial_number == numToSearch) {

			// Print serial_number, name, sex, birth
			printf("%d, %s, %c, %s\n",
					a_ptr->serial_number, a_ptr->name, a_ptr->sex, a_ptr->birth);

			// Print movies
			l_ptr = a_ptr->title;
			while (l_ptr != NULL) {

				// Print 영화 제목
				printf("%s, ", l_ptr->value);

				// 대표작 정보 출력
				if (l_ptr->ref != NULL) {
					printf("%d, %d\n",
							((Movie*)l_ptr->ref)->year, ((Movie*)l_ptr->ref)->time);
				} else {
					printf("-, -\n");
				}
				l_ptr = l_ptr->next;
			}

			// 배우 대표작 리스트에 없는 영화 출력
			m_ptr = context->movie;
			while (m_ptr != NULL) {
				int flag = 0;

				l_ptr = m_ptr->actor;
				while (l_ptr != NULL) {
					if (!strcmp(l_ptr->value, a_ptr->name)) {
						flag = 1;

						l2_ptr = a_ptr->title;
						while (l2_ptr != NULL) {
							if (!strcmp(m_ptr->title, l2_ptr->value)) {
								flag = 0;
								break;
							}
							l2_ptr = l2_ptr->next;
						}

						if (flag) {

							// 이 영화 정보 출력
							printf("%s, %d, %d\n", m_ptr->title, m_ptr->year, m_ptr->time);

							break;
						}
					}
					l_ptr = l_ptr->next;
				}

				m_ptr = m_ptr->next;
			}

			return 0;

		} else if (a_ptr->serial_number > numToSearch) {
			break;

		}
		a_ptr = a_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}
