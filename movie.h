#ifndef _MOVIE_H_
#define _MOVIE_H_

#define FILE_MOVIE_LOG     "movie_log"
#define FILE_MOVIE_LIST    "movie_list"
#define FILE_DIRECTOR_LOG  "director_log"
#define FILE_DIRECTOR_LIST "director_list"
#define FILE_ACTOR_LOG     "actor_log"
#define FILE_ACTOR_LIST    "actor_list"

/*
 * movie, director, actor 구조체 선언
 */

 typedef struct Context {
 	struct Movie* movie;
 	struct Director* director;
 	struct Actor* actor;
 } Context;

typedef struct Movie {
	int serial_number;
	char title[100];
	char genre[50];
	int year;
	int time;
	struct LinkedList* director;
	struct LinkedList* actor;
	struct Movie* next;
} Movie;

typedef struct Director {
	int serial_number;
	char name[50];
	char sex;
	char birth[10];
	struct LinkedList* title;
	struct Director* next;
} Director;

typedef struct Actor {
	int serial_number;
	char name[50];
	char sex;
	char birth[10];
	struct LinkedList* title;
	struct Actor* next;
} Actor;


/**
 * 링크드리스트를 하기 위한 구조체와 함수들
 */

typedef struct LinkedList {
	char value[100];
	void* ref;
	struct LinkedList* next;
} LinkedList;

LinkedList* isExisted	(LinkedList* list, char* value);
LinkedList* addNode (LinkedList** list, char* value);
int deleteNode (LinkedList* list, char* value);
int deleteList (LinkedList** list);
int printList (LinkedList* list);


/**
 * 프로세싱을 위한 함수들
 */

int search (Context* context, int cmdCnt, char* cmdArg[]);
int print  (Context* context, int cmdCnt, char* cmdArg[]);
int add    (Context* context, int cmdCnt, char* cmdArg[]);
int update (Context* context, int cmdCnt, char* cmdArg[]);
int delete (Context* context, int cmdCnt, char* cmdArg[]);
int sort   (Context* context, int cmdCnt, char* cmdArg[]);
int save   (Context* context, int cmdCnt, char* cmdArg[]);
int test   (Context* context, int cmdCnt, char* cmdArg[]);


/**
 * 공통 사용 함수들
 */

// log 파일 초기화
int init(Context* context);

// 키보드로부터 입력받은 버퍼 비우기
void flushing();

// 방해신호 제어
void int_handler(int dummy);

// 옵션 존재여부 체크
int is_option_existed(char* options, int numOption, char an_option);

// 로그와 리스트 파일 쓰기위해 콜론을 "??;"로 바꿈
char* change_colon (char* str);

// 로그와 리스트 파일 쓰기위해 "??;"을 콜론으로 바꿈
char* recover_colon (char* str);

// 파일이 이미 존재하는지 체크
int is_file_existed (char* filename);

// 현 시간부로 파일명 rename
int rename_file (char* filename);

#endif
