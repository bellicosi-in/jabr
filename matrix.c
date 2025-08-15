#include "matrix.h"
#include <stdio.h>

#define RAND_MAX 0x7fffffff

//for allocating a new matrix
mat* new_mat(unsigned int num_rows, unsigned int num_cols){
  if(num_rows == 0 || num_cols == 0){
    fprintf(stderr, "rows and cols must be greater than 0");
    exit(0);
  }

  mat* m = calloc(1, sizeof(*m));
  m->num_rows = num_rows;
  m->num_cols = num_cols;
  m->is_square = (num_rows == num_cols) ? 1 : 0;
  //allocates memory for the column array
  m->values = calloc(m->num_rows, sizeof(*m->values));
  for(int i =0; i < m->num_rows; ++i){
    m->values[i] = calloc(m->num_cols, sizeof(**m->values)); //allocate the data for the cells/cols
    if(m->values[i] == NULL){
      fprintf(stderr, "null value");
    }
    return m;
  }
}

//freeing the matrix
void free_mat(mat* matrix){
  for(int i  = 0; i < matrix->num_rows; ++i){
    free(matrix->values[i]); //free each row's data
  }
  free(matrix->values); //free the  pointer to the row pointer arrays(Array of row pointers)
  free(matrix); //freee the matrix struct
}

//for generating random values
double rand_interval(double min, double max){
  double d;
  d = (double)rand()/((double)RAND_MAX + 1);
  return (min + d*(max-min));
}


//creating random matrices
mat* random_mat(unsigned int num_rows, unsigned int num_cols, double min, double max){
  mat* random_matrix = new_mat(num_rows, num_cols);
  for(int i = 0; i< num_rows; i++){
    for(int j = 0; j < num_cols; j++){
      random_matrix->values[i][j] = rand_interval(min,max);
    }
  }
  return random_matrix;
}

//creating a square matrix
mat* square_mat(unsigned int size){
  return new_mat(size, size);
}

//creating a random square matrix
mat* random_square_mat(unsigned int size, double min, double max){
  return random_mat(size,size,min,max);
}

//creating an identity matrix 
mat* eye_mat(unsigned int size){
  mat* matrix = new_mat(size,size);
  for(int i = 0; i < matrix->num_rows; i++){
    matrix->values[i][i] = 1.0;

  }
  return matrix;

}

//reading from file
mat* read_from_filef(FILE* f){
  int i,j;
  unsigned int num_rows = 0, num_cols = 0;
  printf("enter the number of rows\n");
  fscanf(f, "%d", &num_rows);
  printf("enter the number of cols\n");
  fscanf(f, "%d", &num_cols);
  mat* matrix = new_mat(num_rows, num_cols);
  for(i = 0; i < matrix->num_rows; i++){
    for(j = 0; j < matrix->num_cols; j++){
      fscanf(f, "%lf", &matrix->values[i][j]);

    }
  }
  return matrix;
}
