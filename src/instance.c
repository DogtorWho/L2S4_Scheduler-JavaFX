#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"

#define SIZEOF_ID 3

/***********************************************
 * TASK
 ***********************************************/

Task * newTask(char* id, int proctime, int reltime, int deadline, int weight) {
	assert(proctime > 0);
	assert(reltime >= 0);
	assert((deadline >= reltime + proctime));

	Task *T = malloc(sizeof(Task));
	assert(T != NULL);

	T->id = id;
	T->processingTime = proctime;
	T->releaseTime = reltime;
	T->deadline = deadline;
	T->weight = weight;

	return T;
}

void freeTask(void* task) {
	Task *tmpTask = (Task*)task;

	free(tmpTask->id);
	free(tmpTask);
}

void viewTask(const void *task) {
	const Task *tmpTask = task;

	printf("\t= Task = \n");
	printf(" ------------- id : %s\n", tmpTask->id);
	printf(" - processingTime : %d\n", tmpTask->processingTime);
	printf(" ---- releaseTime : %d\n", tmpTask->releaseTime);
	printf(" ------- deadline : %d\n", tmpTask->deadline);
	printf(" --------- weight : %d\n", tmpTask->weight);
}

/************************************************
 * INSTANCE
 ************************************************/

Instance readInstance(char * filename) {
	FILE *file = fopen(filename, "r");
	assert(file != NULL);

	Instance instance = newList(viewTask, freeTask);
	int p, r, d, w;
	int tmp = 0;

	while (tmp != EOF) {
		char *id = malloc((SIZEOF_ID+1) * sizeof(char));
		tmp = fscanf(file, "%s\t%d\t%d\t%d\t%d", id, &p, &r, &d, &w);
		if(tmp != EOF){
			Task *task = newTask(id, p, r, d, w);
			listInsertLast(instance, task);
		}
		else{
			free(id);
		}
	}

	rewind(file);
	fclose(file);
	return instance;
}

void viewInstance(Instance I) {
	viewList(I);
}

void freeInstance(Instance I, int deleteData) {
	freeList(I, deleteData);
}

/*****************************************************************************
 * Ordonner l'instance
 *****************************************************************************/

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre SPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre SPT : a précède b si
 * (+) durée de a < durée de b
 * (+) durée de a = durée de b ET date de libération de a < date de libération de b
 */
