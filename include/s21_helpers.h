#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include "../include/s21_matrix.h"

/*
 * @brief Creates a minor matrix by crossing out a specified row and column.
 * @param A Pointer to the original matrix.
 * @param result Pointer to the resulting matrix (minor).
 * @param skip_row Index of the row to exclude.
 * @param skip_col Index of the column to exclude.
 * @return None (void function).
 * @note Used for determinant and algebraic complements calculation.
 * @author s21: tyananai
 * @date September 21, 2025
 */
void _crossing_out_matrix_element(matrix_t *A, matrix_t *result, int skip_row,
                                  int skip_col);

/**
 * @brief Validates if the matrix is correctly initialized.
 * @param A Pointer to the matrix to validate.
 * @return `1` if the matrix is invalid, `0` if it is valid.
 * @note Checks dimensions and memory allocation correctness.
 * @author s21: tyananai
 * @date September 21, 2025
 */
int _validation_matrix(const matrix_t *A);

#endif
