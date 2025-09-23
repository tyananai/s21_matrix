#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

static void assert_matrix_eq_expected(matrix_t *M, double expected[][10],
                                      int rows, int cols) {
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      ck_assert_ldouble_eq_tol(M->matrix[i][j], expected[i][j], S21_EPS);
}

static void copy_values(matrix_t *dst, matrix_t *src) {
  _alloc_matrix(dst, src->rows, src->columns);
  for (int i = 0; i < src->rows; ++i)
    for (int j = 0; j < src->columns; ++j)
      dst->matrix[i][j] = src->matrix[i][j];
}

START_TEST(test_inverse_null_A) {
  matrix_t result = {NULL, 0, 0};
  int rc = s21_inverse_matrix(NULL, &result);
  ck_assert_int_eq(rc, S21_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_inverse_null_result) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int rc = s21_inverse_matrix(&A, NULL);
  ck_assert_int_eq(rc, S21_INCORRECT_MATRIX);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_inverse_A_field_null) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;
  matrix_t result = {NULL, 0, 0};
  int rc = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(rc, S21_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_inverse_non_square) {
  matrix_t A;
  _alloc_matrix(&A, 2, 3);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 0.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 1.0;

  matrix_t result = {NULL, 0, 0};
  int rc = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(rc, S21_CALC_ERROR);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_inverse_singular_2x2) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 4.0;

  matrix_t result = {NULL, 0, 0};
  int rc = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(rc, S21_CALC_ERROR);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_inverse_1x1) {
  matrix_t A, inv;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 5.0;

  int rc = s21_inverse_matrix(&A, &inv);
  ck_assert_int_eq(rc, S21_OK);
  ck_assert_int_eq(inv.rows, 1);
  ck_assert_int_eq(inv.columns, 1);

  double expected1x1[1][10] = {{1.0 / 5.0}};
  assert_matrix_eq_expected(&inv, expected1x1, 1, 1);

  _free_matrix(&inv);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_inverse_3x3_given_example) {
  matrix_t A, inv;
  _alloc_matrix(&A, 3, 3);

  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 5.0;
  A.matrix[0][2] = 7.0;
  A.matrix[1][0] = 6.0;
  A.matrix[1][1] = 3.0;
  A.matrix[1][2] = 4.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = -2.0;
  A.matrix[2][2] = -3.0;

  int rc = s21_inverse_matrix(&A, &inv);
  ck_assert_int_eq(rc, S21_OK);
  ck_assert_int_eq(inv.rows, 3);
  ck_assert_int_eq(inv.columns, 3);

  double expected[3][10] = {
      {1.0, -1.0, 1.0}, {-38.0, 41.0, -34.0}, {27.0, -29.0, 24.0}};
  assert_matrix_eq_expected(&inv, expected, 3, 3);

  _free_matrix(&inv);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_inverse_preserve_original) {
  matrix_t A, inv, copy;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 3.0;
  A.matrix[0][1] = 8.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 6.0;

  copy_values(&copy, &A);

  int rc = s21_inverse_matrix(&A, &inv);
  ck_assert_int_eq(rc, S21_OK);

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      ck_assert_ldouble_eq_tol(A.matrix[i][j], copy.matrix[i][j], S21_EPS);

  _free_matrix(&inv);
  _free_matrix(&copy);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_inverse_with_nan_inf) {
  matrix_t A, inv;
  _alloc_matrix(&A, 2, 2);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = 1.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = INFINITY;

  int rc = s21_inverse_matrix(&A, &inv);

  ck_assert((rc == S21_CALC_ERROR) || (rc == S21_OK));

  if (rc == S21_OK) {
    for (int i = 0; i < inv.rows; ++i)
      for (int j = 0; j < inv.columns; ++j)
        ck_assert(!isfinite(inv.matrix[i][j]));
    _free_matrix(&inv);
  }

  _free_matrix(&A);
}
END_TEST

Suite *s21_inverse_matrix_suite(void) {
  Suite *s = suite_create("inverse_matrix_manual");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_inverse_null_A);
  tcase_add_test(tc, test_inverse_null_result);
  tcase_add_test(tc, test_inverse_A_field_null);
  tcase_add_test(tc, test_inverse_non_square);
  tcase_add_test(tc, test_inverse_singular_2x2);

  tcase_add_test(tc, test_inverse_1x1);
  tcase_add_test(tc, test_inverse_3x3_given_example);
  tcase_add_test(tc, test_inverse_preserve_original);
  tcase_add_test(tc, test_inverse_with_nan_inf);

  suite_add_tcase(s, tc);
  return s;
}
