#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "bstree.h"

/*********************************************************************
 * Arbre Binaire de Recherche (non-équilibré).
 *********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche (non-équilibré).
 * Renvoie le nouveau nœud créé.
 */
static BSTNode* newBSTNode(void* key, void* data) {
	BSTNode *Node = malloc(sizeof(BSTNode));
	assert(Node != NULL);

	Node->key = key;
	Node->data = data;
	Node->left = NULL;
	Node->right = NULL;
	//Node->bfactor = 0;

	return Node;
}

BSTree * newBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {

	BSTree *Tree = malloc(sizeof(BSTree));
	assert(Tree != NULL);

	Tree->root = NULL;
	Tree->numelm = 0;
	Tree->preceed = preceed;
	Tree->viewKey = viewKey;
	Tree->viewData = viewData;
	Tree->freeKey = freeKey;
	Tree->freeData = freeData;
	Tree->balanced = 0;

	return Tree;
}

/**
 * @brief
 * Insérer un nouveau nœud de clé key et donnée data
 * au sous-arbre binaire de recherche (non-équilibré) raciné au nœud curr.
 * Les clés sont comparées en utilisant la fonction preceed.
 * NB : fonction récursive.
 */
	/*Thiebault*/
static BSTNode* insertBSTNode(BSTNode* curr, void* key, void* data, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);
	if ((preceed(curr->key, key)) == 1){ // key > curr
		if (curr->right == NULL){
			curr->right = newBSTNode(key,data);
			return curr;
		}
		else{
			return insertBSTNode(curr->right,key,data,preceed);
		}
	}
	else{ // key < curr
		if (curr->left == NULL){
			curr->left = newBSTNode(key,data);
			return curr;
		}else{
			return insertBSTNode(curr->left,key,data,preceed);
		}
	}
}

/**
 * NB : Utiliser la fonction récursive insertBSTNode.
 */
 	/*Thiebault*/
void BSTreeInsert(BSTree* T, void* key, void* data) {
	if(T->root == NULL){
		T->root = newBSTNode(key,data);
	}
	else{
		insertBSTNode(T->root, key, data, T->preceed);
	}
	T->numelm++;
}

/*********************************************************************
 * Arbre Binaire de Recherche Équilibré
 *********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche équilibré (EBST).
 * Renvoie le nouveau nœud créé.
 * N'oubliez pas à initialiser le facteur d'équilibre.
 */
	/*Thiebault*/
static BSTNode* newEBSTNode(void* key, void* data) {
	 BSTNode *Node = malloc(sizeof(BSTNode));
	 assert(Node != NULL);

	 Node->key = key;
	 Node->data = data;
	 Node->left = NULL;
	 Node->right = NULL;
   Node->bfactor = 0;

	 return Node;
}

	/*Thiebault*/
BSTree * newEBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*)) {

	BSTree *Tree = malloc(sizeof(BSTree));
	assert(Tree != NULL);

	Tree->root = NULL;
	Tree->numelm = 0;
	Tree->preceed = preceed;
	Tree->viewKey = viewKey;
	Tree->viewData = viewData;
	Tree->freeKey = freeKey;
	Tree->freeData = freeData;
	Tree->balanced = 0;

	return Tree;
}

/**
 * @brief
 * Effectuer une rotation gauche autour du nœud y.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 4 cas à considérer :
 * (+) bfactor(y)=-2 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=1
 * (+) bfactor(y)=-1 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=0
 * Assurez vous que le nœud y ainsi que son fils droit existent.
 */
static BSTNode* rotateLeft(BSTNode* y) {
	assert(y);
	assert(y->right);

	BSTNode *x = y->right;
	BSTNode *tmp = x->left;
	x->left = y;
	y->right = tmp;

	if((y->bfactor == -2) && (x->bfactor == -1)){
		y->bfactor = 0;
		x->bfactor = 0;
	}
	else if((y->bfactor == -2) && (x->bfactor == -2)){
		y->bfactor = 1;
		x->bfactor = 0;
	}
	else if((y->bfactor == -1) && (x->bfactor == 1)){
		y->bfactor = 0;
		x->bfactor = 2;
	}
	else if((y->bfactor == -1) && (x->bfactor == -1)){
		y->bfactor = 1;
		x->bfactor = 1;
	}
	else if((y->bfactor == -1) && (x->bfactor == 0)){
		y->bfactor = 0;
		x->bfactor = 1;
	}

	return x;
}

