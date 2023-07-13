#!/bin/bash

# Run : chmod +x sxb3318_job_1a.sh ./sxb3318_job_1a.sh

num_processors=32 # Set the number of processors
# num_processors=8 # Test on MacBook Pro M1 = 8 cores
echo "Number of Processor: $num_processors"

# Set the largest number
largest_number=100000
echo "Largest Input Number: $largest_number"

# Set the output file name
output_file="output_1.txt"
echo "Output File    (txt): $output_file"

echo "================================================================="
# Complie the C program for MPI
echo "Compling sxb3318_problem_1.c [num_processor is set $num_processors]"
mpicc -o 1 sxb3318_problem_1.c


echo "================================================================="
# Test the C program for MPI
echo "Testing sxb3318_problem_1.c [Testing on $num_processors]"
mpirun -np $num_processors ./1 $largest_number $num_processors

echo "================================================================="

# Running on all processor from 0-32
echo "TRunning on all processor from 0-$num_processors [ Processor Input: $num_processors ] [ Largest Number: $largest_number ] [ Output File: $output_file]"

echo "================================================================="
# Compile the code
mpicc -o 1 sxb3318_problem_1.c

# Run the code with mpirun
# echo "Running with $num_processors processors..."
# mpirun -np $num_processors ./1a $largest_number $num_processors > $output_file

# Loop through the number of processors from 1 to 32
# for ((num_processors = 1; num_processors <= 32; num_processors++)) # Set the number of processors
for ((num_processors = 1; num_processors <= 8; num_processors++)) # Test on MacBook Pro M1 = 8 cores
do
    echo "Running with $num_processors processor(s)..."
    mpirun -np $num_processors ./1 $largest_number $num_processors >> $output_file
done

echo "================================================================="
# Display the final output
echo "Output saved to $output_file:"
cat $output_file

 echo "Running python to plot $num_processors processor(s)...vs Speedup"
python3 sxb3318_graph_1.py
echo "================================================================="
 echo "Graph is plotted on $num_processors processor(s)...vs Speedup in sxb3318_problem__graph_1.png file."
echo "================================================================="
echo "===================[ COMPLETE ]================================="
echo "================================================================="
# Cleanup the executable file
rm 1
# rm $output_file

