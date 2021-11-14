#ifndef _BSTREE_
#define _BSTREE_

/**
 * @brief
 * Arbre binaire de recherche générique
 * équilibré ou non-équilibré
 */

/**
 * @brief
 * Un élément d'un arbre binaire de recherche contient
 * (+) une clé (key),
 * (+) une donnée (data),
 * (+) le facteur d'équilibre (bfactor) :
 *     différence entre la hauteur du fils gauche et la hauteur du fils droit
 *     (uniquement dans le cas d'un arbre binaire de recherche équilibré)
 *     NB : le facteur d'équilibre d'une feuille est égal à 0,
 * (+) la référence left vers le fils gauche, et
 * (+) la référence right vers le fils droit.
 */
typedef struct BinarySearchTreeNode {
	void * key;
	void * data;
	int bfactor;
	struct BinarySearchTreeNode * left;
	struct BinarySearchTreeNode * right;
} BSTNode;

/**
 * L'arbre binaire de recherche est une structure contenant :
 * (+) une référence (root) sur sa racine,
 * (+) le nombre d'éléments,
 * (+) un pointeur de fonction pour comparer ses clés,
 * (+) un pointeur de fonction pour afficher ses clés,
 * (+) un pointeur de fonction pour afficher ses données,
 * (+) un pointeur de fonction pour libérer la mémoire de ses clés,
 * (+) un pointeur de fonction pour libérer la mémoire de ses données, et
 * (+) une indication (balanced) si l'arbre binaire de recherche
 *     est équilibré (balanced=1) ou pas (balanced=0).
 */
typedef struct BinarySearchTree {
	BSTNode * root;
	int numelm;
	int (*preceed)(const void* a, const void* b);
	void (*viewKey)(const void* key);
	void (*viewData)(const void* data);
	void (*freeKey)(void* key);
	void (*freeData)(void* data);
	int balanced;
} BSTree;

/**
 * @brief
 * Construire un arbre binaire de recherche (non-équilibré) vide
 */
BSTree * newBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*));

/**
 * @brief
 * Construire un arbre binaire de recherche équilibre vide
 */
BSTree * newEBSTree(int (*preceed)(const void*, const void*),
					void (*viewKey)(const void*), void (*viewData)(const void*),
					void (*freeKey)(void*), void (*freeData)(void*));

/**
 * @brief
 * Plusieurs possibilités de supprimer l'arbre binaire de recherche T :
 * (+) Si le paramètre deleteKey vaut 0
 *     Alors les clés (key) des éléments de l'arbre T ne sont pas supprimées ;
 * (+) Si le paramètre deleteKey vaut 1
 *     Alors le pointeur de fonction freeKey de la structure BinarySearchTree
 *     va servir à supprimer les clés (key) des éléments de l'arbre T.
 *
 * (+) Si le paramètre deleteData vaut 0
 *     Alors les données (data) référencées par les éléments
 *     de l'arbre T ne sont pas supprimées ;
 * (+) Si le paramètre deleteData vaut 1
 *     Alors le pointeur de fonction freeData de la structure BinarySearchTree
 *     va servir à supprimer les données (data) référencées par
 *     les éléments de l'arbre T.
 */
void freeBSTree(BSTree * T, int deleteKey, int deleteData);

/**
 * @brief
 * Afficher les éléments de l'arbre binaire de recherche T.
 * Chaque élément est affiché grâce aux pointeurs de fonction
 * de la structure BinarySearchTree:
 * (+) viewKey pour la clé
 * (+) viewData pour les données
 * L'arbre est affiché dans l'ordre infixe.
 */
void viewBSTree(const BSTree* T);

/**
 * @brief
 * Ajouter dans l'arbre binaire de recherche (non-équilibré) T un élément de clé key et de donnée data.
 */
void BSTreeInsert(BSTree* T, void* key, void* data);

/**
 * @brief
 * Ajouter dans l'arbre binaire de recherche équilibré T un élément de clé key et de donnée data.
 */
void EBSTreeInsert(BSTree* T, void* key, void* data);

/**
 * @brief
 * Transformer l'arbre binaire de recherche T à une liste doublement chaînée.
 */
List* BSTreeToList(const BSTree* T);

/**
 * @brief
 * Trouver et renvoyer le nœud de clé minimum du sous-arbre raciné au nœud node.
 * NB : fonction récursive.
 * Assurez vous que node existe.
 */
BSTNode* BSTMin(BSTNode* node);

/**
 * @brief
 * Trouver et renvoyer le nœud de clé maximum du sous-arbre raciné au nœud node.
 * NB : fonction récursive.
 * Assurez vous que node existe.
 */
BSTNode* BSTMax(BSTNode* node);

/**
 * @brief
 * Trouver et renvoyer le nœud qui contient la clé la plus grande
 * qui est plus petite que la clé du nœud node (prédécesseur du node) de l'arbre T.
 * Assurez vous que T n'est pas vide et que node existe.
 */
BSTNode * findPredecessor(const BSTree * T, const BSTNode* node);

/**
 * @brief
 * Trouver et renvoyer le nœud qui contient la clé plus petite
 * qui est plus grande que la clé du nœud node (successeur du node) de l'arbre T.
 * Assurez vous que T n'est pas vide et que node existe.
 */
BSTNode * findSuccessor(const BSTree * T, const BSTNode* node);

#endif // _BSTREE_