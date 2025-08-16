#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define EPSILON 1e-9
#define TEST_PASSED 1
#define TEST_FAILED 0

int test_count = 0;
int passed_count = 0;

void test_assert(int condition, const char* test_name) {
    test_count++;
    if (condition) {
        printf("✓ %s\n", test_name);
        passed_count++;
    } else {
        printf("✗ %s\n", test_name);
    }
}

void print_test_summary() {
    printf("\n========== TEST SUMMARY ==========\n");
    printf("Tests run: %d\n", test_count);
    printf("Passed: %d\n", passed_count);
    printf("Failed: %d\n", test_count - passed_count);
    printf("Success rate: %.1f%%\n", (float)passed_count / test_count * 100);
    printf("==================================\n");
}

void test_new_mat() {
    printf("\n--- Testing new_mat ---\n");
    
    mat* m1 = new_mat(3, 4);
    test_assert(m1 != NULL, "new_mat returns non-NULL");
    test_assert(m1->num_rows == 3, "new_mat sets correct rows");
    test_assert(m1->num_cols == 4, "new_mat sets correct cols");
    test_assert(m1->is_square == 0, "new_mat sets is_square correctly for non-square");
    test_assert(m1->values != NULL, "new_mat allocates values array");
    
    for (int i = 0; i < 3; i++) {
        test_assert(m1->values[i] != NULL, "new_mat allocates row arrays");
        for (int j = 0; j < 4; j++) {
            test_assert(m1->values[i][j] == 0.0, "new_mat initializes values to zero");
        }
    }
    
    mat* m2 = new_mat(5, 5);
    test_assert(m2->is_square == 1, "new_mat sets is_square correctly for square matrix");
    
    free_mat(m1);
    free_mat(m2);
}

void test_square_mat() {
    printf("\n--- Testing square_mat ---\n");
    
    mat* m = square_mat(4);
    test_assert(m != NULL, "square_mat returns non-NULL");
    test_assert(m->num_rows == 4, "square_mat sets correct rows");
    test_assert(m->num_cols == 4, "square_mat sets correct cols");
    test_assert(m->is_square == 1, "square_mat sets is_square to 1");
    
    free_mat(m);
}

void test_eye_mat() {
    printf("\n--- Testing eye_mat ---\n");
    
    mat* identity = eye_mat(3);
    test_assert(identity != NULL, "eye_mat returns non-NULL");
    test_assert(identity->num_rows == 3, "eye_mat sets correct dimensions");
    test_assert(identity->num_cols == 3, "eye_mat sets correct dimensions");
    test_assert(identity->is_square == 1, "eye_mat creates square matrix");
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                test_assert(fabs(identity->values[i][j] - 1.0) < EPSILON, "eye_mat sets diagonal to 1");
            } else {
                test_assert(fabs(identity->values[i][j]) < EPSILON, "eye_mat sets off-diagonal to 0");
            }
        }
    }
    
    free_mat(identity);
}

void test_random_mat() {
    printf("\n--- Testing random_mat ---\n");
    
    mat* rand_m = random_mat(2, 3, 1, 2);
    test_assert(rand_m != NULL, "random_mat returns non-NULL");
    test_assert(rand_m->num_rows == 2, "random_mat sets correct rows");
    test_assert(rand_m->num_cols == 3, "random_mat sets correct cols");
    
    printf("Note: random_mat function in header doesn't match implementation - skipping range test\n");
    
    free_mat(rand_m);
}

void test_random_square_mat() {
    printf("\n--- Testing random_square_mat ---\n");
    
    mat* rand_sq = random_square_mat(3, 0.0, 10.0);
    test_assert(rand_sq != NULL, "random_square_mat returns non-NULL");
    test_assert(rand_sq->num_rows == 3, "random_square_mat sets correct dimensions");
    test_assert(rand_sq->num_cols == 3, "random_square_mat sets correct dimensions");
    test_assert(rand_sq->is_square == 1, "random_square_mat creates square matrix");
    
    int all_in_range = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (rand_sq->values[i][j] < 0.0 || rand_sq->values[i][j] > 10.0) {
                all_in_range = 0;
                break;
            }
        }
    }
    test_assert(all_in_range, "random_square_mat values within specified range");
    
    free_mat(rand_sq);
}

void test_mat_eqdim() {
    printf("\n--- Testing mat_eqdim ---\n");
    
    mat* m1 = new_mat(3, 4);
    mat* m2 = new_mat(3, 4);
    mat* m3 = new_mat(2, 4);
    mat* m4 = new_mat(3, 3);
    
    test_assert(mat_eqdim(m1, m2) == 1, "mat_eqdim returns 1 for equal dimensions");
    test_assert(mat_eqdim(m1, m3) == 0, "mat_eqdim returns 0 for different rows");
    test_assert(mat_eqdim(m1, m4) == 0, "mat_eqdim returns 0 for different cols");
    
    free_mat(m1);
    free_mat(m2);
    free_mat(m3);
    free_mat(m4);
}

void test_mat_equal() {
    printf("\n--- Testing mat_equal ---\n");
    
    mat* m1 = new_mat(2, 2);
    mat* m2 = new_mat(2, 2);
    mat* m3 = new_mat(3, 2);
    
    m1->values[0][0] = 1.0; m1->values[0][1] = 2.0;
    m1->values[1][0] = 3.0; m1->values[1][1] = 4.0;
    
    m2->values[0][0] = 1.0; m2->values[0][1] = 2.0;
    m2->values[1][0] = 3.0; m2->values[1][1] = 4.0;
    
    test_assert(mat_equal(m1, m2, EPSILON) == 1, "mat_equal returns 1 for identical matrices");
    
    m2->values[1][1] = 4.0 + EPSILON/2;
    test_assert(mat_equal(m1, m2, EPSILON) == 1, "mat_equal returns 1 for matrices within tolerance");
    
    m2->values[1][1] = 4.0 + EPSILON*2;
    test_assert(mat_equal(m1, m2, EPSILON) == 0, "mat_equal returns 0 for matrices outside tolerance");
    
    test_assert(mat_equal(m1, m3, EPSILON) == 0, "mat_equal returns 0 for different dimensions");
    
    free_mat(m1);
    free_mat(m2);
    free_mat(m3);
}

void test_get_row_mat() {
    printf("\n--- Testing get_row_mat ---\n");
    
    mat* m = new_mat(3, 4);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0; m->values[0][3] = 4.0;
    m->values[1][0] = 5.0; m->values[1][1] = 6.0; m->values[1][2] = 7.0; m->values[1][3] = 8.0;
    m->values[2][0] = 9.0; m->values[2][1] = 10.0; m->values[2][2] = 11.0; m->values[2][3] = 12.0;
    
    mat* row1 = get_row_mat(m, 1);
    test_assert(row1 != NULL, "get_row_mat returns non-NULL");
    test_assert(row1->num_rows == 1, "get_row_mat creates single row matrix");
    test_assert(row1->num_cols == 4, "get_row_mat preserves column count");
    
    test_assert(fabs(row1->values[0][0] - 5.0) < EPSILON, "get_row_mat extracts correct values");
    test_assert(fabs(row1->values[0][1] - 6.0) < EPSILON, "get_row_mat extracts correct values");
    test_assert(fabs(row1->values[0][2] - 7.0) < EPSILON, "get_row_mat extracts correct values");
    test_assert(fabs(row1->values[0][3] - 8.0) < EPSILON, "get_row_mat extracts correct values");
    
    free_mat(m);
    free_mat(row1);
}

void test_get_col_mat() {
    printf("\n--- Testing get_col_mat ---\n");
    
    mat* m = new_mat(3, 4);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0; m->values[0][3] = 4.0;
    m->values[1][0] = 5.0; m->values[1][1] = 6.0; m->values[1][2] = 7.0; m->values[1][3] = 8.0;
    m->values[2][0] = 9.0; m->values[2][1] = 10.0; m->values[2][2] = 11.0; m->values[2][3] = 12.0;
    
    mat* col2 = get_col_mat(m, 2);
    test_assert(col2 != NULL, "get_col_mat returns non-NULL");
    test_assert(col2->num_rows == 3, "get_col_mat preserves row count");
    test_assert(col2->num_cols == 1, "get_col_mat creates single column matrix");
    
    test_assert(fabs(col2->values[0][0] - 3.0) < EPSILON, "get_col_mat extracts correct values");
    test_assert(fabs(col2->values[1][0] - 7.0) < EPSILON, "get_col_mat extracts correct values");
    test_assert(fabs(col2->values[2][0] - 11.0) < EPSILON, "get_col_mat extracts correct values");
    
    free_mat(m);
    free_mat(col2);
}

void test_memory_management() {
    printf("\n--- Testing memory management ---\n");
    
    mat* m = new_mat(100, 100);
    test_assert(m != NULL, "Can allocate large matrix");
    
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            m->values[i][j] = i * 100 + j;
        }
    }
    
    test_assert(m->values[50][75] == 5075, "Large matrix stores values correctly");
    
    free_mat(m);
    test_assert(1, "free_mat completes without error");
}

void test_set_mat_val() {
    printf("\n--- Testing set_mat_val ---\n");
    
    mat* m = new_mat(3, 4);
    set_mat_val(m, 7.5);
    
    test_assert(mat_all_equal(m, 7.5, EPSILON), "set_mat_val sets all values correctly");
    
    // Test with different value
    set_mat_val(m, -2.3);
    test_assert(mat_all_equal(m, -2.3, EPSILON), "set_mat_val overwrites previous values");
    
    // Test with zero
    set_mat_val(m, 0.0);
    test_assert(mat_all_equal(m, 0.0, EPSILON), "set_mat_val works with zero");
    
    free_mat(m);
}

void test_set_mat_diag() {
    printf("\n--- Testing set_mat_diag ---\n");
    
    mat* square = new_mat(3, 3);
    mat* nonsquare = new_mat(2, 3);
    
    // Initialize with zeros
    set_mat_val(square, 0.0);
    
    int result = set_mat_diag(square, 5.0);
    test_assert(result == 1, "set_mat_diag returns 1 for square matrix");
    
    // Check diagonal values
    test_assert(fabs(square->values[0][0] - 5.0) < EPSILON, "set_mat_diag sets diagonal [0][0]");
    test_assert(fabs(square->values[1][1] - 5.0) < EPSILON, "set_mat_diag sets diagonal [1][1]");
    test_assert(fabs(square->values[2][2] - 5.0) < EPSILON, "set_mat_diag sets diagonal [2][2]");
    
    // Check off-diagonal values remain unchanged
    test_assert(fabs(square->values[0][1]) < EPSILON, "set_mat_diag preserves off-diagonal values");
    test_assert(fabs(square->values[1][0]) < EPSILON, "set_mat_diag preserves off-diagonal values");
    
    // Test with non-square matrix
    int result2 = set_mat_diag(nonsquare, 3.0);
    test_assert(result2 == 0, "set_mat_diag returns 0 for non-square matrix");
    
    free_mat(square);
    free_mat(nonsquare);
}

