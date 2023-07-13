#!/bin/bash

# Compile the C program
mpicc -o 4 sxb3318_problem_4.c

num_processors=32

# Define the combinations to run
combinations=("128 4" "256 4" "256 8" "1024 4" "1024 8" "1024 16")

# Create an output file
output_file="output_4.txt"
# rm -f "$output_file" # Remove the file if it already exists

# Iterate over the combinations and run the program
for combo in "${combinations[@]}"
do
    matrix_size=$(echo $combo | cut -d ' ' -f 1)
    num_processors=$(echo $combo | cut -d ' ' -f 2)

    echo "Running combination: Matrix size = $matrix_size by $matrix_size, number of processors = $num_processors"

    # Run the program and append the output to the file
    mpirun -np $num_processors ./4 >> "$output_file"

done

echo "Program output saved to $output_file"

rm 4
echo "===================[ COMPLETE  4]================================="
