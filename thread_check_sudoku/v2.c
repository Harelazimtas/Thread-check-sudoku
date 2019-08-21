/*
 * v2.c
 *
 *  Created on: 18 May 2019
 *      Author: harel
 */

#include "v2.h"

shareMemory share;

int main(int argc, char *argv[]) {
	pthread_t thread[N];
	int check;
	char *filename;
	pthread_mutex_t solution, misson;
	pthread_cond_t cond;
	filename = (char*) malloc(sizeof(char) * MAXSIZE);
	if (!filename) {
		printf("malloc failed\n");
		return 0;
	}
	if (argc == 1) {
		printf("please enter name of file to check\n");
		scanf("%s", filename);
	} else {
		sscanf(argv[1], "%s", filename);
	}
	check = readFromFile(filename);
	if (!check) {
		printf("read from file failed\n");
		return 0;
	}
	check = initShareStruct(&solution, &misson, &cond);
	if (!check) {
		printf("the mutex or cond failed\n");
		return 0;
	}
	//none of threads can to start mission
	pthread_mutex_lock(share.mutexGetMission);
	for (int i = 0; i < N; i++) {
		check = pthread_create(&thread[i], NULL, checksudoku, NULL);
		if (check) {
			printf("error of create thread");
			return 0;
		}
	}
	//none of threads can write the result of check and can to start mission
	pthread_mutex_lock(share.mutexResult);
	pthread_mutex_unlock(share.mutexGetMission);
	// the while  prevent fake wake from wait
	while (share.counter != 27) {
		//when we wait the threads can write the result of check
		pthread_cond_wait(share.cond, share.mutexResult);
	}
	pthread_mutex_unlock(share.mutexResult);

	if (share.solution == 27) {
		printf("solution is legal\n");
	} else {
		printf("solution is not legal\n");
	}
	for (int i = 0; i < N; i++) {
		pthread_join(thread[i], NULL);
	}
	pthread_mutex_destroy(share.mutexGetMission);
	pthread_mutex_destroy(share.mutexResult);
	pthread_exit(NULL);
}

//mission 0-8:check row 9-17check column 18-27check matrix
void *checksudoku() {
	int nummission = 0, result,counter;
	while (share.counter < 27) {
		//get mission here
		pthread_mutex_lock(share.mutexGetMission);
		if(share.counter <27){
		nummission = share.mission[share.counter];
		share.counter++;
		counter= share.counter;
		}
		else{
		nummission= MAXSIZE;
		}
		pthread_mutex_unlock(share.mutexGetMission);
		if (nummission < 9) {
			//mission of row
			result = checkRow(nummission);
		} else if (nummission > 8 && nummission < 18) {
			//mission of column
			result = checkColumn(nummission);
		} else if(nummission >17 && nummission <27) {
			//mission of matrix
			result = checkMatrix(nummission);
		}
		else{
			result=0;
		}
		// write result of mission
		pthread_mutex_lock(share.mutexResult);
		if (result == 1){
			share.solution++;
		}
		pthread_mutex_unlock(share.mutexResult);
	}
	//only  one thread d
	if (counter == 27)
		pthread_cond_signal(share.cond);
	pthread_exit(NULL);

}

int checkColumn(int numColumn) {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int num;
	num = numColumn - MAX;
	for (int i = 0; i < MAX; i++) {
		if (bucket[share.sudoku[i][num] - 1] == 1) {
			return 0;
		} else {
			bucket[share.sudoku[i][num] - 1] = 1;
		}
	}
	return 1;
}
int checkMatrix(int numMatrix) {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int numMatCheck = numMatrix - (MAX + MAX);
	int startRow, startColumn;
	startRow = (numMatCheck / THREE) * THREE;
	startColumn = (numMatCheck % THREE) * THREE;
	for (int j = startRow; j < startRow + THREE; j++) {
		for (int i = startColumn; i < startColumn + THREE; i++) {
			if (bucket[share.sudoku[j][i] - 1] == 1) {
				return 0;
			} else {
				bucket[share.sudoku[j][i] - 1] = 1;
			}
		}
	}
	return 1;
}
int checkRow(int numRow) {
	int bucket[MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < MAX; i++) {
		if (bucket[share.sudoku[numRow][i] - 1] == 1) {
			return 0;
		} else {
			bucket[share.sudoku[numRow][i] - 1] = 1;
		}
	}
	return 1;
}
//init the struct
int initShareStruct(pthread_mutex_t *solution, pthread_mutex_t *misson,
		pthread_cond_t *cond) {
	int check;
	check = pthread_mutex_init(solution, NULL);
	if (check) {
		return 0;
	}
	check = pthread_mutex_init(misson, NULL);
	if (check) {
		return 0;
	}
	check = pthread_cond_init(cond, NULL);
	if (check) {
		return 0;
	}
	share.mutexGetMission = misson;
	share.mutexResult = solution;
	share.cond = cond;
	share.counter = 0;
	share.solution = 0;
	for (int i = 0; i < NUMTHREAD; i++) {
		share.mission[i] = i;
	}
	return 1;
}

//read form file the sudoku
int readFromFile(char *filename) {
	FILE *fP = fopen(filename, "rb");
	if (!fP) {
		return 0;
	}
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) {
			fscanf(fP, "%d ", &(share.sudoku[i][j]));
		}
	}
	fclose(fP);
	return 1;
}
