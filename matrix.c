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


//matrix dimension equality
int mat_eqdim(mat* m1, mat* m2){
  return (m1->num_rows == m2->num_rows) && (m1->num_cols == m2->num_cols);
}

//matrix equality
int mat_equal(mat* m1, mat* m2, double tolerance){
  if(!mat_eqdim(m1,m2)){
    return 0;
  }
  int i,j;
  for(i = 0; i < m1->num_rows; i++){
    for(j = 0; j < m1->num_cols; j++){
      if(fabs(m1->values[i][j] - m2->values[i][j]) > tolerance){
        return 0;
      }
    }
  }
  return 1;
}

//printing the matrix
void mat_print(mat* matrix){
  mat_printf(matrix, "%lf\t\t");
}

//printing the matrix elements
void mat_printf(mat* matrix, const char* fmt){
  int i,j;
  fprintf(stdout, "\n");
  for(i = 0; i < matrix->num_rows; i++){
    for(j = 0; j < matrix->num_cols; j++){
      fprintf(stdout, fmt, matrix->values[i][j]);
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout,"\n");
}

// retrieving a column
mat* get_col_mat(mat* matrix, int col){
  if(col >= matrix->num_cols){
    fprintf(stderr, "cannot get columns %d", col);
    exit(75);

  }
  mat* new_col_matrix = new_mat(matrix->num_rows, 1);
  for(int j = 0; j< matrix->num_cols; j++){
    new_col_matrix->values[j][0] = matrix->values[j][col];

  }
  return new_col_matrix;
}

//retrieving a row
mat* get_row_mat(mat* matrix, int row){
  if(row >= matrix->num_rows){
    fprintf(stderr, "row cant be fetched %d ",row);
    exit(75);
  }
  mat* new_row_matrix = new_mat(1, matrix->num_cols);
  //memory is contiguous hence memcpy can be used.
  memcpy(new_row_matrix->values[0], matrix->values[row], matrix->num_cols * sizeof(*new_row_matrix->values[0]));
  return new_row_matrix;

}