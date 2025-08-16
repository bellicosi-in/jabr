#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define RAND_MAX 0x7fffffff
#define EPSILON 1e-10

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

//horizontal concatenation
mat* mat_hor_cat(unsigned int mnum, mat** marr){
  //Handle edge cases
  if( mnum == 0) return NULL;
  if( mnum == 1) return mat_cp(marr[0]);

  //Step 1: Validate and calculate dimensions
  unsigned int total_rows = marr[0]->num_rows;
  unsigned int total_cols = 0;

  for(unsigned int k = 0; k < mnum; k++){
    if(marr[k]==NULL){
      fprintf(stderr, "Matrix%u is NULL\n", k);
      return NULL;
    }
    if(marr[k]->num_rows != total_rows){
      fprintf(stderr, "ROW MISMATCH");
      return NULL;
    }
    total_cols += marr[k]->num_cols;

  }
  mat* new_matrix = new_mat(total_rows, total_cols);

  //copy data from all matrics
  for(int i = 0; i < total_rows; i++){
    int current_matrix = 0;
    int cols_copied = 0; //how many columns we have filled so far

    for(int j =0; j < total_cols; j++){
      int col_in_current_matrix = j - cols_copied;
      if(col_in_current_matrix >= marr[current_matrix]->num_cols){
        cols_copied += marr[current_matrix]->num_cols;
        current_matrix++;
        col_in_current_matrix = 0;

      }
      new_matrix->values[i][j] = marr[current_matrix]->values[i][col_in_current_matrix];
    }
  }
  return new_matrix;
}

//vertical concatenation
//TODO REWRITE
mat* mat_vert_cat(unsigned int mnum, mat** marr){
  //handle edge case
  if(mnum == 0) return NULL;
  if(mnum == 1) return mat_cp(marr[0]);

  //validate and calculate dimensions
  unsigned int total_rows = 0;
  unsigned int total_cols = marr[0]->num_cols;

  for(unsigned int k = 0; k < mnum; k++){
    if(marr[k] == NULL){
      fprintf(stderr, "matrix is null %u", k);
      return NULL;

    }
    if(marr[k]->num_cols != total_cols){
      fprintf(stderr, "column mismatch");
      return NULL;
    }
    total_rows += marr[k]->num_rows;
  }
  //create new matrix
  mat* new_matrix = new_mat(total_rows, total_cols);
  int dest_row = 0;
  for(unsigned int k = 0; k < mnum; k++){
    for(unsigned int i = 0; i < marr[k]->num_rows; i++){
      for(unsigned int j = 0; j < total_cols; j++){
        new_matrix->values[dest_row][j] = marr[k]->values[i][j];
      }
      dest_row++;
    }

  } 
  return new_matrix;  
  
}


//ADDING
int mat_add_r(mat* mat1, mat* mat2){
  if(!mat_eqdim(mat1, mat2)){
    fprintf(stderr, "not of saem dimensions");
    return 0;
  }
  for(int i =0; i < mat1->num_rows; i++){
    for(int j = 0; j < mat2->num_cols; j++){
      mat1->values[i][j]+= mat2->values[i][j];
    }
  }
  return 1;
}

mat* mat_add(mat* mat1, mat* mat2){
  mat* new_matrix = mat_cp(mat1);
  if(!mat_add_r(new_matrix, mat2)){
    free_mat(new_matrix);
    return NULL;
  }
  return new_matrix;
}

//subtracting
int mat_sub_r(mat* mat1, mat* mat2){
  if(!mat_eqdim(mat1, mat2)){
    fprintf(stderr, "not of saem dimensions");
    return 0;
  }
  for(int i =0; i < mat1->num_rows; i++){
    for(int j = 0; j < mat2->num_cols; j++){
      mat1->values[i][j]-= mat2->values[i][j];
    }
  }
  return 1;
}