/**
 * @brief
 * Effectuer une rotation droite autour du nœud x.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds modifiés.
 * Il y a 4 cas à considérer :
 * (+) bfactor(x)=2 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=-1
 * (+) bfactor(x)=1 et bfactor(x->left)=0
 * Assurez vous que le nœud x ainsi que son fils gauche existent.
 */
static BSTNode* rotateRight(BSTNode* x) {
	assert(x);
	assert(x->left);

	BSTNode *y = x->left;
	BSTNode *tmp = y->right;
	y->right = x;
	x->left = tmp;

	if((x->bfactor == 2) && (y->bfactor == 1)){
		x->bfactor = 0;
		y->bfactor = 0;
	}
	else if((x->bfactor == 2) && (y->bfactor == 2)){
		x->bfactor = -1;
		y->bfactor = 0;
	}
	else if((x->bfactor == 1) && (y->bfactor == 1)){
		x->bfactor = -1;
		y->bfactor = -1;
	}
	else if((x->bfactor == 1) && (y->bfactor == -1)){
		x->bfactor = 0;
		y->bfactor = -2;
	}
	else if((x->bfactor == 1) && (y->bfactor == 0)){
		x->bfactor = 0;
		y->bfactor = -1;
	}

	return y;
}

/**
 * @brief
 * Insérer un nouveau nœud de clé key et donnée data
 * au sous-arbre binaire de recherche équilibré raciné au nœud curr.
 * Les clés sont comparées en utilisant la fonction preceed.
 * N'oubliez pas à mettre à jour la facteur d'équilibre du nœud curr.
 * N'oubliez pas à faire les rotations nécessaires (4 cas à considérer).
 * NB : fonction récursive.
 */
 	/*Thiebault*/
static BSTNode* insertEBSTNode(BSTNode* curr, void* key, void* data, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);
	assert(key != NULL);

	if ((preceed(curr->key, key)) == 1){
		if (curr->right == NULL){
			curr->right = newEBSTNode(key, data);
			curr->bfactor--;
		}
		else{
			curr->right = insertEBSTNode(curr->right, key, data, preceed);
			if(curr->right->bfactor != 0){
				curr->bfactor--;
			}
		}
	}
	else{
		if (curr->left == NULL){
			curr->left = newEBSTNode(key, data);
			curr->bfactor++;
		}
		else{
		 	curr->left = insertEBSTNode(curr->left, key, data, preceed);
			if(curr->left->bfactor != 0){
				curr->bfactor++;
			}
		}
	}

	if (curr->left != NULL){
		// cas 1
		if( (curr->bfactor == 2) && (curr->left->bfactor == 1) ){
			curr = rotateRight(curr);
		}

		// cas 2
		if( (curr->bfactor == 2) && (curr->left->bfactor == -1) ){
			curr->left = rotateLeft(curr->left);
			curr = rotateRight(curr);
		}
	}

	if (curr->right != NULL){
		// cas 3
		if( (curr->bfactor == -2) && (curr->right->bfactor == -1) ){
			curr = rotateLeft(curr);
		}

		// cas 4
		if( (curr->bfactor == -2) && (curr->right->bfactor == 1) ){
			curr->right = rotateRight(curr->right);
			curr = rotateLeft(curr);
		}
	}

	return curr;
}

/**
 * NB : Utiliser la fonction récursive insertEBSTNode.
 */
 	/*Thiebault*/
void EBSTreeInsert(BSTree* T, void* key, void* data) {
	if(T->root == NULL){
		T->root = newEBSTNode(key,data);
	}
	else{
		T->root = insertEBSTNode(T->root, key, data, T->preceed);
	}
	T->numelm++;
}

