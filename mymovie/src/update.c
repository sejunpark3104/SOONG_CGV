#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

int update_usage() { //사용법
	printf("update m|d|a option serial_number\n");
	return 1;
}

int update_movie   (Context* context, FILE* fp,
		char* option, int numOption, int numToUpdate);
int update_director(Context* context, FILE* fp,
		char* option, int numOption, int numToUpdate);
int update_actor   (Context* context, FILE* fp,
		char* option, int numOption, int numToUpdate);

int update(Context* context, int cmdCnt, char* cmdArg[]) {
	int i, j, result = 0;
	int numOption = 0, numArg = 0;
	char *option=(char *)malloc(sizeof(int)*100);
	FILE* fp;

	// 업데이트할 요소
	char classToUpdate;
	int numToUpdate;

	for (i=1; i<cmdCnt; i++) {
		numArg++;

		// m|d|a 중에서 선택하기
		if (numArg == 1) {
			classToUpdate = *(*(cmdArg+i)+0); // 업데이트 할 항목 저장
		}

		// 두 번째 옵션 선택하기
		if (numArg == 2) {
			sprintf(option, "%s", *(cmdArg+i));
			while (*(option+(numOption)) != '\0') numOption++;
		}

		// 업데이트 할 시리얼 넘버 선택하기
		else if (numArg == 3) {
			numToUpdate = atoi(*(cmdArg+i));
		}
	}

	// 명령어 재입력
	if (numArg < 3) {
		return update_usage();
	}

	// 업데이트 
	switch(classToUpdate) {
	case 'm':
		fp = fopen(FILE_MOVIE_LOG,   "a");
		result = update_movie   (context, fp, option, numOption, numToUpdate);

		fclose(fp);
		break;
	case 'd':
		fp = fopen(FILE_DIRECTOR_LOG, "a");
		result = update_director(context, fp, option, numOption, numToUpdate);
		fclose(fp);
		break;
	case 'a':
		fp = fopen(FILE_ACTOR_LOG,    "a");
		result = update_actor   (context, fp, option, numOption, numToUpdate);
		fclose(fp);
		break;
	default:
		result = update_usage();
		break;
	}
	return result;
}

