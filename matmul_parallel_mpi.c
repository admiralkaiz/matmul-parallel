/*
 * Parallel matrix multiplication with MPI
 * Author: Kaisar Kertarajasa (1301213276)
 *
 */

#include <mpi.h>
#include "functionalities.h"

int main(int argc, char* argv[]) 
{
    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    // Get number of processes
    int proc_count;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    // Get process rank
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    // Matrix multiplication will be done in parallel.
    // Each process will recieve a part of first operand matrix.
    // That is, a few rows from the first matrix.
    int row_per_process = DIM / proc_count; // Number of rows recieved by each processor

    // Initialize matrices
    int M1[DIM][DIM];
    int M2[DIM][DIM];
    int R[DIM][DIM];

    if (proc_rank == 0)
    { 
        // Fill operand matrices with random values
        matrix_randomize((int *)M1);
        matrix_randomize((int *)M2);
        matrix_randomize((int *)R);
    }

    int M1_part[row_per_process][DIM]; // Part of M1 that will be passed (scattered) to each process
    int R_part[row_per_process][DIM];  // Part of R that will be passed (gathered) from each process

    // Scatter M1 parts to each process
    // To scatter means to break down a larger data into bite-sized pieces,
    // then scatter the bits to other processes.
    MPI_Scatter(
        M1,                    // The source of data
        row_per_process * DIM, // Data quantity
        MPI_INT,               // The data type
        M1_part,               // Where will the scattered data saved by each process?
        row_per_process * DIM, // Data quantity
        MPI_INT,               // The data type
        0,                     // Root process. This will responsible to scatter and gather the data
        MPI_COMM_WORLD         // Communicator medium
    );

    // Broadcast M2 to each process
    // To broadcast means to distribute the same data to other processes, copied from the original
    MPI_Bcast(
        M2,             // The data to be broadcasted
        DIM * DIM,      // Data quantity
        MPI_INT,        // Data type
        0,              // Root process
        MPI_COMM_WORLD  // Communicator medium
    );
    
    // Run the matrix multiplication
    for (int i = 0; i < row_per_process; i++) 
    {
        for (int j = 0; j < DIM; j++)
        {
            int res = 0;
            for (int k = 0; k < DIM; k++)
                res += M1_part[i][k] * M2[k][j]; // Each process possess their respective M1_part.
            R_part[i][j] = res; // Each process will store the result on their respective R_part array
        }
    }

    // Collect and gather the data from each process
    MPI_Gather(
        R_part,                 // Data from each process
        row_per_process * DIM,  // Data quantity
        MPI_INT,                // Data type
        R,                      // Where to store gathered data?
        row_per_process * DIM,  // Data quantity
        MPI_INT,                // Data type
        0,                      // Root process
        MPI_COMM_WORLD          // Communication medium
    );

    // Show operand matrices and resultant matrix. This will only work if the user adds an exacly ONE extra argument
    if (proc_rank == 0 && argc == 2)
    {
        printf("Matrix 1:\n");
        matrix_print((int *)M1);
        printf("==============================\n\n");

        printf("Matrix 2:\n");
        matrix_print((int *)M2);
        printf("==============================\n\n");

        printf("Multiplication Result:\n");
        matrix_print((int *)R);
        printf("==============================\n\n");
    }
    
    // Finalize MPI session
    MPI_Finalize();
    
    return 0;
}