#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

START_TEST(test_mult_null_A_pointer) {
  matrix_t B;
  _alloc_matrix(&B, 1, 1);
  B.matrix[0][0] = 2.0;
  int rc = s21_mult_matrix(NULL, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_mult_null_B_pointer) {
  matrix_t A;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 2.0;
  int rc = s21_mult_matrix(&A, NULL, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_mult_null_result_pointer) {
  matrix_t A, B;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);
  A.matrix[0][0] = 3.0;
  B.matrix[0][0] = 4.0;
  int rc = s21_mult_matrix(&A, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_mult_A_matrix_field_null) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;
  matrix_t B;
  _alloc_matrix(&B, 2, 2);
  int rc = s21_mult_matrix(&A, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_mult_B_matrix_field_null) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  matrix_t B;
  B.matrix = NULL;
  B.rows = 2;
  B.columns = 2;
  int rc = s21_mult_matrix(&A, &B, NULL);
  ck_assert_int_eq(rc, 1);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_mult_incompatible_dimensions) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 2, 3);
  _alloc_matrix(&B, 4, 2);
  result.matrix = NULL;
  int rc = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 2);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_mult_1x1) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);
  A.matrix[0][0] = 2.5;
  B.matrix[0][0] = -4.0;
  int rc = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 1);
  ck_assert_int_eq(result.columns, 1);
  ck_assert_ldouble_eq_tol(result.matrix[0][0], -10.0, S21_EPS);
  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_mult_rectangular_2x3_by_3x2) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 2, 3);
  _alloc_matrix(&B, 3, 2);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;

  B.matrix[0][0] = 7;
  B.matrix[0][1] = 8;
  B.matrix[1][0] = 9;
  B.matrix[1][1] = 10;
  B.matrix[2][0] = 11;
  B.matrix[2][1] = 12;

  int rc = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);

  ck_assert_ldouble_eq_tol(result.matrix[0][0], 58.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[0][1], 64.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], 139.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][1], 154.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_mult_given_example_3x2_by_2x3) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 3, 2);
  _alloc_matrix(&B, 2, 3);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 4;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 5;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 6;

  B.matrix[0][0] = 1;
  B.matrix[0][1] = -1;
  B.matrix[0][2] = 1;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 3;
  B.matrix[1][2] = 4;

  int rc = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  double expected[3][3] = {
      {9.0, 11.0, 17.0}, {12.0, 13.0, 22.0}, {15.0, 15.0, 27.0}};

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected[i][j], S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_mult_identity_3x3) {
  matrix_t A, I, result;
  _alloc_matrix(&A, 3, 3);
  _alloc_matrix(&I, 3, 3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) A.matrix[i][j] = i * 3 + j + 1;

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) I.matrix[i][j] = (i == j) ? 1.0 : 0.0;

  int rc = s21_mult_matrix(&A, &I, &result);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      ck_assert_ldouble_eq_tol(result.matrix[i][j], A.matrix[i][j], S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&I);
}
END_TEST

START_TEST(test_mult_with_zero_matrix) {
  matrix_t A, Z, result;
  _alloc_matrix(&A, 2, 2);
  _alloc_matrix(&Z, 2, 2);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = -2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 0.5;

  int rc = s21_mult_matrix(&A, &Z, &result);
  ck_assert_int_eq(rc, 0);

  for (int i = 0; i < result.rows; ++i)
    for (int j = 0; j < result.columns; ++j)
      ck_assert_ldouble_eq_tol(result.matrix[i][j], 0.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&Z);
}
END_TEST

START_TEST(test_mult_with_nan_and_inf) {
  matrix_t A, B, result;
  _alloc_matrix(&A, 2, 2);
  _alloc_matrix(&B, 2, 2);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = 1.0;
  A.matrix[1][0] = INFINITY;
  A.matrix[1][1] = 2.0;

  B.matrix[0][0] = 3.0;
  B.matrix[0][1] = 4.0;
  B.matrix[1][0] = 5.0;
  B.matrix[1][1] = 6.0;

  int rc = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(rc, 0);

  ck_assert(isnan(result.matrix[0][0]));
  ck_assert(isnan(result.matrix[0][1]));
  ck_assert(isinf(result.matrix[1][0]) && result.matrix[1][0] > 0);
  ck_assert(isinf(result.matrix[1][1]) && result.matrix[1][1] > 0);

  _free_matrix(&result);
  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

Suite *s21_mult_matrix_suite(void) {
  Suite *s = suite_create("mult_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_mult_null_A_pointer);
  tcase_add_test(tc, test_mult_null_B_pointer);
  tcase_add_test(tc, test_mult_null_result_pointer);
  tcase_add_test(tc, test_mult_A_matrix_field_null);
  tcase_add_test(tc, test_mult_B_matrix_field_null);
  tcase_add_test(tc, test_mult_incompatible_dimensions);

  tcase_add_test(tc, test_mult_1x1);
  tcase_add_test(tc, test_mult_rectangular_2x3_by_3x2);
  tcase_add_test(tc, test_mult_given_example_3x2_by_2x3);
  tcase_add_test(tc, test_mult_identity_3x3);
  tcase_add_test(tc, test_mult_with_zero_matrix);

  tcase_add_test(tc, test_mult_with_nan_and_inf);

  suite_add_tcase(s, tc);
  return s;
}
