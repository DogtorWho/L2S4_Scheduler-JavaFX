#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "olist.h"

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'une liste doublement chaînée ordonnée.
 * Renvoie le nouveau nœud créé.
 */
static OLNode * newOLNode(void* key, void* data) {
	OLNode *ONode = malloc(sizeof(OLNode));
  assert(ONode != NULL);

	ONode->key = key;
	ONode->data = data;
	ONode->pred = NULL;
	ONode->succ = NULL;

  return ONode;
}

OList * newOList(int (*preceed)(const void*, const void*),
				void (*viewKey)(const void*), void (*viewData)(const void*),
				void (*freeKey)(void*), void (*freeData)(void*)) {

	OList *OL = malloc(sizeof(OList));
	assert(OL != NULL);

	OL->head = NULL;
	OL->tail = NULL;
	OL->numelm = 0;
	OL->preceed = preceed;
	OL->viewKey = viewKey;
	OL->viewData = viewData;
	OL->freeKey = freeKey;
	OL->freeData = freeData;

	return OL;
}

void freeOList(OList * L, int deleteKey, int deleteData) {
	assert(L != NULL);
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);

	if(deleteKey == 0 && deleteData == 0){ //key et data pas supprime
		for(OLNode *i = L->head; i; ) {
			OLNode *N = i;
			i = i->succ;
			free(N);
		}
	}
	else if(deleteKey == 1 && deleteData == 0){ //key supprime et data pas supprime
		for(OLNode *i = L->head; i; ) {
			void *key = i->key;
			L->freeKey(key);
			OLNode *N = i;
			i = i->succ;
			free(N);
		}
	}
	else if(deleteKey == 0 && deleteData == 1){ //key pas supprime et data supprime
		for(OLNode *i = L->head; i; ) {
			void *data = i->data;
			L->freeData(data);
			OLNode *N = i;
			i = i->succ;
			free(N);
		}
	}
	else{ //key et data supprime
		for(OLNode *i = L->head; i; ) {
			void *key = i->key;
			void *data = i->data;
			L->freeKey(key);
			L->freeData(data);
			OLNode *N = i;
			i = i->succ;
			free(N);
		}
	}
	free(L);
}

	/*Thiebault*/
void viewOList(const OList * L) {
	printf("  Nombre d'éléments: %d\n\n", L->numelm);
	if (L->numelm == 0){
		printf(" - List vide\n");
		printf("---------------------------\n");
	}
	else {
		OLNode *iterator = L->head;
		while (iterator) {
			printf(" - key : ");
			L->viewKey (iterator->key);
			printf("  |  data : ");
			L->viewData (iterator->data);
			printf("\n---------------------------\n");
			iterator = iterator->succ;
		}
	}
}

void OListInsert(OList * L, void * key, void * data) {
	assert(L != NULL);

	OLNode *ON = newOLNode(key, data);
	
	if(L->head == NULL){ // liste a 0 element
		L->head = ON;
	}
	else{ // liste a 1 element ou plus
		OLNode *i = L->head;

		if(L->tail == NULL){ // list a 1 element : la tete
			if(L->preceed(i->key, key) == 1){ // i->key < key
				L->tail = ON;
				ON->pred = L->head;
				L->head->succ = ON;
			}
			else{ // i->key >= key
				ON->succ = L->head;
				L->head->pred = ON;

				L->tail = L->head;
				L->head = ON;
			}
		}
		else{ // liste a 2 elements ou plus
			while(L->preceed(i->key, key) == 1){ //while i->key < key
				if(i->succ != NULL){
					i = i->succ;
				}
				else{ // toutes les clés de la liste sont plus petite
					break;
				}
			}
			if(i->succ != NULL){
				if(i == L->head){
					L->head = ON;
				}
				ON->pred = i->pred;
				ON->succ = i;
				if(i->pred != NULL){
					i->pred->succ = ON;
				}
				i->pred = ON;
			}
			else{ // la clé est placée a la fin ou avant le dernier
				if(L->preceed(i->key, key) == 1){ // a la fin
					ON->pred = i;
					i->succ = ON;
					L->tail = ON;
				}
				else{ // avant le dernier element
					ON->pred = i->pred;
					ON->succ = i;
					if(i->pred != NULL){
						i->pred->succ = ON;
					}
					i->pred = ON;
				}
			}
		}
	}
	L->numelm++;
}

	/*Thiebault*/
List* OListToList(const OList* L) {
	assert(L != NULL);
	List *list = newList(L->viewData, L->freeData);
	OLNode *i = L->head;

	listInsertFirst(list, i->data);
	i = i->succ;

	while(i){
		listInsertLast(list, i->data);
		i = i->succ;
	}

	return list;
}
