#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int value;
    struct Node *next;
} Node;

// Function to create a new node
Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at the end of the linked list
void insertNode(Node **head, int value)
{
    Node *newNode = createNode(value);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node *curr = *head;
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

// Function to free the memory allocated for the linked list
void freeLinkedList(Node *head)
{
    Node *curr = head;
    while (curr != NULL)
    {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

int main(int argc, char **argv)
{
    int i;
    int n;
    int p;
    int size;
    int prime;
    int count;
    int global_count;
    long int low_value;
    long int high_value;
    int comm_rank;
    int comm_size;
    Node *primesList = NULL;
    double runtime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    MPI_Barrier(MPI_COMM_WORLD);
    runtime = -MPI_Wtime();

    // Check for the command line arguments
    if (argc != 3)
    {
        if (comm_rank == 0)
            printf("Please supply the largest number and the number of processors.\n");
        MPI_Finalize();
        exit(1);
    }

    n = atoi(argv[1]);
    p = atoi(argv[2]);

    // Bail out if all the primes used for sieving are not all held by process zero
    if ((2 + (n - 1) / p) < (int)sqrt((double)n))
    {
        if (comm_rank == 0)
            printf("Too many processes.\n");
        MPI_Finalize();
        exit(1);
    }

    // Figure out this process's share of the array, as well as the integers represented by the first and last array elements
    low_value = 2 + (long int)(comm_rank) * (long int)(n - 1) / (long int)p;
    high_value = 1 + (long int)(comm_rank + 1) * (long int)(n - 1) / (long int)p;
    size = high_value - low_value + 1;

    // Mark all numbers as prime initially
    char *marked = (char *)calloc(size, sizeof(char));
    if (marked == NULL)
    {
        printf("Cannot allocate enough memory.\n");
        MPI_Finalize();
        exit(1);
    }

    prime = 2;

    do
    {
        // Find the first unmarked number in the current process's range
        long int first;
        if (prime * prime > low_value)
        {
            first = prime * prime - low_value;
        }
        else
        {
            if (low_value % prime == 0)
                first = 0;
            else
                first = prime - (low_value % prime);
        }

        // Mark multiples of the current prime as non-prime
        for (i = first; i < size; i += prime)
            marked[i] = 1;

        // Find the next prime number
        if (comm_rank == 0)
        {
            int next_prime = prime + 1;
            while (next_prime <= high_value && marked[next_prime - low_value])
                next_prime++;
            prime = next_prime;
        }

        // Broadcast the prime number to all processes
        MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);

    } while (prime * prime <= n);

    // Count the number of primes in the current process
    count = 0;
    for (i = 0; i < size; i++)
    {
        if (!marked[i])
        {
            count++;
            insertNode(&primesList, low_value + i);
        }
    }

    // Synchronize the counts across all processes
    MPI_Allreduce(&count, &global_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    runtime += MPI_Wtime();


    if (comm_rank == 0)
    {
        printf("Processor %d took %f seconds to found %d primes less than or equal to in the given number %d.\n", p, runtime, global_count, n);

        /*
        // Print the prime numbers
        printf("\n\tPrime numbers: ");
        Node *curr = primesList;
        while (curr != NULL)
        {
            printf("%d ", curr->value);
            curr = curr->next;
        }
        */


    }


    // Free the allocated memory
    free(marked);
    freeLinkedList(primesList);

    MPI_Finalize();
    return 0;
}
