#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

Schedule * newSchedule(DataStructure structtype, int backfilling) {
	Schedule * sched = calloc(1, sizeof(Schedule));

	sched->structtype = structtype;
	sched->backfilling = backfilling;

	switch (structtype) {
	case OL:
		sched->scheduledTasks = newOList(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	case BST:
		sched->scheduledTasks = newBSTree(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	case EBST:
		sched->scheduledTasks = newEBSTree(compareInt, viewInt, viewTask, freeInt, freeTask);
		break;
	default:
		error("Schedule:newSchedule : invalid data structure.");
		break;
	}

	return sched;
}

void viewSchedule(Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		viewOList(sched->scheduledTasks);
		break;
	case BST:
		viewBSTree(sched->scheduledTasks);
		break;
	case EBST:
		viewBSTree(sched->scheduledTasks);
		break;
	default:
		error("Schedule:viewSchedule : invalid data structure.");
		break;
	}
}

void freeSchedule(Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		freeOList(sched->scheduledTasks, 1, 0);
		break;
	case BST:
		freeBSTree(sched->scheduledTasks, 1, 0);
		break;
	case EBST:
		freeBSTree(sched->scheduledTasks, 1, 0);
		break;
	default:
		error("Schedule:freeSchedule : invalid data structure.");
		break;
	}
	free(sched);
}

void addTaskToSchedule(Schedule* sched, int startingTime, Task* task) {
	int *stime = malloc(sizeof(int));
	*stime = startingTime;
	switch (sched->structtype) {
	case OL:
		OListInsert(sched->scheduledTasks, stime, task);
		break;
	case BST:
		BSTreeInsert(sched->scheduledTasks, stime, task);
		break;
	case EBST:
		EBSTreeInsert(sched->scheduledTasks, stime, task);
		break;
	default:
		error("Schedule:addTaskToSchedule : invalid data structure.");
		break;
	}
}

/*****************************************************************************
 * Compute schedule
 *****************************************************************************/

/**
 * @brief
 * Vérifier si on peut faire du backfilling avec la tâche task dans l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 * Si le backfilling est possible, renvoie la date de début la plus petite.
 * Sinon (on ne peut pas faire du backfilling), renvoie -1.
 * NB : fonction itérative.
 */
static int OLFindBackfillingPosition(const OList* scheduledTasks, const Task* task) {
	assert(scheduledTasks != NULL);
	assert (task != NULL);

	if (scheduledTasks->head == NULL) {
		return -1;
	}

	int backfilling = -1;
	OLNode *i = scheduledTasks->head;

	// si la tache peut etre placée avant la premiere tache
	if( (task->releaseTime + task->processingTime) <= *((int*)i->key) ){
		return task->releaseTime;
	}

	while (i) {
		if(i->succ != NULL){ // si le successeur est null il n'y a pas de backfilling a faire
			if( (task->processingTime) <= ((*((int*)i->succ->key)) - (*((int*)i->key) + (((Task*)i->data)->processingTime))) ){
				// la tache peut rentrer entre la tache d'avant et celle d'apres
				if( (task->releaseTime + task->processingTime) <= (*((int*)i->succ->key)) ){
					// la tache rentre bien dans l'espace (en fonction de son releaseTime)
					if( (task->releaseTime) < (*((int*)i->key) + (((Task*)i->data)->processingTime)) ){
						// la tache est placée juste apres la tache d'avant
						return (*((int*)i->key) + (((Task*)i->data)->processingTime));
					}
					else{
						// la tache est a un endroit quelconque entre les deux autres taches
						return task->releaseTime;
					}
				}
			}
		}
		i = i->succ;
	}

	return backfilling;
}

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute
 * dans la structure scheduledTasks de type liste ordonnée.
 * La date de début dépend de l'application du backfilling (backfilling=1) ou pas (backfilling=0).
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 */
static int OLFindStartingTime(const OList *scheduledTasks, const Task* task, int backfilling) {
	assert(scheduledTasks != NULL);
	assert (task != NULL);

	int startingTime = OLFindBackfillingPosition(scheduledTasks, task);

	if( (backfilling == 0) || ((backfilling == 1) && (startingTime == -1)) ){ // pas de backfilling
		if(scheduledTasks->head != NULL){ // on ajoute a la fin
			if(scheduledTasks->tail != NULL){ // la tache est placée apres la derniere (tail)
				if((*((int*)scheduledTasks->tail->key) + ((Task*)scheduledTasks->tail->data)->processingTime) <= task->releaseTime){ // le debut de la tache est apres la derniere tache
					startingTime = task->releaseTime;
				}
				else{ // le debut de la tache est pendant la derniere tache, on renvoi la fin de la derniere tache
					startingTime = (*((int*)scheduledTasks->tail->key) + (((Task*)scheduledTasks->tail->data)->processingTime));
				}
			}
			else{	// la liste n'a qu'un element
				if((*((int*)scheduledTasks->head->key) + ((Task*)scheduledTasks->head->data)->processingTime) <= task->releaseTime){	// le debut de la tache est apres la seule tache
					startingTime = task->releaseTime;
				}
				else{	// le debut de la tache est pendant la seule tache, on renvoi la fin de la seule tache
					startingTime = (*((int*)scheduledTasks->head->key) + (((Task*)scheduledTasks->head->data)->processingTime));
				}
			}
		}
		else{ // la liste est vide
			startingTime = task->releaseTime;
		}
	}

	else if( (backfilling == 1) && (startingTime != -1) ){ // backfilling
		startingTime = startingTime;
	}

	else{
		error("backfilling non comforme");
	}

	return startingTime;
}

