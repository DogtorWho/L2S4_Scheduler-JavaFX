#ifndef _UTILITIES_
#define _UTILITIES_

/**
 * @brief
 * Une énumération des différentes structures de données :
 * (+) OL : ordered lists
 * (+) BST : Binary Search Tree
 * (+) EBST : Equilibrated Binary Search Tree
 */
typedef enum {OL, BST, EBST} DataStructure;

/**
 * @brief
 * Afficher le message d'erreur msg et interrompre l'exécution du programme.
 */
void error(char * msg);

/**
 * @brief
 * Renvoyer la valeur maximale parmi les entiers a et b.
 */
int max(int a, int b);

/**
 * @brief
 * Renvoyer la valeur minimale parmi les entiers a et b.
 */
int min(int a, int b);

/**
 * @brief
 * Afficher l'entier indiqué par le pointeur i.
 */
void viewInt(const void *i);

/**
 * @brief
 * Libérer la mémoire de l'entier indiqué par le pointeur i.
 */
void freeInt(void * i);

/**
 * @brief
 * Comparer les entiers a et b.
 * Renvoyer 1, si a<b.
 * Sinon, renvoyer 0.
 */
int compareInt(const void* a, const void* b);

#endif // _UTILITIES_