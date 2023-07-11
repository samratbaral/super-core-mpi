#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATRIX_SIZE 1024

// Function to generate the matrix and vector
void generateMatrix(double matrix[MATRIX_SIZE][MATRIX_SIZE], double vector[MATRIX_SIZE]) {
    int i, j;

    // Set values of X0, X1, ..., X127
    for (i = 0; i < MATRIX_SIZE; i++) {
        vector[i] = (double)(i + 1);
    }

    // Generate random values for matrix coefficients
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = ((double)rand() / RAND_MAX) * 2 - 1; // Random value between -1 and 1
        }
    }
}

// Function to perform the Back Substitution algorithm
void backSubstitution(double matrix[MATRIX_SIZE][MATRIX_SIZE], double vector[MATRIX_SIZE], double result[MATRIX_SIZE]) {
    int i, j;

    // Perform the Back Substitution algorithm
    for (i = MATRIX_SIZE - 1; i >= 0; i--) {
        result[i] = vector[i];
        for (j = i + 1; j < MATRIX_SIZE; j++) {
            result[i] -= matrix[i][j] * result[j];
        }
        result[i] /= matrix[i][i];
    }
}

int main(int argc, char** argv) {
    int size, rank;
    double matrix[MATRIX_SIZE][MATRIX_SIZE];
    double vector[MATRIX_SIZE];
    double result[MATRIX_SIZE];
    double start_time, end_time;
    double total_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Check if the matrix size is divisible by the number of processors
    if (MATRIX_SIZE % size != 0) {
        if (rank == 0) {
            printf("Matrix size is not divisible by the number of processors.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Calculate the number of rows per processor
    int rows_per_proc = MATRIX_SIZE / size;

    // Generate the matrix and vector
    if (rank == 0) {
        generateMatrix(matrix, vector);
    }

    // Scatter rows of the matrix to different processors
    double local_matrix[rows_per_proc][MATRIX_SIZE];
    double local_vector[rows_per_proc];
    int sendcounts[size];
    int displs[size];
    int i;
    for (i = 0; i < size; i++) {
        sendcounts[i] = rows_per_proc * MATRIX_SIZE;
        displs[i] = i * rows_per_proc * MATRIX_SIZE;
    }
    MPI_Scatterv(matrix, sendcounts, displs, MPI_DOUBLE, local_matrix,
                 rows_per_proc * MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(vector, sendcounts, displs, MPI_DOUBLE, local_vector, rows_per_proc,
                 MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform the Back Substitution algorithm
    start_time = MPI_Wtime();
    backSubstitution(local_matrix, local_vector,result);
end_time = MPI_Wtime();
total_time = end_time - start_time;
// Gather the results from all processors
MPI_Gatherv(result, rows_per_proc, MPI_DOUBLE, vector, sendcounts, displs,
            MPI_DOUBLE, 0, MPI_COMM_WORLD);

// Calculate the speedup
double max_time;
MPI_Reduce(&total_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

// Calculate the speedup for each combination
if (rank == 0) {
    double sequential_time;
    double speedup;

    // Calculate the sequential time
    start_time = MPI_Wtime();
    backSubstitution(matrix, vector, result);
    end_time = MPI_Wtime();
    sequential_time = end_time - start_time;

    printf("Matrix size = %d by %d, number of processors = %d\n", MATRIX_SIZE, MATRIX_SIZE, size);
    printf("Sequential time: %.6f seconds\n", sequential_time);
    printf("Parallel time: %.6f seconds\n", total_time);

    // Calculate the speedup
    speedup = sequential_time / max_time;
    printf("Speedup: %.2f\n", speedup);
    printf("--------------------------\n");
}

MPI_Finalize();
return 0;
}
