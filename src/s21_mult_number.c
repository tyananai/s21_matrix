#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (_validation_matrix(A) || result == NULL) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  error = s21_create_matrix(A->rows, A->columns, result);

  for (int i = 0; i < A->rows && !error; i++) {
    for (int j = 0; j < A->columns && !error; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }

  return error;
}