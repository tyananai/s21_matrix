#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

START_TEST(test_eq_null_A_pointer) {
  matrix_t B;
  _alloc_matrix(&B, 1, 1);
  B.matrix[0][0] = 1.0;

  int rc = s21_eq_matrix(NULL, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_null_B_pointer) {
  matrix_t A;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 1.0;

  int rc = s21_eq_matrix(&A, NULL);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_eq_matrix_field_null_A) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;

  matrix_t B;
  _alloc_matrix(&B, 2, 2);

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_field_null_B) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);

  matrix_t B;
  B.matrix = NULL;
  B.rows = 2;
  B.columns = 2;

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_eq_different_rows) {
  matrix_t A, B;
  _alloc_matrix(&A, 2, 2);
  _alloc_matrix(&B, 3, 2);

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_different_columns) {
  matrix_t A, B;
  _alloc_matrix(&A, 2, 2);
  _alloc_matrix(&B, 2, 3);

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_exact_equal) {
  matrix_t A, B;
  _alloc_matrix(&A, 2, 3);
  _alloc_matrix(&B, 2, 3);

  for (int i = 0; i < A.rows; ++i)
    for (int j = 0; j < A.columns; ++j) {
      A.matrix[i][j] = (double)(i * 10 + j) + 0.123456;
      B.matrix[i][j] = A.matrix[i][j];
    }

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, SUCCESS);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_diff_less_than_eps) {
  matrix_t A, B;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);

  A.matrix[0][0] = 1.000000;
  B.matrix[0][0] = 1.000000 + (S21_EPS / 2.0);

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, SUCCESS);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_diff_equal_eps) {
  matrix_t A, B;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);

  A.matrix[0][0] = -2.5;
  B.matrix[0][0] = -2.5 + S21_EPS;

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_mismatch_first_element) {
  matrix_t A, B;
  _alloc_matrix(&A, 3, 3);
  _alloc_matrix(&B, 3, 3);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      A.matrix[i][j] = 7.0;
      B.matrix[i][j] = 7.0;
    }

  A.matrix[0][0] = 1.0;
  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_mismatch_last_element) {
  matrix_t A, B;
  _alloc_matrix(&A, 4, 4);
  _alloc_matrix(&B, 4, 4);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) {
      A.matrix[i][j] = 0.5;
      B.matrix[i][j] = 0.5;
    }

  A.matrix[3][3] = 0.5 + 1e-3;
  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_nan_element) {
  matrix_t A, B;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);

  A.matrix[0][0] = NAN;
  B.matrix[0][0] = NAN;

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_posinf_elements) {
  matrix_t A, B;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);

  A.matrix[0][0] = INFINITY;
  B.matrix[0][0] = INFINITY;

  int rc = s21_eq_matrix(&A, &B);

  ck_assert_int_eq(rc, FAILURE);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

START_TEST(test_eq_plus_minus_zero) {
  matrix_t A, B;
  _alloc_matrix(&A, 1, 1);
  _alloc_matrix(&B, 1, 1);

  A.matrix[0][0] = 0.0;
  B.matrix[0][0] = -0.0;

  int rc = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(rc, SUCCESS);

  _free_matrix(&A);
  _free_matrix(&B);
}
END_TEST

Suite *s21_eq_matrix_suite(void) {
  Suite *s = suite_create("eq_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_eq_null_A_pointer);
  tcase_add_test(tc, test_eq_null_B_pointer);
  tcase_add_test(tc, test_eq_matrix_field_null_A);
  tcase_add_test(tc, test_eq_matrix_field_null_B);
  tcase_add_test(tc, test_eq_different_rows);
  tcase_add_test(tc, test_eq_different_columns);
  tcase_add_test(tc, test_eq_exact_equal);
  tcase_add_test(tc, test_eq_diff_less_than_eps);
  tcase_add_test(tc, test_eq_diff_equal_eps);
  tcase_add_test(tc, test_eq_mismatch_first_element);
  tcase_add_test(tc, test_eq_mismatch_last_element);
  tcase_add_test(tc, test_eq_nan_element);
  tcase_add_test(tc, test_eq_posinf_elements);
  tcase_add_test(tc, test_eq_plus_minus_zero);

  suite_add_tcase(s, tc);
  return s;
}
