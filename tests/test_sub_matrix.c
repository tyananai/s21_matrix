#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

START_TEST(test_sub_null_A_pointer) {
  matrix_t B;
  _alloc_matrix(&B, 1, 1);
  B.matrix[0][0] = 1.0;
  int rc = s21_sub_matrix(NULL, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_sub_null_B_pointer) {
  matrix_t A;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 1.0;
  int rc = s21_sub_matrix(&A, NULL, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_sub_null_result_pointer) {
  matrix_t A, B;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);
  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 2.0;
  int rc = s21_sub_matrix(&A, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_sub_A_matrix_field_null) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;
  matrix_t B;
  _alloc_matrix(&B, 2, 2);
  int rc = s21_sub_matrix(&A, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_sub_B_matrix_field_null) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  matrix_t B;
  B.matrix = NULL;
  B.rows = 2;
  B.columns = 2;
  int rc = s21_sub_matrix(&A, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_sub_different_rows) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 2, 2);
  _alloc_matrix(&B, 3, 2);
  result.matrix = NULL;
  int rc = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 2);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_sub_different_columns) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 2, 2);
  _alloc_matrix(&B, 2, 3);
  result.matrix = NULL;
  int rc = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 2);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_sub_simple_2x3) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 2, 3);
  _alloc_matrix(&B, 2, 3);
  double a_vals[2][3] = {{1.0, 2.0, 3.0}, {0.0, 4.0, 5.0}};
  double b_vals[2][3] = {{1.0, 0.0, 0.0}, {2.0, 0.0, 0.0}};
  for (int i = 0; i < A.rows; ++i)
    for (int j = 0; j < A.columns; ++j) {
      A.matrix[i][j] = a_vals[i][j];
      B.matrix[i][j] = b_vals[i][j];
    }

  int rc = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 3);
  for (int i = 0; i < result.rows; ++i)
    for (int j = 0; j < result.columns; ++j) {
      double expected = a_vals[i][j] - b_vals[i][j];
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected, S21_EPS);
    }
  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_sub_with_negative_and_fraction) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 3, 1);
  _alloc_matrix(&B, 3, 1);

  A.matrix[0][0] = -1.25;
  A.matrix[1][0] = 0.333333;
  A.matrix[2][0] = 100.0;

  B.matrix[0][0] = 2.75;
  B.matrix[1][0] = -0.333333;
  B.matrix[2][0] = -50.0;

  int rc = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 1);

  ck_assert_ldouble_eq_tol(result.matrix[0][0], -4.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], 0.666666, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[2][0], 150.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_sub_with_nan_and_inf) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 2, 2);
  _alloc_matrix(&B, 2, 2);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = INFINITY;
  A.matrix[1][0] = -INFINITY;
  A.matrix[1][1] = 1.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = NAN;

  int rc = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 0);

  ck_assert(isnan(result.matrix[0][0]));
  ck_assert(isinf(result.matrix[0][1]) && result.matrix[0][1] > 0);
  ck_assert(isinf(result.matrix[1][0]) && result.matrix[1][0] < 0);
  ck_assert(isnan(result.matrix[1][1]));

  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

Suite *s21_sub_matrix_suite(void) {
  Suite *s = suite_create("sub_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_sub_null_A_pointer);
  tcase_add_test(tc, test_sub_null_B_pointer);
  tcase_add_test(tc, test_sub_null_result_pointer);
  tcase_add_test(tc, test_sub_A_matrix_field_null);
  tcase_add_test(tc, test_sub_B_matrix_field_null);
  tcase_add_test(tc, test_sub_different_rows);
  tcase_add_test(tc, test_sub_different_columns);

  tcase_add_test(tc, test_sub_simple_2x3);
  tcase_add_test(tc, test_sub_with_negative_and_fraction);
  tcase_add_test(tc, test_sub_with_nan_and_inf);

  suite_add_tcase(s, tc);
  return s;
}
