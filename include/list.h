#ifndef _LIST_
#define _LIST_

/**
 * @brief
 * Liste générique homogène doublement chaînée
 */

/**
 * @brief
 * Un élément d'une liste doublement chaînée contient
 * (+) une donnée (data),
 * (+) une référence (succ) à l'élément suivant, et
 * (+) une référence (pred) à l'élément précédent.
 */
typedef struct ListNode {
	void * data;
	struct ListNode * succ, * pred;
} LNode;

/**
 * @brief
 * La liste est une structure contenant :
 * (+) une référence (head) sur son premier élément,
 * (+) une référence (tail) sur son dernier élément,
 * (+) le nombre d'éléments,
 * (+) un pointeur de fonction pour afficher ses données, et
 * (+) un pointeur de fonction pour libérer la mémoire de ses données.
 */
typedef struct List {
	LNode * head, * tail;
	int numelm;
	void (*viewData)(const void* data);
	void (*freeData)(void* data);
} List;

/**
 * @brief
 * Construire et initialiser une liste vide.
 * Renvoie la nouvelle liste créée.
 */
List * newList(void (*viewData)(const void*), void (*freeData)(void*));

/**
 * @brief
 * Deux possibilités de supprimer la liste L :
 * (+) Si le paramètre deleteData vaut 0
 *     Alors les données (data) référencées par les éléments
 *     de la liste L ne sont pas supprimées ;
 * (+) Si le paramètre deleteData vaut 1
 *     Alors le pointeur de fonction freeData de la structure List
 *     va servir à supprimer les données (data) référencées par
 *     les éléments de la liste L.
 */
void freeList(List * L, int deleteData);

/**
 * @brief
 * Afficher les éléments de la liste.
 * Les données de chaque élément sont affichées grâce au pointeur
 * de fonction viewData de la structure List.
 */
void viewList(const List * L);

/**
 * @brief
 * Ajouter en tête de la liste L un élément de donnée data.
 */
void listInsertFirst(List * L, void * data);

/**
 * @brief
 * Ajouter à la fin de la liste L un élément de donnée data.
 */
void listInsertLast(List * L, void * data);

/**
 * @brief
 * Insérer un élément de liste de donnée data dans la liste L
 * après le pointeur d'élément ptrelm.
 * 
 * NB : le pointeur d'élément ptrelm désigne obligatoirement
 *      un élément de la liste L.
 */
void listInsertAfter(List * L, void * data, LNode * ptrelm);

/**
 * @brief
 * Supprimer et renvoyer le premier élément de liste L.
 * La mémoire de cet élément n'est pas libérée.
 * Assurez vous que la liste n'est pas vide.
 */
LNode* listRemoveFirst(List * L);

/**
 * @brief
 * Supprimer et renvoyer le dernier élément de liste L.
 * La mémoire de cet élément n'est pas libérée.
 * Assurez vous que la liste n'est pas vide.
 */
LNode* listRemoveLast(List * L);

/**
 * @brief
 * Supprimer et renvoyer l'élément de la liste L pointé par node.
 * La mémoire de cet élément n'est pas libérée.
 * L'élément est supposé appartenir effectivement à la liste.
 */
LNode* listRemoveNode(List * L, LNode * node);

#endif // _LIST_