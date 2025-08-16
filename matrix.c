#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
      exit(1);
    }
  }
  return m;
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

//copying a matrix
mat* mat_cp(mat* matrix){
  mat* new_matrix = new_mat(matrix->num_rows,matrix->num_cols);
  for(int i = 0; i < matrix->num_rows; i++){
    for(int j = 0; j < matrix->num_cols; j++){
      new_matrix->values[i][j] = matrix -> values[i][j];
    }
  }
  return new_matrix;
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
mat* get_col_mat(mat* matrix,unsigned int col){
  if(col >= matrix->num_cols){
    fprintf(stderr, "cannot get columns %d", col);
    exit(75);

  }
  mat* new_col_matrix = new_mat(matrix->num_rows, 1);
  for(int j = 0; j< matrix->num_rows; j++){
    new_col_matrix->values[j][0] = matrix->values[j][col];

  }
  return new_col_matrix;
}

//retrieving a row
mat* get_row_mat(mat* matrix,unsigned int row){
  if(row >= matrix->num_rows){
    fprintf(stderr, "row cant be fetched %d ",row);
    exit(75);
  }
  mat* new_row_matrix = new_mat(1, matrix->num_cols);
  //memory is contiguous hence memcpy can be used.
  memcpy(new_row_matrix->values[0], matrix->values[row], matrix->num_cols * sizeof(*new_row_matrix->values[0]));
  return new_row_matrix;

}

//setting all the cells of the matrix to a particular value
void set_mat_val(mat* matrix, double value){
  for(int i=0; i < matrix->num_rows; i++){
    for(int j = 0; j < matrix->num_cols; j++){
      matrix->values[i][j] = value;
    }
  }
}

//setting the diagonal of a sq matrix to a particular value
int set_mat_diag(mat* matrix, double value){
  if(!matrix->is_square){
    fprintf(stderr, "not a square matrix");
    return 0;
  }
  for(int i = 0; i< matrix->num_rows; i++){
    matrix->values[i][i] = value;
  }
  return 1;
}

//check if all values in matrix equal a specific value
int mat_all_equal(mat* matrix, double value, double tolerance){
  for(int i = 0; i < matrix->num_rows; i++){
    for(int j = 0; j < matrix->num_cols; j++){
      if(fabs(matrix->values[i][j] - value) > tolerance){
        return 0; // Found a value that doesn't match
      }
    }
  }
  return 1; // All values match
}

//multiply a row with scalar
int mat_row_mult_r(mat* matrix, unsigned int row, double num){
  if(matrix->num_rows <= row){
    fprintf(stderr, "row not available");
    return 0;
  }
  for(int i = 0; i < matrix->num_cols; i++){
    matrix->values[row][i] *= num;
  }
  return 1;
}

mat* mat_row_mult(mat* matrix, unsigned int row, double num){
  mat* new_matrix = mat_cp(matrix);
  if(!mat_row_mult_r(new_matrix, row, num)){
    free_mat(new_matrix);
    return NULL;
  }
  return new_matrix;
}


//multiply a column with a scalar
int mat_col_mult_r(mat* matrix, unsigned int col, double num){
  if(matrix->num_cols <= col){
    fprintf(stderr, "col not present");
    return 0;
  }
  for(int i = 0; i < matrix->num_rows; i++){
    matrix->values[i][col] *= num;
  }
  return 1;
}


mat* mat_col_mult(mat* matrix, unsigned int col, double num){
  mat* new_matrix = mat_cp(matrix);
  if(!(mat_col_mult_r(new_matrix, col, num))){
    free_mat(new_matrix);
    return NULL;
  }
  return new_matrix;

}

// adding two rows
int mat_row_addrow_r(mat* matrix, unsigned int where, unsigned int row, double multiplier){
  if(where >= matrix->num_rows || row >= matrix->num_rows){
    fprintf(stderr, "cannot add to row");
    return 0;
  }
  int i = 0;
  for(i = 0; i < matrix->num_cols; i++){
    matrix->values[where][i] += multiplier * matrix->values[row][i];
  }
  return 1;
}

mat* mat_row_addrow(mat* matrix, unsigned int where, unsigned int row, double multiplier){
  mat* new_matrix = mat_cp(matrix);
  if(!mat_row_addrow_r(new_matrix, where, row, multiplier)){
    free_mat(new_matrix);
    return NULL;
  }
  return new_matrix;
}

mat* mat_smult(mat* matrix, double num){
  mat* new_matrix = mat_cp(matrix);
  mat_smult_r(new_matrix, num);
  return new_matrix;
}

int mat_smult_r(mat* matrix, double num){
  for(int i = 0; i < matrix->num_rows; i++){
    for(int j = 0; j < matrix->num_cols; j++){
      matrix->values[i][j] *= num;
    }

  }
  return 1;
}

//removing a column
mat* mat_remove_column(mat* matrix, unsigned int column){
  if(column >= matrix->num_cols){
    fprintf(stderr, "this column cannot be removed");
    return NULL;
  }
  if(matrix->num_cols <= 1){
    fprintf(stderr, "cannot remove column - would result in matrix with 0 columns");
    return NULL;
  }
  mat* new_matrix = new_mat(matrix->num_rows, matrix->num_cols-1);
  for(int i = 0; i < matrix->num_rows; i++){
    int dest_col = 0;//position in destination matrix

    for(int src_col = 0; src_col < matrix->num_cols; src_col++){
      if(src_col != column){
        new_matrix->values[i][dest_col] = matrix->values[i][src_col];
        dest_col++;
      }
      
    }
  }
  return new_matrix;
}


//removing a row
//TODO implement it using memcpy
mat* mat_remove_row(mat* matrix, unsigned int row){
  if(matrix->num_rows <= row){
    fprintf(stderr, "this row cant be removed");
    return NULL;
  }
  if(matrix->num_rows <= 1){
    fprintf(stderr, "cannot remove row - would result in matrix with 0 rows");
    return NULL;
  }
  mat* new_matrix = new_mat(matrix->num_rows -  1, matrix->num_cols);
  int dest_row = 0;
  for(int src_row = 0; src_row < matrix->num_rows; src_row++){
    if(src_row!= row){ //if this is not the row to remove
      for(int j = 0; j < matrix->num_cols; j++){
        new_matrix->values[dest_row][j] = matrix->values[src_row][j];
        
      }
      dest_row++; //increment after copying the entire row
    }
  }
  return new_matrix;
}

//swapping the rows
int mat_row_swap_r(mat* matrix, unsigned int row1, unsigned int row2){
  if((row1 >= matrix->num_rows) || (row2 >= matrix->num_rows)){
    fprintf(stderr, "cannot swap these rows");
    return 0;
  }
  double* temp = matrix->values[row1];
  matrix->values[row1] = matrix->values[row2];
  matrix->values[row2] = temp;
  return 1;
}

mat* mat_row_swap(mat* matrix, unsigned int row1, unsigned int row2){
  mat* new_matrix = mat_cp(matrix);
  if(!(mat_row_swap_r(new_matrix, row1, row2))){
    free_mat(new_matrix);
    return NULL;
  }  
  return new_matrix;
}

int mat_col_swap_r(mat* matrix, unsigned int col1, unsigned int col2){
  if(col1 >= matrix->num_cols || col2 >= matrix->num_cols){
    fprintf(stderr, "cannot swap these columns");
    return 0;
  }
  double tmp;
  int j;
  for(j = 0; j < matrix->num_rows; j++){
    tmp = matrix->values[j][col1];
    matrix->values[j][col1] = matrix->values[j][col2];
    matrix->values[j][col2] = tmp;
    
  }
  return 1;
}

mat* mat_col_swap(mat* matrix, unsigned int col1, unsigned int col2){
  mat* new_matrix = mat_cp(matrix);
  if(!mat_col_swap_r(new_matrix, col1, col2)){
    free_mat(new_matrix);
    return NULL;
  }
  return new_matrix;
}