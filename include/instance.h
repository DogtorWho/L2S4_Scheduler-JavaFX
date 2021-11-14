#ifndef _INSTANCE_
#define _INSTANCE_

/**
 * @brief
 * Une instance d'un problème d'ordonnancement.
 */

/**
 * @brief
 * Une tâche est caractérisée par les attributs suivants :
 * (+) un identifiant (id),
 * (+) sa durée (processingTime),
 * (+) sa date de libération (releaseTime),
 *     c'est-à-dire le moment à partir duquel on peut commencer à l'exécuter
 *     (ne pas confondre avec la date de début de son exécution;
 *     cela est une caractéristique de l'ordonnancement et pas de l'instance)
 * (+) sa date limite (dealine) qui pourrait être stricte ou pas, et
 * (+) son poids (weight) qui indique son importance/priorité.
 */
typedef struct Task {
    char* id;
    int processingTime;
    int releaseTime;
    int deadline;
    int weight;
} Task;

/**
 * @brief
 * Construire une nouvelle tâche.
 */
Task * newTask(char* id, int proctime, int reltime, int deadline, int weight);

/**
 * @brief
 * Afficher les caractéristiques de la tâche task.
 */
void viewTask(const void *task);

/**
 * @brief
 * Supprimer et libérer la mémoire de la tâche task.
 */
void freeTask(void *task);

/**
 * @brief
 * Une instance est une liste doublement chaînée des tâches.
 */
typedef List * Instance;

/**
 * @brief
 * Construire une nouvelle instance avec le contenu du fichier filename.
 */
Instance readInstance(char * filename);

/**
 * @brief
 * Afficher les tâches de l'instance I.
 */
void viewInstance(Instance I);

/**
 * @brief
 * Supprimer l'instance I.
 * Si le paramètre deleteData vaut 1,
 * les données correspondant aux tâches de l'instance sont aussi supprimer.
 */
void freeInstance(Instance I, int deleteData);

/**
 * @brief
 * Une énumération des différents ordres d'ordonnancement :
 * (+) SPT : Shortest Processing Time first
 * (+) LPT : Longest Processing Time first
 * (+) WSPT : Weighted Shortest Processing Time first
 * (+) FCFS : First Come First Serve
 */
typedef enum {SPT, LPT, WSPT, FCFS} Order;

/**
 * @brief
 * Trier l'instance I par rapport à l'ordre order.
 * Un algorithme rassemblant au "tri par insertion" est utilisé
 * en utilisant une structure de données ordonnée de type structtype.
 * L'algorithme insère un par un les éléments de l'instance d'entrée
 * dans la structure de données ordonnée.
 * A la fin, cette structure ordonnée est transformée en une instance (List*).
 * NB : l'argument I est en entrée/sortie.
 *      N'oubliez pas à libérer la mémoire de l'instance de l'entrée.
 */
Instance reorderInstance(Instance I, DataStructure structtype, Order order);

#endif // _INSTANCE_