/*********************************************************************
 * Fonctions communes pour :
 * Arbre Binaire de Recherche (non-équilibré), et
 * Arbre Binaire de Recherche Équilibré.
 *********************************************************************/

/**
 * @brief
 * Libérer récursivement le sous-arbre raciné au nœud curr.
 * Dans le cas où le pointeur de fonction freeKey (resp. freeData) n'est pas NULL,
 * la mémoire de la clé (resp. de la donnée) du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 */
static void freeBSTNode(BSTNode* curr, void (*freeKey)(void*), void (*freeData)(void*)) {
	if (curr != NULL) {
		if(freeKey == NULL && freeData == NULL){ //key et data pas supprime
			freeBSTNode(curr->left, NULL, NULL);
			freeBSTNode(curr->right, NULL, NULL);

			free(curr);
		}
		else if(freeKey != NULL && freeData == NULL){ //key supprime et data pas supprime
			freeBSTNode(curr->left, freeKey, NULL);
			freeBSTNode(curr->right, freeKey, NULL);

			freeKey(curr->key);
			free(curr);
		}
		else if(freeKey == NULL && freeData != NULL){ //key pas supprime et data supprime
			freeBSTNode(curr->left, NULL, freeData);
			freeBSTNode(curr->right, NULL, freeData);

			freeData(curr->data);
			free(curr);
		}
		else{ //key et data supprime
			freeBSTNode(curr->left, freeKey, freeData);
			freeBSTNode(curr->right, freeKey, freeData);

			freeKey(curr->key);
			freeData(curr->data);
			free(curr);
		}
	}
}

/**
 * NB : Utiliser la procédure récursive freeBSTNode.
 * Vous devez utiliser les bons paramètres freeKey et freeData
 * par rapport aux valeurs deleteKey et deleteData.
 */
void freeBSTree(BSTree* T, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);

	if(deleteKey == 0 && deleteData == 0){ //key et data pas supprime
		freeBSTNode(T->root, NULL, NULL);
	}
	else if(deleteKey == 1 && deleteData == 0){ //key supprime et data pas supprime
		freeBSTNode(T->root, T->freeKey, NULL);
	}
	else if(deleteKey == 0 && deleteData == 1){ //key pas supprime et data supprime
		freeBSTNode(T->root, NULL, T->freeData);
	}
	else{ //key et data supprime
		freeBSTNode(T->root, T->freeKey, T->freeData);
	}
	free(T);
}

/**
 * @brief
 * Afficher récursivement le sous-arbre raciné au nœud curr
 * en utilisant un ordre infixe.
 * NB : procédure récursive.
 */
static void inorderView(BSTNode *curr, void (*viewKey)(const void*), void (*viewData)(const void*)) {
	if (curr != NULL) {
		inorderView(curr->left, viewKey, viewData);

		printf(" - Key : ");
		viewKey(curr->key);
		printf("  |   Data : ");
		viewData(curr->data);

		if(curr->bfactor > -3 && curr->bfactor < 3){
			printf("  |   bFactor : %d", curr->bfactor);
		}
		printf("\n");

		printf("-------------------------------------------------------\n");

		inorderView(curr->right, viewKey, viewData);
	}
}

/**
 * NB : Utiliser la procédure récursive inorderView.
 */
void viewBSTree(const BSTree* T) {
	printf("   = Affichage de l'arbre a %d elements =\n\n", T->numelm);
	if(T->numelm == 0){
		printf(" - Arbre vide, rien a afficher\n");
		printf("-------------------------------------------------------\n");
	}
	else{
		inorderView(T->root, T->viewKey, T->viewData);
	}
}

/**
 * @brief
 * Transformer récursivement le sous-arbre raciné au nœud curr
 * à une liste doublement chaînée.
 * L'argument list est en entrée/sortie.
 * La liste va contenir les éléments de l'arbre dans l'ordre infixe.
 * NB : procédure récursive.
 */
 	/*Thiebault*/
