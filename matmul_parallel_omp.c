/*
 * Parallel matrix multiplication with OpenMP
 * Author: Kaisar Kertarajasa (1301213276)
 *
 */

#include <omp.h>
#include "functionalities.h"

int main(int argc, char* argv[]) 
{
    // Initialize matrices
    int M1[DIM][DIM];
    int M2[DIM][DIM];
    int R[DIM][DIM];

    // Randomize values
    matrix_randomize((int *)M1);
    matrix_randomize((int *)M2);
    matrix_randomize((int *)R);

    // Set number of threads to 4
    omp_set_num_threads(4);
    int thread_count = 1;
    #pragma omp parallel
    {
        #pragma omp single
        thread_count = omp_get_num_threads();
    }

    // Run the matrix multiplication
    // The directive below parallelizes the first two loops of this nested loop
    printf("Executing %d x %d matrix multiplication with parallel execution using OpenMP...\n", DIM, DIM);
    printf("Calculating with %d threads\n", thread_count);
    #pragma omp parallel for collapse(2) 
    for (int i = 0; i < DIM; i++) // Paralellized
    {
        for (int j = 0; j < DIM; j++) // Paralellized
        {
            int res = 0;

            for (int k = 0; k < DIM; k++)
                res += M1[i][k] * M2[k][j];
            R[i][j] = res;
        }
    }
    printf("Done!\n");

    // Show operand matrices and resultant matrix. This will only work if the user adds an exacly ONE extra argument
    if (argc == 2)
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
        
    return 0;
}