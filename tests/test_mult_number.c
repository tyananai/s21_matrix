#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

START_TEST(test_mult_number_null_A_pointer) {
  int rc = s21_mult_number(NULL, 2.0, NULL);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_mult_number_null_result_pointer) {
  matrix_t A;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 3.14;
  int rc = s21_mult_number(&A, 2.0, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_A_field_null) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;
  matrix_t result;
  result.matrix = NULL;
  int rc = s21_mult_number(&A, 5.0, &result);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_mult_number_simple_2x2) {
  matrix_t A, result;
  _alloc_matrix(&A, 2, 2);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = -3.0;
  A.matrix[1][1] = 0.5;

  int rc = s21_mult_number(&A, 2.0, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);

  ck_assert_ldouble_eq_tol(result.matrix[0][0], 2.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[0][1], 4.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], -6.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][1], 1.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_zero) {
  matrix_t A, result;
  _alloc_matrix(&A, 2, 1);
  A.matrix[0][0] = 123.456;
  A.matrix[1][0] = -7.89;

  int rc = s21_mult_number(&A, 0.0, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(result.matrix[0][0], 0.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], 0.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_negative_and_fraction) {
  matrix_t A, result;
  _alloc_matrix(&A, 3, 1);
  A.matrix[0][0] = -1.25;
  A.matrix[1][0] = 0.333333;
  A.matrix[2][0] = 4.0;

  int rc = s21_mult_number(&A, -2.5, &result);
  ck_assert_int_eq(rc, 0);

  ck_assert_ldouble_eq_tol(result.matrix[0][0], 3.125, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], -0.8333325, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[2][0], -10.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_mult_number_nan_and_inf) {
  matrix_t A, result;
  _alloc_matrix(&A, 2, 2);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = 1.0;
  A.matrix[1][0] = INFINITY;
  A.matrix[1][1] = -INFINITY;

  int rc = s21_mult_number(&A, 2.0, &result);
  ck_assert_int_eq(rc, 0);

  ck_assert(isnan(result.matrix[0][0]));
  ck_assert_ldouble_eq_tol(result.matrix[0][1], 2.0, S21_EPS);
  ck_assert(isinf(result.matrix[1][0]) && result.matrix[1][0] > 0);
  ck_assert(isinf(result.matrix[1][1]) && result.matrix[1][1] < 0);

  _free_matrix(&result);

  rc = s21_mult_number(&A, NAN, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert(isnan(result.matrix[0][1]));
  ck_assert(isnan(result.matrix[1][0]));
  ck_assert(isnan(result.matrix[1][1]));
  ck_assert(isnan(result.matrix[0][0]));

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

Suite *s21_mult_number_suite(void) {
  Suite *s = suite_create("mult_number");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_mult_number_null_A_pointer);
  tcase_add_test(tc, test_mult_number_null_result_pointer);
  tcase_add_test(tc, test_mult_number_A_field_null);

  tcase_add_test(tc, test_mult_number_simple_2x2);
  tcase_add_test(tc, test_mult_number_zero);
  tcase_add_test(tc, test_mult_number_negative_and_fraction);
  tcase_add_test(tc, test_mult_number_nan_and_inf);

  suite_add_tcase(s, tc);
  return s;
}