/**
 * @brief
 * Vérifier si on peut faire du backfilling avec la tâche task dans l'ordonnancement
 * représenté par le sous-arbre raciné à curr de l'arbre binaire de recherche scheduledTasks.
 * Si le backfilling est possible, renvoie la date de début la plus petite.
 * Sinon (on ne peut pas faire du backfilling), renvoie -1.
 * NB : fonction récursive, l'ordre infixe est conseillé.
 */
static int BSTFindBackfillingPosition(const BSTree* scheduledTasks, const BSTNode* curr, const Task* task) {
	assert(scheduledTasks != NULL);
	assert(task != NULL);

	if(scheduledTasks->numelm == 0){
		return -1;
	}

	BSTNode * Pred = findPredecessor(scheduledTasks, curr);
	BSTNode * Succ = findSuccessor(scheduledTasks, curr);

	//on est sur le plus petit noeud de l'arbre
	if(Pred == NULL){
		//on vérifie si il peut pas s'insérer avant
		if( (task->releaseTime + task->processingTime) <= *((int*)curr->key) ){
			//(*((int*)curr->key) - task->releaseTime) >= task->processingTime
			return task->releaseTime;
		}
	}
	else {
		//Si prédecesseur est plus petit que task, on regarde si on peut ajouter task avant curr en partant de releaseTime
		if( (task->processingTime) <= ((*((int*)curr->key)) - (*((int*)Pred->key) + (((Task*)Pred->data)->processingTime))) ){
			// la tache peut rentrer entre la tache d'avant et celle d'apres
			if( (task->releaseTime + task->processingTime) <= (*((int*)curr->key)) ){
				// la tache rentre bien dans l'espace (en fonction de son releaseTime)
				if( (task->releaseTime) < (*((int*)Pred->key) + (((Task*)Pred->data)->processingTime)) ){
					// la tache est placée juste apres la tache d'avant
					return (*((int*)Pred->key) + (((Task*)Pred->data)->processingTime));
				}
				else{
					// la tache est a un endroit quelconque entre les deux autres taches
					return task->releaseTime;
				}
			}
		}
	}

	//on est sur le dernier noeud donc nous avons pas trouvé de place avant pour la task, on revoit -1 et va venir se placer en fin d'arbre
	if(Succ == NULL){
		return -1;
	}

	if(*((int*)curr->key) < task->releaseTime){
		return(BSTFindBackfillingPosition(scheduledTasks,Succ,task));
	}

	//on regarde si il y a de la place entre le succ et curr
	if( (task->processingTime) <= ((*((int*)Succ->key)) - (*((int*)curr->key) + (((Task*)curr->data)->processingTime))) ){
		// la tache peut rentrer entre la tache d'avant et celle d'apres
		if( (task->releaseTime + task->processingTime) <= (*((int*)Succ->key)) ){
			// la tache rentre bien dans l'espace (en fonction de son releaseTime)
			if( (task->releaseTime) < (*((int*)curr->key) + (((Task*)curr->data)->processingTime)) ){
				// la tache est placée juste apres la tache d'avant
				return (*((int*)curr->key) + (((Task*)curr->data)->processingTime));
			}
			else{
				// la tache est a un endroit quelconque entre les deux autres taches
				return task->releaseTime;
			}
		}
	}
	else {
		return(BSTFindBackfillingPosition(scheduledTasks,Succ,task));
	}

	return -1;
}

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute
 * dans la structure scheduledTasks de type arbre binaire de recherche.
 * La date de début dépend de l'application du backfilling (backfilling=1) ou pas (backfilling=0).
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 *      Utiliser la fonction récursive findBackfillingPosition.
 */
