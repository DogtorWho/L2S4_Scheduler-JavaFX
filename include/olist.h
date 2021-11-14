#ifndef _OLIST_
#define _OLIST_

/**
 * @brief
 * Liste doublement chaînée ordonnée
 */

/**
 * @brief
 * Un élément d'une liste doublement chaînée ordonnée contient :
 * (+) une clé (key),
 * (+) une donnée (data),
 * (+) une référence (suc) à l'élément suivant, et
 * (+) une référence (pred) à l'élément précédent.
 */
typedef struct OrderedListNode {
	void * key;
	void * data;
	struct OrderedListNode * succ, * pred;
} OLNode;

/**
 * @brief
 * La liste doublement chaînée ordonnée est une structure contenant
 * (+) une référence (head) sur son premier élément,
 * (+) une référence (tail) sur son dernier élément,
 * (+) le nombre d'éléments,
 * (+) un pointeur de fonction pour comparer ses clés,
 * (+) un pointeur de fonction pour afficher ses clés,
 * (+) un pointeur de fonction pour afficher ses données,
 * (+) un pointeur de fonction pour libérer la mémoire de ses clés, et
 * (+) un pointeur de fonction pour libérer la mémoire de ses données.
 */
typedef struct OrderedList {
	OLNode * head, * tail;
	int numelm;
	int (*preceed)(const void* a, const void* b);
	void (*viewKey)(const void* key);
	void (*viewData)(const void* data);
	void (*freeKey)(void* key);
	void (*freeData)(void* data);
} OList;

/**
 * @brief
 * Construire une liste doublement chaînée ordonnée vide
 */
OList * newOList(int (*preceed)(const void*, const void*),
				void (*viewKey)(const void*), void (*viewData)(const void*),
				void (*freeKey)(void*), void (*freeData)(void*));

/**
 * @brief
 * Plusieurs possibilités de supprimer la liste ordonnée L :
 * (+) Si le paramètre deleteKey vaut 0
 *     Alors les clés (key) des éléments de la liste L ne sont pas supprimées ;
 * (+) Si le paramètre deleteData vaut 1
 *     Alors le pointeur de fonction freeKey de la structure OrderedList
 *     va servir à supprimer les clés (key) des éléments de la liste L.
 * 
 * (+) Si le paramètre deleteData vaut 0
 *     Alors les données (data) référencées par les éléments
 *     de la liste L ne sont pas supprimées ;
 * (+) Si le paramètre deleteData vaut 1
 *     Alors le pointeur de fonction freeData de la structure OrderedList
 *     va servir à supprimer les données (data) référencées par
 *     les éléments de la liste L.
 */
void freeOList(OList * L, int deleteKey, int deleteData);

/**
 * @brief
 * Afficher les éléments de la liste ordonnée.
 * Chaque élément est affiché grâce aux pointeurs de fonction
 * de la structure OrderedList :
 * (+) viewKey pour la clé
 * (+) viewData pour les données
 */
void viewOList(const OList * L);

/**
 * @brief
 * Ajouter dans la liste ordonnée L un élément de clé key et de donnée data.
 */
void OListInsert(OList * L, void * key, void * data);

/**
 * @brief
 * Transformer la liste doublement chaînée ordonnée L
 * à une liste doublement chaînée classique (non-ordonnée).
 */
List* OListToList(const OList* L);

#endif // _OLIST_