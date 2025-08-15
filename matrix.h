#ifndef jabr_matrix_h
#define jabr_matrix_h

#include <stdlib.h>
#include <stdio.h>

//struct to define a matrix
typedef struct{
  unsigned int num_rows;
  unsigned int num_cols;
  double** values;
  int is_square;
}mat;

//constructor function for the matrix
mat* new_mat(unsigned int num_rows,unsigned int num_cols);
void free_mat(mat* matrix);
mat* random_mat(unsigned int num_rows, unsigned int num_cols, double min, double max);
mat* square_mat(unsigned int size);
mat* random_square_mat(unsigned int size, double min, double max);
mat* eye_mat(unsigned int size);
mat* read_from_filef(FILE* f);


//matrix equality
int mat_eqdim(mat* m1, mat* m2);
int mat_equal(mat* m1,  mat* m2, double tolerance);

//print the matrix
void mat_print(mat* matrix);
void mat_printf(mat* matrix, const char* fmt);

//retrieving a row/col 
mat* get_col_mat(mat* matrix, unsigned int col);
mat* get_row_mat(mat* matrix, unsigned int row);

//setting values
void set_mat_val(mat* matrix, double value);
int set_mat_diag(mat* matrix, double value);

//checking values
int mat_all_equal(mat* matrix, double value, double tolerance);

#endif