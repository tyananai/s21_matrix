#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (_validation_matrix(A) || result == NULL) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  if (A->rows != A->columns) {
    error = S21_CALC_ERROR;
  }

  double detA = 0.0;

  if (!error) {
    error = s21_determinant(A, &detA);
  }

  if (!error && fabsl(detA) < S21_EPS) {
    error = S21_CALC_ERROR;
  }

  matrix_t cof = {NULL, 0, 0};
  if (!error) {
    error = s21_calc_complements(A, &cof);
  }

  matrix_t adj = {NULL, 0, 0};
  if (!error) {
    error = s21_transpose(&cof, &adj);
  }
  s21_remove_matrix(&cof);

  if (!error) {
    double scalar = 1.0 / detA;
    error = s21_mult_number(&adj, scalar, result);
  }
  s21_remove_matrix(&adj);

  return error;
}
