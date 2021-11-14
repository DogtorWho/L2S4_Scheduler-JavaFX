#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

int main(int argv, char *argc[]) {
/*
	Instance I = readInstance("./data/exemple");
	viewInstance(I);
	reorderInstance(I, EBST, LPT);
	viewInstance(I);

	Schedule *SOL = newSchedule(OL, 1);
	computeSchedule(SOL, I);
	viewSchedule(SOL);
	saveSchedule(SOL, "./data/output_OL");
	printf("Makespan=%ld\n", makespan(SOL));
	printf("SumWjCj=%ld\n", SumWjCj(SOL));
	printf("SumWjFj=%ld\n", SumWjFj(SOL));
	printf("SumWjTj=%ld\n", SumWjTj(SOL));
	freeSchedule(SOL);

	Schedule *SBST = newSchedule(BST, 1);
	computeSchedule(SBST, I);
	viewSchedule(SBST);
	saveSchedule(SBST, "./data/output_BST");
	printf("Makespan=%ld\n", makespan(SBST));
	printf("SumWjCj=%ld\n", SumWjCj(SBST));
	printf("SumWjFj=%ld\n", SumWjFj(SBST));
	printf("SumWjTj=%ld\n", SumWjTj(SBST));
	freeSchedule(SBST);

	Schedule *SEBST = newSchedule(EBST, 1);
	computeSchedule(SEBST, I);
	viewSchedule(SEBST);
	saveSchedule(SEBST, "./data/output_EBST");
	printf("Makespan=%ld\n", makespan(SEBST));
	printf("SumWjCj=%ld\n", SumWjCj(SEBST));
	printf("SumWjFj=%ld\n", SumWjFj(SEBST));
	printf("SumWjTj=%ld\n", SumWjTj(SEBST));
	freeSchedule(SEBST);

	freeInstance(I, 1);
*/

/*
	// TEST LIST.C
	int *data1 = malloc(sizeof(int));
	int *data2 = malloc(sizeof(int));
	int *data3 = malloc(sizeof(int));
	int *data4 = malloc(sizeof(int));
	*data1 = 1;
	*data2 = 2;
	*data3 = 3;
	*data4 = 4;

	List *list = newList(viewInt, freeInt);

	printf("\t=Vide=\n");
	viewList(list);
	printf("\n");

	printf("\t=Head=\n");
	listInsertFirst(list, data1);
	viewList(list);
	printf("\n");

	printf("\t=2eme=\n");
	listInsertAfter(list, data2, list->head);
	viewList(list);
	printf("\n");

	printf("\t=3eme=\n");
	listInsertAfter(list, data3, list->head);
	viewList(list);
	printf("\n");

	printf("\t=4eme=\n");
	listInsertAfter(list, data4, list->head);
	viewList(list);
	printf("\n");

	freeList(list, 1);
*/

/*
	// TEST OLIST.C
	int *data1 = malloc(sizeof(int));
	int *key1 = malloc(sizeof(int));
	*data1 = 1;
	*key1 = 21;

	int *data2 = malloc(sizeof(int));
	int *key2 = malloc(sizeof(int));
	*data2 = 2;
	*key2 = 2;

	int *data3 = malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	*data3 = 3;
	*key3 = 87;

	int *data4 = malloc(sizeof(int));
	int *key4 = malloc(sizeof(int));
	*data4 = 4;
	*key4 = 12;

	OList *list = newOList(compareInt, viewInt, viewInt, freeInt, freeInt);

	printf("\t=Vide=\n");
	viewOList(list);
	printf("\n");

	printf("\t=1er=\n");
	OListInsert(list, key1, data1);
	viewOList(list);
	printf("\n");

	printf("\t=2eme=\n");
	OListInsert(list, key2, data2);
	viewOList(list);
	printf("\n");

	printf("\t=3eme=\n");
	OListInsert(list, key3, data3);
	viewOList(list);
	printf("\n");

	printf("\t=4eme=\n");
	OListInsert(list, key4, data4);
	viewOList(list);
	printf("\n");

	printf(" - Head : ");
	viewInt(list->head->key);
	printf("  |  Tail : ");
	viewInt(list->tail->key);
	printf("\n\n");

	List *Tlist = OListToList(list);
	viewList(Tlist);

	freeList(Tlist, 1);
*/

/*
	// TEST BSTREE.C
	int *data1 = malloc(sizeof(int));
	int *key1 = malloc(sizeof(int));
	*data1 = 1;
	*key1 = 13;

	int *data2 = malloc(sizeof(int));
	int *key2 = malloc(sizeof(int));
	*data2 = 2;
	*key2 = 9;

	int *data3 = malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	*data3 = 3;
	*key3 = 22;

	int *data4 = malloc(sizeof(int));
	int *key4 = malloc(sizeof(int));
	*data4 = 4;
	*key4 = 4;

	int *data5 = malloc(sizeof(int));
	int *key5 = malloc(sizeof(int));
	*data5 = 5;
	*key5 = 12;

	int *data6 = malloc(sizeof(int));
	int *key6 = malloc(sizeof(int));
	*data6 = 6;
	*key6 = 17;

	int *data7 = malloc(sizeof(int));
	int *key7 = malloc(sizeof(int));
	*data7 = 7;
	*key7 = 31;

	int *data8 = malloc(sizeof(int));
	int *key8 = malloc(sizeof(int));
	*data8 = 8;
	*key8 = 1;

	int *data9 = malloc(sizeof(int));
	int *key9 = malloc(sizeof(int));
	*data9 = 9;
	*key9 = 6;

	int *data10 = malloc(sizeof(int));
	int *key10 = malloc(sizeof(int));
	*data10 = 10;
	*key10 = 11;

	int *data11 = malloc(sizeof(int));
	int *key11 = malloc(sizeof(int));
	*data11 = 11;
	*key11 = 16;

	int *data12 = malloc(sizeof(int));
	int *key12 = malloc(sizeof(int));
	*data12 = 12;
	*key12 = 8;


	BSTree *tree = newBSTree(compareInt, viewInt, viewInt, freeInt, freeInt);

	printf("\t=Vide=\n");
	viewBSTree(tree);
	printf("\n");

	printf("\t=Root=\n");
	BSTreeInsert(tree, key1, data1);
	viewBSTree(tree);
	printf("\n");

	printf("\t=2eme=\n");
	BSTreeInsert(tree, key2, data2);
	printf("\n");

	printf("\t=3eme=\n");
	BSTreeInsert(tree, key3, data3);
	printf("\n");

	printf("\t=4eme=\n");
	BSTreeInsert(tree, key4, data4);
	printf("\n");

	printf("\t=5eme=\n");
	BSTreeInsert(tree, key5, data5);
	printf("\n");

	printf("\t=6eme=\n");
	BSTreeInsert(tree, key6, data6);
	printf("\n");

	printf("\t=7eme=\n");
	BSTreeInsert(tree, key7, data7);
	printf("\n");

	printf("\t=8eme=\n");
	BSTreeInsert(tree, key8, data8);
	printf("\n");

	printf("\t=9eme=\n");
	BSTreeInsert(tree, key9, data9);
	printf("\n");

	printf("\t=10eme=\n");
	BSTreeInsert(tree, key10, data10);
	printf("\n");

	printf("\t=11eme=\n");
	BSTreeInsert(tree, key11, data11);
	printf("\n");

	printf("\t=12eme=\n");
	BSTreeInsert(tree, key12, data12);
	viewBSTree(tree);
	printf("\n");


	List *list = BSTreeToList(tree);
	viewList(list);
	printf("\n");

	printf(" ==predecessor==\n");
	printf("Avant : ");
	viewInt(tree->root->key);
	printf("\n");
	printf("Apres : ");
	viewInt(findPredecessor(tree, tree->root)->key);
	printf("\n");

	printf(" ==successor==\n");
	printf("Avant : ");
	viewInt(tree->root->key);
	printf("\n");
	printf("Apres : ");
	viewInt(findSuccessor(tree, tree->root)->key);
	printf("\n");


	freeList(list, 0);
	freeBSTree(tree, 1, 1);
*/

/*
	// TEST EBSTREE
	int *data1 = malloc(sizeof(int));
	int *key1 = malloc(sizeof(int));
	*data1 = 1;
	*key1 = 72;

	int *data2 = malloc(sizeof(int));
	int *key2 = malloc(sizeof(int));
	*data2 = 2;
	*key2 = 20;

	int *data3 = malloc(sizeof(int));
	int *key3 = malloc(sizeof(int));
	*data3 = 3;
	*key3 = 29;

	int *data4 = malloc(sizeof(int));
	int *key4 = malloc(sizeof(int));
	*data4 = 4;
	*key4 = 99;

	int *data5 = malloc(sizeof(int));
	int *key5 = malloc(sizeof(int));
	*data5 = 5;
	*key5 = 41;

	int *data6 = malloc(sizeof(int));
	int *key6 = malloc(sizeof(int));
	*data6 = 6;
	*key6 = 50;

	int *data7 = malloc(sizeof(int));
	int *key7 = malloc(sizeof(int));
	*data7 = 7;
	*key7 = 91;

	int *data8 = malloc(sizeof(int));
	int *key8 = malloc(sizeof(int));
	*data8 = 8;
	*key8 = 65;

	int *data9 = malloc(sizeof(int));
	int *key9 = malloc(sizeof(int));
	*data9 = 9;
	*key9 = 11;

	int *data10 = malloc(sizeof(int));
	int *key10 = malloc(sizeof(int));
	*data10 = 10;
	*key10 = 32;


	BSTree *tree = newEBSTree(compareInt, viewInt, viewInt, freeInt, freeInt);

	printf("\t=Vide=\n");
	viewBSTree(tree);
	printf("\n");

	printf("\t=Root=\n");
	EBSTreeInsert(tree, key1, data1);
	viewBSTree(tree);
	printf("\n");

	printf("\t=2eme=\n");
	EBSTreeInsert(tree, key2, data2);
	viewBSTree(tree);
	printf("\n");

	printf("\t=3eme=\n");
	EBSTreeInsert(tree, key3, data3);
	viewBSTree(tree);
	printf("\n");

	printf("\t=4eme=\n");
	EBSTreeInsert(tree, key4, data4);
	viewBSTree(tree);
	printf("\n");

	printf("\t=5eme=\n");
	EBSTreeInsert(tree, key5, data5);
	viewBSTree(tree);
	printf("\n");

	printf("\t=6eme=\n");
	EBSTreeInsert(tree, key6, data6);
	viewBSTree(tree);
	printf("\n");

	printf("\t=7eme=\n");
	EBSTreeInsert(tree, key7, data7);
	viewBSTree(tree);
	printf("\n");

	printf("\t=8eme=\n");
	EBSTreeInsert(tree, key8, data8);
	viewBSTree(tree);
	printf("\n");

	printf("\t=9eme=\n");
	EBSTreeInsert(tree, key9, data9);
	viewBSTree(tree);
	printf("\n");

	printf("\t=10eme=\n");
	EBSTreeInsert(tree, key10, data10);
	viewBSTree(tree);
	printf("\n");

	freeBSTree(tree, 1, 1);
*/

/*
	// TEST INSTANCE.C
	Instance instance = readInstance("data/exemple");

	instance = reorderInstance(instance, OL, SPT);
	viewList(instance);

	freeInstance(instance, 1);
*/

/*
	// TEST SCHEDULE.C
	Instance instance = readInstance("data/exemple");
	instance = reorderInstance(instance, BST, SPT);
	viewList(instance);

	Schedule* sched = newSchedule(BST, 1);
	computeSchedule(sched, instance);
	viewSchedule(sched);
	printf("makespan : %ld\n", makespan(sched));
	printf("SumWjCj : %ld\n", SumWjCj(sched));
	printf("SumWjFj : %ld\n", SumWjFj(sched));
	printf("SumWjTj : %ld\n", SumWjTj(sched));

	saveSchedule(sched, "testSave.txt");
	Instance Save = readInstance("testSave.txt");
	viewList(Save);
	freeInstance(Save, 1);

	freeSchedule(sched);
	freeInstance(instance, 1);
*/

/*
	// TEST TEMPS D'EXECUTION
	clock_t start, end;
	double cpu_time_used;

	Instance I1 = readInstance("data/expe/instance_0500_1");
	Schedule* sched1 = newSchedule(OL, 1);

	Instance I2 = readInstance("data/expe/instance_0500_2");
	Schedule* sched2 = newSchedule(BST, 0);

	Instance I3 = readInstance("data/expe/instance_0500_3");
	Schedule* sched3 = newSchedule(BST, 0);

	Instance I4 = readInstance("data/expe/instance_0500_4");
	Schedule* sched4 = newSchedule(BST, 0);

	Instance I5 = readInstance("data/expe/instance_0500_5");
	Schedule* sched5 = newSchedule(BST, 0);

	printf("============================= 1 =============================\n");
	// entourer le sous-programme à évaluer avec deux appels à clock()
	start = clock();
	I1 = reorderInstance(I1, OL, SPT);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 1 Time used for reorderInstance : \t%lf sec\n", cpu_time_used);

	start = clock();
	computeSchedule(sched1, I1);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 1 Time used for computeSchedule : \t%lf sec\n\n", cpu_time_used);

	printf("-makespan : %ld\n", makespan(sched1));
	printf("--SumWjCj : %ld\n", SumWjCj(sched1));
	printf("--SumWjFj : %ld\n", SumWjFj(sched1));
	printf("--SumWjTj : %ld\n", SumWjTj(sched1));

	freeSchedule(sched1);
	freeInstance(I1, 1);
	/*
	printf("============================= 2 =============================\n");
	// entourer le sous-programme à évaluer avec deux appels à clock()
	start = clock();
	I2 = reorderInstance(I2, BST, SPT);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 2 Time used for reorderInstance : \t%lf sec\n", cpu_time_used);

	start = clock();
	computeSchedule(sched2, I2);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 2 Time used for computeSchedule : \t%lf sec\n\n", cpu_time_used);

	printf(" makespan : %ld\n", makespan(sched2));
	printf(" SumWjCj : %ld\n", SumWjCj(sched2));
	printf(" SumWjFj : %ld\n", SumWjFj(sched2));
	printf(" SumWjTj : %ld\n", SumWjTj(sched2));

	freeSchedule(sched2);
	freeInstance(I2, 1);


	printf("============================= 3 =============================\n");
	// entourer le sous-programme à évaluer avec deux appels à clock()
	start = clock();
	I3 = reorderInstance(I3, BST, SPT);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 3 Time used for reorderInstance : \t%lf sec\n", cpu_time_used);

	start = clock();
	computeSchedule(sched3, I3);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 3 Time used for computeSchedule : \t%lf sec\n\n", cpu_time_used);

	printf(" makespan : %ld\n", makespan(sched3));
	printf(" SumWjCj : %ld\n", SumWjCj(sched3));
	printf(" SumWjFj : %ld\n", SumWjFj(sched3));
	printf(" SumWjTj : %ld\n", SumWjTj(sched3));

	freeSchedule(sched3);
	freeInstance(I3, 1);


	printf("============================= 4 =============================\n");
	// entourer le sous-programme à évaluer avec deux appels à clock()
	start = clock();
	I4 = reorderInstance(I4, BST, SPT);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 4 Time used for reorderInstance : \t%lf sec\n", cpu_time_used);

	start = clock();
	computeSchedule(sched4, I4);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 4 Time used for computeSchedule : \t%lf sec\n\n", cpu_time_used);

	printf(" makespan : %ld\n", makespan(sched4));
	printf(" SumWjCj : %ld\n", SumWjCj(sched4));
	printf(" SumWjFj : %ld\n", SumWjFj(sched4));
	printf(" SumWjTj : %ld\n", SumWjTj(sched4));

	freeSchedule(sched4);
	freeInstance(I4, 1);


	printf("============================= 5 =============================\n");
	// entourer le sous-programme à évaluer avec deux appels à clock()
	start = clock();
	I5 = reorderInstance(I5, BST, SPT);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 5 Time used for reorderInstance : \t%lf sec\n", cpu_time_used);

	start = clock();
	computeSchedule(sched5, I5);
	end = clock();

	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" 5 Time used for computeSchedule : \t%lf sec\n\n", cpu_time_used);

	printf(" makespan : %ld\n", makespan(sched5));
	printf(" SumWjCj : %ld\n", SumWjCj(sched5));
	printf(" SumWjFj : %ld\n", SumWjFj(sched5));
	printf(" SumWjTj : %ld\n", SumWjTj(sched5));

	freeSchedule(sched5);
	freeInstance(I5, 1);
*/

	getSchedule("data/expe/instance_0500_1", "Save_OL_BF_500_1.txt", 0, 0, 1);
	printf("\n");
	//getSchedule("data/expe/instance_0500_1", "Save_BST_BF_500_1.txt", 1, 0, 1);

	return EXIT_SUCCESS;
}
