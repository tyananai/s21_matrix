#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "../include/s21_matrix.h"

/**
 * @brief Allocates memory for a matrix with the given dimensions (test helper).
 * @param m Pointer to the matrix structure to allocate.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return None (void function).
 * @note Used only in tests, not part of the public API.
 *       All elements are initialized to zero by default.
 * @author s21: tyananai
 * @date September 21, 2025
 */
void _alloc_matrix(matrix_t *m, int rows, int cols);

/**
 * @brief Frees memory allocated for the matrix (test helper).
 * @param m Pointer to the matrix structure to free.
 * @return None (void function).
 * @note Used only in tests to release allocated memory.
 *       After the call, rows, cols, and pointer are reset to zero/NULL.
 * @author s21: tyananai
 * @date September 21, 2025
 */
void _free_matrix(matrix_t *m);

#endif
