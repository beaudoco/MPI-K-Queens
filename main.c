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
#include <time.h>

// N x N chessboard
#define N 14

/*********************************************************
This is the main function of the code, it is used to keep
track of the run time for the program and call the
solution finder with the blank board.
*********************************************************/

int main()
{
	//DECLARE VARS
	int board[N][N];

	//FILL BOARD WITH NO RESULTS
	memset(board, 0, sizeof board);

	//SETUP TIMER FOR FILE
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

	//START SOLVING BOARD
	findKQueen(board, 0);

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
@return: none
*********************************************************/

void findKQueen(int board[][N], int col)
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

	//GO THRU BOARD
	for (int i = 0; i < N; i++)
	{
		//CHECK IF MOVE IS ALLOWED
		if (isValid(board, col, i))
		{
			//PLACE QUEEN
			board[col][i] = 1;

			//MOVE TO NEXT COLUMN
			findKQueen(board, col + 1);

			//THIS PIECE WAS NO GOOD GO BACK
			board[col][i] = 0;
		}
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

	//NO ISSUES
	return true;
}
