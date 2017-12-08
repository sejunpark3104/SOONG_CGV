#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

int add_usage() { //add 사용법
	printf("add m|d|a\n");
	return 1;
}
//add 관련 함수 선언
int add_movie   (Context* context, FILE* fp);
int add_director(Context* context, FILE* fp);
int add_actor   (Context* context, FILE* fp);

int add(Context* context, int cmdCnt, char* cmdArg[]) {
	int i, j, result = 0;
	int numOption = 0, numArg = 0;
	char *option=(char *)malloc(sizeof(char)*100);
	FILE* fp;

	//add 하기 위한 매개변수
	char *strSearch=(char *)malloc(sizeof(char)*1000);

	for (i=1; i<cmdCnt; i++) {    // add 명령 입력 받기
		if (*(*(cmdArg+i)+0) == '-') {

			for (j=1; *(*(cmdArg+i)+j) != '\0'; j++) {			  // 옵션 받기
				*(option+(numOption)) = *(*(cmdArg+i)+j);
				numOption++;
			}
		} else {
			numArg++;

			
			if (numArg == 1) {
				sprintf(strSearch, "%s", *(cmdArg+i));
			}
		}
	}

	if (numArg < 1) {
		return add_usage();
	}

	
	switch(*(strSearch+0)) {   // 옵션에 따른 추가 지점 설정
	case 'm':
		fp = fopen(FILE_MOVIE_LOG,    "a");
		result = add_movie(context, fp);
		fclose(fp);
		break;
	case 'd':
		fp = fopen(FILE_DIRECTOR_LOG, "a");
		result = add_director(context, fp);
		fclose(fp);
		break;
	case 'a':
		fp = fopen(FILE_ACTOR_LOG,    "a");
		result = add_actor(context, fp);
		fclose(fp);
		break;
	default:
		result = add_usage();
		break;
	}

	return result;
}

int add_movie(Context* context, FILE* fp) { //영화 정보 추가
	Movie* new_movie;
	Movie** m_ptr;
	Director* d_ptr;
	Actor* a_ptr;
	LinkedList* l_ptr;
	//동적할당 받기
	char *a_director=(char *)malloc(sizeof(char)*100);
	char *actors=(char *)malloc(sizeof(char)*500);
	char *an_actor=(char *)malloc(sizeof(char)*100);
	char* cbuf;
	int i, j;
	//new_movie 비우기
	new_movie = (Movie*) malloc(sizeof(Movie));
	new_movie->serial_number = 0;
	new_movie->director = NULL;
	new_movie->actor = NULL;
	new_movie->next = NULL;

	// add m 이후 입력되는 정보 받기
	printf("  title > ");  scanf("%[^\t\n]s", new_movie->title);  flushing();

	m_ptr = &(context->movie);
	while (*m_ptr != NULL) {   //이미 있는 영화이면 덮어쓸 것인지 결정
		if (!strcmp((*m_ptr)->title, new_movie->title)) {
			char ch;
			printf("    %s is already exists.\n", new_movie->title);
			printf("    Are you sure to add it as new record? (y/N) ");
			scanf("%c", &ch);  flushing();
			if (ch != 'y' && ch != 'Y') {
				free(new_movie);
				return 1;
			}
		}
		new_movie->serial_number = (*m_ptr)->serial_number;
		m_ptr = &((*m_ptr)->next);
	}
	new_movie->serial_number++; //입력된 후 시리얼 넘버 1 추가
	*m_ptr = new_movie;

	// 
	d_ptr = context->director;
	while (d_ptr != NULL) { //입력된 영화의 감독 == 감독 리스트의 감독 ==> 연결
		l_ptr = d_ptr->title;
		while (l_ptr != NULL) {
			if (!strcmp(new_movie->title, l_ptr->value)) {
				l_ptr->ref = new_movie;
				break;
			}
			l_ptr = l_ptr->next;
		}
		d_ptr = d_ptr->next;
	}

	
	a_ptr = context->actor;
	while (a_ptr != NULL) { //입력된 영화의 배우 == 배우 리스트의 배우 ==> 연결
		l_ptr = a_ptr->title;
		while (l_ptr != NULL) {
			if (!strcmp(new_movie->title, l_ptr->value)) {
				l_ptr->ref = new_movie;
				break;
			}
			l_ptr = l_ptr->next;
		}
		a_ptr = a_ptr->next;
	}
	//다른 영화 정보 입력 받기
	printf("  genre > ");     scanf("%[^\t\n]s", new_movie->genre);  flushing();
	printf("  director > ");  scanf("%[^\t\n]s", a_director);        flushing();
	printf("  year > ");      scanf("%d",   &new_movie->year);       flushing();
	printf("  run time > ");  scanf("%d",   &new_movie->time);       flushing();
	printf("  actors > ");    scanf("%[^\t\n]s", actors);            flushing();

	//감독 노드 연결
	l_ptr = addNode(&(new_movie->director), a_director);

	// 존재한다면 감독 정보와 연결
	d_ptr = context->director;
	while (d_ptr != NULL) {
		if (!strcmp(a_director, d_ptr->name)) {
			l_ptr->ref = (void*) d_ptr;
			break;
		}
		d_ptr = d_ptr->next;
	}

	// 배우 콤마로 구분해서 받기
	*(an_actor+0) = '\0';
	for (i=0, j=0; 1; i++) {
		if (j == 0 && *(actors+i) == ' ') {
			continue;
		} else if (*(actors+i) != ',' && *(actors+i) != '\0') {
			*(an_actor+(j++)) = *(actors+i);
		} else {
			// 배우 노드 연결
			*(an_actor+j) = '\0';
			l_ptr = addNode(&(new_movie->actor), an_actor);
			j=0;

			// 존재한다면 배우 정보와 연결
			a_ptr = context->actor;
			while (a_ptr != NULL) {
				if (!strcmp(an_actor, a_ptr->name)) {
					l_ptr->ref = (void*) a_ptr;
					break;
				}
				a_ptr = a_ptr->next;
			}
		}

		if (*(actors+i) == '\0') {
			break;
		}
	}

	//로그파일에 쓰기
	cbuf = change_colon (new_movie->title);
	fprintf(fp, "add:%d:%s:%s:%s:%d:%d:%s\n",
		new_movie->serial_number,
		cbuf,
		new_movie->genre,
		new_movie->director->value,
		new_movie->year,
		new_movie->time,
		actors
	);
	free(cbuf);

	return 0;
}

