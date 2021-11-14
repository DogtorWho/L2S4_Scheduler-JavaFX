#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée.
 * Renvoie le nouveau nœud créé.
 */
static LNode * newLNode(void * data) {
	LNode *Node = malloc(sizeof(LNode));
  assert(Node != NULL);

	Node->data = data;
	Node->pred = NULL;
	Node->succ = NULL;

  return Node;
}

List * newList(void (*viewData)(const void*), void (*freeData)(void*)) {
	List *L = malloc(sizeof(List));
	assert(L != NULL);

	L->head = NULL;
	L->tail = NULL;
	L->numelm = 0;
	L->viewData = viewData;
	L->freeData = freeData;

	return L;
}

void freeList(List * L, int deleteData){
	assert(L != NULL);
	assert(deleteData == 0 || deleteData == 1);

	if(deleteData == 0){ //data pas supprime
		for(LNode *i = L->head; i; ) {
      LNode *N = i;
      i = i->succ;
      free(N);
    }
	}
	else{ //data supprime
		for(LNode *i = L->head; i; ){
      void *data = i->data;
      L->freeData(data);
      LNode* N = i;
			i = i->succ;
      free(N);
    }
	}
	free(L);
}

	/*Thiebault*/
void viewList(const List * L){
	assert(L != NULL);
	printf("Nombre d'éléments : %d\n", L->numelm);

	if (L->numelm == 0){
		printf(" La liste est vide\n");
	}
	else {
		LNode *iterator = L->head;
		while(iterator != NULL){
			printf(" Donnee : ");
			L->viewData(iterator->data);
			printf("\n");
			iterator = iterator->succ;
		}
		free(iterator);
	}
}

void listInsertFirst(List * L, void *data){
	assert(L != NULL);
	LNode *Node = newLNode(data);

	if(L->head == NULL){
		L->head = Node;
	}
	else{
		Node->succ = L->head;
		L->head->pred = Node;
		L->head = Node;
	}
	L->numelm++;
}

void listInsertLast(List * L, void *data){
	assert(L != NULL);
	LNode *Node = newLNode(data);

	if(L->head == NULL){
		L->head = Node;
	}
	else if(L->tail == NULL){
		L->tail = Node;
		L->tail->pred = L->head;
		L->head->succ = L->tail;
	}
	else{
		Node->pred = L->tail;
		L->tail->succ = Node;
		L->tail = Node;
	}
	L->numelm++;
}

void listInsertAfter(List * L, void * data, LNode * ptrelm) {
	assert(L != NULL);
	assert(data != NULL);
	assert(ptrelm != NULL);

	if(ptrelm->succ == NULL){ //dernier element de la liste
		listInsertLast(L, data);
	}
	else{
		LNode *Node = newLNode(data);
		Node->pred = ptrelm;
		Node->succ = ptrelm->succ;
		ptrelm->succ->pred = Node;
		ptrelm->succ = Node;

		L->numelm++;
	}
}

	/*Thiebault*/
LNode* listRemoveFirst(List * L) {
	assert(L->head);

	LNode *rmNode = L->head;

	if(L->head == L->tail){
		L->head = NULL;
		L->tail = NULL;
		L->numelm = 0;
	}
	else{
		L->head = L->head->succ;
		if(L->head != NULL){
			L->head->pred = NULL;
		}
		rmNode->succ = NULL;

		L->numelm--;
	}

	return rmNode;
}

	/*Thiebault*/
LNode* listRemoveLast(List * L) {
	assert(L->tail);

	LNode* rmNode = L->tail;

	if(L->head == L->tail){
		L->head = NULL;
		L->tail = NULL;
		L->numelm = 0;
	}
	else{
		L->tail = L->tail->pred;
		if(L->tail != NULL){
			L->tail->succ = NULL;
		}
		rmNode->pred = NULL;

		L->numelm--;
	}

	return rmNode;
}

	/*Thiebault*/
LNode* listRemoveNode(List * L, LNode * node){
	if(node == L->head){
		listRemoveFirst(L);
	}
	else if(node == L->tail){
		listRemoveLast(L);
	}
	else{
		if(node->pred != NULL){
			node->pred->succ = node->succ;
		}
		if(node->succ != NULL){
			node->succ->pred = node->pred;
		}

		node->pred = NULL;
		node->succ = NULL;

		L->numelm--;
	}

	return node;
}
