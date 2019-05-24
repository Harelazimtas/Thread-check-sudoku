# Thread-check-sudoku
This project has two versions to check the integrity of Sudoku.

file : v1.c v1.h v1.

In the first version :

The main program reads the Sudoku file and make twenty-seven Threads.
Each Thread is responsible for checking a row or column or matrixץ
The main Thread waits for the other Threads to end with the join and then informs if the file is valid.

file : v2.c v2.h v2

In the second version : 

The main program reads the Sudoku file and make N(constant number) Thread.
The main program creates a list of tasks that each task describes what to check, that is, 
whether to check a column or a row or a matrix and also what specific location should be checked.
The tasks are protected by a lock and a variable that tells us what task we have reached and whether we have finished.
And another variable that indicates whether Sudoku is proper.
For each task, the Thread will access the variable locked by a lock and raise it by 1 if task is true,else not.
During the check sudoku the main thread wait and at the end of checking the last Thread  will inform(singal) him that he will continue.
Then the main thread will tell if the sudoku file is correct.