int add_director(Context* context, FILE* fp) { //감독 정보 추가
	Director* new_director;
	Director** d_ptr;
	Movie* m_ptr;
	LinkedList* l_ptr;

	char *titles=(char *)malloc(sizeof(char)*500);
	char *a_title=(char *)malloc(sizeof(char)*100);
	int i, j;

	new_director = (Director*) malloc(sizeof(Director));
	new_director->serial_number = 0;
	new_director->title = NULL;
	new_director->next = NULL;

	// add d 후 정보 입력 받기
	printf("  name > ");  scanf("%[^\t\n]s", new_director->name);  flushing();

	d_ptr = &(context->director);
	while (*d_ptr != NULL) { // 중복시 덮어쓸 것인지 결정
		if (!strcmp((*d_ptr)->name, new_director->name)) {
			char ch;
			printf("    %s is already exists.\n", new_director->name);
			printf("    Are you sure to add it as new record? (y/N) ");
			scanf("%c", &ch);  flushing();
			if (ch != 'y' && ch != 'Y') {
				free(new_director);
				return 1;
			}
		}
		new_director->serial_number = (*d_ptr)->serial_number;
		d_ptr = &((*d_ptr)->next);
	}
	new_director->serial_number++;
	*d_ptr = new_director;

	//입력된 영화의 감독 == 감독 리스트의 감독 ==> 연결
	m_ptr = context->movie;
	while (m_ptr != NULL) {
		l_ptr = m_ptr->director;
		if (!strcmp(new_director->name, l_ptr->value)) {
			l_ptr->ref = new_director;
		}
		m_ptr = m_ptr->next;
	}

	printf("  sex(M|F) > "); scanf("%c", &new_director->sex);         flushing();
	printf("  birth > ");    scanf("%[^\t\n]s", new_director->birth); flushing();
	printf("  title > ");    scanf("%[^\t\n]s", titles);              flushing();

	// 대표작 콤마로 구분해서 받기
	*(a_title+0) = '\0';
	for (i=0, j=0; 1; i++) {
		if (j == 0 && *(titles+i) == ' ') {
			continue;
		} else if (*(titles+i) != ',' && *(titles+i) != '\0') {
			*(a_title+(j++)) = *(titles+i);
		} else {
			// 제목 노드 연결
			*(a_title+j) = '\0';
			l_ptr = addNode(&(new_director->title), a_title);
			j=0;

			//제목의 레퍼런스를 영화에 추가
			m_ptr = context->movie;
			while (m_ptr != NULL) {
				if (!strcmp(a_title, m_ptr->title)) {
					l_ptr->ref = (void*) m_ptr;
					break;
				}
				m_ptr = m_ptr->next;
			}
		}

		if (*(titles+i) == '\0') {
			break;
		}
	}

	// Write to log file
	fprintf(fp, "add:%d:%s:%c:%s:%s\n",
		new_director->serial_number,
		new_director->name,
		new_director->sex,
		new_director->birth,
		titles
	);

	return 0;
}

