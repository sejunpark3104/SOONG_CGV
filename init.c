#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

/**
 * log file 초기화
 */
int init(Context* context) {
	FILE* fp;
	int   result = 0, isScaned;
	int   i, j;
	char  buffer[500];
	char* cmdArg[2];
	char* cbuf;
	Movie      *m_ptr, *a_movie;
	Director   *d_ptr, *a_director;
	Actor      *a_ptr, *an_actor;
	LinkedList *l_ptr;

	// 로그파일로 부터 리스트 데이터 받아옴

	// 1. Movie
	fp = fopen(FILE_MOVIE_LOG, "r");
	while (fp) {

		// 파일로부터 한 줄 읽음
		isScaned = fscanf(fp, "%[^\n]\n", buffer);

		if (isScaned != -1) {

			//콜론으로 분리함
			cbuf = strtok(buffer, ":");

			if (!strcmp(cbuf, "add")) {

				// 새 영화 추가
				a_movie = (Movie*) malloc(sizeof(Movie));
				a_movie->director = NULL;
				a_movie->actor = NULL;
				a_movie->next = NULL;

				// 영화 데이터 설정
				a_movie->serial_number = atoi(strtok(NULL, ":"));
				cbuf = recover_colon(strtok(NULL, ":"));
				sprintf(a_movie->title, "%s", cbuf); free(cbuf);
				sprintf(a_movie->genre, "%s", strtok(NULL, ":"));
				cbuf = strtok(NULL, ":");
				if (cbuf != '\0')
					addNode(&(a_movie->director), cbuf);
				a_movie->year = atoi(strtok(NULL, ":"));
				a_movie->time = atoi(strtok(NULL, ":"));

				// 배우 설정
				cbuf = strtok(NULL, ":");

				// 콤마로 배우 분리
				buffer[0] = '\0';
				for (i=0, j=0; 1; i++) {
					if (j == 0 && cbuf[i] == ' ') {
						continue;
					} else if (cbuf[i] != ',' && cbuf[i] != '\0') {
						buffer[j++] = cbuf[i];
					} else {
						// 제목 추가
						buffer[j] = '\0';
						addNode(&(a_movie->actor), buffer);
						j=0;
					}

					if (cbuf[i] == '\0') {
						break;
					}
				}

				// 새 영화를 마지막 요소로 링크
				if (context->movie == NULL) {
					context->movie = a_movie;

				} else {
					m_ptr = context->movie;
					while (m_ptr->next != NULL) m_ptr = m_ptr->next;
					m_ptr->next = a_movie;

				}

			} else if (!strcmp(cbuf, "update")) {

				i = atoi(strtok(NULL, ":")); // 업데이트하기 위한 시리얼넘버
				a_movie = context->movie;

				while (a_movie != NULL) {

					if (a_movie->serial_number == i) {

						// title
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							cbuf = recover_colon(cbuf);
							sprintf(a_movie->title, "%s", cbuf);
							free(cbuf);
						}

						// genre
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							sprintf(a_movie->genre, "%s", cbuf);
						}

						// director
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							deleteList(&(a_movie->director));
							addNode(&(a_movie->director), cbuf);
						}

						// year
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							a_movie->year = atoi(cbuf);
						}

						// time
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							a_movie->time = atoi(cbuf);
						}

						// actors
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							deleteList(&(a_movie->actor));

							// 콤마로 배우 분리
							buffer[0] = '\0';
							for (i=0, j=0; 1; i++) {
								if (j == 0 && cbuf[i] == ' ') {
									continue;
								} else if (cbuf[i] != ',' && cbuf[i] != '\0') {
									buffer[j++] = cbuf[i];
								} else {
									// 제목 추가
									buffer[j] = '\0';
									l_ptr = addNode(&(a_movie->actor), buffer);
									j=0;
								}

								if (cbuf[i] == '\0') {
									break;
								}
							}
						}

						break;
					}

					a_movie = a_movie->next;
				}

			} else if (!strcmp(cbuf, "delete")) {

				i = atoi(strtok(NULL, ":")); // delete 위한 시리얼 넘버
				a_movie = context->movie;

				while (a_movie != NULL) {
					if (a_movie->serial_number == i) {
						if (a_movie == context->movie) {
							context->movie = a_movie->next;
							free(a_movie);
						} else {
							m_ptr->next = a_movie->next;
							free(a_movie);
						}
						break;
					}
					m_ptr = a_movie;
					a_movie = a_movie->next;
				}

			}

		} else {
			fclose(fp);
			break;

		}
	}

	// 2. 감독
	fp = fopen(FILE_DIRECTOR_LOG, "r");
	while (fp) {

		// 파일로부터 한 줄 읽음
		isScaned = fscanf(fp, "%[^\n]\n", buffer);

		if (isScaned != -1) {

			// 콜론으로 분리
			cbuf = strtok(buffer, ":");

			if (!strcmp(cbuf, "add")) {

				// 새 감독 추가
				a_director = (Director*) malloc(sizeof(Director));
				a_director->title = NULL;
				a_director->next = NULL;

				// 감독 정보 설정
				a_director->serial_number = atoi(strtok(NULL, ":"));
				sprintf(a_director->name, "%s", strtok(NULL, ":"));
				a_director->sex = (strtok(NULL, ":"))[0];
				sprintf(a_director->birth, "%s", strtok(NULL, ":"));

				// 제목 설정
				cbuf = strtok(NULL, ":");

				// 콤마로 제목 분리
				buffer[0] = '\0';
				for (i=0, j=0; 1; i++) {
					if (j == 0 && cbuf[i] == ' ') {
						continue;
					} else if (cbuf[i] != ',' && cbuf[i] != '\0') {
						buffer[j++] = cbuf[i];
					} else {
						// 제목 추가
						buffer[j] = '\0';
						addNode(&(a_director->title), buffer);
						j=0;
					}

					if (cbuf[i] == '\0') {
						break;
					}
				}

				// 새 감독을 마지막 요소로 링크
				if (context->director == NULL) {
					context->director = a_director;

				} else {
					d_ptr = context->director;
					while (d_ptr->next != NULL) d_ptr = d_ptr->next;
					d_ptr->next = a_director;

				}

			} else if (!strcmp(cbuf, "update")) {

				i = atoi(strtok(NULL, ":")); // update 하기 위한 시리얼넘버
				a_director = context->director;

				while (a_director != NULL) {

					if (a_director->serial_number == i) {

						// name
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							sprintf(a_director->name, "%s", cbuf);
						}

						// sex
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							a_director->sex = cbuf[0];
						}

						// birth
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							sprintf(a_director->birth, "%s", cbuf);
						}

						// titles
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							deleteList(&(a_director->title));

							// 콤마로 분리하여 제목 추가
							buffer[0] = '\0';
							for (i=0, j=0; 1; i++) {
								if (j == 0 && cbuf[i] == ' ') {
									continue;
								} else if (cbuf[i] != ',' && cbuf[i] != '\0') {
									buffer[j++] = cbuf[i];
								} else {
									// 제목 추가
									buffer[j] = '\0';
									addNode(&(a_director->title), buffer);
									j=0;
								}

								if (cbuf[i] == '\0') {
									break;
								}
							}
						}

						break;
					}

					a_director = a_director->next;
				}

			} else if (!strcmp(cbuf, "delete")) {

				i = atoi(strtok(NULL, ":")); // delete 하기 위한 시리얼넘버
				a_director = context->director;

				while (a_director != NULL) {
					if (a_director->serial_number == i) {
						if (a_director == context->director) {
							context->director = a_director->next;
							free(a_director);
						} else {
							d_ptr->next = a_director->next;
							free(a_director);
						}
						break;
					}
					d_ptr = a_director;
					a_director = a_director->next;
				}

			}

		} else {
			fclose(fp);
			break;

		}
	}

	// 3. Actor
	fp = fopen(FILE_ACTOR_LOG, "r");
	while (fp) {

		// 파일로부터 한 줄 읽음
		isScaned = fscanf(fp, "%[^\n]\n", buffer);

		if (isScaned != -1) {

			// 콜론을 분리
			cbuf = strtok(buffer, ":");

			if (!strcmp(cbuf, "add")) {

				// 새 배우 추가
				an_actor = (Actor*) malloc(sizeof(Actor));
				an_actor->title = NULL;
				an_actor->next = NULL;

				// 배우 정보 설정
				an_actor->serial_number = atoi(strtok(NULL, ":"));
				sprintf(an_actor->name, "%s", strtok(NULL, ":"));
				an_actor->sex = (strtok(NULL, ":"))[0];
				sprintf(an_actor->birth, "%s", strtok(NULL, ":"));

				// 제목 설정
				cbuf = strtok(NULL, ":");

				// 콤마로 분리하여 제목 추가
				buffer[0] = '\0';
				for (i=0, j=0; 1; i++) {
					if (j == 0 && cbuf[i] == ' ') {
						continue;
					} else if (cbuf[i] != ',' && cbuf[i] != '\0') {
						buffer[j++] = cbuf[i];
					} else {
						// 제목 추가
						buffer[j] = '\0';
						addNode(&(an_actor->title), buffer);
						j=0;
					}

					if (cbuf[i] == '\0') {
						break;
					}
				}

				// 새 배우 마지막 요소로 링크
				if (context->actor == NULL) {
					context->actor = an_actor;

				} else {
					a_ptr = context->actor;
					while (a_ptr->next != NULL) a_ptr = a_ptr->next;
					a_ptr->next = an_actor;

				}
			} else if (!strcmp(cbuf, "update")) {

				i = atoi(strtok(NULL, ":")); // update 하기 위한 시리얼넘버
				an_actor = context->actor;

				while (an_actor != NULL) {

					if (an_actor->serial_number == i) {

						// name
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							sprintf(an_actor->name, "%s", cbuf);
						}

						// sex
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							an_actor->sex = cbuf[0];
						}

						// birth
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							sprintf(an_actor->birth, "%s", cbuf);
						}

						// titles
						cbuf = strtok(NULL, ":");
						if (cbuf[0] != '=') {
							deleteList(&(an_actor->title));

							// 콤마로 분리하여 제목 추가
							buffer[0] = '\0';
							for (i=0, j=0; 1; i++) {
								if (j == 0 && cbuf[i] == ' ') {
									continue;
								} else if (cbuf[i] != ',' && cbuf[i] != '\0') {
									buffer[j++] = cbuf[i];
								} else {
									// 제목 추가
									buffer[j] = '\0';
									addNode(&(an_actor->title), buffer);
									j=0;
								}

								if (cbuf[i] == '\0') {
									break;
								}
							}
						}

						break;
					}

					an_actor = an_actor->next;
				}

			} else if (!strcmp(cbuf, "delete")) {

				i = atoi(strtok(NULL, ":")); // delete 하기 위한 시리얼넘버
				an_actor = context->actor;

				while (an_actor != NULL) {
					if (an_actor->serial_number == i) {
						if (an_actor == context->actor) {
							context->actor = an_actor->next;
							free(an_actor);
						} else {
							a_ptr->next = an_actor->next;
							free(an_actor);
						}
						break;
					}
					a_ptr = an_actor;
					an_actor = an_actor->next;
				}

			}

		} else {
			fclose(fp);
			break;

		}
	}

	// movie|director|actor node들이 서로 링크하게 
	// 1) Movie
	m_ptr = context->movie;
	while (m_ptr != NULL) {

		// 감독과 링크
		l_ptr = m_ptr->director;
		while (l_ptr != NULL) {
			d_ptr = context->director;
			while (d_ptr != NULL) {
				if (!strcmp(l_ptr->value, d_ptr->name)) {
					l_ptr->ref = d_ptr;
					break;
				}
				d_ptr = d_ptr->next;
			}
			l_ptr = l_ptr->next;
		}

		// 배우와 링크
		l_ptr = m_ptr->actor;
		while (l_ptr != NULL) {
			a_ptr = context->actor;
			while (a_ptr != NULL) {
				if (!strcmp(l_ptr->value, a_ptr->name)) {
					l_ptr->ref = a_ptr;
					break;
				}
				a_ptr = a_ptr->next;
			}
			l_ptr = l_ptr->next;
		}

		m_ptr = m_ptr->next;
	}

	// 2) Director
	d_ptr = context->director;
	while (d_ptr != NULL) {

		// 영화와 링크
		l_ptr = d_ptr->title;
		while (l_ptr != NULL) {
			m_ptr = context->movie;
			while (m_ptr != NULL) {
				if (!strcmp(l_ptr->value, m_ptr->title)) {
					l_ptr->ref = m_ptr;
					break;
				}
				m_ptr = m_ptr->next;
			}
			l_ptr = l_ptr->next;
		}

		d_ptr = d_ptr->next;
	}

	// 3) Actor
	a_ptr = context->actor;
	while (a_ptr != NULL) {

		// 영화와 링크
		l_ptr = a_ptr->title;
		while (l_ptr != NULL) {
			m_ptr = context->movie;
			while (m_ptr != NULL) {
				if (!strcmp(l_ptr->value, m_ptr->title)) {
					l_ptr->ref = m_ptr;
					break;
				}
				m_ptr = m_ptr->next;
			}
			l_ptr = l_ptr->next;
		}

		a_ptr = a_ptr->next;
	}

	// 모든 리스트들을 파일에 적음
	cmdArg[1] = buffer;
	buffer[1] = '\0';
	buffer[0] = 'm';  save(context, 2, cmdArg);
	buffer[0] = 'd';  save(context, 2, cmdArg);
	buffer[0] = 'a';  save(context, 2, cmdArg);

	return result;
}