mat* mat_sub(mat* mat1, mat* mat2){
  mat* new_matrix = mat_cp(mat1);
  if(!mat_sub_r(new_matrix, mat2)){
    free_mat(new_matrix);
    return NULL;
  }
  return new_matrix;
}

//dot multiplication of two matrices
mat* mat_dot_r(mat* mat1, mat* mat2){
  if(mat1->num_cols != mat2->num_rows){
    fprintf(stderr, "cannot muliply. ");
    return NULL;
  }
  
  mat* new_matrix = new_mat(mat1->num_rows, mat2->num_cols);
  for(int i = 0; i < new_matrix->num_rows; i++){
    for(int j = 0; j < new_matrix->num_cols; j++){
      for(int k = 0; k < mat1->num_cols; k++){
        new_matrix->values [i][j] += mat1->values[i][k] * mat2->values[k][j];
      }
    }
  }
  return new_matrix;
}

//scalar dot product of a matrix = 
int mat_row_add_scaled(mat* m, unsigned int row_dest, unsigned int row_src, double scalar){
  if(row_dest >= m->num_rows || row_src >= m->num_rows){
    fprintf(stderr, "invalid row indices");
    return 0;
  }
  for(unsigned int j = 0; j < m->num_cols; j++){
    m->values[row_dest][j] += scalar* m->values[row_src][j];
  }
  return 1;
}

//finding the pivot
int find_pivot_row(mat* m, unsigned int col, unsigned int starting_row){

  for(unsigned int i = starting_row; i < m->num_rows; i++){
    if(fabs(m->values[i][col]) > EPSILON){
      return i;
    }
  }
  return -1;
}

// find the max pivot
int find_max_pivot_row(mat* m, unsigned int col, unsigned int starting_row){

  double max_val = 0.0;
  int max_row = -1;

  for(unsigned int i = starting_row; i < m->num_rows; i++){
    double abs_val = fabs(m->values[i][col]);

    if(abs_val > max_val){
      max_val = abs_val;
      max_row = (int)i;
    }
  }
  if(max_val > EPSILON){
    return max_row;
  }
  return -1;
}


//finding the row echelong form
mat* mat_to_ref(mat* m){
  unsigned int current_row = 0;
  unsigned int current_col = 0;
  mat* result = mat_cp(m);
  while(current_row < m->num_rows && current_col < m->num_cols){
    int pivot_row = find_pivot_row(result,current_col, current_row);
    if(pivot_row == -1){
      printf(" Column %u has no pivot", current_col);
      current_col++;
      continue;
    }
    
    //swapping rows if necessary
    if((unsigned int)pivot_row != current_row){
      mat_row_swap_r(result, current_row, (unsigned int)pivot_row);
    }
    for(unsigned int row = current_row + 1; row < result->num_rows; row++){

      if(fabs(result->values[row][current_col]) < EPSILON){
        continue;
      }


      //calculate the multiplier
      double multiplier = result->values[row][current_col] / result->values[current_row][current_col];
      mat_row_add_scaled(result, row, current_row, -multiplier);

    }
    current_row++;
    current_col++;
  }
  return result;
}