int add_actor(Context* context, FILE* fp) {
	Actor* new_actor;
	Actor** a_ptr;
	Movie* m_ptr;
	LinkedList* l_ptr;

	char *titles=(char *)malloc(sizeof(char)*500);
	char *a_title=(char *)malloc(sizeof(char)*100);
	int i, j;

	new_actor = (Actor*) malloc(sizeof(Actor));
	new_actor->serial_number = 0;
	new_actor->title = 0;
	new_actor->next = NULL;

	// 명령 실행시 입력 받기
	printf("  name > ");  scanf("%[^\t\n]s", new_actor->name);  flushing();

	a_ptr = &(context->actor);
	while (*a_ptr != NULL) { // 중복 체크
		if (!strcmp((*a_ptr)->name, new_actor->name)) {
			char ch;
			printf("    %s is already exists.\n", new_actor->name);
			printf("    Are you sure to add it as new record? (y/N) ");
			scanf("%c", &ch);  flushing();
			if (ch != 'y' && ch != 'Y') {
				free(new_actor);
				return 1;
			}
		}
		new_actor->serial_number = (*a_ptr)->serial_number;
		a_ptr = &((*a_ptr)->next);
	}
	new_actor->serial_number++;
	*a_ptr = new_actor;

	// 영화의 배우를 이 배우와 연결
	m_ptr = context->movie;
	while (m_ptr != NULL) {
		l_ptr = m_ptr->actor;
		while (l_ptr != NULL) {
			if (!strcmp(new_actor->name, l_ptr->value)) {
				l_ptr->ref = new_actor;
				break;
			}
			l_ptr = l_ptr->next;
		}
		m_ptr = m_ptr->next;
	}
	//배우 정보 입력
	printf("  sex(M|F) > ");  scanf("%c", &new_actor->sex);          flushing();
	printf("  birth > ");     scanf("%[^\t\n]s", new_actor->birth);  flushing();
	printf("  title > ");     scanf("%[^\t\n]s", titles);            flushing();

	// 콤마로 대표작 나누기
	*(a_title+0) = '\0';
	for (i=0, j=0; 1; i++) {
		if (j == 0 && *(titles+i) == ' ') {
			continue;
		} else if (*(titles+i) != ',' && *(titles+i) != '\0') {
			*(a_title+(j++)) = *(titles+i);
		} else {
			// 대표작과 영화제목 연결
			*(a_title+j) = '\0';
			l_ptr = addNode(&(new_actor->title), a_title);
			j=0;

			// 제목의 레퍼런스를 영화에 추가
			m_ptr = context->movie;
			while (m_ptr != NULL) {
				if (!strcmp(a_title, m_ptr->title)) {
					l_ptr->ref = (void*) m_ptr;
					break;
				}
				m_ptr = m_ptr->next;
			}
		}

		if (*(titles+i) == '\0') {
			break;
		}
	}

	// Write to log file
	fprintf(fp, "add:%d:%s:%c:%s:%s\n",
		new_actor->serial_number,
		new_actor->name,
		new_actor->sex,
		new_actor->birth,
		titles
	);

	return 0;
}
