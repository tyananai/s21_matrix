#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

START_TEST(test_create_matrix_success) {
  matrix_t m;
  int rc = s21_create_matrix(3, 4, &m);
  ck_assert_int_eq(rc, 0);

  ck_assert_int_eq(m.rows, 3);
  ck_assert_int_eq(m.columns, 4);
  ck_assert_ptr_nonnull(m.matrix);

  for (int i = 0; i < m.rows; ++i) {
    ck_assert_ptr_nonnull(m.matrix[i]);
    for (int j = 0; j < m.columns; ++j) {
      ck_assert_double_le(fabs(m.matrix[i][j]), S21_EPS);
    }
  }

  _free_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_zero_rows) {
  matrix_t m;
  int rc = s21_create_matrix(0, 5, &m);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_create_matrix_negative_columns) {
  matrix_t m;
  int rc = s21_create_matrix(3, -2, &m);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_create_matrix_null_result) {
  int rc = s21_create_matrix(2, 2, NULL);
  ck_assert_int_eq(rc, 1);
}
END_TEST

START_TEST(test_remove_matrix_clears_fields) {
  matrix_t m;
  int rc = s21_create_matrix(2, 2, &m);
  ck_assert_int_eq(rc, 0);

  m.matrix[0][0] = 3.14;
  m.matrix[1][1] = -2.71;

  _free_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_one_by_one_assign_and_remove) {
  matrix_t m;
  int rc = s21_create_matrix(1, 1, &m);
  ck_assert_int_eq(rc, 0);
  ck_assert_int_eq(m.rows, 1);
  ck_assert_int_eq(m.columns, 1);
  ck_assert_ptr_nonnull(m.matrix);
  ck_assert_ptr_nonnull(m.matrix[0]);

  m.matrix[0][0] = 42.123456;
  ck_assert_double_eq_tol(m.matrix[0][0], 42.123456, S21_EPS);

  _free_matrix(&m);
}
END_TEST

Suite *s21_create_matrix_suite(void) {
  Suite *s = suite_create("create_matrix");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_create_matrix_success);
  tcase_add_test(tc, test_create_matrix_zero_rows);
  tcase_add_test(tc, test_create_matrix_negative_columns);
  tcase_add_test(tc, test_create_matrix_null_result);
  tcase_add_test(tc, test_remove_matrix_clears_fields);
  tcase_add_test(tc, test_create_matrix_one_by_one_assign_and_remove);

  suite_add_tcase(s, tc);
  return s;
}