#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (_validation_matrix(A) || _validation_matrix(B) || result == NULL) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  if (A->columns != B->rows) {
    error = S21_CALC_ERROR;
  }

  if (!error) {
    error = s21_create_matrix(A->rows, B->columns, result);
  }

  for (int i = 0; i < A->rows && !error; i++) {
    for (int j = 0; j < B->columns && !error; j++) {
      for (int k = 0; k < A->columns && !error; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }

  return error;
}