//finding the reduced row echelon form
mat* mat_to_rref(mat* m){
  mat* result = mat_cp(m);
  unsigned int current_row = 0;
  unsigned int current_col = 0;
  while(current_row < result->num_rows && current_col < result->num_cols){

    int pivot_row = find_max_pivot_row(result, current_col, current_row);
    if(pivot_row == -1){
      current_col++;
      continue;
    }

    if((unsigned int)pivot_row != current_row){
      mat_row_swap_r(result, current_row, (unsigned int)pivot_row);
    }

    double pivot_val = result->values[current_row][current_col];
    mat_row_mult_r(result, current_row, 1.0 / pivot_val);

    for(unsigned int row = 0; row < result->num_rows; row++){
      if(row == current_row) continue;
      double val = result->values[row][current_col];
      if(fabs(val) > EPSILON){
        mat_row_add_scaled(result, row, current_row, -val);
      }
    }
    current_row++;
    current_col++;
  }
  return result;
}
// LUP Decomposition: A = P^-1 * L * U
// Where P is permutation matrix, L is lower triangular, U is upper triangular
int mat_lup_decomp(mat* A, mat** L, mat** U, mat** P) {
  if (!A || !A->is_square) {
    fprintf(stderr, "LUP decomposition requires square matrix");
    return 0;
  }
  
  unsigned int n = A->num_rows;
  
  // Initialize matrices
  *L = eye_mat(n);        // L starts as identity matrix
  *U = mat_cp(A);         // U starts as copy of A
  *P = eye_mat(n);        // P starts as identity matrix
  
  // Perform LUP decomposition with partial pivoting
  for (unsigned int k = 0; k < n - 1; k++) {
    // Step 1: Find the row with maximum absolute value in column k (partial pivoting)
    int pivot_row = find_max_pivot_row(*U, k, k);
    
    if (pivot_row == -1) {
      // Matrix is singular - all remaining elements in column k are effectively zero
      continue;
    }
    
    // Step 2: Swap rows if necessary for numerical stability
    if ((unsigned int)pivot_row != k) {
      mat_row_swap_r(*U, k, (unsigned int)pivot_row);
      mat_row_swap_r(*P, k, (unsigned int)pivot_row);
      
      // Also swap corresponding rows in L (for the part that's already computed)
      for (unsigned int j = 0; j < k; j++) {
        double temp = (*L)->values[k][j];
        (*L)->values[k][j] = (*L)->values[pivot_row][j];
        (*L)->values[pivot_row][j] = temp;
      }
    }
    
    // Step 3: Eliminate entries below the pivot
    double pivot_val = (*U)->values[k][k];
    
    if (fabs(pivot_val) < EPSILON) {
      continue; // Skip if pivot is too small
    }
    
    for (unsigned int i = k + 1; i < n; i++) {
      // Calculate the multiplier
      double multiplier = (*U)->values[i][k] / pivot_val;
      
      // Store multiplier in L matrix
      (*L)->values[i][k] = multiplier;
      
      // Eliminate the entry in U by subtracting multiplier * row k from row i
      for (unsigned int j = k; j < n; j++) {
        (*U)->values[i][j] -= multiplier * (*U)->values[k][j];
      }
    }
  }
  
  return 1; // Success
}

// Solve the system Ax = b using LUP decomposition
// We have PA = LU, so PAx = Pb
// This gives us LUx = Pb
// We solve this in two steps: Ly = Pb (forward substitution), then Ux = y (backward substitution)
mat* mat_lup_solve(mat* L, mat* U, mat* P, mat* b) {
  if (!L || !U || !P || !b) {
    fprintf(stderr, "Invalid input matrices for LUP solve");
    return NULL;
  }
  
  if (!L->is_square || !U->is_square || !P->is_square || 
      L->num_rows != U->num_rows || L->num_rows != P->num_rows ||
      b->num_rows != L->num_rows || b->num_cols != 1) {
    fprintf(stderr, "Matrix dimensions incompatible for LUP solve");
    return NULL;
  }
  
  unsigned int n = L->num_rows;
  
  // Step 1: Compute Pb (apply permutation to b)
  mat* Pb = mat_dot_r(P, b);
  
  // Step 2: Forward substitution - solve Ly = Pb
  mat* y = new_mat(n, 1);
  for (unsigned int i = 0; i < n; i++) {
    double sum = 0.0;
    for (unsigned int j = 0; j < i; j++) {
      sum += L->values[i][j] * y->values[j][0];
    }
    y->values[i][0] = (Pb->values[i][0] - sum) / L->values[i][i];
  }
  
  // Step 3: Backward substitution - solve Ux = y
  mat* x = new_mat(n, 1);
  for (int i = (int)n - 1; i >= 0; i--) { // Note: using int to handle i >= 0 properly
    double sum = 0.0;
    for (unsigned int j = (unsigned int)i + 1; j < n; j++) {
      sum += U->values[i][j] * x->values[j][0];
    }
    
    if (fabs(U->values[i][i]) < EPSILON) {
      fprintf(stderr, "Matrix is singular - cannot solve system");
      free_mat(Pb);
      free_mat(y);
      free_mat(x);
      return NULL;
    }
    
    x->values[i][0] = (y->values[i][0] - sum) / U->values[i][i];
  }
  
  // Cleanup temporary matrices
  free_mat(Pb);
  free_mat(y);
  
  return x;
}

