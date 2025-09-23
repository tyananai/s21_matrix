#include <check.h>

#include "../include/s21_matrix.h"
#include "../include/s21_suites.h"
#include "../include/test_helpers.h"

static double det_3x3(const matrix_t *M) {
  double a = M->matrix[0][0], b = M->matrix[0][1], c = M->matrix[0][2];
  double d = M->matrix[1][0], e = M->matrix[1][1], f = M->matrix[1][2];
  double g = M->matrix[2][0], h = M->matrix[2][1], i = M->matrix[2][2];
  return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

START_TEST(test_calc_complements_null_A) {
  matrix_t result = {NULL, 0, 0};
  int rc = s21_calc_complements(NULL, &result);
  ck_assert_int_eq(rc, S21_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_calc_complements_null_result) {
  matrix_t A;
  _alloc_matrix(&A, 2, 2);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int rc = s21_calc_complements(&A, NULL);
  ck_assert_int_eq(rc, S21_INCORRECT_MATRIX);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_A_field_null) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 2;
  A.columns = 2;
  matrix_t result = {NULL, 0, 0};
  int rc = s21_calc_complements(&A, &result);
  ck_assert_int_eq(rc, S21_INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_calc_complements_non_square) {
  matrix_t A;
  _alloc_matrix(&A, 2, 3);
  matrix_t result = {NULL, 0, 0};
  int rc = s21_calc_complements(&A, &result);
  ck_assert_int_eq(rc, S21_CALC_ERROR);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_1x1) {
  matrix_t A, result;
  _alloc_matrix(&A, 1, 1);
  A.matrix[0][0] = 7.0;

  int rc = s21_calc_complements(&A, &result);
  ck_assert_int_eq(rc, S21_OK);
  ck_assert_int_eq(result.rows, 1);
  ck_assert_int_eq(result.columns, 1);

  ck_assert_ldouble_eq_tol(result.matrix[0][0], 1.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_example_readme) {
  matrix_t A, result;
  _alloc_matrix(&A, 3, 3);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;

  int rc = s21_calc_complements(&A, &result);
  ck_assert_int_eq(rc, S21_OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  double expected[3][3] = {
      {0.0, 10.0, -20.0}, {4.0, -14.0, 8.0}, {-8.0, -2.0, 4.0}};

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected[i][j], S21_EPS);

  ck_assert_ldouble_eq_tol(A.matrix[0][0], 1.0, S21_EPS);
  ck_assert_ldouble_eq_tol(A.matrix[1][1], 4.0, S21_EPS);
  ck_assert_ldouble_eq_tol(A.matrix[2][2], 1.0, S21_EPS);

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_nan_inf_and_preserve) {
  matrix_t A, result;
  _alloc_matrix(&A, 2, 2);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = 1.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = INFINITY;

  int rc = s21_calc_complements(&A, &result);
  ck_assert_int_eq(rc, S21_OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);

  ck_assert(isinf(result.matrix[0][0]) && result.matrix[0][0] > 0.0);
  ck_assert_ldouble_eq_tol(result.matrix[0][1], -2.0, S21_EPS);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], -1.0, S21_EPS);
  ck_assert(isnan(result.matrix[1][1]));

  ck_assert(isnan(A.matrix[0][0]));
  ck_assert(isinf(A.matrix[1][1]));

  _free_matrix(&result);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_3x3_adjugate_property) {
  matrix_t A;
  _alloc_matrix(&A, 3, 3);

  A.matrix[0][0] = 3;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 0;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 4;
  A.matrix[2][1] = 1;
  A.matrix[2][2] = 3;

  matrix_t cof;
  cof.matrix = NULL;
  int rc = s21_calc_complements(&A, &cof);
  ck_assert_int_eq(rc, S21_OK);

  matrix_t adj;
  _alloc_matrix(&adj, 3, 3);
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) adj.matrix[j][i] = cof.matrix[i][j];

  matrix_t prod;
  _alloc_matrix(&prod, 3, 3);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      double sum = 0.0;
      for (int k = 0; k < 3; ++k) sum += A.matrix[i][k] * adj.matrix[k][j];
      prod.matrix[i][j] = sum;
    }
  }

  double det = det_3x3(&A);

  matrix_t expected;
  _alloc_matrix(&expected, 3, 3);
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) expected.matrix[i][j] = (i == j) ? det : 0.0;

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      ck_assert_ldouble_eq_tol(prod.matrix[i][j], expected.matrix[i][j],
                               S21_EPS);

  _free_matrix(&cof);
  _free_matrix(&adj);
  _free_matrix(&prod);
  _free_matrix(&expected);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_non_square_3x4) {
  matrix_t A;
  _alloc_matrix(&A, 3, 4);

  A.matrix[0][0] = 1.0;
  A.matrix[1][1] = 2.0;
  A.matrix[2][2] = 3.0;

  matrix_t result;
  result.matrix = NULL;
  int rc = s21_calc_complements(&A, &result);
  ck_assert_int_eq(rc, S21_CALC_ERROR);

  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_5x5_diagonal) {
  matrix_t A;
  _alloc_matrix(&A, 5, 5);

  double diag[5] = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 5; ++j) A.matrix[i][j] = (i == j) ? diag[i] : 0.0;

  matrix_t cof;
  cof.matrix = NULL;
  int rc = s21_calc_complements(&A, &cof);
  ck_assert_int_eq(rc, S21_OK);

  double det = 1;
  for (int i = 0; i < 5; ++i) det *= diag[i];

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (i == j) {
        double expected = det / diag[i];
        ck_assert_ldouble_eq_tol(cof.matrix[i][j], expected, S21_EPS);
      } else {
        ck_assert_ldouble_eq_tol(cof.matrix[i][j], 0.0, S21_EPS);
      }
    }
  }

  _free_matrix(&cof);
  _free_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_6x6_diagonal) {
  matrix_t A;
  _alloc_matrix(&A, 6, 6);

  double diag[6] = {1, 2, 3, 4, 5, 6};
  for (int i = 0; i < 6; ++i)
    for (int j = 0; j < 6; ++j) A.matrix[i][j] = (i == j) ? diag[i] : 0.0;

  matrix_t cof;
  cof.matrix = NULL;
  int rc = s21_calc_complements(&A, &cof);
  ck_assert_int_eq(rc, S21_OK);

  double det = 1;
  for (int i = 0; i < 6; ++i) det *= diag[i];

  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (i == j) {
        double expected = det / diag[i];
        ck_assert_ldouble_eq_tol(cof.matrix[i][j], expected, S21_EPS);
      } else {
        ck_assert_ldouble_eq_tol(cof.matrix[i][j], 0.0, S21_EPS);
      }
    }
  }

  _free_matrix(&cof);
  _free_matrix(&A);
}
END_TEST

Suite *s21_calc_complements_suite(void) {
  Suite *s = suite_create("calc_complements");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_calc_complements_null_A);
  tcase_add_test(tc, test_calc_complements_null_result);
  tcase_add_test(tc, test_calc_complements_A_field_null);
  tcase_add_test(tc, test_calc_complements_non_square);

  tcase_add_test(tc, test_calc_complements_1x1);
  tcase_add_test(tc, test_calc_complements_example_readme);
  tcase_add_test(tc, test_calc_complements_nan_inf_and_preserve);

  tcase_add_test(tc, test_calc_complements_3x3_adjugate_property);
  tcase_add_test(tc, test_calc_complements_non_square_3x4);
  tcase_add_test(tc, test_calc_complements_5x5_diagonal);
  tcase_add_test(tc, test_calc_complements_6x6_diagonal);

  suite_add_tcase(s, tc);
  return s;
}
