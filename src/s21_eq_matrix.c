#include "../include/s21_helpers.h"
#include "../include/s21_matrix.h"

static int _eq_double(const double a, const double b) {
  return fabsl(a - b) < S21_EPS;
}

// cppcheck-suppress constParameterPointer
int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (_validation_matrix(A) || _validation_matrix(B) || A->rows != B->rows ||
      A->columns != B->columns) {
    return FAILURE;
  }

  int result = SUCCESS;

  for (int i = 0; result && i < A->rows; i++) {
    for (int j = 0; result && j < A->columns; j++) {
      if (!_eq_double(A->matrix[i][j], B->matrix[i][j])) {
        result = FAILURE;
      }
    }
  }

  return result;
}