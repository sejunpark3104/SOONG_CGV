#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

/**
 * function isExisted()
 * 링크드리스트안에 value가 이미 존재하는지 확인
 * 존재하면 0, 아니면 -1
 */
LinkedList* isExisted (LinkedList* list, char* value) {
	LinkedList* node;
	int idx = 0;
	while (node != NULL) {
		if (!strcmp(value, node->value)) {
			return node;
		}
		node = node->next;
		idx++;
	}
	return NULL;
}

/**
 * function addNode()
 * 링크드리스트 안에 노드를 추가함
 */
 LinkedList* addNode (LinkedList** list, char* value) {
	LinkedList** node = list;
	while (*node != NULL) {
		node = &((*node)->next);
	}
	*node = (LinkedList*)
			malloc(sizeof(LinkedList));
	strcpy((*node)->value, value);
	(*node)->ref = NULL;
	(*node)->next = NULL;
	return *node;
}

/**
 * function deleteNode()
 * 매개변수 안에 value를 가지는 node 삭제
 * 삭제 되면 1, 아니면 0
 */
int deleteNode (LinkedList* list, char* value) {
	LinkedList* node = list;
	LinkedList* prev;
	while (node != NULL) {
		if (!strcmp(node->value, value)) {
			prev->next = node->next;
			free(node);
			return 0;
		}
		prev = node;
		node = node->next;
	}
	return 1;
}

/**
 * function deleteList()
 * 링크드리스트 안에 모든 노드 value 삭제
 */
int deleteList (LinkedList** list) {
	LinkedList* node = *list;
	LinkedList* prev = node;
	while (prev != NULL) {
		node = prev->next;
		free(prev);
		prev = node;
	}
	*list = NULL;
	return 0;
}

/**
 * function printList()
 * 링크드리스트 안에 모든 노드 value 출력
 */
int printList (LinkedList* list) {
	LinkedList* node = list;
	while (node != NULL) {
		printf("%s\n", node->value);
		node = node->next;
	}
	return 0;
}
