#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (_validation_matrix(A) || result == NULL) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  error = s21_create_matrix(A->columns, A->rows, result);

  for (int i = 0; i < result->rows && !error; i++) {
    for (int j = 0; j < result->columns && !error; j++) {
      result->matrix[i][j] += A->matrix[j][i];
    }
  }

  return error;
}