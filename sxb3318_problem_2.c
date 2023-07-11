#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ROOT_PROCESS 0

typedef struct Node {
    double value;
    struct Node* next;
} Node;

void insert(Node** head, double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = *head;
    *head = newNode;
}

double compute_integral(int rank, int size, double a, double b, double h, int intervals) {
    double sum = 0.0;
    int i;
    int start = rank * (intervals / size);
    int end = start + (intervals / size);

    for (i = start; i < end; i++) {
        double x = a + i * h;
        double fx = 4.0 / (1.0 + x * x);
        sum += fx;
    }

    return sum;
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main(int argc, char** argv) {
    int rank, size;
    double a = 0.0, b = 1.0; // Integration limits
    int intervals; // Number of intervals
    double h; // Width of each interval
    Node* local_head = NULL;
    double local_sum = 0.0;
    double global_sum = 0.0;
    long double pi;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == ROOT_PROCESS) {
            fprintf(stderr, "Usage: mpirun -np <num_procs> test <intervals>\n");
        }
        MPI_Finalize();
        return 1;
    }

    intervals = atoi(argv[1]);

    MPI_Bcast(&intervals, 1, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    h = (b - a) / intervals;

    local_sum = compute_integral(rank, size, a, b, h, intervals);
    insert(&local_head, local_sum);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, ROOT_PROCESS, MPI_COMM_WORLD);

    if (rank == ROOT_PROCESS) {
        pi = h * global_sum;
        printf("Approximation of pi: %.100Le\n", pi);
    }

    free_list(local_head);

    MPI_Finalize();

    return 0;
}
