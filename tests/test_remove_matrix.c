#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/test_helpers.h"

START_TEST(test_remove_matrix_null_pointer) {
  s21_remove_matrix(NULL);
  ck_assert_int_eq(1, 1);
}
END_TEST

START_TEST(test_remove_matrix_matrix_field_null) {
  matrix_t m;
  m.matrix = NULL;
  m.rows = 5;
  m.columns = 7;

  s21_remove_matrix(&m);

  ck_assert_ptr_null(m.matrix);
  ck_assert_int_eq(m.rows, 5);
  ck_assert_int_eq(m.columns, 7);
}
END_TEST

START_TEST(test_remove_matrix_free_allocated) {
  matrix_t m;
  _alloc_matrix(&m, 3, 4);

  m.matrix[0][0] = 3.14;
  m.matrix[2][3] = -1.23;

  s21_remove_matrix(&m);

  ck_assert_ptr_null(m.matrix);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
}
END_TEST

START_TEST(test_remove_matrix_partial_allocation) {
  matrix_t m;
  m.rows = 4;
  m.columns = 2;
  m.matrix = calloc(m.rows, sizeof(double *));
  m.matrix[0] = calloc(m.columns, sizeof(double));
  m.matrix[1] = calloc(m.columns, sizeof(double));
  m.matrix[2] = NULL;
  m.matrix[3] = NULL;

  s21_remove_matrix(&m);

  ck_assert_ptr_null(m.matrix);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
}
END_TEST

START_TEST(test_remove_matrix_idempotent) {
  matrix_t m;
  _alloc_matrix(&m, 2, 2);

  s21_remove_matrix(&m);

  ck_assert_ptr_null(m.matrix);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);

  s21_remove_matrix(&m);

  ck_assert_ptr_null(m.matrix);
  ck_assert_int_eq(m.rows, 0);
  ck_assert_int_eq(m.columns, 0);
}
END_TEST

Suite *s21_remove_matrix_suite(void) {
  Suite *s = suite_create("remove_matrix");
  TCase *tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_remove_matrix_null_pointer);
  tcase_add_test(tc_core, test_remove_matrix_matrix_field_null);
  tcase_add_test(tc_core, test_remove_matrix_free_allocated);
  tcase_add_test(tc_core, test_remove_matrix_partial_allocation);
  tcase_add_test(tc_core, test_remove_matrix_idempotent);

  suite_add_tcase(s, tc_core);
  return s;
}