#include "../include/s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (result == NULL || rows <= 0 || columns <= 0) {
    return S21_INCORRECT_MATRIX;
  }

  int error = S21_OK;

  result->rows = rows;
  result->columns = columns;
  result->matrix = (double **)calloc(rows, sizeof(double *));
  if (result->matrix != NULL) {
    for (int i = 0; i < rows && !error; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        error = S21_INCORRECT_MATRIX;
        s21_remove_matrix(result);
      }
    }
  } else {
    error = S21_INCORRECT_MATRIX;
  }

  return error;
}