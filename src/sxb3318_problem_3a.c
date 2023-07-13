#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_ITERATIONS 100
#define MSG_SIZES_START 0
#define MSG_SIZES_END 512
#define MSG_SIZES_STEP 32
#define MSG_SIZES_2_START 1024
#define MSG_SIZES_2_END 128 * 1024
#define MSG_SIZES_2_STEP 1024

typedef struct _node
{
    int data_size;
    double latency;
    double comm_time;
    struct _node *next;
} node;

node *create_node(int data_size, double latency, double comm_time)
{
    node *new_node = (node *)malloc(sizeof(node));
    new_node->data_size = data_size;
    new_node->latency = latency;
    new_node->comm_time = comm_time;
    new_node->next = NULL;
    return new_node;
}

void append_node(node **head_ref, int data_size, double latency, double comm_time)
{
    node *new_node = create_node(data_size, latency, comm_time);
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
    }
    else
    {
        node *current = *head_ref;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

void delete_list(node **head_ref)
{
    node *current = *head_ref;
    while (current != NULL)
    {
        node *next = current->next;
        free(current);
        current = next;
    }
    *head_ref = NULL;
}

int main(int argc, char **argv)
{
    int num_procs, rank, partner, msg_size, i;
    double t_start, t_end, t_latency, t_comm;
    char *message;
    FILE *fp;
    node *data_head = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (num_procs != 2)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Error: program requires exactly 2 processes\n");
        }
        MPI_Finalize();
        return 1;
    }

    message = (char *)malloc(MSG_SIZES_2_END);
    if (message == NULL)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Error: failed to allocate memory for message\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        fp = fopen("output_3a.csv", "w");
        if (fp == NULL)
        {
            fprintf(stderr, "Error: failed to open output file\n");
            MPI_Finalize();
            return 1;
        }
        fprintf(fp, "Data size,Latency,Communication time\n");
    }

    for (msg_size = MSG_SIZES_START; msg_size <= MSG_SIZES_END; msg_size += MSG_SIZES_STEP)
    {
        for (i = 0; i < NUM_ITERATIONS; i++)
        {
            partner = 1 - rank;

            MPI_Barrier(MPI_COMM_WORLD);
            if (rank == 0)
            {
                t_start = MPI_Wtime();
                MPI_Send(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
                MPI_Recv(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                t_end = MPI_Wtime();
            }
            else
            {
                MPI_Recv(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
            }

            t_latency = (t_end - t_start) / 2.0;
            MPI_Allreduce(&t_latency, &t_latency, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
            t_comm = (t_end - t_start) - t_latency;

            if (rank == 0)
            {
                append_node(&data_head, msg_size, t_latency, t_comm);
            }
        }
    }

    for (msg_size = MSG_SIZES_2_START; msg_size <= MSG_SIZES_2_END; msg_size += MSG_SIZES_2_STEP)
    {
        for (i = 0; i < NUM_ITERATIONS; i++)
        {
            partner = 1 - rank;

            MPI_Barrier(MPI_COMM_WORLD);
            if (rank == 0)
            {
                t_start = MPI_Wtime();
                MPI_Send(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
                MPI_Recv(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                t_end = MPI_Wtime();
            }
            else
            {
                MPI_Recv(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(message, msg_size, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
            }

            t_latency = (t_end - t_start) / 2.0;
            MPI_Allreduce(&t_latency, &t_latency, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
            t_comm = (t_end - t_start) - t_latency;

            if (rank == 0)
            {
                append_node(&data_head, msg_size, t_latency, t_comm);
            }
        }
    }

    if (rank == 0)
    {
        node *current = data_head;
        while (current != NULL)
        {
            fprintf(fp, "%d,%f,%f\n", current->data_size, current->latency, current->comm_time);
            current = current->next;
        }
        fclose(fp);
        delete_list(&data_head);
    }

    free(message);
    MPI_Finalize();
    return 0;
}



// #include <mpi.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// #define MIN_SIZE 0
// #define MAX_SMALL_SIZE 512
// #define MAX_LARGE_SIZE 128 * 1024

// int main(int argc, char **argv)
// {
//     int rank, size, partner;
//     double start_time, end_time;
//     int i, j;
//     char *message;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     if (size != 2)
//     {
//         fprintf(stderr, "Program requires exactly 2 MPI processes\n");
//         MPI_Abort(MPI_COMM_WORLD, 1);
//     }

//     partner = 1 - rank;
//     message = (char *)malloc(MAX_LARGE_SIZE);

//     if (rank == 0)
//     {
//         printf("Data Size (bytes)    Latency (microseconds)    Communication Time (microseconds)\n");
//     }

//     for (i = MIN_SIZE; i <= MAX_SMALL_SIZE; i += 32)
//     {
//         MPI_Barrier(MPI_COMM_WORLD);

//         if (rank == 0)
//         {
//             start_time = MPI_Wtime();
//             for (j = 0; j < 1000; j++)
//             {
//                 MPI_Send(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
//                 MPI_Recv(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//             }
//             end_time = MPI_Wtime();
//             printf("%d          \t\t%f             \t\t%f\n", i, (end_time - start_time) / 2000.0 * 1000000.0, (end_time - start_time) / 1000.0 * 1000000.0);
//         }
//         else
//         {
//             for (j = 0; j < 1000; j++)
//             {
//                 MPI_Recv(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//                 MPI_Send(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
//             }
//         }
//     }

//     for (i = 1024; i <= MAX_LARGE_SIZE; i += 1024)
//     {
//         MPI_Barrier(MPI_COMM_WORLD);

//         if (rank == 0)
//         {
//             start_time = MPI_Wtime();
//             for (j = 0; j < 1000; j++)
//             {
//                 MPI_Send(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
//                 MPI_Recv(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//             }
//             end_time = MPI_Wtime();
//             printf("%d      \t\t%f          \t\t%f\n", i, (end_time - start_time) / 2000.0 * 1000000.0, (end_time - start_time) / 1000.0 * 1000000.0);
//         }
//         else
//         {
//             for (j = 0; j < 1000; j++)
//             {
//                 MPI_Recv(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//                 MPI_Send(message, i, MPI_CHAR, partner, 0, MPI_COMM_WORLD);
//             }
//         }
//     }
//     free(message);
//     MPI_Finalize();

//     return 0;
// }

// /*
// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>

// #define DATA_SIZE_LIMIT 128 * 1024
// #define DATA_SIZE_INCREMENT 1024

// typedef struct Node {
//     int dataSize;
//     struct Node* next;
// } Node;

// void insertNode(Node** head, int dataSize) {
//     Node* newNode = (Node*)malloc(sizeof(Node));
//     newNode->dataSize = dataSize;
//     newNode->next = *head;
//     *head = newNode;
// }

// void deleteList(Node** head) {
//     Node* current = *head;
//     while (current != NULL) {
//         Node* temp = current;
//         current = current->next;
//         free(temp);
//     }
//     *head = NULL;
// }

// void pingPong(int dataSize, int numIterations, int rank, int partnerRank) {
//     double startTime, endTime, totalTime;
//     char* sendBuffer = (char*)malloc(dataSize * sizeof(char));
//     char* recvBuffer = (char*)malloc(dataSize * sizeof(char));

//     // Perform warm-up iteration
//     MPI_Sendrecv(sendBuffer, dataSize, MPI_CHAR, partnerRank, 0, recvBuffer, dataSize, MPI_CHAR, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     // Perform ping pong iterations and measure time
//     startTime = MPI_Wtime();
//     for (int i = 0; i < numIterations; i++) {
//         MPI_Sendrecv(sendBuffer, dataSize, MPI_CHAR, partnerRank, 0, recvBuffer, dataSize, MPI_CHAR, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//     }
//     endTime = MPI_Wtime();
//     totalTime = (endTime - startTime) / (2 * numIterations);

//     if (rank == 0) {
//         printf("Data Size: %d bytes, Latency: %lf ms, Communication Time: %lf ms\n", dataSize, totalTime * 1000, totalTime * dataSize * 1000);
//     }

//     free(sendBuffer);
//     free(recvBuffer);
// }

// void runPingPong(Node* head, int numIterations, int rank, int partnerRank) {
//     Node* current = head;
//     while (current != NULL) {
//         pingPong(current->dataSize, numIterations, rank, partnerRank);
//         current = current->next;
//     }
// }

// int main(int argc, char** argv) {
//     int numTasks, rank;
//     int partnerRank;
//     int numIterations = 100;
//     Node* dataSizeList = NULL;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//     if (numTasks != 2) {
//         if (rank == 0) {
//             printf("This program requires exactly 2 MPI tasks.\n");
//         }
//         MPI_Finalize();
//         return 1;
//     }

//     partnerRank = (rank + 1) % 2;

//     // Generate linked list for varying data sizes
//     for (int dataSize = 0; dataSize <= DATA_SIZE_LIMIT; dataSize += DATA_SIZE_INCREMENT) {
//         insertNode(&dataSizeList, dataSize);
//     }

//     runPingPong(dataSizeList, numIterations, rank, partnerRank);

//     deleteList(&dataSizeList);

//     MPI_Finalize();
//     return 0;
// }
// */