int update_movie   (Context* context, FILE* fp,
		char* option, int numOption, int numToUpdate) {
	Movie* m_ptr = context->movie;
	Movie* mm_ptr;
	Director* d_ptr;
	Actor* a_ptr;
	LinkedList* l_ptr = NULL;
	char t = 0, g = 0, d = 0, y = 0, r = 0, a = 0;
	char *line=(char *)malloc(sizeof(char)*1000);
	char *buf=(char *)malloc(sizeof(char)*1000);
	char *an_actor=(char *)malloc(sizeof(char)*100);
	char* cbuf;
	int i, j;

	while (m_ptr != NULL) {
		if (m_ptr->serial_number == numToUpdate) {

			// 로그파일에 들어갈 내용 저장
			sprintf(line, "update:%d", numToUpdate);

			// 옵션 설정하기
			if (numOption == 0) {
				t = g = d = y = r = a = 1;
			} else { // 옵션 확인하기
				if (is_option_existed(option, numOption, 't')) t = 1;
				if (is_option_existed(option, numOption, 'g')) g = 1;
				if (is_option_existed(option, numOption, 'd')) d = 1;
				if (is_option_existed(option, numOption, 'y')) y = 1;
				if (is_option_existed(option, numOption, 'r')) r = 1;
				if (is_option_existed(option, numOption, 'a')) a = 1;
			}

			// 업데이트하기
			if (t) {
				// 새로운 제목 설정
				printf("  title > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 중복되는지 확인
				mm_ptr = context->movie;
				while (mm_ptr != NULL) {
					if (!strcmp(buf, mm_ptr->title)) { 
						char ch;
						printf("    %s is already exists. (Serial Number: %d)\n",
								buf, mm_ptr->serial_number);
						printf("    Are you sure to update? (y/N) ");
						scanf("%c", &ch);  flushing();
						if (ch != 'y' && ch != 'Y') {
							return 1;
						}
					}
					mm_ptr = mm_ptr -> next;
				}

				// 감독, 배우와 관련된 노드 삭제하기
				// 새로운 노드 연결
				d_ptr = context->director;
				while (d_ptr != NULL) {
					l_ptr = d_ptr->title;
					while (l_ptr != NULL) {
						if (l_ptr->ref == m_ptr)    // 시리얼 넘버가 같으면 비워주기 
							l_ptr->ref = NULL;
						if (!strcmp(l_ptr->value, buf))   // 제목이 같으면  연결해주기
							l_ptr->ref = m_ptr;
						l_ptr = l_ptr->next;
					}
					d_ptr = d_ptr->next;
				}

				a_ptr = context->actor;
				while (a_ptr != NULL) {
					l_ptr = a_ptr->title;
					while (l_ptr != NULL) {
						if (l_ptr->ref == m_ptr)   // 시리얼 넘버가 같으면 비워주기     
							l_ptr->ref = NULL;
						if (!strcmp(l_ptr->value, buf))  // 제목이 같으면 연결해주기
							l_ptr->ref = m_ptr;
						l_ptr = l_ptr->next;
					}
					a_ptr = a_ptr->next;
				}

				// 새로운 제목 설정
				sprintf(m_ptr->title, "%s", buf);

				// 로그파일에 들어갈 내용 저장
				cbuf = change_colon(buf);
				sprintf(line, "%s:%s", line, cbuf);
				free(cbuf);

			} else { // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (g) {
				// 새로운 장르 입력
				printf("  genre > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 저장
				sprintf(m_ptr->genre, "%s", buf);

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else { // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (d) {
				// 새로운 감독 입력
				printf("  director > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 이전꺼 삭제
				deleteList(&(m_ptr->director));

				// 노드 추가(새로운 감독 설정)
				addNode(&(m_ptr->director), buf);

				// 링크연결
				m_ptr->director->ref = NULL;
				d_ptr = context->director;
				while (d_ptr != NULL) {
					if (!strcmp(d_ptr->name, buf)) {  // 감독이름 연결
						m_ptr->director->ref = d_ptr;
						break;
					}
					d_ptr = d_ptr->next;
				}

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {  // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (y) {
				// 년도 입력
				printf("  year > ");  scanf("%d", &(m_ptr->year));  flushing();

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%d", line, m_ptr->year);

			} else {   // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (r) {
				// 러닝타임 입력
				printf("  time > ");  scanf("%d", &(m_ptr->time));  flushing();

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%d", line, m_ptr->time);

			} else {  // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (a) {
				// 배우 입력
				printf("  actors > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 이전꺼 삭제
				deleteList(&(m_ptr->actor));

				// 콤마기준으로 저장
				*(an_actor + 0) = '\0';
				for (i=0, j=0; 1; i++) {
					if (j == 0 && *(buf+i) == ' ') {
						continue;
					} else if (*(buf+i) != ',' && *(buf+i) != '\0') { // 콤마, 개행이 아닌 문자열인 경우 글자 하나씩 an_actor에 배정
						*(an_actor+(j++)) = *(buf+i);
					} else {
						// 배우 이름 저장
						*(an_actor+j) = '\0';

						l_ptr = addNode(&(m_ptr->actor), an_actor);  
						j=0;

						// 나머지 정보들 연결(저장)하기
						a_ptr = context->actor;
						while (a_ptr != NULL) {
							if (!strcmp(an_actor, a_ptr->name)) {  
								l_ptr->ref = (void*) a_ptr;
								break;
							}
							a_ptr = a_ptr->next;
						}
					}

					if (*(buf+i) == '\0') {  // 저장 종료
						break;
					}
				}

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {  //수정 x
				sprintf(line, "%s:=", line);
			}

			// 로그파일에 저장
			fprintf(fp, "%s\n", line);

			printf("  movie %d is successfully updated.\n", numToUpdate);
			return 0;

		} else if (m_ptr->serial_number > numToUpdate) {
			break;

		}
		m_ptr = m_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}

int update_director   (Context* context, FILE* fp,
		char* option, int numOption, int numToUpdate) {
	Director* d_ptr = context->director;
	Director* dd_ptr;
	Movie* m_ptr;
	Actor* a_ptr;
	LinkedList* l_ptr = NULL;
	char n = 0, s = 0, b = 0, m = 0;
	char *line=(char *)malloc(sizeof(char)*1000);
	char *buf=(char *)malloc(sizeof(char)*1000);
	char *a_title=(char *)malloc(sizeof(char)*100);
	int i, j;

	while (d_ptr != NULL) {
		if (d_ptr->serial_number == numToUpdate) {

			// 로그파일에 들어갈 내용 저장
			sprintf(line, "update:%d", numToUpdate);

			// 옵션 설정(선택)
			if (numOption == 0) {
				n = s = b = m = 1;
			} else {
				if (is_option_existed(option, numOption, 'n')) n = 1;
				if (is_option_existed(option, numOption, 's')) s = 1;
				if (is_option_existed(option, numOption, 'b')) b = 1;
				if (is_option_existed(option, numOption, 'm')) m = 1;
			}

			// 항목 업데이트
			if (n) {
				// 새로운 이름 입력
				printf("  name > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 중복되는지 확인
				dd_ptr = context->director;
				while (dd_ptr != NULL) {
					if (!strcmp(buf, dd_ptr->name)) {
						char ch;
						printf("    %s is already exists. (Serial Number: %d)\n",
								buf, dd_ptr->serial_number);
						printf("    Are you sure to update? (y/N) ");
						scanf("%c", &ch);  flushing();
						if (ch != 'y' && ch != 'Y') {
							return 1;
						}
					}
					dd_ptr = dd_ptr -> next;
				}

				// 해당 감독과 관련된 링크(노드) 삭제하기
				// 새로운 노드 연결
				m_ptr = context->movie;
				while (m_ptr != NULL) {
					l_ptr = m_ptr->director;
					while (l_ptr != NULL) {
						if (l_ptr->ref == d_ptr)        
							l_ptr->ref = NULL;
						if (!strcmp(l_ptr->value, buf)) 
							l_ptr->ref = d_ptr;
						l_ptr = l_ptr->next;
					}
					m_ptr = m_ptr->next;
				}

				// 새로운 이름 설정
				sprintf(d_ptr->name, "%s", buf);

				// 로그파일에 들어갈 내용 저장 
				sprintf(line, "%s:%s", line, buf);

			} else {  // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (s) {  
				// 성별 입력
				printf("  sex(M|F) > ");  scanf("%c", &(d_ptr->sex));  flushing();

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {  //수정 x
				sprintf(line, "%s:=", line);
			}

			if (b) {
				// 생일 입력
				printf("  birth > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 생일항목에 저장
				sprintf(d_ptr->birth, "%s", buf);

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {  // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (m) {
				// 영화 제목 입력
				printf("  title > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 이전꺼 삭제
				deleteList(&(d_ptr->title));

				// 콤마기준으로 저장
				*(a_title+0) = '\0';
				for (i=0, j=0; 1; i++) {
					if (j == 0 && *(buf+i) == ' ') {
						continue;
					} else if (*(buf+i) != ',' && *(buf+i) != '\0') {
						*(a_title+(j++)) = *(buf+i);
					} else {
						// 배우 이름 저장
						*(a_title+j) = '\0';
						l_ptr = addNode(&(d_ptr->title), a_title); // 배우 이름 저장
						j=0;

						// 나머지 정보들 저장(연결)하기
						m_ptr = context->movie;
						while (m_ptr != NULL) {
							if (!strcmp(a_title, m_ptr->title)) {
								l_ptr->ref = (void*) m_ptr;
								break;
							}
							m_ptr = m_ptr->next;
						}
					}

					if (*(buf+i) == '\0') {
						break;
					}
				}

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {  // 수정 x
				sprintf(line, "%s:=", line);
			}

			// 로그파일에 저장
			fprintf(fp, "%s\n", line);

			printf("  director %d is successfully updated.\n", numToUpdate);
			return 0;

		} else if (d_ptr->serial_number > numToUpdate) {
			break;

		}
		d_ptr = d_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}

int update_actor   (Context* context, FILE* fp,
		char* option, int numOption, int numToUpdate) {
	Actor* a_ptr = context->actor;
	Actor* aa_ptr;
	Movie* m_ptr;
	Director* d_ptr;
	LinkedList* l_ptr = NULL;
	char n = 0, s = 0, b = 0, m = 0;
	char *line=(char *)malloc(sizeof(char)*1000);
	char *buf=(char *)malloc(sizeof(char)*1000);
	char *a_title=(char *)malloc(sizeof(char)*100);
	int i, j;

	while (a_ptr != NULL) {
		if (a_ptr->serial_number == numToUpdate) {

			// 로그파일에 들어갈 내용 저장
			sprintf(line, "update:%d", numToUpdate);

			// 옵션 설정(선택)하기
			if (numOption == 0) {
				n = s = b = m = 1;
			} else {
				if (is_option_existed(option, numOption, 'n')) 
					n = 1;
				if (is_option_existed(option, numOption, 's')) 
					s = 1;
				if (is_option_existed(option, numOption, 'b')) 
					b = 1;
				if (is_option_existed(option, numOption, 'm')) 
					m = 1;
			}

			// 항목 업데이트
			if (n) {
				// 이름 입력
				printf("  name > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 중복되는지 검사
				aa_ptr = context->actor;
				while (aa_ptr != NULL) {
					if (!strcmp(buf, aa_ptr->name)) {
						char ch;
						printf("    %s is already exists. (Serial Number: %d)\n",
								buf, aa_ptr->serial_number);
						printf("    Are you sure to update? (y/N) ");
						scanf("%c", &ch);  flushing();
						if (ch != 'y' && ch != 'Y') {
							return 1;
						}
					}
					aa_ptr = aa_ptr -> next;
				}

				// 해당 배우와 관련된 링크(노드)삭제
				// 새로운 노드 생성
				m_ptr = context->movie;
				while (m_ptr != NULL) {
					l_ptr = m_ptr->actor;
					while (l_ptr != NULL) {
						if (l_ptr->ref == a_ptr)        
							l_ptr->ref = NULL;
						if (!strcmp(l_ptr->value, buf)) 
							l_ptr->ref = a_ptr;
						l_ptr = l_ptr->next;
					}
					m_ptr = m_ptr->next;
				}

				// 이름 저장
				sprintf(a_ptr->name, "%s", buf);

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {   // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (s) {
				// 성별 입력
				printf("  sex(M|F) > ");  scanf("%c", &(a_ptr->sex));  flushing();

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%c", line, a_ptr->sex);

			} else {  // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (b) {
				// 생일 입력
				printf("  birth > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 저장
				sprintf(a_ptr->birth, "%s", buf);

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {    // 수정 x
				sprintf(line, "%s:=", line);
			}

			if (m) {
				// 영화 제목 입력
				printf("  title > ");  scanf("%[^\t\n]s", buf);  flushing();

				// 이전꺼 삭제
				deleteList(&(a_ptr->title));

				// 콤마기준으로 저장
				*(a_title+0) = '\0';
				for (i=0, j=0; 1; i++) {
					if (j == 0 && *(buf+i) == ' ') {
						continue;
					} else if (*(buf+i) != ',' && *(buf+i) != '\0') {
						*(a_title+(j++)) = *(buf+i);

					} else {
						// 영화 이름 저장
						*(a_title+j) = '\0';
						l_ptr = addNode(&(a_ptr->title), a_title);  
						j=0;

						// 나머지 정보들 저장(연결)하기
						m_ptr = context->movie;
						while (m_ptr != NULL) {
							if (!strcmp(a_title, m_ptr->title)) {
								l_ptr->ref = (void*) m_ptr;
								break;
							}
							m_ptr = m_ptr->next;
						}
					}

					if (*(buf+i) == '\0') {  // 저장 종료
						break;
					}
				}

				// 로그파일에 들어갈 내용 저장
				sprintf(line, "%s:%s", line, buf);

			} else {  // 수정 x
				sprintf(line, "%s:=", line);
			}

			// 로그파일에 저장
			fprintf(fp, "%s\n", line);

			printf("  actor %d is successfully updated.\n", numToUpdate);
			return 0;

		} else if (a_ptr->serial_number > numToUpdate) {
			break;

		}
		a_ptr = a_ptr->next;
	}
	printf("  No such record.\n");
	return 1;
}
