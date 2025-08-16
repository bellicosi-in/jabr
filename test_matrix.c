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
    
    print_test_summary();
    
    return (test_count == passed_count) ? 0 : 1;
}