#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (_validation_matrix(A) || _validation_matrix(B) || result == NULL) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  if (A->rows != B->rows || A->columns != B->columns) {
    error = S21_CALC_ERROR;
  }

  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
  }

  for (int i = 0; i < A->rows && !error; i++) {
    for (int j = 0; j < A->columns && !error; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }

  return error;
}