static int BSTFindStartingTime(const BSTree *scheduledTasks, const Task* task, int backfilling) {
	assert(scheduledTasks != NULL);
	assert (task != NULL);

	//si il y n'y pas de valeur dans l'arbre on place la première
	if (scheduledTasks->root == NULL){
		return task->releaseTime;
	}

	//on se met sur le plus petit noeud de l'arbre en point de départ
	int startingTime = BSTFindBackfillingPosition(scheduledTasks, BSTMin(scheduledTasks->root), task);
	BSTNode* i = scheduledTasks->root;

	if( (backfilling == 0) || ((backfilling == 1) && (startingTime == -1)) ){ // pas de backfilling
		// on ajoute a la fin
		BSTNode* Max = i;
		while(Max->right != NULL){ // BSTNode* Max = BSTMax(i); sans recursion
			Max = Max->right;
		}

		if((*((int*)Max->key) + ((Task*)Max->data)->processingTime) <= task->releaseTime){ // le debut de la tache est apres la derniere tache
			startingTime = task->releaseTime;
		}
		else{ // le debut de la tache est pendant la derniere tache, on renvoi la fin de la derniere tache
			startingTime = (*((int*)Max->key) + ((Task*)Max->data)->processingTime);
		}
	}

	else if( (backfilling == 1) && (startingTime != -1) ){ // backfilling
		startingTime = startingTime;
	}

	else{
		error("backfilling non comforme");
	}

	return startingTime;
}

int findStartingTime(const Schedule *sched, const Task* task) {
	switch (sched->structtype) {
	case OL:
		return OLFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	case BST:
		return BSTFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	case EBST:
		return BSTFindStartingTime(sched->scheduledTasks, task, sched->backfilling);
		break;
	default:
		error("Schedule:findStartingTime : invalid data structure.");
		return -1;
	}
}

void computeSchedule(Schedule *sched, const Instance I) {
	for (LNode* curr = I->head; curr; curr = curr->succ)
		addTaskToSchedule(sched, findStartingTime(sched, curr->data), curr->data);
}

/*****************************************************************************
 * Save schedule
 *****************************************************************************/

/**
 * @brief
 * Sauvegarder l'ordonnancement représenté par la liste ordonnée scheduledTasks
 * dans le ficher indiqué par le descripteur fd.
 * NB : Procédure itérative
 */
static void OLSaveSchedule(const OList* scheduledTasks, FILE* fd) {
	assert(scheduledTasks != NULL);
	assert(fd != NULL);

	OLNode *i = scheduledTasks->head;
	while(i){
		fprintf(fd, "%s\t%d\t%d\t%d\t%d\t%d\n", (((Task*)i->data)->id), (((Task*)i->data)->processingTime), (((Task*)i->data)->releaseTime), (((Task*)i->data)->deadline), (((Task*)i->data)->weight), (*((int*)i->key)));
		i = i->succ;
	}
}

