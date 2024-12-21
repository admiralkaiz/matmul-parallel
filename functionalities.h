/*
 * Filename: functionalities.h
 * Provides functionalities for serial and parallel square matrix multiplication program in C.
 * Functionalities provided: matrix_randomize() and matrix_print()
 */

#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
 * The dimensionality of the square matrix. 
 * This can be changed to check the performance with differing matrix dimension
 */
#define DIM 750

void matrix_randomize(int *mat)
/*
 * Populate a square matrix (two-dimensional integer array) with random numbers, 
 * ranging from 0 to 9
 */
{
    srand(time(NULL));
    for (int i = 0; i < DIM; i++) 
    {
        for (int j = 0; j < DIM; j++) 
        {
            *(mat + i * DIM + j) = rand() % 10;
        }
    }
}

void matrix_print(int *mat)
/*
 * Print the entries of a square matrix
 */
{
    for (int i = 0; i < DIM; i++) 
    {
        for (int j = 0; j < DIM; j++)
            printf("%d ", *(mat + i * DIM + j));
        printf("\n");
    }
}

#endif