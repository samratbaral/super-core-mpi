#!/bin/bash

# Compile the C program
mpicc -o test test.c

# Run the program and save the output to "output.txt"
mpirun -np 2 ./test


python3 test.py
