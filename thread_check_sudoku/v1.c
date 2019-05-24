/*
 * v1.c
 *
 *  Created on: 14 May 2019
 *      Author: harel
 */

#include "v1.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMTHREAD 27
#define MAX 9
#define THREE 3
#define MAXSIZE 100

int sudoku[MAX][MAX];
int arrayResult[NUMTHREAD];

int main(int argc, char *argv[]) {
	pthread_t thread[THREE][MAX];
	int tid[THREE][MAX];
	int  check, counter = 0;
	char *filename;
	filename = (char*) malloc(sizeof(char) * MAXSIZE);

	if (argc == 1) {
		printf("please enter name of file to check\n");
		scanf("%s", filename);
	}

	else {
		sscanf(argv[1], "%s", filename);
	}
	readFromFile(filename);
	//check row or column or matrix
	for (int j = 0; j < THREE; j++)
		for (int i = 0; i < MAX; i++) {
			tid[j][i] = i;
			if (j == 0) {
				check = pthread_create(&thread[j][i], NULL, checkRow,
						(void*) (&tid[j][i]));
			} else if (j == 1) {
				check = pthread_create(&thread[j][i], NULL, checkColumn,
						(void*) (&tid[j][i]));
			} else {
				check = pthread_create(&thread[j][i], NULL, checkMatrix,
						(void*) (&tid[j][i]));
			}

			if (check) {
				printf("error of create thread");
				return 0;
			}
		}
	for (int j = 0; j < THREE; j++)
		for (int i = 0; i < MAX; i++) {
			pthread_join(thread[j][i], NULL);
		}

	for (int i = 0; i < NUMTHREAD; i++) {
		if (arrayResult[i] == 1)
			counter++;
	}
	if (counter == 27)
		printf("solution is legal\n");
	else
		printf("solution is not legal\n");

	pthread_exit(NULL);
	return 1;

}

void *checkRow(void *numCheck) {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int numRowCheck = *(int*) numCheck;
	int bool = 1;
	for (int i = 0; i < MAX; i++) {
		if (bucket[sudoku[numRowCheck][i] - 1] == 1) {
			bool = 0;
			break;
		} else {
			bucket[sudoku[numRowCheck][i] - 1] = 1;
		}
	}
	arrayResult[numRowCheck] = bool;
	pthread_exit(NULL);
}

void *checkColumn(void *numCheck) {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int numColumnCheck = *(int*) numCheck;
	int bool = 1;
	for (int i = 0; i < MAX; i++) {
		if (bucket[sudoku[i][numColumnCheck] - 1] == 1) {
			bool = 0;
			break;
		} else {
			bucket[sudoku[i][numColumnCheck] - 1] = 1;
		}
	}
	arrayResult[numColumnCheck + MAX] = bool;
	pthread_exit(NULL);

}

void *checkMatrix(void *numCheck) {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int numMatCheck = *(int*) numCheck;
	int bool = 1, startRow, startColumn;
	startRow = (numMatCheck / THREE) * THREE;
	startColumn = (numMatCheck % THREE) * THREE;
	for (int j = startRow; j < startRow + 3; j++) {
		for (int i = startColumn; i < startColumn + THREE; i++) {
			if (bucket[sudoku[j][i] - 1] == 1) {
				bool = 0;
				break;
			} else {
				bucket[sudoku[j][i] - 1] = 1;
			}
		}
	}
	arrayResult[numMatCheck + MAX * 2] = bool;
	pthread_exit(NULL);
}


//read form file the sudoku
int readFromFile(char *filename) {
	FILE *fP = fopen(filename, "rb");
	if (!fP) {
		return 0;
	}
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			fscanf(fP, "%d ", &(sudoku[i][j]));
		}
	}
	fclose(fP);
	return 1;
}
