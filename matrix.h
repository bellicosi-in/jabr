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
mat* random_mat(unsigned int num_rows, unsigned int num_cols);
mat* square_mat(unsigned int size);
mat* random_square_mat(unsigned int size, double min, double max);
mat* eye_mat(unsigned int size);
mat* read_from_filef(FILE* f);





#endif