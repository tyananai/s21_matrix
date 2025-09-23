/**
 * @file s21_matrix.h
 * @brief Matrix operations library interface
 */

#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#include <math.h>
#include <stdlib.h>

/**
 * @brief Matrix structure
 *
 * matrix - pointer to an array of row pointers; each row is an array of double
 * rows   - number of rows
 * columns- number of columns
 */
typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

/*======================================================================
    STATUS CODE DEFINITIONS
======================================================================*/

/**
 * @brief Operation completed successfully.
 */
#define S21_OK 0

/**
 * @brief Invalid matrix (wrong parameters or allocation failure).
 */
#define S21_INCORRECT_MATRIX 1

/**
 * @brief Calculation error (dimension mismatch, singular matrix, etc.).
 */
#define S21_CALC_ERROR 2

/**
 * @brief Boolean value: matrices are equal.
 */
#define SUCCESS 1

/**
 * @brief Boolean value: matrices are not equal.
 */
#define FAILURE 0

/** @brief Comparison tolerance
 */
#define S21_EPS 1e-6

/*======================================================================
    MATRIX OPERATIONS
======================================================================*/

/**
 * @brief Creates a new matrix with the given dimensions.
 * @param rows Number of rows.
 * @param columns Number of columns.
 * @param result Pointer to the resulting matrix structure.
 * @return Error code: `0` (OK), `1` (incorrect matrix).
 * @author s21: tyananai
 * @date September 1, 2025
 */
int s21_create_matrix(int rows, int columns, matrix_t *result);

/**
 * @brief Frees memory and destroys the matrix.
 * @param A Pointer to the matrix to remove.
 * @return None (void function).
 * @author s21: tyananai
 * @date September 1, 2025
 */
void s21_remove_matrix(matrix_t *A);

/**
 * @brief Compares two matrices for equality up to 6 decimal places.
 * @param A Pointer to the first matrix.
 * @param B Pointer to the second matrix.
 * @return `1` (SUCCESS) if matrices are equal, `0` (FAILURE) otherwise.
 * @author s21: tyananai
 * @date September 4, 2025
 */
int s21_eq_matrix(matrix_t *A, matrix_t *B);

/**
 * @brief Adds two matrices of the same dimensions.
 * @param A Pointer to the first matrix.
 * @param B Pointer to the second matrix.
 * @param result Pointer to store the resulting matrix.
 * @return Error code: `0` (OK), `1` (incorrect matrix), `2` (calculation error,
 * e.g. mismatched sizes).
 * @author s21: tyananai
 * @date September 6, 2025
 */
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Subtracts matrix B from matrix A.
 * @param A Pointer to the first matrix.
 * @param B Pointer to the second matrix.
 * @param result Pointer to store the resulting matrix.
 * @return Error code: `0` (OK), `1` (incorrect matrix), `2` (calculation error,
 * e.g. mismatched sizes).
 * @author s21: tyananai
 * @date September 6, 2025
 */
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Multiplies a matrix by a scalar number.
 * @param A Pointer to the input matrix.
 * @param number Scalar multiplier.
 * @param result Pointer to store the resulting matrix.
 * @return Error code: `0` (OK), `1` (incorrect matrix).
 * @author s21: tyananai
 * @date September 8, 2025
 */
int s21_mult_number(matrix_t *A, double number, matrix_t *result);

/**
 * @brief Multiplies two matrices (A × B).
 * @param A Pointer to the first matrix.
 * @param B Pointer to the second matrix.
 * @param result Pointer to store the resulting matrix.
 * @return Error code: `0` (OK), `1` (incorrect matrix), `2` (calculation error,
 * e.g. mismatched sizes).
 * @author s21: tyananai
 * @date September 8, 2025
 */
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

/**
 * @brief Transposes a matrix (swaps rows with columns).
 * @param A Pointer to the input matrix.
 * @param result Pointer to store the transposed matrix.
 * @return Error code: `0` (OK), `1` (incorrect matrix).
 * @author s21: tyananai
 * @date September 8, 2025
 */
int s21_transpose(matrix_t *A, matrix_t *result);

/**
 * @brief Calculates the matrix of algebraic complements.
 * @param A Pointer to the input matrix.
 * @param result Pointer to store the complements matrix.
 * @return Error code: `0` (OK), `1` (incorrect matrix), `2` (calculation error,
 * e.g. non-square matrix).
 * @author s21: tyananai
 * @date September 12, 2025
 */
int s21_calc_complements(matrix_t *A, matrix_t *result);

/**
 * @brief Calculates the determinant of a square matrix.
 * @param A Pointer to the input matrix.
 * @param result Pointer to store the determinant value.
 * @return Error code: `0` (OK), `1` (incorrect matrix), `2` (calculation error,
 * e.g. non-square matrix).
 * @author s21: tyananai
 * @date September 12, 2025
 */
int s21_determinant(matrix_t *A, double *result);

/**
 * @brief Calculates the inverse of a square matrix.
 * @param A Pointer to the input matrix.
 * @param result Pointer to store the inverse matrix.
 * @return Error code: `0` (OK), `1` (incorrect matrix), `2` (calculation error,
 * e.g. determinant is 0).
 * @author s21: tyananai
 * @date September 12, 2025
 */
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

#endif