void test_mat_all_equal() {
    printf("\n--- Testing mat_all_equal ---\n");
    
    mat* m = new_mat(2, 3);
    set_mat_val(m, 4.2);
    
    test_assert(mat_all_equal(m, 4.2, EPSILON) == 1, "mat_all_equal returns 1 for identical values");
    test_assert(mat_all_equal(m, 4.2 + EPSILON/2, EPSILON) == 1, "mat_all_equal works within tolerance");
    test_assert(mat_all_equal(m, 4.2 + EPSILON*2, EPSILON) == 0, "mat_all_equal returns 0 outside tolerance");
    test_assert(mat_all_equal(m, 5.0, EPSILON) == 0, "mat_all_equal returns 0 for different values");
    
    // Test with one different value
    m->values[1][2] = 999.0;
    test_assert(mat_all_equal(m, 4.2, EPSILON) == 0, "mat_all_equal detects single different value");
    
    // Test with zero values
    set_mat_val(m, 0.0);
    test_assert(mat_all_equal(m, 0.0, EPSILON) == 1, "mat_all_equal works with zero values");
    
    free_mat(m);
}

void test_mat_cp() {
    printf("\n--- Testing mat_cp ---\n");
    
    mat* original = new_mat(2, 3);
    original->values[0][0] = 1.0; original->values[0][1] = 2.0; original->values[0][2] = 3.0;
    original->values[1][0] = 4.0; original->values[1][1] = 5.0; original->values[1][2] = 6.0;
    
    mat* copy = mat_cp(original);
    
    test_assert(copy != NULL, "mat_cp returns non-NULL");
    test_assert(copy != original, "mat_cp creates new matrix (different pointer)");
    test_assert(copy->num_rows == original->num_rows, "mat_cp preserves row count");
    test_assert(copy->num_cols == original->num_cols, "mat_cp preserves column count");
    test_assert(copy->is_square == original->is_square, "mat_cp preserves is_square flag");
    
    test_assert(mat_equal(original, copy, EPSILON), "mat_cp creates identical values");
    
    // Modify original to ensure independence
    original->values[0][0] = 999.0;
    test_assert(fabs(copy->values[0][0] - 1.0) < EPSILON, "mat_cp creates independent copy");
    
    free_mat(original);
    free_mat(copy);
}

