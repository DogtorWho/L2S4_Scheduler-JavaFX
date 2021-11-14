#ifndef _SCHEDULE_
#define _SCHEDULE_

/**
 * @brief
 * La structure qui crée, calcule et gère un ordonnancement.
 */

/**
 * @brief
 * Un ordonnancement est une structure contenant :
 * (+) un pointeur (scheduledTasks) vers une structure de données ordonnée
 *     de type structtype qui contient les tâches ordonnancées
 *     (la clé correspond à la date de début de chaque tâche dans l'ordonnancement
 *     et la donnée correspond à la tâche elle même),
 * (+) le type de la structure de données utilisée (structtype), et
 * (+) une indication si on impose (backfilling=1) ou pas (backfilling=0)
 *     le backfilling des tâches.
 */
typedef struct Schedule {
	void * scheduledTasks;
	DataStructure structtype;
	int backfilling;
} Schedule;

/**
 * @brief
 * Construire un nouveau ordonnancement en définissant ses propriétés.
 */
Schedule * newSchedule(DataStructure structtype, int backfilling);

/**
 * @brief
 * Afficher les tâches de l'ordonnancement sched en appelant la procédure
 * qui correspond à la structure de données scheduledTasks.
 */
void viewSchedule(Schedule* sched);

/**
 * @brief
 * Ajouter la nouvelle tâche task dans l'ordonnancement sched.
 * La tâche commence son execution au temps startingTime (clé de la structure scheduledTasks).
 */
void addTaskToSchedule(Schedule* sched, int startingTime, Task* task);

/**
 * @brief
 * Supprimer l'ordonnancement sched et libérer la mémoire en appelant la procédure
 * qui correspond à la structure de données scheduledTasks.
 * La clé (date de début) est libérée mais pas la donnée (tâche).
 */
void freeSchedule(Schedule* sched);

/**
 * @brief
 * Trouver la date de début de la tâche task si on l'ajoute dans l'ordonnancement sched.
 * NB : La fonction n'ajoute pas la tâche dans l'ordonnancement !
 */
int findStartingTime(const Schedule *sched, const Task* task);

/**
 * @brief
 * Un ordonnancement pour l'instance I (ordonné ou pas) est calculé
 * et stocké dans la structure scheduledTasks de l'ordonnancement sched.
 */
void computeSchedule(Schedule *sched, const Instance I);

/**
 * @brief
 * Sauvegarder l'ordonnancement sched dans le ficher filename.
 */
void saveSchedule(const Schedule * sched, char* filename);

/**
 * @brief
 * Calculer le "makespan" de l'ordonnancement sched.
 */
long makespan(const Schedule * sched);

/**
 * @brief
 * Calculer la "somme pondérée de temps de fin" de l'ordonnancement sched.
 */
long SumWjCj(const Schedule* sched);

/**
 * @brief
 * Calculer la "somme pondérée de temps de réponse" de l'ordonnancement sched.
 */
long SumWjFj(const Schedule* sched);

/**
 * @brief
 * Calculer la "somme pondérée de temps de retard" de l'ordonnancement sched.
 */
long SumWjTj(const Schedule* sched);


#endif // _SCHEDULE_
