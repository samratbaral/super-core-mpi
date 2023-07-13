#!/bin/bash

# Run : chmod +x sxb3318_job_3.sh ./sxb3318_job_3.sh

# num_processors=32 # Set the number of processors
num_processors=2 # Test on MacBook Pro M1 = 8 cores
echo "Number of Processor: $num_processors"


# Set the output file name
output_file_3a="output_3a.csv"
output_file_3b="output_3b.csv"
echo "Output File    (CSV): $output_file_3a , $output_file_3b"
echo "================================================================="

echo "===================[ START  3a]================================="
# Complie the C program for MPI
echo "Compling sxb3318_problem_3a.c [num_processor is set $num_processors]"
mpicc -o 3a sxb3318_problem_3a.c
echo "================================================================="

# Test the C program for MPI
echo "Testing sxb3318_problem_3a.c [Testing on $num_processors]"
mpirun -np $num_processors ./3a

echo "================================================================="
# Display the final output
echo "Output saved to $output_file_3a:"

echo "================================================================="
python3 sxb3318_graph_3a.py
 echo "Saving plot Ping-pong latency & Communication time (0-512 bytes, 32-byte increments) "
 echo "Saving plot Ping-pong latency & Communication time (1-128 KB, 1 KB increments) "
echo "================================================================="
# Cleanup the executable file
rm 3a
# rm $output_file_3a
echo "===================[ COMPLETE  3a]================================="


echo "===================[ START  3b]================================="
# Complie the C program for MPI
echo "Compling sxb3318_problem_3b.c [num_processor is set $num_processors]"
mpicc -o 3b sxb3318_problem_3b.c
echo "================================================================="

# Test the C program for MPI
echo "Testing sxb3318_problem_3b.c [Testing on $num_processors]"
mpirun -np $num_processors ./3b

echo "================================================================="
# Display the final output
echo "Output saved to $output_file_3b:"

echo "================================================================="
python3 sxb3318_graph_3b.py
 echo "Saving plot Hot-Potato latency & Communication time (0-512 bytes, 32-byte increments) "
 echo "Saving plot Hot-Potato latency & Communication time (1-128 KB, 1 KB increments) "
echo "================================================================="
# Cleanup the executable file
rm 3b
# rm $output_file_3b
echo "===================[ COMPLETE  3b]================================="
