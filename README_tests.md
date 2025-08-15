# Matrix Library Test Suite

This directory contains comprehensive tests for the matrix library functions.

## Files

- `test_matrix.c` - Complete test suite (requires bug fixes to compile)
- `test_matrix_fixed.c` - Modified test suite that works with current bugs
- `Makefile` - Build system for compiling and running tests

## Running Tests

```bash
# Build and run tests (with current bugs)
make clean
gcc -Wall -Wextra -std=c99 -g -O2 test_matrix_fixed.c matrix.c -o test_matrix_fixed -lm
./test_matrix_fixed

# Or using make (after fixing bugs):
make test
```

## Bugs Found in matrix.c

### Critical Bug 1: Memory allocation in new_mat()
**Location**: Line 24
**Issue**: Return statement is inside the for loop, so only the first row is allocated.
**Fix**: Move return statement outside the loop.

```c
// Current (buggy):
for(int i =0; i < m->num_rows; ++i){
    m->values[i] = calloc(m->num_cols, sizeof(**m->values));
    if(m->values[i] == NULL){
        fprintf(stderr, "null value");
    }
    return m;  // BUG: This should be outside the loop
}

// Fixed:
for(int i =0; i < m->num_rows; ++i){
    m->values[i] = calloc(m->num_cols, sizeof(**m->values));
    if(m->values[i] == NULL){
        fprintf(stderr, "null value");
        // Should also handle the error properly
    }
}
return m;
```

### Bug 2: Function signature mismatch in random_mat()
**Issue**: Header declares 2 parameters, implementation has 4.
**Fix**: Update header to match implementation or vice versa.

### Bug 3: Function signature mismatch in get_col_mat() and get_row_mat()
**Issue**: Header uses `unsigned int`, implementation uses `int`.
**Fix**: Make signatures consistent.

### Bug 4: Wrong loop condition in get_col_mat()
**Location**: Line 146
**Issue**: Loop iterates over `num_cols` instead of `num_rows`.
**Fix**: Change loop condition to `matrix->num_rows`.

```c
// Current (buggy):
for(int j = 0; j< matrix->num_cols; j++){
    new_col_matrix->values[j][0] = matrix->values[j][col];
}

// Fixed:
for(int j = 0; j< matrix->num_rows; j++){
    new_col_matrix->values[j][0] = matrix->values[j][col];
}
```

## Test Coverage

The test suite covers:

- ✅ Matrix creation (new_mat, square_mat, eye_mat)
- ✅ Random matrix generation (random_mat, random_square_mat) 
- ✅ Matrix comparison (mat_eqdim, mat_equal)
- ✅ Matrix access (get_row_mat, get_col_mat)
- ✅ Memory management (free_mat)
- ✅ Edge cases and error conditions
- ✅ Large matrix handling

## Test Framework Features

- Simple assertion-based testing
- Clear pass/fail indicators
- Test summary with success rate
- Memory leak detection support (via valgrind target)
- Comprehensive error reporting