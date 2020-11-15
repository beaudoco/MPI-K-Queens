#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

#define MASTER  0
#define TAG     0
#define MSGSIZE 100
#define MAX 25
// N x N chessboard
#define N 8

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

void findKQueen(int mat[][N], int col, int argc, char* argv[])
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
                    printf("%d,", j);
                }
			}
		}
		printf("\n \n");

		return;
	}

	// place Queen at every square in current row r
	// and recur for each valid movement
    if (col > 0) {
        for (int i = 0; i < N; i++)
        {
            // if no two queens threaten each other
            if (isValid(mat, col, i))
            {
                // place queen on current square
                mat[col][i] = 1;

                // recur for next row
                findKQueen(mat, col + 1, argc, argv);

                // backtrack and remove queen from current square
                mat[col][i] = 0;
            }
        }
    }

    if (col == 0) {
        int my_rank, source, num_nodes;
        char my_host[MAX];
        char message[MSGSIZE];

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

        if (my_rank != MASTER) {
            gethostname (my_host, MAX);
            sprintf(message, "Hello from process %d on host %s!", my_rank, my_host);
            if (isValid(mat, col, my_rank))
            {
                // place queen on current square
                mat[col][my_rank] = 1;

                // recur for next row
                findKQueen(mat, col + 1, argc, argv);

                // backtrack and remove queen from current square
                mat[col][my_rank] = 0;
            }
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, MASTER, TAG, MPI_COMM_WORLD);
        }
        else {
            gethostname (my_host, MAX);
            // printf ("Num_nodes: %d\n", num_nodes);
            // printf ("Hello from Master (process %d) on host %s!\n", my_rank, my_host);
            if (isValid(mat, col, my_rank))
            {
                // place queen on current square
                mat[col][my_rank] = 1;

                // recur for next row
                findKQueen(mat, col + 1, argc, argv);

                // backtrack and remove queen from current square
                mat[col][my_rank] = 0;
            }
            for (source = 1; source < num_nodes; source++) {
                MPI_Recv(message, MSGSIZE, MPI_CHAR, source, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // printf("%s\n", message);
            }
        }

        MPI_Finalize();
    }
}

int main(int argc, char* argv[])
{
    // mat[][] keeps track of position of Queens in current configuration
	int mat[N][N];

	// initialize mat[][] by '-'
	memset(mat, 0, sizeof mat);

	findKQueen(mat, 0, argc, argv);

    return 0;
}