/**
 * @brief
 * Sauvegarder l'ordonnancement représenté par le sous-arbre raciné au nœud curr
 * dans le ficher indiqué par le descripteur fd.
 * NB : procédure récursive
 *      Pensez à un parcours infixe.
 */
static void BSTSaveSchedule(const BSTNode* curr, FILE* fd) {
	assert(curr != NULL);
	assert(fd != NULL);

	if(curr->left != NULL){
		BSTSaveSchedule(curr->left, fd);
	}

	fprintf(fd, "%s\t%d\t%d\t%d\t%d\t%d\n", (((Task*)curr->data)->id), (((Task*)curr->data)->processingTime), (((Task*)curr->data)->releaseTime), (((Task*)curr->data)->deadline), (((Task*)curr->data)->weight), (*((int*)curr->key)));

	if(curr->right != NULL){
		BSTSaveSchedule(curr->right, fd);
	}
}

void saveSchedule(const Schedule * sched, char* filename) {
	FILE* fd;
	if ((fd = fopen(filename, "w")) == NULL)
		error("schedule:saveSchedule : Error while opening the file");

	switch (sched->structtype) {
	case OL:
		OLSaveSchedule(sched->scheduledTasks, fd);
		break;
	case BST:
		BSTSaveSchedule(((BSTree*) sched->scheduledTasks)->root, fd);
		break;
	case EBST:
		BSTSaveSchedule(((BSTree*) sched->scheduledTasks)->root, fd);
		break;
	default:
		error("Schedule:saveSchedule : invalid data structure.");
		break;
	}
	fclose(fd);
}

/*****************************************************************************
 * OBJECTIFS
 *****************************************************************************/

/////////////////////// makespan ///////////////////////

long makespan(const Schedule * sched) {
	assert(sched != NULL);

	if(sched->structtype == OL){
		if(((OList*)sched->scheduledTasks)->head == NULL){
			return 0;
		}
		else if(((OList*)sched->scheduledTasks)->head != NULL && ((OList*)sched->scheduledTasks)->tail == NULL){ // lista a 1 element
			return (*((int*)((OList*)sched->scheduledTasks)->head->key) + (((Task*)((OList*)sched->scheduledTasks)->head->data)->processingTime));
		}
		else{
			return (*((int*)((OList*)sched->scheduledTasks)->tail->key) + (((Task*)((OList*)sched->scheduledTasks)->tail->data)->processingTime));
		}
	}

	else if(sched->structtype == BST || sched->structtype == EBST){
		if(((BSTree*)sched->scheduledTasks)->root == NULL){
			return 0;
		}
		else if(((BSTree*)sched->scheduledTasks)->root->left == NULL && ((BSTree*)sched->scheduledTasks)->root->right == NULL){ // lista a 1 element
			return (*((int*)((BSTree*)sched->scheduledTasks)->root->key) + (((Task*)((BSTree*)sched->scheduledTasks)->root->data)->processingTime));
		}
		else{
			BSTNode *i = ((BSTree*)sched->scheduledTasks)->root;
			while(i->right != NULL){
				i = i->right;
			}
			return (*((int*)i->key) + (((Task*)i->data)->processingTime));
		}
	}

	else{
		error("Schedule:makespan : invalid data structure.");
		return -1;
	}
}

