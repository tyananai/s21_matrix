#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

START_TEST(test_transpose_null_A_pointer) {
  matrix_t result = {NULL, 0, 0};
  int rc = s21_transpose(NULL, &result);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_transpose_null_result_pointer) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int rc = s21_transpose(&A, NULL);
  ck_assert_int_eq(rc, 1);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_transpose_A_field_null) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;
  matrix_t result = {NULL, 0, 0};

  int rc = s21_transpose(&A, &result);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_transpose_1x1) {
  matrix_t A, result;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 5.5;

  int rc = s21_transpose(&A, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 1);
  ck_assert_int_eq(result.columns, 1);
  ck_assert_ldouble_eq_tol(result.matrix[0][0], 5.5, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_transpose_square_3x3) {
  matrix_t A, result;
  _alloc_matrix(&A, 3, 3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) A.matrix[i][j] = (double)(i * 3 + j + 1);

  int rc = s21_transpose(&A, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      ck_assert_ldouble_eq_tol(result.matrix[i][j], A.matrix[j][i], S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_transpose_rectangular_2x3_to_3x2) {
  matrix_t A, result;
  _alloc_matrix(&A, 2, 3);

  double vals[2][3] = {{1.0, 2.0, 3.0}, {4.5, -6.0, 7.25}};
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 3; ++j) A.matrix[i][j] = vals[i][j];

  int rc = s21_transpose(&A, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);

  for (int i = 0; i < result.rows; ++i)
    for (int j = 0; j < result.columns; ++j)
      ck_assert_ldouble_eq_tol(result.matrix[i][j], A.matrix[j][i], S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_transpose_example_3x2_to_2x3) {
  matrix_t A, result;
  _alloc_matrix(&A, 3, 2);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 4.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 5.0;
  A.matrix[2][0] = 3.0;
  A.matrix[2][1] = 6.0;

  int rc = s21_transpose(&A, &result);
  ck_assert_int_eq(rc, 0);

  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 3);

  double expected[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

  for (int i = 0; i < result.rows; ++i)
    for (int j = 0; j < result.columns; ++j)
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected[i][j], S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_transpose_preserves_original) {
  matrix_t A, result;
  _alloc_matrix(&A, 2, 2);

  A.matrix[0][0] = 9.0;
  A.matrix[0][1] = -1.5;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.2;

  double copy[2][2];
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) copy[i][j] = A.matrix[i][j];

  int rc = s21_transpose(&A, &result);
  ck_assert_int_eq(rc, 0);

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      ck_assert_ldouble_eq_tol(copy[i][j], A.matrix[i][j], S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_transpose_with_nan_and_inf) {
  matrix_t A, result;
  _alloc_matrix(&A, 2, 2);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = INFINITY;
  A.matrix[1][0] = -INFINITY;
  A.matrix[1][1] = 7.0;

  int rc = s21_transpose(&A, &result);
  ck_assert_int_eq(rc, 0);

  ck_assert(isnan(result.matrix[0][0]));
  ck_assert(isinf(result.matrix[0][1]) && result.matrix[0][1] < 0);
  ck_assert(isinf(result.matrix[1][0]) && result.matrix[1][0] > 0);
  ck_assert_ldouble_eq_tol(result.matrix[1][1], 7.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

Suite *s21_transpose_suite(void) {
  Suite *s = suite_create("transpose");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_transpose_null_A_pointer);
  tcase_add_test(tc, test_transpose_null_result_pointer);
  tcase_add_test(tc, test_transpose_A_field_null);

  tcase_add_test(tc, test_transpose_1x1);
  tcase_add_test(tc, test_transpose_square_3x3);
  tcase_add_test(tc, test_transpose_rectangular_2x3_to_3x2);
  tcase_add_test(tc, test_transpose_example_3x2_to_2x3);
  tcase_add_test(tc, test_transpose_preserves_original);
  tcase_add_test(tc, test_transpose_with_nan_and_inf);

  suite_add_tcase(s, tc);
  return s;
}