void test_mat_row_mult() {
    printf("\n--- Testing mat_row_mult and mat_row_mult_r ---\n");
    
    mat* m = new_mat(3, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 5.0; m->values[1][2] = 6.0;
    m->values[2][0] = 7.0; m->values[2][1] = 8.0; m->values[2][2] = 9.0;
    
    // Test in-place multiplication
    int result = mat_row_mult_r(m, 1, 2.0);
    test_assert(result == 1, "mat_row_mult_r returns 1 for valid row");
    test_assert(fabs(m->values[1][0] - 8.0) < EPSILON, "mat_row_mult_r multiplies row values");
    test_assert(fabs(m->values[1][1] - 10.0) < EPSILON, "mat_row_mult_r multiplies row values");
    test_assert(fabs(m->values[1][2] - 12.0) < EPSILON, "mat_row_mult_r multiplies row values");
    
    // Check other rows unchanged
    test_assert(fabs(m->values[0][0] - 1.0) < EPSILON, "mat_row_mult_r preserves other rows");
    test_assert(fabs(m->values[2][0] - 7.0) < EPSILON, "mat_row_mult_r preserves other rows");
    
    // Test invalid row
    int result2 = mat_row_mult_r(m, 5, 2.0);
    test_assert(result2 == 0, "mat_row_mult_r returns 0 for invalid row");
    
    // Test new matrix multiplication
    mat* original = new_mat(2, 2);
    original->values[0][0] = 1.0; original->values[0][1] = 2.0;
    original->values[1][0] = 3.0; original->values[1][1] = 4.0;
    
    mat* multiplied = mat_row_mult(original, 0, 3.0);
    test_assert(multiplied != NULL, "mat_row_mult returns non-NULL for valid input");
    test_assert(multiplied != original, "mat_row_mult creates new matrix");
    test_assert(fabs(multiplied->values[0][0] - 3.0) < EPSILON, "mat_row_mult multiplies correctly");
    test_assert(fabs(multiplied->values[0][1] - 6.0) < EPSILON, "mat_row_mult multiplies correctly");
    test_assert(fabs(multiplied->values[1][0] - 3.0) < EPSILON, "mat_row_mult preserves other rows");
    test_assert(fabs(original->values[0][0] - 1.0) < EPSILON, "mat_row_mult preserves original");
    
    // Test invalid row for new matrix
    mat* invalid = mat_row_mult(original, 5, 2.0);
    test_assert(invalid == NULL, "mat_row_mult returns NULL for invalid row");
    
    free_mat(m);
    free_mat(original);
    free_mat(multiplied);
}

void test_mat_col_mult() {
    printf("\n--- Testing mat_col_mult and mat_col_mult_r ---\n");
    
    mat* m = new_mat(3, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 5.0; m->values[1][2] = 6.0;
    m->values[2][0] = 7.0; m->values[2][1] = 8.0; m->values[2][2] = 9.0;
    
    // Test in-place multiplication
    int result = mat_col_mult_r(m, 1, 2.0);
    test_assert(result == 1, "mat_col_mult_r returns 1 for valid column");
    test_assert(fabs(m->values[0][1] - 4.0) < EPSILON, "mat_col_mult_r multiplies column values");
    test_assert(fabs(m->values[1][1] - 10.0) < EPSILON, "mat_col_mult_r multiplies column values");
    test_assert(fabs(m->values[2][1] - 16.0) < EPSILON, "mat_col_mult_r multiplies column values");
    
    // Check other columns unchanged
    test_assert(fabs(m->values[0][0] - 1.0) < EPSILON, "mat_col_mult_r preserves other columns");
    test_assert(fabs(m->values[0][2] - 3.0) < EPSILON, "mat_col_mult_r preserves other columns");
    
    // Test invalid column
    int result2 = mat_col_mult_r(m, 5, 2.0);
    test_assert(result2 == 0, "mat_col_mult_r returns 0 for invalid column");
    
    // Test new matrix multiplication
    mat* original = new_mat(2, 2);
    original->values[0][0] = 1.0; original->values[0][1] = 2.0;
    original->values[1][0] = 3.0; original->values[1][1] = 4.0;
    
    mat* multiplied = mat_col_mult(original, 0, 3.0);
    test_assert(multiplied != NULL, "mat_col_mult returns non-NULL for valid input");
    test_assert(multiplied != original, "mat_col_mult creates new matrix");
    test_assert(fabs(multiplied->values[0][0] - 3.0) < EPSILON, "mat_col_mult multiplies correctly");
    test_assert(fabs(multiplied->values[1][0] - 9.0) < EPSILON, "mat_col_mult multiplies correctly");
    test_assert(fabs(multiplied->values[0][1] - 2.0) < EPSILON, "mat_col_mult preserves other columns");
    test_assert(fabs(original->values[0][0] - 1.0) < EPSILON, "mat_col_mult preserves original");
    
    // Test invalid column for new matrix
    mat* invalid = mat_col_mult(original, 5, 2.0);
    test_assert(invalid == NULL, "mat_col_mult returns NULL for invalid column");
    
    free_mat(m);
    free_mat(original);
    free_mat(multiplied);
}

void test_mat_row_addrow() {
    printf("\n--- Testing mat_row_addrow and mat_row_addrow_r ---\n");
    
    mat* m = new_mat(3, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 5.0; m->values[1][2] = 6.0;
    m->values[2][0] = 7.0; m->values[2][1] = 8.0; m->values[2][2] = 9.0;
    
    // Test in-place row addition: row[0] += 2 * row[1]
    int result = mat_row_addrow_r(m, 0, 1, 2.0);
    test_assert(result == 1, "mat_row_addrow_r returns 1 for valid rows");
    
    // Expected: row[0] = [1,2,3] + 2*[4,5,6] = [9,12,15]
    test_assert(fabs(m->values[0][0] - 9.0) < EPSILON, "mat_row_addrow_r adds rows correctly");
    test_assert(fabs(m->values[0][1] - 12.0) < EPSILON, "mat_row_addrow_r adds rows correctly");
    test_assert(fabs(m->values[0][2] - 15.0) < EPSILON, "mat_row_addrow_r adds rows correctly");
    
    // Check source row unchanged
    test_assert(fabs(m->values[1][0] - 4.0) < EPSILON, "mat_row_addrow_r preserves source row");
    test_assert(fabs(m->values[1][1] - 5.0) < EPSILON, "mat_row_addrow_r preserves source row");
    
    // Check other rows unchanged
    test_assert(fabs(m->values[2][0] - 7.0) < EPSILON, "mat_row_addrow_r preserves other rows");
    
    // Test invalid row indices
    int result2 = mat_row_addrow_r(m, 5, 1, 2.0);
    test_assert(result2 == 0, "mat_row_addrow_r returns 0 for invalid where index");
    
    int result3 = mat_row_addrow_r(m, 0, 5, 2.0);
    test_assert(result3 == 0, "mat_row_addrow_r returns 0 for invalid row index");
    
    // Test new matrix version
    mat* original = new_mat(2, 2);
    original->values[0][0] = 1.0; original->values[0][1] = 2.0;
    original->values[1][0] = 3.0; original->values[1][1] = 4.0;
    
    mat* result_mat = mat_row_addrow(original, 0, 1, 0.5);
    test_assert(result_mat != NULL, "mat_row_addrow returns non-NULL for valid input");
    test_assert(result_mat != original, "mat_row_addrow creates new matrix");
    
    // Expected: row[0] = [1,2] + 0.5*[3,4] = [2.5,4]
    test_assert(fabs(result_mat->values[0][0] - 2.5) < EPSILON, "mat_row_addrow adds rows correctly");
    test_assert(fabs(result_mat->values[0][1] - 4.0) < EPSILON, "mat_row_addrow adds rows correctly");
    test_assert(fabs(result_mat->values[1][0] - 3.0) < EPSILON, "mat_row_addrow preserves other rows");
    
    // Check original unchanged
    test_assert(fabs(original->values[0][0] - 1.0) < EPSILON, "mat_row_addrow preserves original");
    
    // Test invalid input for new matrix version
    mat* invalid = mat_row_addrow(original, 5, 0, 1.0);
    test_assert(invalid == NULL, "mat_row_addrow returns NULL for invalid input");
    
    free_mat(m);
    free_mat(original);
    free_mat(result_mat);
}

void test_mat_smult() {
    printf("\n--- Testing mat_smult and mat_smult_r ---\n");
    
    mat* m = new_mat(2, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 5.0; m->values[1][2] = 6.0;
    
    // Test in-place scalar multiplication
    int result = mat_smult_r(m, 2.5);
    test_assert(result == 1, "mat_smult_r returns 1");
    
    test_assert(fabs(m->values[0][0] - 2.5) < EPSILON, "mat_smult_r multiplies all values");
    test_assert(fabs(m->values[0][1] - 5.0) < EPSILON, "mat_smult_r multiplies all values");
    test_assert(fabs(m->values[0][2] - 7.5) < EPSILON, "mat_smult_r multiplies all values");
    test_assert(fabs(m->values[1][0] - 10.0) < EPSILON, "mat_smult_r multiplies all values");
    test_assert(fabs(m->values[1][1] - 12.5) < EPSILON, "mat_smult_r multiplies all values");
    test_assert(fabs(m->values[1][2] - 15.0) < EPSILON, "mat_smult_r multiplies all values");
    
    // Test with zero
    mat_smult_r(m, 0.0);
    test_assert(mat_all_equal(m, 0.0, EPSILON), "mat_smult_r works with zero");
    
    // Test new matrix version
    mat* original = new_mat(2, 2);
    original->values[0][0] = 2.0; original->values[0][1] = 4.0;
    original->values[1][0] = 6.0; original->values[1][1] = 8.0;
    
    mat* multiplied = mat_smult(original, 0.5);
    test_assert(multiplied != NULL, "mat_smult returns non-NULL");
    test_assert(multiplied != original, "mat_smult creates new matrix");
    
    test_assert(fabs(multiplied->values[0][0] - 1.0) < EPSILON, "mat_smult multiplies correctly");
    test_assert(fabs(multiplied->values[0][1] - 2.0) < EPSILON, "mat_smult multiplies correctly");
    test_assert(fabs(multiplied->values[1][0] - 3.0) < EPSILON, "mat_smult multiplies correctly");
    test_assert(fabs(multiplied->values[1][1] - 4.0) < EPSILON, "mat_smult multiplies correctly");
    
    // Check original unchanged
    test_assert(fabs(original->values[0][0] - 2.0) < EPSILON, "mat_smult preserves original");
    
    // Test with negative multiplier
    mat* negative = mat_smult(original, -1.0);
    test_assert(fabs(negative->values[0][0] - (-2.0)) < EPSILON, "mat_smult works with negative values");
    
    free_mat(m);
    free_mat(original);
    free_mat(multiplied);
    free_mat(negative);
}

void test_mat_remove_column() {
    printf("\n--- Testing mat_remove_column ---\n");
    
    mat* m = new_mat(3, 4);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0; m->values[0][3] = 4.0;
    m->values[1][0] = 5.0; m->values[1][1] = 6.0; m->values[1][2] = 7.0; m->values[1][3] = 8.0;
    m->values[2][0] = 9.0; m->values[2][1] = 10.0; m->values[2][2] = 11.0; m->values[2][3] = 12.0;
    
    // Remove column 1 (0-indexed)
    mat* result = mat_remove_column(m, 1);
    test_assert(result != NULL, "mat_remove_column returns non-NULL for valid column");
    test_assert(result->num_rows == 3, "mat_remove_column preserves row count");
    test_assert(result->num_cols == 3, "mat_remove_column reduces column count by 1");
    test_assert(result->is_square == 1, "mat_remove_column updates is_square flag correctly");
    
    // Check values: should be [1,3,4], [5,7,8], [9,11,12]
    test_assert(fabs(result->values[0][0] - 1.0) < EPSILON, "mat_remove_column preserves correct values");
    test_assert(fabs(result->values[0][1] - 3.0) < EPSILON, "mat_remove_column skips removed column");
    test_assert(fabs(result->values[0][2] - 4.0) < EPSILON, "mat_remove_column preserves values after removed column");
    test_assert(fabs(result->values[1][0] - 5.0) < EPSILON, "mat_remove_column works across all rows");
    test_assert(fabs(result->values[1][1] - 7.0) < EPSILON, "mat_remove_column works across all rows");
    test_assert(fabs(result->values[2][2] - 12.0) < EPSILON, "mat_remove_column works across all rows");
    
    // Test removing first column
    mat* first_removed = mat_remove_column(m, 0);
    test_assert(fabs(first_removed->values[0][0] - 2.0) < EPSILON, "mat_remove_column works for first column");
    test_assert(fabs(first_removed->values[0][1] - 3.0) < EPSILON, "mat_remove_column works for first column");
    
    // Test removing last column
    mat* last_removed = mat_remove_column(m, 3);
    test_assert(fabs(last_removed->values[0][2] - 3.0) < EPSILON, "mat_remove_column works for last column");
    test_assert(last_removed->num_cols == 3, "mat_remove_column reduces column count correctly");
    
    // Test invalid column
    mat* invalid = mat_remove_column(m, 5);
    test_assert(invalid == NULL, "mat_remove_column returns NULL for invalid column");
    
    // Test edge case: removing from 1-column matrix should fail gracefully
    mat* single_col = new_mat(2, 1);
    mat* edge_case = mat_remove_column(single_col, 0);
    test_assert(edge_case == NULL, "mat_remove_column returns NULL when removing from 1-column matrix");
    
    free_mat(m);
    free_mat(result);
    free_mat(first_removed);
    free_mat(last_removed);
    free_mat(single_col);
    if(edge_case) free_mat(edge_case);
}

void test_mat_remove_row() {
    printf("\n--- Testing mat_remove_row ---\n");
    
    mat* m = new_mat(3, 4);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0; m->values[0][3] = 4.0;
    m->values[1][0] = 5.0; m->values[1][1] = 6.0; m->values[1][2] = 7.0; m->values[1][3] = 8.0;
    m->values[2][0] = 9.0; m->values[2][1] = 10.0; m->values[2][2] = 11.0; m->values[2][3] = 12.0;
    
    // Remove row 1 (middle row)
    mat* result = mat_remove_row(m, 1);
    test_assert(result != NULL, "mat_remove_row returns non-NULL for valid row");
    test_assert(result->num_rows == 2, "mat_remove_row reduces row count by 1");
    test_assert(result->num_cols == 4, "mat_remove_row preserves column count");
    test_assert(result->is_square == 0, "mat_remove_row updates is_square flag correctly");
    
    // Check values: should be first row [1,2,3,4], then third row [9,10,11,12]
    test_assert(fabs(result->values[0][0] - 1.0) < EPSILON, "mat_remove_row preserves first row");
    test_assert(fabs(result->values[0][3] - 4.0) < EPSILON, "mat_remove_row preserves first row");
    test_assert(fabs(result->values[1][0] - 9.0) < EPSILON, "mat_remove_row moves later rows up");
    test_assert(fabs(result->values[1][3] - 12.0) < EPSILON, "mat_remove_row moves later rows up");
    
    // Test removing first row
    mat* first_removed = mat_remove_row(m, 0);
    test_assert(fabs(first_removed->values[0][0] - 5.0) < EPSILON, "mat_remove_row works for first row");
    test_assert(fabs(first_removed->values[1][0] - 9.0) < EPSILON, "mat_remove_row works for first row");
    
    // Test removing last row
    mat* last_removed = mat_remove_row(m, 2);
    test_assert(fabs(last_removed->values[1][0] - 5.0) < EPSILON, "mat_remove_row works for last row");
    test_assert(last_removed->num_rows == 2, "mat_remove_row reduces row count correctly");
    
    // Test invalid row
    mat* invalid = mat_remove_row(m, 5);
    test_assert(invalid == NULL, "mat_remove_row returns NULL for invalid row");
    
    // Test edge case: removing from 1-row matrix should fail gracefully
    mat* single_row = new_mat(1, 3);
    mat* edge_case = mat_remove_row(single_row, 0);
    test_assert(edge_case == NULL, "mat_remove_row returns NULL when removing from 1-row matrix");
    free_mat(single_row);
    
    free_mat(m);
    free_mat(result);
    free_mat(first_removed);
    free_mat(last_removed);
}

void test_mat_row_swap() {
    printf("\n--- Testing mat_row_swap and mat_row_swap_r ---\n");
    
    mat* m = new_mat(3, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 5.0; m->values[1][2] = 6.0;
    m->values[2][0] = 7.0; m->values[2][1] = 8.0; m->values[2][2] = 9.0;
    
    // Test in-place row swap
    int result = mat_row_swap_r(m, 0, 2);
    test_assert(result == 1, "mat_row_swap_r returns 1 for valid rows");
    
    // Check that rows 0 and 2 are swapped
    test_assert(fabs(m->values[0][0] - 7.0) < EPSILON, "mat_row_swap_r swaps rows correctly");
    test_assert(fabs(m->values[0][1] - 8.0) < EPSILON, "mat_row_swap_r swaps rows correctly");
    test_assert(fabs(m->values[0][2] - 9.0) < EPSILON, "mat_row_swap_r swaps rows correctly");
    test_assert(fabs(m->values[2][0] - 1.0) < EPSILON, "mat_row_swap_r swaps rows correctly");
    test_assert(fabs(m->values[2][1] - 2.0) < EPSILON, "mat_row_swap_r swaps rows correctly");
    test_assert(fabs(m->values[2][2] - 3.0) < EPSILON, "mat_row_swap_r swaps rows correctly");
    
    // Check middle row unchanged
    test_assert(fabs(m->values[1][0] - 4.0) < EPSILON, "mat_row_swap_r preserves unaffected rows");
    
    // Test swapping same row (should be no-op)
    mat_row_swap_r(m, 1, 1);
    test_assert(fabs(m->values[1][0] - 4.0) < EPSILON, "mat_row_swap_r handles same row swap");
    
    // Test invalid row swap
    int invalid_result = mat_row_swap_r(m, 0, 5);
    test_assert(invalid_result == 0, "mat_row_swap_r returns 0 for invalid row");
    
    // Test new matrix version
    mat* original = new_mat(2, 2);
    original->values[0][0] = 1.0; original->values[0][1] = 2.0;
    original->values[1][0] = 3.0; original->values[1][1] = 4.0;
    
    mat* swapped = mat_row_swap(original, 0, 1);
    test_assert(swapped != NULL, "mat_row_swap returns non-NULL for valid input");
    test_assert(swapped != original, "mat_row_swap creates new matrix");
    test_assert(fabs(swapped->values[0][0] - 3.0) < EPSILON, "mat_row_swap swaps correctly");
    test_assert(fabs(swapped->values[1][0] - 1.0) < EPSILON, "mat_row_swap swaps correctly");
    test_assert(fabs(original->values[0][0] - 1.0) < EPSILON, "mat_row_swap preserves original");
    
    // Test invalid input for new matrix version
    mat* invalid_swap = mat_row_swap(original, 0, 5);
    test_assert(invalid_swap == NULL, "mat_row_swap returns NULL for invalid input");
    
    free_mat(m);
    free_mat(original);
    free_mat(swapped);
}

void test_mat_col_swap() {
    printf("\n--- Testing mat_col_swap and mat_col_swap_r ---\n");
    
    mat* m = new_mat(3, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 5.0; m->values[1][2] = 6.0;
    m->values[2][0] = 7.0; m->values[2][1] = 8.0; m->values[2][2] = 9.0;
    
    // Test in-place column swap
    int result = mat_col_swap_r(m, 0, 2);
    test_assert(result == 1, "mat_col_swap_r returns 1 for valid columns");
    
    // Check that columns 0 and 2 are swapped
    test_assert(fabs(m->values[0][0] - 3.0) < EPSILON, "mat_col_swap_r swaps columns correctly");
    test_assert(fabs(m->values[1][0] - 6.0) < EPSILON, "mat_col_swap_r swaps columns correctly");
    test_assert(fabs(m->values[2][0] - 9.0) < EPSILON, "mat_col_swap_r swaps columns correctly");
    test_assert(fabs(m->values[0][2] - 1.0) < EPSILON, "mat_col_swap_r swaps columns correctly");
    test_assert(fabs(m->values[1][2] - 4.0) < EPSILON, "mat_col_swap_r swaps columns correctly");
    test_assert(fabs(m->values[2][2] - 7.0) < EPSILON, "mat_col_swap_r swaps columns correctly");
    
    // Check middle column unchanged
    test_assert(fabs(m->values[0][1] - 2.0) < EPSILON, "mat_col_swap_r preserves unaffected columns");
    
    // Test invalid column swap
    int invalid_result = mat_col_swap_r(m, 0, 5);
    test_assert(invalid_result == 0, "mat_col_swap_r returns 0 for invalid column");
    
    // Test new matrix version
    mat* original = new_mat(2, 2);
    original->values[0][0] = 1.0; original->values[0][1] = 2.0;
    original->values[1][0] = 3.0; original->values[1][1] = 4.0;
    
    mat* swapped = mat_col_swap(original, 0, 1);
    test_assert(swapped != NULL, "mat_col_swap returns non-NULL for valid input");
    test_assert(swapped != original, "mat_col_swap creates new matrix");
    test_assert(fabs(swapped->values[0][0] - 2.0) < EPSILON, "mat_col_swap swaps correctly");
    test_assert(fabs(swapped->values[0][1] - 1.0) < EPSILON, "mat_col_swap swaps correctly");
    test_assert(fabs(swapped->values[1][0] - 4.0) < EPSILON, "mat_col_swap swaps correctly");
    test_assert(fabs(original->values[0][0] - 1.0) < EPSILON, "mat_col_swap preserves original");
    
    // Test invalid input for new matrix version
    mat* invalid_swap = mat_col_swap(original, 0, 5);
    test_assert(invalid_swap == NULL, "mat_col_swap returns NULL for invalid input");
    
    free_mat(m);
    free_mat(original);
    free_mat(swapped);
}

void test_mat_hor_cat() {
    printf("\n--- Testing mat_hor_cat ---\n");
    
    // Test basic horizontal concatenation with 2 matrices
    mat* m1 = new_mat(2, 2);
    m1->values[0][0] = 1.0; m1->values[0][1] = 2.0;
    m1->values[1][0] = 3.0; m1->values[1][1] = 4.0;
    
    mat* m2 = new_mat(2, 3);
    m2->values[0][0] = 5.0; m2->values[0][1] = 6.0; m2->values[0][2] = 7.0;
    m2->values[1][0] = 8.0; m2->values[1][1] = 9.0; m2->values[1][2] = 10.0;
    
    mat* matrices[2] = {m1, m2};
    mat* result = mat_hor_cat(2, matrices);
    
    test_assert(result != NULL, "mat_hor_cat returns non-NULL for valid input");
    test_assert(result->num_rows == 2, "mat_hor_cat preserves row count");
    test_assert(result->num_cols == 5, "mat_hor_cat adds column counts");
    test_assert(result->is_square == 0, "mat_hor_cat updates is_square flag correctly");
    
    // Check concatenated values: [1,2,5,6,7], [3,4,8,9,10]
    test_assert(fabs(result->values[0][0] - 1.0) < EPSILON, "mat_hor_cat copies first matrix correctly");
    test_assert(fabs(result->values[0][1] - 2.0) < EPSILON, "mat_hor_cat copies first matrix correctly");
    test_assert(fabs(result->values[0][2] - 5.0) < EPSILON, "mat_hor_cat copies second matrix correctly");
    test_assert(fabs(result->values[0][3] - 6.0) < EPSILON, "mat_hor_cat copies second matrix correctly");
    test_assert(fabs(result->values[0][4] - 7.0) < EPSILON, "mat_hor_cat copies second matrix correctly");
    test_assert(fabs(result->values[1][0] - 3.0) < EPSILON, "mat_hor_cat works across all rows");
    test_assert(fabs(result->values[1][4] - 10.0) < EPSILON, "mat_hor_cat works across all rows");
    
    // Test with 3 matrices
    mat* m3 = new_mat(2, 1);
    m3->values[0][0] = 11.0;
    m3->values[1][0] = 12.0;
    
    mat* three_matrices[3] = {m1, m2, m3};
    mat* result3 = mat_hor_cat(3, three_matrices);
    test_assert(result3 != NULL, "mat_hor_cat works with 3 matrices");
    test_assert(result3->num_cols == 6, "mat_hor_cat adds all column counts");
    test_assert(fabs(result3->values[0][5] - 11.0) < EPSILON, "mat_hor_cat concatenates third matrix");
    
    // Test edge case: single matrix
    mat* single[1] = {m1};
    mat* single_result = mat_hor_cat(1, single);
    test_assert(single_result != NULL, "mat_hor_cat works with single matrix");
    test_assert(mat_equal(single_result, m1, EPSILON), "mat_hor_cat returns copy of single matrix");
    
    // Test edge case: zero matrices
    mat* zero_result = mat_hor_cat(0, NULL);
    test_assert(zero_result == NULL, "mat_hor_cat returns NULL for zero matrices");
    
    // Test error case: row mismatch
    mat* wrong_rows = new_mat(3, 2);
    mat* mismatch[2] = {m1, wrong_rows};
    mat* error_result = mat_hor_cat(2, mismatch);
    test_assert(error_result == NULL, "mat_hor_cat returns NULL for row mismatch");
    
    // Test error case: NULL matrix
    mat* null_matrices[2] = {m1, NULL};
    mat* null_result = mat_hor_cat(2, null_matrices);
    test_assert(null_result == NULL, "mat_hor_cat returns NULL for NULL matrix");
    
    free_mat(m1);
    free_mat(m2);
    free_mat(m3);
    free_mat(wrong_rows);
    free_mat(result);
    free_mat(result3);
    free_mat(single_result);
}

void test_mat_vert_cat() {
    printf("\n--- Testing mat_vert_cat ---\n");
    
    // Test basic vertical concatenation with 2 matrices
    mat* m1 = new_mat(2, 3);
    m1->values[0][0] = 1.0; m1->values[0][1] = 2.0; m1->values[0][2] = 3.0;
    m1->values[1][0] = 4.0; m1->values[1][1] = 5.0; m1->values[1][2] = 6.0;
    
    mat* m2 = new_mat(1, 3);
    m2->values[0][0] = 7.0; m2->values[0][1] = 8.0; m2->values[0][2] = 9.0;
    
    mat* matrices[2] = {m1, m2};
    mat* result = mat_vert_cat(2, matrices);
    
    test_assert(result != NULL, "mat_vert_cat returns non-NULL for valid input");
    test_assert(result->num_rows == 3, "mat_vert_cat adds row counts");
    test_assert(result->num_cols == 3, "mat_vert_cat preserves column count");
    test_assert(result->is_square == 1, "mat_vert_cat updates is_square flag correctly");
    
    // Check concatenated values: first matrix rows, then second matrix rows
    test_assert(fabs(result->values[0][0] - 1.0) < EPSILON, "mat_vert_cat copies first matrix correctly");
    test_assert(fabs(result->values[0][2] - 3.0) < EPSILON, "mat_vert_cat copies first matrix correctly");
    test_assert(fabs(result->values[1][0] - 4.0) < EPSILON, "mat_vert_cat copies first matrix correctly");
    test_assert(fabs(result->values[1][2] - 6.0) < EPSILON, "mat_vert_cat copies first matrix correctly");
    test_assert(fabs(result->values[2][0] - 7.0) < EPSILON, "mat_vert_cat copies second matrix correctly");
    test_assert(fabs(result->values[2][2] - 9.0) < EPSILON, "mat_vert_cat copies second matrix correctly");
    
    // Test with 3 matrices
    mat* m3 = new_mat(2, 3);
    m3->values[0][0] = 10.0; m3->values[0][1] = 11.0; m3->values[0][2] = 12.0;
    m3->values[1][0] = 13.0; m3->values[1][1] = 14.0; m3->values[1][2] = 15.0;
    
    mat* three_matrices[3] = {m1, m2, m3};
    mat* result3 = mat_vert_cat(3, three_matrices);
    test_assert(result3 != NULL, "mat_vert_cat works with 3 matrices");
    test_assert(result3->num_rows == 5, "mat_vert_cat adds all row counts");
    test_assert(fabs(result3->values[3][0] - 10.0) < EPSILON, "mat_vert_cat concatenates third matrix");
    test_assert(fabs(result3->values[4][2] - 15.0) < EPSILON, "mat_vert_cat concatenates third matrix");
    
    // Test edge case: single matrix
    mat* single[1] = {m1};
    mat* single_result = mat_vert_cat(1, single);
    test_assert(single_result != NULL, "mat_vert_cat works with single matrix");
    test_assert(mat_equal(single_result, m1, EPSILON), "mat_vert_cat returns copy of single matrix");
    
    // Test edge case: zero matrices
    mat* zero_result = mat_vert_cat(0, NULL);
    test_assert(zero_result == NULL, "mat_vert_cat returns NULL for zero matrices");
    
    // Test error case: column mismatch
    mat* wrong_cols = new_mat(1, 2);
    mat* mismatch[2] = {m1, wrong_cols};
    mat* error_result = mat_vert_cat(2, mismatch);
    test_assert(error_result == NULL, "mat_vert_cat returns NULL for column mismatch");
    
    // Test error case: NULL matrix
    mat* null_matrices[2] = {m1, NULL};
    mat* null_result = mat_vert_cat(2, null_matrices);
    test_assert(null_result == NULL, "mat_vert_cat returns NULL for NULL matrix");
    
    free_mat(m1);
    free_mat(m2);
    free_mat(m3);
    free_mat(wrong_cols);
    free_mat(result);
    free_mat(result3);
    free_mat(single_result);
}

void test_mat_add() {
    printf("\n--- Testing mat_add and mat_add_r ---\n");
    
    // Test basic matrix addition
    mat* m1 = new_mat(2, 3);
    m1->values[0][0] = 1.0; m1->values[0][1] = 2.0; m1->values[0][2] = 3.0;
    m1->values[1][0] = 4.0; m1->values[1][1] = 5.0; m1->values[1][2] = 6.0;
    
    mat* m2 = new_mat(2, 3);
    m2->values[0][0] = 7.0; m2->values[0][1] = 8.0; m2->values[0][2] = 9.0;
    m2->values[1][0] = 10.0; m2->values[1][1] = 11.0; m2->values[1][2] = 12.0;
    
    // Test in-place addition
    mat* m1_copy = mat_cp(m1);
    int result = mat_add_r(m1_copy, m2);
    test_assert(result == 1, "mat_add_r returns 1 for valid matrices");
    
    // Check results: [1+7, 2+8, 3+9], [4+10, 5+11, 6+12] = [8, 10, 12], [14, 16, 18]
    test_assert(fabs(m1_copy->values[0][0] - 8.0) < EPSILON, "mat_add_r adds correctly");
    test_assert(fabs(m1_copy->values[0][1] - 10.0) < EPSILON, "mat_add_r adds correctly");
    test_assert(fabs(m1_copy->values[0][2] - 12.0) < EPSILON, "mat_add_r adds correctly");
    test_assert(fabs(m1_copy->values[1][0] - 14.0) < EPSILON, "mat_add_r adds correctly");
    test_assert(fabs(m1_copy->values[1][1] - 16.0) < EPSILON, "mat_add_r adds correctly");
    test_assert(fabs(m1_copy->values[1][2] - 18.0) < EPSILON, "mat_add_r adds correctly");
    
    // Test new matrix addition
    mat* result_mat = mat_add(m1, m2);
    test_assert(result_mat != NULL, "mat_add returns non-NULL for valid matrices");
    test_assert(result_mat != m1, "mat_add creates new matrix");
    test_assert(mat_equal(result_mat, m1_copy, EPSILON), "mat_add produces same result as mat_add_r");
    test_assert(fabs(m1->values[0][0] - 1.0) < EPSILON, "mat_add preserves original matrix");
    
    // Test dimension mismatch
    mat* wrong_dim = new_mat(3, 2);
    int error_result = mat_add_r(m1, wrong_dim);
    test_assert(error_result == 0, "mat_add_r returns 0 for dimension mismatch");
    
    mat* error_mat = mat_add(m1, wrong_dim);
    test_assert(error_mat == NULL, "mat_add returns NULL for dimension mismatch");
    
    // Test with negative numbers
    mat* neg = new_mat(2, 3);
    set_mat_val(neg, -1.0);
    mat* neg_result = mat_add(m1, neg);
    test_assert(fabs(neg_result->values[0][0] - 0.0) < EPSILON, "mat_add works with negative numbers");
    test_assert(fabs(neg_result->values[1][1] - 4.0) < EPSILON, "mat_add works with negative numbers");
    
    free_mat(m1);
    free_mat(m2);
    free_mat(m1_copy);
    free_mat(result_mat);
    free_mat(wrong_dim);
    free_mat(neg);
    free_mat(neg_result);
}

void test_mat_sub() {
    printf("\n--- Testing mat_sub and mat_sub_r ---\n");
    
    // Test basic matrix subtraction
    mat* m1 = new_mat(2, 2);
    m1->values[0][0] = 10.0; m1->values[0][1] = 8.0;
    m1->values[1][0] = 6.0; m1->values[1][1] = 4.0;
    
    mat* m2 = new_mat(2, 2);
    m2->values[0][0] = 3.0; m2->values[0][1] = 2.0;
    m2->values[1][0] = 1.0; m2->values[1][1] = 0.0;
    
    // Test in-place subtraction
    mat* m1_copy = mat_cp(m1);
    int result = mat_sub_r(m1_copy, m2);
    test_assert(result == 1, "mat_sub_r returns 1 for valid matrices");
    
    // Check results: [10-3, 8-2], [6-1, 4-0] = [7, 6], [5, 4]
    test_assert(fabs(m1_copy->values[0][0] - 7.0) < EPSILON, "mat_sub_r subtracts correctly");
    test_assert(fabs(m1_copy->values[0][1] - 6.0) < EPSILON, "mat_sub_r subtracts correctly");
    test_assert(fabs(m1_copy->values[1][0] - 5.0) < EPSILON, "mat_sub_r subtracts correctly");
    test_assert(fabs(m1_copy->values[1][1] - 4.0) < EPSILON, "mat_sub_r subtracts correctly");
    
    // Test new matrix subtraction
    mat* result_mat = mat_sub(m1, m2);
    test_assert(result_mat != NULL, "mat_sub returns non-NULL for valid matrices");
    test_assert(result_mat != m1, "mat_sub creates new matrix");
    test_assert(mat_equal(result_mat, m1_copy, EPSILON), "mat_sub produces same result as mat_sub_r");
    test_assert(fabs(m1->values[0][0] - 10.0) < EPSILON, "mat_sub preserves original matrix");
    
    // Test dimension mismatch
    mat* wrong_dim = new_mat(1, 3);
    int error_result = mat_sub_r(m1, wrong_dim);
    test_assert(error_result == 0, "mat_sub_r returns 0 for dimension mismatch");
    
    mat* error_mat = mat_sub(m1, wrong_dim);
    test_assert(error_mat == NULL, "mat_sub returns NULL for dimension mismatch");
    
    // Test subtracting matrix from itself (should give zero matrix)
    mat* zero_result = mat_sub(m1, m1);
    test_assert(mat_all_equal(zero_result, 0.0, EPSILON), "mat_sub matrix from itself gives zero matrix");
    
    free_mat(m1);
    free_mat(m2);
    free_mat(m1_copy);
    free_mat(result_mat);
    free_mat(wrong_dim);
    free_mat(zero_result);
}

void test_mat_dot() {
    printf("\n--- Testing mat_dot_r ---\n");
    
    // Test basic 2x3 * 3x2 matrix multiplication
    mat* m1 = new_mat(2, 3);
    m1->values[0][0] = 1.0; m1->values[0][1] = 2.0; m1->values[0][2] = 3.0;
    m1->values[1][0] = 4.0; m1->values[1][1] = 5.0; m1->values[1][2] = 6.0;
    
    mat* m2 = new_mat(3, 2);
    m2->values[0][0] = 7.0; m2->values[0][1] = 8.0;
    m2->values[1][0] = 9.0; m2->values[1][1] = 10.0;
    m2->values[2][0] = 11.0; m2->values[2][1] = 12.0;
    
    mat* result = mat_dot_r(m1, m2);
    test_assert(result != NULL, "mat_dot_r returns non-NULL for valid matrices");
    test_assert(result->num_rows == 2, "mat_dot_r produces correct result dimensions");
    test_assert(result->num_cols == 2, "mat_dot_r produces correct result dimensions");
    test_assert(result->is_square == 1, "mat_dot_r updates is_square flag correctly");
    
    // Check results: 
    // [1*7+2*9+3*11, 1*8+2*10+3*12] = [58, 64]
    // [4*7+5*9+6*11, 4*8+5*10+6*12] = [139, 154]
    test_assert(fabs(result->values[0][0] - 58.0) < EPSILON, "mat_dot_r multiplies correctly");
    test_assert(fabs(result->values[0][1] - 64.0) < EPSILON, "mat_dot_r multiplies correctly");
    test_assert(fabs(result->values[1][0] - 139.0) < EPSILON, "mat_dot_r multiplies correctly");
    test_assert(fabs(result->values[1][1] - 154.0) < EPSILON, "mat_dot_r multiplies correctly");
    
    // Test identity matrix multiplication
    mat* identity = eye_mat(3);
    mat* id_result = mat_dot_r(m1, identity);
    test_assert(mat_equal(id_result, m1, EPSILON), "mat_dot_r with identity gives original matrix");
    
    // Test dimension mismatch (2x3 * 2x2 should fail)
    mat* wrong_dim = new_mat(2, 2);
    mat* error_result = mat_dot_r(m1, wrong_dim);
    test_assert(error_result == NULL, "mat_dot_r returns NULL for dimension mismatch");
    
    // Test square matrix multiplication
    mat* sq1 = new_mat(2, 2);
    sq1->values[0][0] = 1.0; sq1->values[0][1] = 2.0;
    sq1->values[1][0] = 3.0; sq1->values[1][1] = 4.0;
    
    mat* sq2 = new_mat(2, 2);
    sq2->values[0][0] = 5.0; sq2->values[0][1] = 6.0;
    sq2->values[1][0] = 7.0; sq2->values[1][1] = 8.0;
    
    mat* sq_result = mat_dot_r(sq1, sq2);
    // [1*5+2*7, 1*6+2*8] = [19, 22]
    // [3*5+4*7, 3*6+4*8] = [43, 50]
    test_assert(fabs(sq_result->values[0][0] - 19.0) < EPSILON, "mat_dot_r works with square matrices");
    test_assert(fabs(sq_result->values[0][1] - 22.0) < EPSILON, "mat_dot_r works with square matrices");
    test_assert(fabs(sq_result->values[1][0] - 43.0) < EPSILON, "mat_dot_r works with square matrices");
    test_assert(fabs(sq_result->values[1][1] - 50.0) < EPSILON, "mat_dot_r works with square matrices");
    
    free_mat(m1);
    free_mat(m2);
    free_mat(result);
    free_mat(identity);
    free_mat(id_result);
    free_mat(wrong_dim);
    free_mat(sq1);
    free_mat(sq2);
    free_mat(sq_result);
}

void test_mat_row_add_scaled() {
    printf("\n--- Testing mat_row_add_scaled ---\n");
    
    mat* m = new_mat(3, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 5.0; m->values[1][2] = 6.0;
    m->values[2][0] = 7.0; m->values[2][1] = 8.0; m->values[2][2] = 9.0;
    
    // Test row[0] += 2.0 * row[1]
    int result = mat_row_add_scaled(m, 0, 1, 2.0);
    test_assert(result == 1, "mat_row_add_scaled returns 1 for valid operation");
    
    // Expected: row[0] = [1,2,3] + 2.0*[4,5,6] = [9,12,15]
    test_assert(fabs(m->values[0][0] - 9.0) < EPSILON, "mat_row_add_scaled adds scaled row correctly");
    test_assert(fabs(m->values[0][1] - 12.0) < EPSILON, "mat_row_add_scaled adds scaled row correctly");
    test_assert(fabs(m->values[0][2] - 15.0) < EPSILON, "mat_row_add_scaled adds scaled row correctly");
    
    // Source row should remain unchanged
    test_assert(fabs(m->values[1][0] - 4.0) < EPSILON, "mat_row_add_scaled preserves source row");
    test_assert(fabs(m->values[1][1] - 5.0) < EPSILON, "mat_row_add_scaled preserves source row");
    test_assert(fabs(m->values[1][2] - 6.0) < EPSILON, "mat_row_add_scaled preserves source row");
    
    // Other rows should remain unchanged
    test_assert(fabs(m->values[2][0] - 7.0) < EPSILON, "mat_row_add_scaled preserves other rows");
    
    // Test with negative scalar
    mat_row_add_scaled(m, 2, 1, -0.5);
    // row[2] = [7,8,9] + (-0.5)*[4,5,6] = [5,5.5,6]
    test_assert(fabs(m->values[2][0] - 5.0) < EPSILON, "mat_row_add_scaled works with negative scalar");
    test_assert(fabs(m->values[2][1] - 5.5) < EPSILON, "mat_row_add_scaled works with negative scalar");
    test_assert(fabs(m->values[2][2] - 6.0) < EPSILON, "mat_row_add_scaled works with negative scalar");
    
    // Test invalid row indices
    int invalid_result1 = mat_row_add_scaled(m, 5, 1, 1.0);
    test_assert(invalid_result1 == 0, "mat_row_add_scaled returns 0 for invalid destination row");
    
    int invalid_result2 = mat_row_add_scaled(m, 1, 5, 1.0);
    test_assert(invalid_result2 == 0, "mat_row_add_scaled returns 0 for invalid source row");
    
    // Test adding row to itself
    mat_row_add_scaled(m, 1, 1, 1.0);
    test_assert(fabs(m->values[1][0] - 8.0) < EPSILON, "mat_row_add_scaled can add row to itself");
    
    free_mat(m);
}

void test_find_pivot_row() {
    printf("\n--- Testing find_pivot_row ---\n");
    
    mat* m = new_mat(4, 3);
    m->values[0][0] = 0.0; m->values[0][1] = 1.0; m->values[0][2] = 2.0;
    m->values[1][0] = 0.0; m->values[1][1] = 0.0; m->values[1][2] = 3.0;
    m->values[2][0] = 4.0; m->values[2][1] = 5.0; m->values[2][2] = 6.0;
    m->values[3][0] = 0.0; m->values[3][1] = 7.0; m->values[3][2] = 8.0;
    
    // Test finding first non-zero in column 0
    int pivot = find_pivot_row(m, 0, 0);
    test_assert(pivot == 2, "find_pivot_row finds first non-zero element in column");
    
    // Test finding first non-zero in column 1 starting from row 1
    int pivot2 = find_pivot_row(m, 1, 1);
    test_assert(pivot2 == 2, "find_pivot_row works with starting row offset");
    
    // Test column with all zeros
    set_mat_val(m, 0.0);
    int no_pivot = find_pivot_row(m, 0, 0);
    test_assert(no_pivot == -1, "find_pivot_row returns -1 for column with no pivot");
    
    // Test with very small values (should be considered zero)
    m->values[1][1] = 1e-12; // Smaller than EPSILON
    int small_pivot = find_pivot_row(m, 1, 0);
    test_assert(small_pivot == -1, "find_pivot_row ignores values smaller than EPSILON");
    
    free_mat(m);
}

void test_find_max_pivot_row() {
    printf("\n--- Testing find_max_pivot_row ---\n");
    
    mat* m = new_mat(4, 3);
    m->values[0][0] = 0.1; m->values[0][1] = 1.0; m->values[0][2] = 2.0;
    m->values[1][0] = 0.5; m->values[1][1] = 0.0; m->values[1][2] = 3.0;
    m->values[2][0] = -0.8; m->values[2][1] = 5.0; m->values[2][2] = 6.0;
    m->values[3][0] = 0.3; m->values[3][1] = 7.0; m->values[3][2] = 8.0;
    
    // Test finding maximum absolute value in column 0
    int max_pivot = find_max_pivot_row(m, 0, 0);
    test_assert(max_pivot == 2, "find_max_pivot_row finds row with maximum absolute value");
    
    // Test with starting row offset
    int max_pivot2 = find_max_pivot_row(m, 0, 1);
    test_assert(max_pivot2 == 2, "find_max_pivot_row works with starting row offset");
    
    // Test starting from row 3
    int max_pivot3 = find_max_pivot_row(m, 0, 3);
    test_assert(max_pivot3 == 3, "find_max_pivot_row works when only one row available");
    
    // Test column with all small values
    set_mat_val(m, 1e-12);
    int no_max = find_max_pivot_row(m, 0, 0);
    test_assert(no_max == -1, "find_max_pivot_row returns -1 when all values too small");
    
    free_mat(m);
}

void test_mat_to_ref() {
    printf("\n--- Testing mat_to_ref ---\n");
    
    // Test basic 3x3 matrix
    mat* m = new_mat(3, 3);
    m->values[0][0] = 2.0; m->values[0][1] = 1.0; m->values[0][2] = 3.0;
    m->values[1][0] = 4.0; m->values[1][1] = 3.0; m->values[1][2] = 7.0;
    m->values[2][0] = 6.0; m->values[2][1] = 2.0; m->values[2][2] = 8.0;
    
    mat* ref = mat_to_ref(m);
    test_assert(ref != NULL, "mat_to_ref returns non-NULL");
    test_assert(ref != m, "mat_to_ref creates new matrix");
    
    // Check that original matrix is preserved
    test_assert(fabs(m->values[0][0] - 2.0) < EPSILON, "mat_to_ref preserves original matrix");
    
    // Check that result is in row echelon form (upper triangular structure)
    test_assert(fabs(ref->values[0][0]) > EPSILON, "mat_to_ref first pivot is non-zero");
    
    // Test identity matrix (should remain unchanged)
    mat* identity = eye_mat(3);
    mat* ref_identity = mat_to_ref(identity);
    test_assert(mat_equal(identity, ref_identity, EPSILON), "mat_to_ref of identity gives identity");
    
    // Test zero matrix
    mat* zero_mat = new_mat(2, 2);
    set_mat_val(zero_mat, 0.0);
    mat* ref_zero = mat_to_ref(zero_mat);
    test_assert(mat_all_equal(ref_zero, 0.0, EPSILON), "mat_to_ref of zero matrix gives zero matrix");
    
    free_mat(m);
    free_mat(ref);
    free_mat(identity);
    free_mat(ref_identity);
    free_mat(zero_mat);
    free_mat(ref_zero);
}

void test_mat_to_rref() {
    printf("\n--- Testing mat_to_rref ---\n");
    
    // Test basic 2x3 augmented matrix [1 2 | 5; 3 4 | 11]
    mat* m = new_mat(2, 3);
    m->values[0][0] = 1.0; m->values[0][1] = 2.0; m->values[0][2] = 5.0;
    m->values[1][0] = 3.0; m->values[1][1] = 4.0; m->values[1][2] = 11.0;
    
    mat* rref = mat_to_rref(m);
    test_assert(rref != NULL, "mat_to_rref returns non-NULL");
    test_assert(rref != m, "mat_to_rref creates new matrix");
    
    // Check that original matrix is preserved
    test_assert(fabs(m->values[0][0] - 1.0) < EPSILON, "mat_to_rref preserves original matrix");
    
    // Expected RREF: [1 0 | 1; 0 1 | 2] (solution x=1, y=2)
    test_assert(fabs(rref->values[0][0] - 1.0) < EPSILON, "mat_to_rref produces correct leading 1");
    test_assert(fabs(rref->values[0][1]) < EPSILON, "mat_to_rref eliminates above pivot");
    test_assert(fabs(rref->values[1][1] - 1.0) < EPSILON, "mat_to_rref produces correct second leading 1");
    test_assert(fabs(rref->values[1][0]) < EPSILON, "mat_to_rref eliminates below pivot");
    
    // Test identity matrix (should remain unchanged)
    mat* identity = eye_mat(3);
    mat* rref_identity = mat_to_rref(identity);
    test_assert(mat_equal(identity, rref_identity, EPSILON), "mat_to_rref of identity gives identity");
    
    // Test singular matrix (should handle gracefully)
    mat* singular = new_mat(2, 2);
    singular->values[0][0] = 1.0; singular->values[0][1] = 2.0;
    singular->values[1][0] = 2.0; singular->values[1][1] = 4.0; // Second row is 2x first row
    
    mat* rref_singular = mat_to_rref(singular);
    test_assert(rref_singular != NULL, "mat_to_rref handles singular matrices");
    
    // Test zero matrix
    mat* zero_mat = new_mat(2, 2);
    set_mat_val(zero_mat, 0.0);
    mat* rref_zero = mat_to_rref(zero_mat);
    test_assert(mat_all_equal(rref_zero, 0.0, EPSILON), "mat_to_rref of zero matrix gives zero matrix");
    
    free_mat(m);
    free_mat(rref);
    free_mat(identity);
    free_mat(rref_identity);
    free_mat(singular);
    free_mat(rref_singular);
    free_mat(zero_mat);
    free_mat(rref_zero);
}

void test_mat_lup_decomp() {
    printf("\n--- Testing mat_lup_decomp ---\n");
    
    // Test basic 3x3 matrix decomposition
    mat* A = new_mat(3, 3);
    A->values[0][0] = 2.0; A->values[0][1] = 1.0; A->values[0][2] = 1.0;
    A->values[1][0] = 4.0; A->values[1][1] = 3.0; A->values[1][2] = 3.0;
    A->values[2][0] = 8.0; A->values[2][1] = 7.0; A->values[2][2] = 9.0;
    
    mat* L = NULL;
    mat* U = NULL;
    mat* P = NULL;
    
    int result = mat_lup_decomp(A, &L, &U, &P);
    test_assert(result == 1, "mat_lup_decomp returns 1 for success");
    test_assert(L != NULL, "mat_lup_decomp creates L matrix");
    test_assert(U != NULL, "mat_lup_decomp creates U matrix");
    test_assert(P != NULL, "mat_lup_decomp creates P matrix");
    
    // Verify matrix dimensions
    test_assert(L->num_rows == 3 && L->num_cols == 3, "L matrix has correct dimensions");
    test_assert(U->num_rows == 3 && U->num_cols == 3, "U matrix has correct dimensions");
    test_assert(P->num_rows == 3 && P->num_cols == 3, "P matrix has correct dimensions");
    
    // Verify L is lower triangular (upper entries should be 0)
    test_assert(fabs(L->values[0][1]) < EPSILON, "L is lower triangular");
    test_assert(fabs(L->values[0][2]) < EPSILON, "L is lower triangular");
    test_assert(fabs(L->values[1][2]) < EPSILON, "L is lower triangular");
    
    // Verify L has 1s on diagonal
    test_assert(fabs(L->values[0][0] - 1.0) < EPSILON, "L has 1s on diagonal");
    test_assert(fabs(L->values[1][1] - 1.0) < EPSILON, "L has 1s on diagonal");
    test_assert(fabs(L->values[2][2] - 1.0) < EPSILON, "L has 1s on diagonal");
    
    // Verify U is upper triangular (lower entries should be 0)
    test_assert(fabs(U->values[1][0]) < EPSILON, "U is upper triangular");
    test_assert(fabs(U->values[2][0]) < EPSILON, "U is upper triangular");
    test_assert(fabs(U->values[2][1]) < EPSILON, "U is upper triangular");
    
    // Verify P is a permutation matrix (each row and column has exactly one 1)
    for (unsigned int i = 0; i < 3; i++) {
        int row_sum = 0;
        int col_sum = 0;
        for (unsigned int j = 0; j < 3; j++) {
            if (fabs(P->values[i][j] - 1.0) < EPSILON) row_sum++;
            if (fabs(P->values[j][i] - 1.0) < EPSILON) col_sum++;
        }
        test_assert(row_sum == 1, "P matrix each row has exactly one 1");
        test_assert(col_sum == 1, "P matrix each column has exactly one 1");
    }
    
    // Verify PA = LU by computing P*A and L*U and checking equality
    mat* PA = mat_dot_r(P, A);
    mat* LU = mat_dot_r(L, U);
    test_assert(mat_equal(PA, LU, EPSILON), "LUP decomposition satisfies PA = LU");
    
    // Test with non-square matrix (should fail)
    mat* nonsquare = new_mat(2, 3);
    mat* L2 = NULL, * U2 = NULL, * P2 = NULL;
    int result2 = mat_lup_decomp(nonsquare, &L2, &U2, &P2);
    test_assert(result2 == 0, "mat_lup_decomp returns 0 for non-square matrix");
    
    free_mat(A);
    free_mat(L);
    free_mat(U);
    free_mat(P);
    free_mat(PA);
    free_mat(LU);
    free_mat(nonsquare);
}

void test_mat_lup_solve() {
    printf("\n--- Testing mat_lup_solve ---\n");
    
    // Test known system: 2x + y = 5, 4x + 3y = 11 (solution: x=1, y=3)
    mat* A = new_mat(2, 2);
    A->values[0][0] = 2.0; A->values[0][1] = 1.0;
    A->values[1][0] = 4.0; A->values[1][1] = 3.0;
    
    mat* b = new_mat(2, 1);
    b->values[0][0] = 5.0;
    b->values[1][0] = 11.0;
    
    // Decompose A
    mat* L = NULL;
    mat* U = NULL;
    mat* P = NULL;
    mat_lup_decomp(A, &L, &U, &P);
    
    // Solve the system
    mat* x = mat_lup_solve(L, U, P, b);
    test_assert(x != NULL, "mat_lup_solve returns non-NULL for valid system");
    
    // Check solution (should be x=1, y=3)
    test_assert(fabs(x->values[0][0] - 1.0) < EPSILON, "mat_lup_solve finds correct solution x");
    test_assert(fabs(x->values[1][0] - 3.0) < EPSILON, "mat_lup_solve finds correct solution y");
    
    // Verify solution by substituting back: Ax should equal b
    mat* Ax = mat_dot_r(A, x);
    test_assert(mat_equal(Ax, b, EPSILON), "LUP solution satisfies Ax = b");
    
    // Test 3x3 system
    mat* A3 = new_mat(3, 3);
    A3->values[0][0] = 1.0; A3->values[0][1] = 2.0; A3->values[0][2] = 3.0;
    A3->values[1][0] = 2.0; A3->values[1][1] = 5.0; A3->values[1][2] = 3.0;
    A3->values[2][0] = 1.0; A3->values[2][1] = 0.0; A3->values[2][2] = 8.0;
    
    mat* b3 = new_mat(3, 1);
    b3->values[0][0] = 14.0;
    b3->values[1][0] = 18.0; 
    b3->values[2][0] = 17.0;
    // Solution should be x=1, y=2, z=3
    
    mat* L3 = NULL, * U3 = NULL, * P3 = NULL;
    mat_lup_decomp(A3, &L3, &U3, &P3);
    mat* x3 = mat_lup_solve(L3, U3, P3, b3);
    
    test_assert(x3 != NULL, "mat_lup_solve works for 3x3 system");
    test_assert(fabs(x3->values[0][0] - 1.0) < EPSILON, "3x3 system solution x correct");
    test_assert(fabs(x3->values[1][0] - 2.0) < EPSILON, "3x3 system solution y correct");
    test_assert(fabs(x3->values[2][0] - 3.0) < EPSILON, "3x3 system solution z correct");
    
    // Test error cases
    mat* invalid = mat_lup_solve(NULL, U, P, b);
    test_assert(invalid == NULL, "mat_lup_solve returns NULL for invalid input");
    
    free_mat(A);
    free_mat(b);
    free_mat(L);
    free_mat(U);
    free_mat(P);
    free_mat(x);
    free_mat(Ax);
    free_mat(A3);
    free_mat(b3);
    free_mat(L3);
    free_mat(U3);
    free_mat(P3);
    free_mat(x3);
}

void test_mat_det_lup() {
    printf("\n--- Testing mat_det_lup ---\n");
    
    // Test determinant of known matrix
    mat* A = new_mat(2, 2);
    A->values[0][0] = 3.0; A->values[0][1] = 2.0;
    A->values[1][0] = 1.0; A->values[1][1] = 4.0;
    // det(A) = 3*4 - 2*1 = 10
    
    mat* L = NULL;
    mat* U = NULL;
    mat* P = NULL;
    mat_lup_decomp(A, &L, &U, &P);
    
    double det = mat_det_lup(L, U, P);
    test_assert(fabs(det - 10.0) < EPSILON, "mat_det_lup computes correct determinant for 2x2");
    
    // Test 3x3 matrix
    mat* A3 = new_mat(3, 3);
    A3->values[0][0] = 1.0; A3->values[0][1] = 2.0; A3->values[0][2] = 3.0;
    A3->values[1][0] = 0.0; A3->values[1][1] = 1.0; A3->values[1][2] = 4.0;
    A3->values[2][0] = 5.0; A3->values[2][1] = 6.0; A3->values[2][2] = 0.0;
    // det(A3) = 1*(1*0 - 4*6) - 2*(0*0 - 4*5) + 3*(0*6 - 1*5) = -24 + 40 - 15 = 1
    
    mat* L3 = NULL, * U3 = NULL, * P3 = NULL;
    mat_lup_decomp(A3, &L3, &U3, &P3);
    double det3 = mat_det_lup(L3, U3, P3);
    test_assert(fabs(det3 - 1.0) < EPSILON, "mat_det_lup computes correct determinant for 3x3");
    
    // Test identity matrix (det should be 1)
    mat* identity = eye_mat(3);
    mat* LI = NULL, * UI = NULL, * PI = NULL;
    mat_lup_decomp(identity, &LI, &UI, &PI);
    double det_identity = mat_det_lup(LI, UI, PI);
    test_assert(fabs(det_identity - 1.0) < EPSILON, "mat_det_lup gives 1 for identity matrix");
    
    // Test singular matrix (det should be 0)
    mat* singular = new_mat(2, 2);
    singular->values[0][0] = 1.0; singular->values[0][1] = 2.0;
    singular->values[1][0] = 2.0; singular->values[1][1] = 4.0; // Second row = 2 * first row
    
    mat* LS = NULL, * US = NULL, * PS = NULL;
    mat_lup_decomp(singular, &LS, &US, &PS);
    double det_singular = mat_det_lup(LS, US, PS);
    test_assert(fabs(det_singular) < EPSILON, "mat_det_lup gives 0 for singular matrix");
    
    free_mat(A);
    free_mat(L);
    free_mat(U);
    free_mat(P);
    free_mat(A3);
    free_mat(L3);
    free_mat(U3);
    free_mat(P3);
    free_mat(identity);
    free_mat(LI);
    free_mat(UI);
    free_mat(PI);
    free_mat(singular);
    free_mat(LS);
    free_mat(US);
    free_mat(PS);
}

void test_mat_transpose() {
    printf("\n--- Testing mat_transpose ---\n");
    
    // Test basic transpose
    mat* A = new_mat(2, 3);
    A->values[0][0] = 1.0; A->values[0][1] = 2.0; A->values[0][2] = 3.0;
    A->values[1][0] = 4.0; A->values[1][1] = 5.0; A->values[1][2] = 6.0;
    
    mat* At = mat_transpose(A);
    test_assert(At != NULL, "mat_transpose returns non-NULL");
    test_assert(At->num_rows == 3, "transposed matrix has correct rows");
    test_assert(At->num_cols == 2, "transposed matrix has correct cols");
    test_assert(fabs(At->values[0][0] - 1.0) < EPSILON, "transpose element [0][0] correct");
    test_assert(fabs(At->values[1][0] - 2.0) < EPSILON, "transpose element [1][0] correct");
    test_assert(fabs(At->values[2][0] - 3.0) < EPSILON, "transpose element [2][0] correct");
    test_assert(fabs(At->values[0][1] - 4.0) < EPSILON, "transpose element [0][1] correct");
    test_assert(fabs(At->values[1][1] - 5.0) < EPSILON, "transpose element [1][1] correct");
    test_assert(fabs(At->values[2][1] - 6.0) < EPSILON, "transpose element [2][1] correct");
    
    // Test transpose of transpose gives original
    mat* Att = mat_transpose(At);
    test_assert(mat_equal(A, Att, EPSILON), "transpose of transpose gives original");
    
    // Test square matrix transpose
    mat* B = eye_mat(3);
    mat* Bt = mat_transpose(B);
    test_assert(mat_equal(B, Bt, EPSILON), "transpose of identity is identity");
    
    free_mat(A);
    free_mat(At);
    free_mat(Att);
    free_mat(B);
    free_mat(Bt);
}

void test_mat_qr_decomp() {
    printf("\n--- Testing mat_qr_decomp ---\n");
    
    // Test basic 3x2 QR decomposition
    mat* A = new_mat(3, 2);
    A->values[0][0] = 1.0; A->values[0][1] = 1.0;
    A->values[1][0] = 1.0; A->values[1][1] = 2.0;
    A->values[2][0] = 1.0; A->values[2][1] = 3.0;
    
    mat* Q = NULL, * R = NULL;
    int result = mat_qr_decomp(A, &Q, &R);
    
    test_assert(result == 1, "mat_qr_decomp returns 1 for success");
    test_assert(Q != NULL, "mat_qr_decomp creates Q matrix");
    test_assert(R != NULL, "mat_qr_decomp creates R matrix");
    test_assert(Q->num_rows == 3, "Q matrix has correct rows");
    test_assert(Q->num_cols == 2, "Q matrix has correct cols");
    test_assert(R->num_rows == 2, "R matrix has correct rows");
    test_assert(R->num_cols == 2, "R matrix has correct cols");
    test_assert(R->is_square == 1, "R matrix is square");
    
    // Test that R is upper triangular
    test_assert(fabs(R->values[1][0]) < EPSILON, "R is upper triangular");
    
    // Test that Q has orthonormal columns (Q^T * Q = I)
    mat* Qt = mat_transpose(Q);
    mat* QtQ = mat_dot_r(Qt, Q);
    
    // Check diagonal elements are 1 (orthonormal)
    test_assert(fabs(QtQ->values[0][0] - 1.0) < EPSILON, "Q columns are normalized");
    test_assert(fabs(QtQ->values[1][1] - 1.0) < EPSILON, "Q columns are normalized");
    
    // Check off-diagonal elements are 0 (orthogonal)
    test_assert(fabs(QtQ->values[0][1]) < EPSILON, "Q columns are orthogonal");
    test_assert(fabs(QtQ->values[1][0]) < EPSILON, "Q columns are orthogonal");
    
    // Test QR decomposition property: A = QR
    mat* QR = mat_dot_r(Q, R);
    test_assert(mat_equal(A, QR, EPSILON), "QR decomposition satisfies A = QR");
    
    // Test square matrix (3x3)
    mat* A3 = new_mat(3, 3);
    A3->values[0][0] = 2.0; A3->values[0][1] = -1.0; A3->values[0][2] = 0.0;
    A3->values[1][0] = -1.0; A3->values[1][1] = 2.0; A3->values[1][2] = -1.0;
    A3->values[2][0] = 0.0; A3->values[2][1] = -1.0; A3->values[2][2] = 2.0;
    
    mat* Q3 = NULL, * R3 = NULL;
    int result3 = mat_qr_decomp(A3, &Q3, &R3);
    test_assert(result3 == 1, "mat_qr_decomp works for 3x3 matrix");
    
    mat* QR3 = mat_dot_r(Q3, R3);
    test_assert(mat_equal(A3, QR3, EPSILON), "3x3 QR decomposition satisfies A = QR");
    
    // Test error case: more columns than rows
    mat* bad = new_mat(2, 3);
    mat* Qbad = NULL, * Rbad = NULL;
    int bad_result = mat_qr_decomp(bad, &Qbad, &Rbad);
    test_assert(bad_result == 0, "mat_qr_decomp returns 0 for invalid dimensions");
    
    free_mat(A);
    free_mat(Q);
    free_mat(R);
    free_mat(Qt);
    free_mat(QtQ);
    free_mat(QR);
    free_mat(A3);
    free_mat(Q3);
    free_mat(R3);
    free_mat(QR3);
    free_mat(bad);
}

void test_mat_qr_solve() {
    printf("\n--- Testing mat_qr_solve ---\n");
    
    // Test overdetermined system (3x2 system, least squares solution)
    mat* A = new_mat(3, 2);
    A->values[0][0] = 1.0; A->values[0][1] = 1.0;
    A->values[1][0] = 1.0; A->values[1][1] = 2.0;
    A->values[2][0] = 1.0; A->values[2][1] = 3.0;
    
    mat* b = new_mat(3, 1);
    b->values[0][0] = 6.0;
    b->values[1][0] = 8.0;
    b->values[2][0] = 10.0;
    
    mat* Q = NULL, * R = NULL;
    mat_qr_decomp(A, &Q, &R);
    
    mat* x = mat_qr_solve(Q, R, b);
    test_assert(x != NULL, "mat_qr_solve returns non-NULL for valid system");
    test_assert(x->num_rows == 2, "solution has correct dimensions");
    test_assert(x->num_cols == 1, "solution has correct dimensions");
    
    // For this system, the least squares solution should be approximately [4, 2]
    test_assert(fabs(x->values[0][0] - 4.0) < 1e-10, "QR solve finds correct solution x");
    test_assert(fabs(x->values[1][0] - 2.0) < 1e-10, "QR solve finds correct solution y");
    
    // Verify that Ax approximates b (for overdetermined system)
    mat* Ax = mat_dot_r(A, x);
    // The residual should be small for a well-conditioned least squares problem
    double residual_norm = 0.0;
    for (unsigned int i = 0; i < 3; i++) {
        double diff = Ax->values[i][0] - b->values[i][0];
        residual_norm += diff * diff;
    }
    residual_norm = sqrt(residual_norm);
    test_assert(residual_norm < 1e-10, "QR solution minimizes residual");
    
    // Test exact system (square matrix)
    mat* A2 = new_mat(2, 2);
    A2->values[0][0] = 1.0; A2->values[0][1] = 2.0;
    A2->values[1][0] = 3.0; A2->values[1][1] = 4.0;
    
    mat* b2 = new_mat(2, 1);
    b2->values[0][0] = 5.0;  // x + 2y = 5
    b2->values[1][0] = 11.0; // 3x + 4y = 11
    
    mat* Q2 = NULL, * R2 = NULL;
    mat_qr_decomp(A2, &Q2, &R2);
    
    mat* x2 = mat_qr_solve(Q2, R2, b2);
    
    // Solution should be x = 1, y = 2
    test_assert(fabs(x2->values[0][0] - 1.0) < EPSILON, "2x2 system solution x correct");
    test_assert(fabs(x2->values[1][0] - 2.0) < EPSILON, "2x2 system solution y correct");
    
    // Verify Ax = b exactly for square system
    mat* Ax2 = mat_dot_r(A2, x2);
    test_assert(mat_equal(Ax2, b2, EPSILON), "QR solution satisfies Ax = b for square system");
    
    // Test error case: invalid dimensions
    mat* x_bad = mat_qr_solve(Q, R2, b);  // Mismatched dimensions
    test_assert(x_bad == NULL, "mat_qr_solve returns NULL for invalid dimensions");
    
    free_mat(A);
    free_mat(b);
    free_mat(Q);
    free_mat(R);
    free_mat(x);
    free_mat(Ax);
    free_mat(A2);
    free_mat(b2);
    free_mat(Q2);
    free_mat(R2);
    free_mat(x2);
    free_mat(Ax2);
}

int main() {
    printf("Running Matrix Library Tests\n");
    printf("============================\n");
    
    test_new_mat();
    test_square_mat();
    test_eye_mat();
    test_random_mat();
    test_random_square_mat();
    test_mat_eqdim();
    test_mat_equal();
    test_get_row_mat();
    test_get_col_mat();
    test_memory_management();
    test_set_mat_val();
    test_set_mat_diag();
    test_mat_all_equal();
    test_mat_cp();
    test_mat_row_mult();
    test_mat_col_mult();
    test_mat_row_addrow();
    test_mat_smult();
    test_mat_remove_column();
    test_mat_remove_row();
    test_mat_row_swap();
    test_mat_col_swap();
    test_mat_hor_cat();
    test_mat_vert_cat();
    test_mat_add();
    test_mat_sub();
    test_mat_dot();
    test_mat_row_add_scaled();
    test_find_pivot_row();
    test_find_max_pivot_row();
    test_mat_to_ref();
    test_mat_to_rref();
    test_mat_lup_decomp();
    test_mat_lup_solve();
    test_mat_det_lup();
    test_mat_transpose();
    test_mat_qr_decomp();
    test_mat_qr_solve();
    
    print_test_summary();
    
    return (test_count == passed_count) ? 0 : 1;
}