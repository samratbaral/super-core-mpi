# Question 1

## Requirement
-Make sure you have the MPI library installed on your system. If not, you can install it based on your system's package manager or download it from the MPI website [MPI Library](https://www.mpi-forum.org/).

- General Run of C Program with MPI
 ```bash
    # Complie the C program for MPI
    mpicc -o <NametheScript> <NameofCprogram.c>
    # Run the Script of <NameoftheScript> on How many <Numberofprocessor>
    mpirun -np <Numberofprocessor> <NametheScript> [Largestnumber] [NumberofProcessor]

```

## Setup Virtual Environment
 ```bash
        python3 -m venv venv
```
- the name nenv can be set to any name

## Virtual Environment Setup
```bash
        . venv/bin/activate
```
## Upgrade Python
```bash
        python3 -m pip3 install --upgrade pip3
```

## Need Pandas and Mathplotlib from pip
```bash
        pip install pandas matplotlib
```

Please see the [Questions](/hw5-6-s2023.pdf) of directory for each assigned problem documentation.

- Run the Question 1
 ```bash

     chmod +x sxb3318_job_1.sh

     ./sxb3318_job_1.sh
```
- Run the Question 2
 ```bash

     chmod +x sxb3318_job_2.sh

     ./sxb3318_job_2.sh
```
- Run the Question 3
 ```bash

     chmod +x sxb3318_job_3.sh

     ./sxb3318_job_3.sh
```
- Run the Question 4
 ```bash

     chmod +x sxb3318_job_4.sh

     ./sxb3318_job_4.sh
```


- Run test
 ```bash

     chmod +x test.sh

     ./test.sh
```
