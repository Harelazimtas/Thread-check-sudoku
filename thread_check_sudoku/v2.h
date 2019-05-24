/*
 * v2.h
 *
 *  Created on: 18 May 2019
 *      Author: harel
 */

#ifndef V2_H_
#define V2_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUMTHREAD 27
#define MAX 9
#define THREE 3
#define MAXSIZE 100
#define N 5

typedef struct{
	int mission[NUMTHREAD];
	int sudoku[MAX][MAX];
	int counter;
	int solution;
	pthread_cond_t *cond;
	pthread_mutex_t *mutexGetMission;
	pthread_mutex_t *mutexResult;
}shareMemory;

int readFromFile(char *filename);
int initShareStruct(pthread_mutex_t *solution,pthread_mutex_t *misson,pthread_cond_t *cond);
void *checksudoku();
int checkColumn(int numColumn);
int checkRow(int numRow);
int checkMatrix(int numMatrix);

#endif /* V2_H_ */
