# Matrix Multiplier Using Multi-threading
## Description
This program is an application on the following concepts 
- Threads
- Processes
- Multithreading
## How to run
- The program needs to be run in Linux environment either on a virtual machine or as the main operating system.
- Download the source code and run the following command in the terminal
     ```
     make
    ./shell
    ```
## How to use
- When the program runs, the user is asked to enter the names of the input and output files.
- If the user does not provide any names the program uses its defaults which is a and b for the input files and c for the output files.
- There are 3 output files such that
    - A file for the first method (A thread per matrix).
    - A file for the second method (A thread per row).
    - A file for the third method (A thread per element). 
- For Example
    - The user can provide files names as follows
    ```
    a b c
    ```
    - The files names will be formatted as follows
    ```
    input files:
        a.txt
        b.txt
    output files:
        c_per_matrix.txt
        c_per_row.txt
        c_per_element.txt
    ```
- For the input files they should have the following format
    ```
    row=5   column=10
    1   2   3   4   5   6   7   8   9   10
    1   2   3   4   5   6   7   8   9   10
    1   2   3   4   5   6   7   8   9   10
    1   2   3   4   5   6   7   8   9   10
    1   2   3   4   5   6   7   8   9   10
    ```

## Features
- It is assumed that the maximum size of the matrix is 20 x 20

## Short Demo
https://youtu.be/3A_T7hLzd_E