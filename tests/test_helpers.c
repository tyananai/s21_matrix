#include "../include/test_helpers.h"

void _alloc_matrix(matrix_t *m, int rows, int cols) {
  if (!m) return;
  m->rows = rows;
  m->columns = cols;
  m->matrix = calloc(rows, sizeof(double *));
  for (int i = 0; i < rows; ++i) {
    m->matrix[i] = calloc(cols, sizeof(double));
  }
}

void _free_matrix(matrix_t *m) {
  if (!m) return;
  if (m->matrix) {
    for (int i = 0; i < m->rows; ++i) {
      free(m->matrix[i]);
      m->matrix[i] = NULL;
    }
    free(m->matrix);
    m->matrix = NULL;
  }
  m->rows = 0;
  m->columns = 0;
}