// Compute determinant using LUP decomposition
// det(A) = det(P^-1) * det(L) * det(U)
// det(P^-1) = (-1)^(number of row swaps)
// det(L) = 1 (since L has 1s on diagonal)  
// det(U) = product of diagonal elements
double mat_det_lup(mat* L, mat* U, mat* P) {
  if (!L || !U || !P) {
    fprintf(stderr, "Invalid matrices for determinant computation");
    return 0.0;
  }
  
  if (!L->is_square || !U->is_square || !P->is_square || 
      L->num_rows != U->num_rows || L->num_rows != P->num_rows) {
    fprintf(stderr, "Matrices must be square and same size for determinant");
    return 0.0;
  }
  
  unsigned int n = L->num_rows;
  
  // Step 1: Count the number of row swaps in P
  // We do this by counting inversions in the permutation
  int swap_count = 0;
  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < n; j++) {
      if (fabs(P->values[i][j] - 1.0) < EPSILON) {
        // Found the 1 in row i, it should be in column i for identity
        if (j != i) {
          // Count this as contributing to swaps
          for (unsigned int k = i + 1; k < n; k++) {
            for (unsigned int l = 0; l < j; l++) {
              if (fabs(P->values[k][l] - 1.0) < EPSILON) {
                swap_count++;
              }
            }
          }
        }
        break;
      }
    }
  }
  
  // Step 2: Compute det(U) = product of diagonal elements
  double det_U = 1.0;
  for (unsigned int i = 0; i < n; i++) {
    det_U *= U->values[i][i];
  }
  
  // Step 3: det(A) = (-1)^swap_count * det(U)
  double det_P_inv = (swap_count % 2 == 0) ? 1.0 : -1.0;
  
  return det_P_inv * det_U;
}

// Matrix transpose - needed for QR decomposition
mat* mat_transpose(mat* matrix) {
  if (!matrix) {
    fprintf(stderr, "Cannot transpose NULL matrix\n");
    return NULL;
  }
  
  mat* transposed = new_mat(matrix->num_cols, matrix->num_rows);
  
  for (unsigned int i = 0; i < matrix->num_rows; i++) {
    for (unsigned int j = 0; j < matrix->num_cols; j++) {
      transposed->values[j][i] = matrix->values[i][j];
    }
  }
  
  return transposed;
}

