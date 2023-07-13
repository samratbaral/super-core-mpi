#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_DATA_SIZE 131072
#define DATA_SIZE_STEP 1024
#define DATA_SIZE_LIMIT 128 * 1024

typedef struct {
    int rank;
    double start_time;
    double end_time;
    int data_size;
} TimingData;

typedef struct node {
    TimingData data;
    struct node* next;
} Node;

Node* create_node(TimingData timing_data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = timing_data;
    node->next = NULL;
    return node;
}

void add_node(Node** head, TimingData timing_data) {
    if (*head == NULL) {
        *head = create_node(timing_data);
        return;
    }
    Node* curr = *head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = create_node(timing_data);
}

void free_list(Node* head) {
    Node* curr = head;
    while (curr != NULL) {
        Node* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

int main(int argc, char** argv) {
    int rank, size;
    int buffer[MAX_DATA_SIZE / sizeof(int)];
    MPI_Status status;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("This program must be run with exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    Node* timings_list = NULL;

    for (int data_size = 0; data_size <= DATA_SIZE_LIMIT; data_size += DATA_SIZE_STEP) {
        if (rank == 0) {
            MPI_Send(buffer, data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            start_time = MPI_Wtime();
            MPI_Recv(buffer, data_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &status);
            end_time = MPI_Wtime();
            TimingData timing_data = {rank, start_time, end_time, data_size};
            add_node(&timings_list, timing_data);
        } else {
            MPI_Recv(buffer, data_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(buffer, data_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        FILE* csv_file = fopen("output_3b.csv", "w");
        fprintf(csv_file, "Data Size (bytes),Latency (seconds),Communication Time (seconds)\n");
        Node* curr = timings_list;
        while (curr != NULL) {
            double latency = (curr->data.end_time - curr->data.start_time) / 2.0;
            double comm_time = curr->data.end_time - curr->data.start_time;
            fprintf(csv_file, "%d,%f,%f\n", curr->data.data_size, latency, comm_time);
            curr = curr->next;
        }
        fclose(csv_file);
    }

    free_list(timings_list);

    MPI_Finalize();
    return 0;
}



// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>

// #define MAX_ITERATIONS 1000000

// int main(int argc, char** argv) {
//     int rank, size, msg, dest, source, tag = 0;
//     double t1, t2, t_comm, t_latency;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     if (size != 2) {
//         fprintf(stderr, "This program requires exactly two processes\n");
//         MPI_Finalize();
//         exit(1);
//     }

//     if (rank == 0) {
//         dest = 1;
//         source = 1;
//         msg = 0;

//         t1 = MPI_Wtime();
//         for (int i = 0; i < MAX_ITERATIONS; i++) {
//             MPI_Send(&msg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
//             MPI_Recv(&msg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         }
//         t2 = MPI_Wtime();

//         t_comm = (t2 - t1) / (2 * MAX_ITERATIONS);
//         t_latency = t_comm / 2;
//     } else {
//         dest = 0;
//         source = 0;

//         for (int i = 0; i < MAX_ITERATIONS; i++) {
//             MPI_Recv(&msg, 1, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//             MPI_Send(&msg, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
//         }
//     }

//     if (rank == 0) {
//         printf("Communication time: %f microseconds\n", t_comm * 1e6);
//         printf("Latency: %f microseconds\n", t_latency * 1e6);
//     }

//     MPI_Finalize();
//     return 0;
// }
