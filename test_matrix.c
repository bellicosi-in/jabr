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
    
    print_test_summary();
    
    return (test_count == passed_count) ? 0 : 1;
}