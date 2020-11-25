#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// N x N chessboard
#define N 14

// Function to check if two queens threaten each other or not
bool isValid(int mat[][N], int col, int row)
{
	// return false if two queens share the same column
	for (int i = 0; i < col; i++)
    {
        if (mat[i][row] == 1)
        {
            return false;
        }
    }

	// return false if two queens share the same \ diagonal
	for (int i = col, j = row; i >= 0 && j >= 0; i--, j--)
    {
        if (mat[i][j] == 1)
        {
            return false;
        }
    }


	// return false if two queens share the same / diagonal
	for (int i = col, j = row; i >= 0 && j < N; i--, j++)
    {
        if (mat[i][j] == 1)
        {
            return false;
        }

    }

	return true;
}

void findKQueen(int mat[][N], int col)
{
	// if N queens are placed successfully, print the solution
	if (col == N)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
            {
                if (mat[i][j] == 1)
                {
                    //printf("%d,", j);
                }
			}
		}
		//printf("\n \n");

		return;
	}

	// place Queen at every square in current row r
	// and recur for each valid movement
	for (int i = 0; i < N; i++)
	{
		// if no two queens threaten each other
		if (isValid(mat, col, i))
		{
			// place queen on current square
			mat[col][i] = 1;

			// recur for next row
			findKQueen(mat, col + 1);

			// backtrack and remove queen from current square
			mat[col][i] = 0;
		}
	}
}

int main()
{
	// mat[][] keeps track of position of Queens in current configuration
	int mat[N][N];

	// initialize mat[][] by '-'
	memset(mat, 0, sizeof mat);

	//SETUP TIMER FOR FILE
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

	findKQueen(mat, 0);

	//END CLOCK AND GET TIME
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("time taken %f\n",elapsed);

	return 0;
}
