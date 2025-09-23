#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

START_TEST(test_det_null_A_pointer) {
  double det = 0.0;
  int rc = s21_determinant(NULL, &det);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_det_null_result_pointer) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int rc = s21_determinant(&A, NULL);
  ck_assert_int_eq(rc, 1);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_A_field_null) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;
  double det = 0.0;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_det_non_square) {
  matrix_t A;
  _alloc_matrix(&A, 2, 3);
  double det = 0.0;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 2);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_1x1) {
  matrix_t A;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 5.0;
  double det = NAN;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(det, 5.0, S21_EPS);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_2x2_basic) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;
  double det = 0.0;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(det, -2.0, S21_EPS);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_3x3_singular) {
  matrix_t A;
  _alloc_matrix(&A, 3, 3);
  double v = 1.0;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) A.matrix[i][j] = v++;
  double det = 12345.0;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(det, 0.0, S21_EPS);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_3x3_from_readme) {
  matrix_t A;
  _alloc_matrix(&A, 3, 3);
  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;
  double det = NAN;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(det, -1.0, S21_EPS);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_3x3_1_to_9_zero) {
  matrix_t A;
  _alloc_matrix(&A, 3, 3);

  double v = 1.0;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) A.matrix[i][j] = v++;

  double det = NAN;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(det, 0.0, S21_EPS);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_upper_triangular_4x4) {
  matrix_t A;
  _alloc_matrix(&A, 4, 4);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[1][3] = 7;
  A.matrix[2][2] = 8;
  A.matrix[2][3] = 9;
  A.matrix[3][3] = 10;
  double det = 0.0;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(det, 400.0, S21_EPS);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_fractional_precision) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 1.000001;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;
  double det = NAN;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert_ldouble_eq_tol(det, -1.999996, S21_EPS);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_det_with_nan_and_inf) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = 1.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 3.0;
  double det = 0.0;
  int rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert(isnan(det));

  A.matrix[0][0] = INFINITY;
  A.matrix[0][1] = 0.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 2.0;
  rc = s21_determinant(&A, &det);
  ck_assert_int_eq(rc, 0);
  ck_assert(isinf(det) && det > 0.0);

  _free_matrix(&A);
}
END_TEST

Suite *s21_determinant_suite(void) {
  Suite *s = suite_create("determinant");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_det_null_A_pointer);
  tcase_add_test(tc, test_det_null_result_pointer);
  tcase_add_test(tc, test_det_A_field_null);
  tcase_add_test(tc, test_det_non_square);

  tcase_add_test(tc, test_det_1x1);
  tcase_add_test(tc, test_det_2x2_basic);
  tcase_add_test(tc, test_det_3x3_singular);
  tcase_add_test(tc, test_det_3x3_from_readme);
  tcase_add_test(tc, test_det_3x3_1_to_9_zero);
  tcase_add_test(tc, test_det_upper_triangular_4x4);
  tcase_add_test(tc, test_det_fractional_precision);
  tcase_add_test(tc, test_det_with_nan_and_inf);

  suite_add_tcase(s, tc);
  return s;
}
