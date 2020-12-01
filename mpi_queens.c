/*********************************************************
This program takes a given chessboard and attempts to
find a unique column, row, diagonal for an individual
queen to be placed. The queen locations are subsequently
printed out to the screen 

@author: Collin Beaudoin
@version: November 2020
*********************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>
#include <time.h>

#define MASTER  0
#define TAG     0
#define MSGSIZE 100
#define MAX 25
// N x N chessboard
#define N 14

/*********************************************************
This is the main function of the code, it is used to keep
track of the run time for the program and call the
solution finder with the blank board.

@parameter argc: The amount of processes to run
@parameter argv: The the machines to be used for
*********************************************************/

int main(int argc, char* argv[])
{
    //DECLARE VARS
	int board[N][N];

	//FILL BOARD WITH NO RESULTS
	memset(board, 0, sizeof board);

	//SETUP TIMER FOR FILE
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

	//START SOLVING BOARD
    findKQueen(board, 0, argc, argv);

    //END CLOCK AND GET TIME
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("time taken %f\n",elapsed);

    return 0;
}

/*********************************************************
This function is used to recursively to iterate over the
entire board. This is where the computation of valid 
solutions occurs

@parameter board: The current chess board
@parameter col: The current column to look for a solution
@parameter argc: The amount of processes to run
@parameter argv: The the machines to be used for
processes
@return: none
*********************************************************/

void findKQueen(int board[][N], int col, int argc, char* argv[])
{
    //IF ALL COLS ARE FILLED WE HAVE A SOLUTION
	if (col == N)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
            {
                if (board[i][j] == 1)
                {
                    //printf("%d,", j);
                }
			}
		}
		//printf("\n \n");

		return;
	}

	//CHECK THAT NOT IN COL 0
    if (col > 0) {

        //GO THRU BOARD
        for (int i = 0; i < N; i++)
        {
            //CHECK IF MOVE IS ALLOWED
            if (isValid(board, col, i))
            {
                //PLACE QUEEN
                board[col][i] = 1;

                //MOVE TO NEXT COLUMN
                findKQueen(board, col + 1, argc, argv);

                //THIS PIECE WAS NO GOOD GO BACK
                board[col][i] = 0;
            }
        }
    }

    //WE ARE IN THE FIRST COL LETS USE MPI
    if (col == 0) {

        //DECLARE VARS
        int my_rank, source, num_nodes;
        char my_host[MAX];
        char message[MSGSIZE];

        //INITIALIZE MPI
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

        //CHECK THAT WE AREN'T THE MASTER PROCESS
        if (my_rank != MASTER) {
            
            //WHO ARE WE????
            gethostname (my_host, MAX);
            sprintf(message, "Hello from process %d on host %s!", my_rank, my_host);
            
            //CHECK IF MOVE IS ALLOWED
            if (isValid(board, col, my_rank))
            {
                //PLACE QUEEN
                board[col][my_rank] = 1;

                //MOVE TO NEXT COLUMN
                findKQueen(board, col + 1, argc, argv);

                //THIS PIECE WAS NO GOOD GO BACK
                board[col][my_rank] = 0;
            }

            //WE ARE DONE, LET THE MASTER PROCESS KNOW
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, MASTER, TAG, MPI_COMM_WORLD);

        }
        //MASTER PROCESS
        else {
            //WHO ARE WE????
            gethostname (my_host, MAX);
            
            //CHECK IF MOVE IS ALLOWED
            if (isValid(board, col, my_rank))
            {
                //PLACE QUEEN
                board[col][my_rank] = 1;

                //MOVE TO NEXT COLUMN
                findKQueen(board, col + 1, argc, argv);

                //THIS PIECE WAS NO GOOD GO BACK
                board[col][my_rank] = 0;
            }
            //LETS GET THE MESSAGES FROM EVERYONE
            for (source = 1; source < num_nodes; source++) {
                MPI_Recv(message, MSGSIZE, MPI_CHAR, source, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // printf("%s\n", message);
            }
        }

        //ALL RESULTS ARE BACK STOP USING MPI
        MPI_Finalize();
    }
}

/*********************************************************
This function is used to check the validity of the queen
that is about to be placed. This is used to ensure there
are no "attackable" queens

@parameter board: The current board
@parameter col: The current column to check
@parameter row: The current row to check
@return: bool
*********************************************************/

bool isValid(int board[][N], int col, int row)
{
	//SAME ROW CHECK
	for (int i = 0; i < col; i++)
    {
        if (board[i][row] == 1)
        {
            return false;
        }
    }

	//SAME \ DIAGONAL CHECK
	for (int i = col, j = row; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j] == 1)
        {
            return false;
        }
    }

	//SAME / DIAGONAL CHECK
	for (int i = col, j = row; i >= 0 && j < N; i--, j++)
    {
        if (board[i][j] == 1)
        {
            return false;
        }
    }

	return true;
}