// QR Decomposition using Modified Gram-Schmidt process
// A = QR where Q is orthogonal and R is upper triangular
int mat_qr_decomp(mat* A, mat** Q, mat** R) {
  if (!A) {
    fprintf(stderr, "QR decomposition requires non-NULL matrix\n");
    return 0;
  }
  
  if (A->num_rows < A->num_cols) {
    fprintf(stderr, "QR decomposition requires m >= n (rows >= cols)\n");
    return 0;
  }
  
  unsigned int m = A->num_rows;
  unsigned int n = A->num_cols;
  
  // Initialize Q and R matrices
  *Q = new_mat(m, n);        // Q is m x n (same size as A for thin QR)
  *R = new_mat(n, n);        // R is n x n upper triangular
  
  // Copy A to Q initially (we'll modify Q during the process)
  for (unsigned int i = 0; i < m; i++) {
    for (unsigned int j = 0; j < n; j++) {
      (*Q)->values[i][j] = A->values[i][j];
    }
  }
  
  // Modified Gram-Schmidt process
  for (unsigned int k = 0; k < n; k++) {
    // Step 1: Compute R[k][k] = ||q_k|| (norm of column k in Q)
    double norm_squared = 0.0;
    for (unsigned int i = 0; i < m; i++) {
      norm_squared += (*Q)->values[i][k] * (*Q)->values[i][k];
    }
    
    double norm = sqrt(norm_squared);
    
    if (norm < EPSILON) {
      fprintf(stderr, "Matrix is rank deficient - QR decomposition may be unstable\n");
      // Continue anyway, but this indicates linear dependence
      norm = EPSILON; // Avoid division by zero
    }
    
    (*R)->values[k][k] = norm;
    
    // Step 2: Normalize column k of Q: q_k = q_k / ||q_k||
    for (unsigned int i = 0; i < m; i++) {
      (*Q)->values[i][k] /= norm;
    }
    
    // Step 3: Orthogonalize remaining columns against q_k
    for (unsigned int j = k + 1; j < n; j++) {
      // Compute R[k][j] = q_k^T * q_j (dot product)
      double dot_product = 0.0;
      for (unsigned int i = 0; i < m; i++) {
        dot_product += (*Q)->values[i][k] * (*Q)->values[i][j];
      }
      
      (*R)->values[k][j] = dot_product;
      
      // Update q_j = q_j - R[k][j] * q_k
      for (unsigned int i = 0; i < m; i++) {
        (*Q)->values[i][j] -= dot_product * (*Q)->values[i][k];
      }
    }
  }
  
  return 1; // Success
}

// Solve the system Ax = b using QR decomposition
// We have A = QR, so QRx = b
// This gives us Rx = Q^T * b (since Q^T * Q = I for orthogonal Q)
// We solve this by backward substitution since R is upper triangular
mat* mat_qr_solve(mat* Q, mat* R, mat* b) {
  if (!Q || !R || !b) {
    fprintf(stderr, "Invalid input matrices for QR solve\n");
    return NULL;
  }
  
  if (Q->num_rows != b->num_rows || b->num_cols != 1 || 
      Q->num_cols != R->num_rows || !R->is_square) {
    fprintf(stderr, "Matrix dimensions incompatible for QR solve\n");
    return NULL;
  }
  
  unsigned int m = Q->num_rows;
  unsigned int n = Q->num_cols;
  
  // Step 1: Compute Q^T * b
  mat* QtB = new_mat(n, 1);
  for (unsigned int i = 0; i < n; i++) {
    double sum = 0.0;
    for (unsigned int j = 0; j < m; j++) {
      sum += Q->values[j][i] * b->values[j][0]; // Q^T[i][j] = Q[j][i]
    }
    QtB->values[i][0] = sum;
  }
  
  // Step 2: Backward substitution to solve Rx = Q^T * b
  mat* x = new_mat(n, 1);
  for (int i = (int)n - 1; i >= 0; i--) { // Use int to handle i >= 0 properly
    double sum = 0.0;
    
    // Sum up R[i][j] * x[j] for j > i
    for (unsigned int j = (unsigned int)i + 1; j < n; j++) {
      sum += R->values[i][j] * x->values[j][0];
    }
    
    // Check for singular matrix (zero diagonal element)
    if (fabs(R->values[i][i]) < EPSILON) {
      fprintf(stderr, "R matrix is singular - cannot solve system\n");
      free_mat(QtB);
      free_mat(x);
      return NULL;
    }
    
    // Solve for x[i]
    x->values[i][0] = (QtB->values[i][0] - sum) / R->values[i][i];
  }
  
  // Cleanup temporary matrix
  free_mat(QtB);
  
  return x;
}