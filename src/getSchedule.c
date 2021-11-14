#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"
#include "getSchedule.h"


// =============== FONCTION POUR JAVA ===============

/**
 * @brief
 * char * inFileName : le nom du fichier qui contient une instance.
 * char * outFileName : le nom du fichier qui contiendra l’ordonnancement créé.
 * int datastructure : indique le type de structure des données à utiliser dans la création de
l’ordonnancement (0-listes ordonnées, 1-arbres binaires de recherche (non équilibrés), 2-arbres
binaires de recherche équilibrés).
 * int order : indique l’ordre de traitement (0-SPT, 1-LPT, 2-WSPT, 3-FCFS).
 * int backfilling : indique si on fait du remplissage ou pas (0-pas de remplissage, 1-remplissage).
 */
JNIEXPORT void JNICALL Java_application_Controller_1MainPage_getSchedule (JNIEnv *env, jobject obj, jstring inFileName, jstring outFileName, jint datastructure, jint order, jint backfilling){

  const char *fileIn = (*env)->GetStringUTFChars(env, inFileName, NULL);
  const char *fileOut = (*env)->GetStringUTFChars(env, outFileName, NULL);

  Instance instance = readInstance(fileIn);
	instance = reorderInstance(instance, datastructure, order);

	Schedule* sched = newSchedule(datastructure, backfilling);
	computeSchedule(sched, instance);

	saveSchedule(sched, fileOut);

	freeSchedule(sched);
	freeInstance(instance, 1);
}
