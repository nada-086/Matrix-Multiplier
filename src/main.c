#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include <sys/time.h>

#define FILE_NAME 100

// Global Variables
char ** files_names;
int num_threads = 0;
int step_i = 0;

struct Matrix{
    int array[20][20];
    int row;
    int column;
};

struct Element{
    int row;
    int column;
};

struct Matrix matrix_1;
struct Matrix matrix_2;
struct Matrix matrix_3;

// Functions
char** cutting_input();
int find_length(char** array);
struct Matrix read_file(FILE *file, int n);
int number(char* string);
void controller();
void print_matrix(char* name, unsigned long time);
void* matrix(void* arg);
void* row(void* arg);
void* element(void* arg);
void matrix_thread();
void row_thread();
void element_thread();
void reset_matrix();


int main(){
    controller();
    return 0;
}


char** cutting_input(char *line){
    int size = 100;
    int index = 0;
    char **array = malloc(size * sizeof(char*));
    char *c = strtok(line, " \t\n");
    while(c != NULL){
        array[index++] = c;
        if(index >= size){
            size += 100;
            array = realloc(array, sizeof(char*) * size);
        }
        c = strtok(NULL, " \t\n");
    }
    return array;
}

char** file_names(char** names){
    int length = find_length(names);
    char** file_names = malloc(100 * sizeof(char*));
    if (length > 1){
        file_names[0] = malloc(100); strcpy(file_names[0], names[0]); strcat(file_names[0], ".txt");
        file_names[1] = malloc(100); strcpy(file_names[1], names[1]); strcat(file_names[1], ".txt");
        file_names[2] = malloc(100); strcpy(file_names[2], names[2]); strcat(file_names[2], "_per_matrix.txt");
        file_names[3] = malloc(100); strcpy(file_names[3], names[2]); strcat(file_names[3], "_per_row.txt");
        file_names[4] = malloc(100); strcpy(file_names[4], names[2]); strcat(file_names[4], "_per_element.txt");
    }
    else{
        file_names[0] = "a.txt"; file_names[1] = "b.txt";
        file_names[2] = "c_per_matrix.txt"; file_names[3] = "c_per_row.txt"; file_names[4] = "c_per_element.txt";
    }
    return file_names;
}

int find_length(char** array){
    int k = 0;
    while(array[k]){
        k++;
    }
    return k;
}

struct Matrix read_file(FILE *file, int n){
    int k;
    struct Matrix matrix;
    char* line; size_t length = 0; getline(&line, &length, file);
    char** dim = cutting_input(line);
    matrix.row = number(dim[0]);
    matrix.column = number(dim[1]);
    for(int i = 0; i < matrix.row; i++){
        for(int j = 0; j < matrix.column; j++){
            fscanf(file, "%d", &k);
            matrix.array[i][j] = k;
        }
    }
    return matrix;
}

int number(char *string){
    char *number = malloc(50);
    int k = 0;
    int i = 0;
    while(string[k++] != '='){}
    while(string[k]){
        number[i++] = string[k++];
    }
    return atoi(number);
}

void print_matrix(char* name, unsigned long time){
    FILE *file = fopen(name, "w+");
    if(strcmp(name, files_names[2]) == 0){
        fprintf(file, "Matrix: A thread per matrix\n\n");
    }
    else  if(strcmp(name, files_names[3]) == 0){
         fprintf(file, "Matrix: A thread per row\n\n");
    }
    else{
         fprintf(file, "Matrix: A thread per element\n\n");
    }

    fprintf(file, "row=%d\tcolumn=%d\n\n", matrix_3.row, matrix_3.column);
    for(int i = 0; i < matrix_3.row; i++){
        for(int j = 0; j < matrix_3.column; j++){
           fprintf(file, "%d   ", matrix_3.array[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\nExecution Time: %lu microseconds", time);
    fprintf(file, "\nNumber of Threads: %d", num_threads);
    fclose(file);
}

void* matrix(void *arg){
    for(int i = 0; i < matrix_3.row; i++){
        for(int j = 0; j < matrix_2.column; j++){
            for(int k = 0; k < matrix_1.column; k++){
                matrix_3.array[i][j] += matrix_1.array[i][k] * matrix_2.array[k][j];
            }
        }
    }
    pthread_exit(0);
}

void* row(void *arg){
    int i = step_i++;
    for (int j = 0; j < matrix_2.column; j++){
        for (int k = 0; k < matrix_1.column; k++){
            matrix_3.array[i][j] += matrix_1.array[i][k] * matrix_2.array[k][j];
        }
    }
    pthread_exit(0);
}

void* element(void *arg){
    struct Element *pos = arg;
    int row = pos->row;
    int column = pos->column;
    for(int i = 0; i < matrix_1.column; i++){
        matrix_3.array[row][column] += matrix_1.array[row][i] * matrix_2.array[i][column];
    }
    pthread_exit(0);
}

void matrix_thread(){
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    pthread_t thread;
    int* p = NULL;
    num_threads = 1;
    pthread_create(&thread, NULL, matrix, (void*)(p));
    pthread_join(thread, NULL);
    gettimeofday(&stop, NULL);
    print_matrix(files_names[2], stop.tv_usec - start.tv_usec);
}

void row_thread(){
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    pthread_t threads_row[matrix_1.row];
    for (int i = 0; i < matrix_1.row; i++) {
        int* p = NULL;
        pthread_create(&threads_row[i], NULL, row, (void*)(p));
    }
    for (int i = 0; i < matrix_1.row; i++){
       pthread_join(threads_row[i], NULL);
    }
    num_threads = matrix_1.row;
    gettimeofday(&stop, NULL);
    print_matrix(files_names[3], stop.tv_usec - start.tv_usec);

}

void element_thread(){
    int k = 0;
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    pthread_t threads_elements[matrix_1.row * matrix_2.column];
    struct Element pos[matrix_1.row * matrix_2.column];
    for(int i = 0; i < matrix_3.row; i++){
        for(int j = 0; j < matrix_3.column; j++){
            pos[k].row = i;
            pos[k].column = j;
            pthread_create(&threads_elements[k], NULL, element, (void*) &pos[k]);
            k++;
        }
    }
    num_threads = k;
    for(int i = 0; i < k; i++){
           pthread_join(threads_elements[i], NULL);
    }
    gettimeofday(&stop, NULL);
    print_matrix(files_names[4], stop.tv_usec - start.tv_usec);
}

void controller(){
    char* line; size_t length = 0; getline(&line, &length, stdin);
    char** names = cutting_input(line);
    files_names = file_names(names);
    FILE *matrix1 = fopen(files_names[0], "r");
    FILE *matrix2 = fopen(files_names[1], "r");
    if(matrix1 == NULL || matrix2 == NULL){
      printf("Error! opening files\n");
        exit(1);
    }

    matrix_1 = read_file(matrix1, 1);
    matrix_2 = read_file(matrix2, 2);

    fclose(matrix1);
    fclose(matrix2);

    if(matrix_1.column != matrix_2.row){
        printf("\n=============Error: Matrices can not be multiplied!============\n");
        exit(0);
    }

    matrix_3.row = matrix_1.row; matrix_3.column = matrix_2.column;
    reset_matrix();
    matrix_thread();
    reset_matrix();
    row_thread();
    reset_matrix();
    element_thread();
}

void reset_matrix(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            matrix_3.array[i][j] = 0;
        }
    }
}
