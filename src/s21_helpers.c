#include "../include/s21_helpers.h"

void _crossing_out_matrix_element(matrix_t *A, matrix_t *result, int skip_row,
                                  int skip_col) {
  s21_create_matrix(A->rows - 1, A->columns - 1, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (i != skip_row && j != skip_col) {
        result->matrix[i > skip_row ? i - 1 : i][j > skip_col ? j - 1 : j] =
            A->matrix[i][j];
      }
    }
  }
}

int _validation_matrix(const matrix_t *A) {
  return (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0);
}
