#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

static double _determinant(matrix_t *A) {
  if (A->rows == 1 && A->columns == 1) {
    return A->matrix[0][0];
  }
  if (A->rows == 2 && A->columns == 2) {
    return A->matrix[0][0] * A->matrix[1][1] -
           A->matrix[0][1] * A->matrix[1][0];
  }
  double detA = 0.0;
  for (int i = 0; i < A->rows; i++) {
    matrix_t tmp = {NULL, 0, 0};
    _crossing_out_matrix_element(A, &tmp, 0, i);
    double sign = (i % 2 == 0 ? 1.0 : -1.0);
    detA += sign * A->matrix[0][i] * _determinant(&tmp);
    s21_remove_matrix(&tmp);
  }
  return detA;
}

int s21_determinant(matrix_t *A, double *result) {
  if (_validation_matrix(A) || result == NULL) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  if (A->rows != A->columns) {
    error = S21_CALC_ERROR;
  }

  if (!error) {
    *result = _determinant(A);
  }

  return error;
}