static int spt(const void* a, const void* b) {
	const Task* taska = a;
	const Task* taskb = b;
	if(taska->processingTime < taskb->processingTime){
		return 1;
	}else if((taska->processingTime == taskb->processingTime) && (taska->releaseTime < taskb->releaseTime)){
		return 1;
	} else {
		return 0;
	}
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre LPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre LPT : a précède b si
 * (+) durée de a > durée de b
 * (+) durée de a = durée de b ET date de libération de a < date de libération de b
 */
static int lpt(const void* a, const void* b) {
	const Task* taska = a;
	const Task* taskb = b;
	if(taska->processingTime > taskb->processingTime){
		return 1;
	}else if((taska->processingTime == taskb->processingTime) && (taska->releaseTime < taskb->releaseTime)){
		return 1;
	} else {
		return 0;
	}
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre WSPT :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre WSPT : a précède b si
 * (+) poids / durée de a > poids / durée de b
 * (+) poids / durée de a = poids / durée de b
 *     ET durée de a < durée de b
 * (+) poids / durée de a = poids / durée de b
 *     ET durée de a = durée de b
 *     ET date de libération de a < date de libération de b
 */
static int wspt(const void* a, const void* b) {
	const Task* taska = a;
	const Task* taskb = b;
	if((taska->weight / taska->processingTime) > (taskb->weight / taskb->processingTime)){
		return 1;
	}
	else if(((taska->weight / taska->processingTime) == (taskb->weight / taskb->processingTime)) && (taska->processingTime < taskb->processingTime)){
		return 1;
	}
	else if(((taska->weight / taska->processingTime) == (taskb->weight / taskb->processingTime)) && (taska->processingTime == taskb->processingTime) && (taska->releaseTime < taskb->releaseTime)){
		return 1;
	}
	else{
		return 0;
	}
}

/**
 * @brief
 * Comparer les tâches a et b par rapport à l'ordre FCFS :
 * (+) Renvoie 1, si a précède b
 * (+) Renvoie 0, si b précède a
 * Ordre FCFS : a précède b si
 * (+) date de libération de a < date de libération de b
 * (+) date de libération de a = date de libération de b
 *     ET durée de a > durée de b
 */
static int fcfs(const void* a, const void* b) {
	const Task* taska = a;
	const Task* taskb = b;
	if(taska->releaseTime < taskb->releaseTime){
		return 1;
	}else if((taska->releaseTime == taskb->releaseTime) && (taska->processingTime > taskb->processingTime)){
		return 1;
	} else {
		return 0;
	}
}


Instance reorderInstance(Instance I,  DataStructure structtype, Order order) {
	assert(I != NULL);

	void* type = NULL;
	void (*fonctionDataStructure)(void* type, void* key, void* data);

	if(structtype == OL){
		fonctionDataStructure = &OListInsert;
		type = newOList(compareInt, viewInt, I->viewData, freeInt, I->freeData);
	}
	else if(structtype == BST){
		fonctionDataStructure = &BSTreeInsert;
		type = newBSTree(compareInt, viewInt, I->viewData, freeInt, I->freeData);
	}
	else if(structtype == EBST){
		fonctionDataStructure = &EBSTreeInsert;
		type = newEBSTree(compareInt, viewInt, I->viewData, freeInt, I->freeData);
	}
	else{
		error("structtype non valide");
	}

	int (*fonctionOrder)(const void* a, const void* b);
	if(order == SPT){
		fonctionOrder = &spt;
	}
	else if(order == LPT){
		fonctionOrder = &lpt;
	}
	else if(order == WSPT){
		fonctionOrder = &wspt;
	}
	else if(order == FCFS){
		fonctionOrder = &fcfs;
	}
	else{
		error("order non valide");
	}

	if(I->head != NULL){
		LNode *a = I->head;
    LNode *b = I->head->succ;
		int cpt = 1;

	  while (I->numelm != 0){
	    int *key = malloc(sizeof(int));
	    *key = cpt;
	    if(b == NULL){
	      fonctionDataStructure(type, key, a->data);
	      a = listRemoveFirst(I);
				free(a);
	    }
			else{
	      for(int i=0; i < I->numelm; i++){
	        if(fonctionOrder(a->data, b->data) == 1){
	          if(b->succ != NULL){
	            b = b->succ;
	          }
	        }
	        else{
	          if(b->succ != NULL){
	            a = b;
	           	b = b->succ;
	          }
	        }
	      }
        if(fonctionOrder(a->data, b->data) == 1){
          fonctionDataStructure(type, key, a->data);
					if (a==I->head){
						a=listRemoveFirst(I);
					}
					else{
						a = listRemoveNode(I, a);
					}
					free(a);
        }
        else{
          fonctionDataStructure(type, key, b->data);
          b = listRemoveNode(I, b);
					free(b);
        	}
				}
				if (I->head!=NULL){
					a = I->head;
					b = I->head->succ;
				}
      cpt++;
		}
	}

	if(structtype == OL){
		List *tmpI = OListToList(type);
		I->head = tmpI->head;
		I->tail = tmpI->tail;
		I->numelm = tmpI->numelm;

		free(tmpI);
		freeOList(type,1,0);
	}
	else if(structtype == BST || structtype == EBST){
		List *tmpI = BSTreeToList(type);
		I->head = tmpI->head;
		I->tail = tmpI->tail;
		I->numelm = tmpI->numelm;

		free(tmpI);
		freeBSTree(type, 1, 0);
	}
	else{
		error("structtype non valide");
	}

	return I;
}
