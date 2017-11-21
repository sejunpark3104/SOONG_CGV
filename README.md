SOURCE CODE
----
main.c : 메인 함수 (CLI 구현)
movie.h : Linked List, Movie, Director, Actor 등의 자기참조 구조체 정의
init.c : 프로그램 실행시에 log 파일을 읽고 리스트를 초기화하는 기능 구현
add.c, delete.c, print.c, save.c, search.c, sort.c, update.c : DB 기능 구현


TODO List
----
1. 동적 메모리 할당 및 자기 참조구조체 ( O )
2. 신호 처리 ( O )
3. log 파일 읽고 list 파일 생성 ( O )
4. search 명령어 구현 ( O )
 - 메타 문자 *, ? 처리, 옵션 처리 ( O )
5. print 명령 구현 ( O )
 - 연결된 세부 내용 출력 ( O )
6. add 명령 구현 ( O )
 - 중복 레코드 처리, log 파일, 리스트 수정 ( O )
7. update 명령 구현 ( O )
 - 중복 레코드 처리, log 파일, 리스트 수정 ( O )
8. delete 명령 구현 ( O )
 - log 파일, 리스트 수정 ( O )
9. sort 명령 구현 ( O )
 - 옵션 처리 ( O )
10. save 명령 구현 ( O )
 - 옵션 처리 ( O )
11. end 명령 구현 ( O )
 - 옵션 처리 ( O )
12. 제출 문서
 - 함수 별로 설계 ( O )
 - 주석 ( O )

+ 텍스트 파일로 출력 ( O )
+ 파일의 모든 내용은 영어로 작성 ( O )
+ 파일 저장시, 영화 제목 내에 콜론이 있다면 "??;"로 치환 ( O )
+ log 파일은 프로그램이 실행될 때마다 읽어서 처리해야 함 & 처리 후 list 파일 저장 ( O )
+ list 파일 저장시, 이미 존재한다면 기존 파일을 movie_list.2017082340 처럼 변경 ( O )