static void treetolist(BSTNode* curr, List* list) {
	if(curr != NULL){
		treetolist(curr->left, list);

		listInsertLast(list, curr->data);

		treetolist(curr->right, list);
	}
}

/**
 * NB : Utiliser la procédure récursive treetolist.
 */
 	/*Thiebault*/
List* BSTreeToList(const BSTree* T) {
	List* list = newList(T->viewData, T->freeData);
	treetolist(T->root, list);
	return list;
}

BSTNode* BSTMin(BSTNode* node) {
	assert(node != NULL);

	if (node->left != NULL) {
		return BSTMin(node->left);
	}
	else{
		return node;
	}
}

BSTNode* BSTMax(BSTNode* node) {
	assert(node != NULL);

	if (node->right != NULL) {
		return BSTMax(node->right);
	}
	else{
		return node;
	}
}

/**
 * @brief
 * Chercher récursivement dans le sous-arbre raciné au nœud curr
 * et renvoyer le noeud qui contient la clé qui précède la clé key (prédécesseur).
 * Le pointeur de fonction preceed donne l'ordre entre deux clés.
 * NB : fonction récursive.
 */
 	/*Thiebault*/
static BSTNode* predecessor(BSTNode* curr, void* key, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);

	if(preceed(key, curr->key) == 1){ // key < curr
		if(curr->left != NULL){
			BSTNode* tmp = predecessor(curr->left, key, preceed);
			if(tmp != NULL){
				return tmp;
			}
			else{
				if(preceed(curr->key, key) == 1){
					return curr;
				}
			}
		}
	}
	else if(preceed(curr->key, key) == 1){ // key > curr
		if(curr->right != NULL){
			BSTNode* tmp = predecessor(curr->right, key, preceed);
			if(tmp != NULL){
				return tmp;
			}
			else{
				if(preceed(curr->key, key) == 1){
					return curr;
				}
			}
		}
	}
	else if(curr->key == key){ // key = curr
		if(curr->left != NULL){
			return BSTMax(curr->left);
		}
	}

	return NULL;
}

/**
 * NB : Utiliser la fonction récursive predecessor.
 */
 	/*Thiebault*/
BSTNode * findPredecessor(const BSTree * T, const BSTNode* node) {
	assert(T != NULL);
	assert(T->root != NULL);
	assert(node != NULL);
	assert(node->key != NULL);

	return predecessor(T->root, node->key, T->preceed);
}

/**
 * @brief
 * Chercher récursivement dans le sous-arbre raciné au nœud curr
 * et renvoyer le noeud qui contient la clé qui succède la clé key (successeur).
 * Le pointeur de fonction preceed donne l'ordre entre deux clés.
 * NB : fonction récursive.
 */
 	/*Thiebault*/
static BSTNode* successor(BSTNode* curr, void* key, int (*preceed)(const void*, const void*)) {
	assert(curr != NULL);

	if(preceed(key, curr->key) == 1){ // key < curr
		if(curr->left != NULL){
			BSTNode* tmp = successor(curr->left, key, preceed);
			if(tmp != NULL){
				return tmp;
			}
			else{
				if(preceed(key, curr->key) == 1){
					return curr;
				}
			}
		}
	}
	else if(preceed(curr->key, key) == 1){ // key > curr
		if(curr->right != NULL){
			BSTNode* tmp = successor(curr->right, key, preceed);
			if(tmp != NULL){
				return tmp;
			}
			else{
				if(preceed(key, curr->key) == 1){
					return curr;
				}
			}
		}
	}
	else if(curr->key == key){ // key = curr
		if(curr->right != NULL){
			return BSTMin(curr->right);
		}
	}
	return NULL;
}

/**
 * NB : Utiliser la fonction récursive successor.
 */
 	/*Thiebault*/
BSTNode * findSuccessor(const BSTree * T, const BSTNode* node) {
	assert(T != NULL);
	assert(T->root != NULL);
	assert(node != NULL);
	assert(node->key != NULL);

	return successor(T->root, node->key, T->preceed);
}
