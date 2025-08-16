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
mat* mat_cp(mat* matrix);
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

//multiplying a row/column with scalar
int mat_row_mult_r(mat* matrix, unsigned int row, double num);
mat* mat_row_mult(mat* matrix, unsigned int row, double num);
int mat_col_mult_r(mat* matrix, unsigned int col, double num);
mat* mat_col_mult(mat* matrix, unsigned int col, double num);

//adding two rows
int mat_row_addrow_r(mat* matrix, unsigned int where, unsigned int row, double multiplier);
mat* mat_row_addrow(mat* matrix, unsigned int where, unsigned int row, double multiplier);

//multiplying the matrix with a scalar
mat* mat_smult(mat* matrix, double num);
int mat_smult_r(mat* matrix, double num);

//modifying the structure of a matrix
//removing a column
mat* mat_remove_column(mat* matrix, unsigned int column);
//removing a row
mat* mat_remove_row(mat* matrix, unsigned int row);
//swapping rows
int mat_row_swap_r(mat* matrix, unsigned int row1, unsigned int row2); //modifies the matrix itself
mat* mat_row_swap(mat* matrix, unsigned int row1, unsigned int row2); //returns a new matrix

//swapping columns
int mat_col_swap_r(mat* matrix, unsigned int col1, unsigned int col2);
mat* mat_col_swap(mat* matrix, unsigned int col1, unsigned int col2);

//concatenation
mat* mat_hor_cat(unsigned int mnum, mat** marr);
mat* mat_vert_cat(unsigned int mnum, mat** marr);
#endif