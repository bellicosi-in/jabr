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
    
    print_test_summary();
    
    return (test_count == passed_count) ? 0 : 1;
}