#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (_validation_matrix(A) || result == NULL) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  if (A->rows != A->columns) {
    error = S21_CALC_ERROR;
  }

  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
  }

  if (A->rows == 1) {
    if (!error) {
      result->matrix[0][0] = 1.0;
    }
  } else {
    for (int i = 0; i < A->rows && !error; i++) {
      for (int j = 0; j < A->columns && !error; j++) {
        double detA = 0.0;
        matrix_t tmp = {NULL, 0, 0};
        _crossing_out_matrix_element(A, &tmp, i, j);
        error = s21_determinant(&tmp, &detA);
        s21_remove_matrix(&tmp);
        double sign = ((i + j) % 2 == 0 ? 1.0 : -1.0);
        result->matrix[i][j] = sign * detA;
      }
    }
  }

  return error;
}