/////////////////////// SumWjCj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjCj(const OList* scheduledTasks) {
	assert(scheduledTasks != NULL);

	long cpt = 0;
	if(scheduledTasks->head != NULL){
		OLNode *i = scheduledTasks->head;
		while(i){
			cpt += ( (((Task*)i->data)->weight) * ((*((int*)i->key)) + (((Task*)i->data)->processingTime)) );
			i = i->succ;
		}
	}

	return cpt;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjCj(const BSTNode* curr) {
	if(curr != NULL){
		if((curr->left != NULL) && (curr->right != NULL)){
			return (((((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime)) ) + BSTSumWjCj(curr->left) + BSTSumWjCj(curr->right));
		}
		else if((curr->left != NULL) && (curr->right == NULL)){
			return (((((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime)) ) + BSTSumWjCj(curr->left));
		}
		else if((curr->left == NULL) && (curr->right != NULL)){
			return (((((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime)) ) + BSTSumWjCj(curr->right));
		}
		else{
			return ((((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime)));
		}
	}
	else{
		return 0;
	}
}

long SumWjCj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjCj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjCj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjCj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjCj : invalid data structure.");
		return -1;
	}
}

/////////////////////// SumWjFj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjFj(const OList* scheduledTasks) {
	assert(scheduledTasks != NULL);

	long cpt = 0;
	if(scheduledTasks->head != NULL){
		OLNode *i = scheduledTasks->head;
		while(i){
				cpt += ( (((Task*)i->data)->weight) * ((*((int*)i->key)) + (((Task*)i->data)->processingTime) - (((Task*)i->data)->releaseTime)) );
			i = i->succ;
		}
	}

	return cpt;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjFj(const BSTNode* curr) {
	if(curr != NULL){
		if((curr->left != NULL) && (curr->right != NULL)){
			return (( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->releaseTime)) ) + BSTSumWjFj(curr->left) + BSTSumWjFj(curr->right));
		}else if((curr->left != NULL) && (curr->right == NULL)){
			return (( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->releaseTime)) ) + BSTSumWjFj(curr->left));
		} else if((curr->left == NULL) && (curr->right != NULL)){
			return (( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->releaseTime)) ) + BSTSumWjFj(curr->right));
		}else{
			return ( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->releaseTime)) );
		}
	}else{
		return 0;
	}
}

long SumWjFj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjFj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjFj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjFj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjFj : invalid data structure.");
		return -1;
	}
}

/////////////////////// SumWjTj ///////////////////////

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement
 * représenté par la liste ordonnée scheduledTasks.
 */
static long OLSumWjTj(const OList* scheduledTasks) {
	assert(scheduledTasks != NULL);

	long cpt = 0;
	if(scheduledTasks->head != NULL){
		OLNode *i = scheduledTasks->head;
		while(i){
			if( (*((int*)i->key)) + (((Task*)i->data)->processingTime) - (((Task*)i->data)->deadline) > 0 ){
				cpt += ( (((Task*)i->data)->weight) * ((*((int*)i->key)) + (((Task*)i->data)->processingTime) - (((Task*)i->data)->deadline)) );
			}
			i = i->succ;
		}
	}

	return cpt;
}

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement
 * représenté par l'arbre binaire de recherche raciné au nœud curr.
 * NB : fonction récursive
 */
static long BSTSumWjTj(const BSTNode* curr) {
	if(curr != NULL){
		if((curr->left != NULL) && (curr->right != NULL)){
			if( (*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline) > 0 ){
				return ( ( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline)) ) + BSTSumWjTj(curr->left) + BSTSumWjTj(curr->right));
			}else{
				return (0 + BSTSumWjTj(curr->left) + BSTSumWjTj(curr->right));
			}

		}else if((curr->left != NULL) && (curr->right == NULL)){
			if( (*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline) > 0 ){
				return ( ( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline)) ) + BSTSumWjTj(curr->left));
			}else{
				return (0 + BSTSumWjTj(curr->left));
			}

		} else if((curr->left == NULL) && (curr->right != NULL)){
			if( (*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline) > 0 ){
				return ( ( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline)) ) + BSTSumWjTj(curr->right));
			}else{
				return (0 + BSTSumWjTj(curr->right));
			}

		}else{
			if( (*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline) > 0 ){
				return ( (((Task*)curr->data)->weight) * ((*((int*)curr->key)) + (((Task*)curr->data)->processingTime) - (((Task*)curr->data)->deadline)) );
			}else{
				return 0;
			}
		}

	}else{
		return 0;
	}
}

long SumWjTj(const Schedule* sched) {
	switch (sched->structtype) {
	case OL:
		return OLSumWjTj(sched->scheduledTasks);
		break;
	case BST:
		return BSTSumWjTj(((BSTree*) sched->scheduledTasks)->root);
		break;
	case EBST:
		return BSTSumWjTj(((BSTree*) sched->scheduledTasks)->root);
		break;
	default:
		error("Schedule:SumWjTj : invalid data structure.");
		return -1;
	}
}
