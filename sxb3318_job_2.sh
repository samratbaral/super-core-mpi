#!/bin/bash

# Run : chmod +x sxb3318_job_2.sh ./sxb3318_job_2.sh

num_processors=32 # Set the number of processors
# num_processors=8 # Test on MacBook Pro M1 = 8 cores
echo "Number of Processor: $num_processors"

# Set the largest number
interval_number=10000000
echo "Largest Input Number: $interval_number"

# Set the output file name
output_file="output_2.txt"
echo "Output File    (txt): $output_file"

echo "================================================================="
# Complie the C program for MPI
echo "Compling sxb3318_problem_2.c [num_processor is set $num_processors]"
mpicc -o 2 sxb3318_problem_2.c


echo "================================================================="
# Test the C program for MPI
echo "Testing sxb3318_problem_2.c [Testing on $num_processors]"
mpirun -np $num_processors ./2 $interval_number

echo "================================================================="

# Running on all processor from 0-32
echo "TRunning on all processor from 0-$num_processors [ Processor Input: $num_processors ] [ Interval Number: $interval_number ] [ Output File: $output_file]"

echo "================================================================="
# Compile the code
mpicc -o 2 sxb3318_problem_2.c

# Run the code with mpirun
# echo "Running with $num_processors processors..."
# mpirun -np $num_processors ./1a $largest_number $num_processors > $output_file

# Loop through the number of processors from 1 to 32
# for ((num_processors = 1; num_processors <= 32; num_processors++)) # Set the number of processors
for ((num_processors = 1; num_processors <= 8; num_processors++)) # Test on MacBook Pro M1 = 8 cores
do
    echo "Running with $num_processors processor(s)..."
    mpirun -np $num_processors ./2 $interval_number >> $output_file
done

echo "================================================================="
# Display the final output
echo "Output saved to $output_file:"
cat $output_file

 echo "Running python to plot $num_processors processor(s)"
python3 sxb3318_graph_2.py
echo "================================================================="
 echo "Graph is plotted on $num_processors processor(s).. in sxb3318_problem_2_graph_scalability_vs_processors.png file."
  echo "Graph is plotted on $num_processors processor(s).. in sxb3318_problem_2_graph_accuracy_vs_processors.png file."
echo "================================================================="
echo "===================[ COMPLETE ]================================="

# Cleanup the executable file
rm 2
# rm $output_file
echo "================================================================="
