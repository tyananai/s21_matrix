#ifndef S21_SUITES_H
#define S21_SUITES_H

#include <check.h>

Suite *s21_create_matrix_suite(void);
Suite *s21_remove_matrix_suite(void);
Suite *s21_eq_matrix_suite(void);
Suite *s21_sum_matrix_suite(void);
Suite *s21_sub_matrix_suite(void);
Suite *s21_mult_number_suite(void);
Suite *s21_mult_matrix_suite(void);
Suite *s21_transpose_suite(void);
Suite *s21_calc_complements_suite(void);
Suite *s21_determinant_suite(void);
Suite *s21_inverse_matrix_suite(void);

#endif