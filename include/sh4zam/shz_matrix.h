/*! \file
    \brief   API for operating on MxN matrices within memory.
    \ingroup matrix

    This file provides a collection of routines for manipulating
    MxN matrices which are stored within memory, rather than in
    the XMTRX FP register back-bank.

    Some of these routines are simply loading into the back-bank
    and are performing work there, temporarily. Some of them have
    implementations which have specific optimizations for when the
    matrices are NOT within such registers, which are faster than
    having to go through XMTRX and clobbering the back-bank.

    \todo
        - shz_mat4x4_add_symmetric_skew()
        - shz_mat4x4_add_diagonal()
        - shz_mat4x4_apply_transpose()
        - shz_mat4x4_apply_unaligned()
        - shz_mat4x4_mult_transpose()
        - shz_mat4x4_mult_unaligned()

    \author    2025, 2026 Falco Girgis
    \author    2025 Daniel Fairchild

    \copyright MIT License
*/

#ifndef SHZ_MATRIX_H
#define SHZ_MATRIX_H

#include "shz_vector.h"
#include "shz_quat.h"
#include "shz_xmtrx.h"

/*! \defgroup matrix Matrices
    \brief    In-Memory Matrix Manipulation

    These types and their corresponding functions are for working
    with matrices stored within memory, as opposed to being preloaded
    within the XMTRX back-bank of FP registers. Typically this is
    desirable for one-off operations where there is no batching of
    matrix operations, especially when clobbering XMTRX is undesirable.

    For most transform types, this API offers 4 different "versions" of
    the operation. Using translation as an example:
        - shz_mat4x4_init_translation(): **Initializes** the the matrix
          to a given transform, setting the other components to identity.
        - shz_mat4x4_set_translation(): **Sets** only the values
          corresponding to the given transform, leaving the others alone.
        - shz_mat4x4_apply_translation(): **Apply** transform operation,
          updates only the values corresponding to the given transform based
          on their current values (additively in this case, multiplicatively
          for scaling and rotation).
        - shz_mat4x4_translate(): **GL-based** transform operation,
          multiplying and accumulating the given matrix by a matrix which
          has been initialized to the given transform.

    \warning
    Beware that some of these routines clobber the matrix currently loaded
    as the active 4x4 matrix, XMTRX.

    \sa xmtrx
*/

SHZ_DECLS_BEGIN

/*! Structure representing a 4x4 column-major matrix.

    \warning
    This structure MUST be aligned on 8-byte boundaries!
*/
typedef SHZ_ALIGNAS(8) struct shz_mat4x4 {
    union {                     //!< Inner convenience union.
        float      elem[16];        //!< Access the matrix as a 1D array of 16 single-precision floats.
        float      elem2D[4][4];    //!< Access the matrix as a 2D array of 4x4 single-precision floats.
        shz_vec4_t col[4];          //!< Access the matrix as an array of 4 1x4 column vectors.
        struct {                //!< Named column vectors.
            shz_vec4_t left;        //!< Access the first column of the matrix as a 1x4 vector.
            shz_vec4_t up;          //!< Access the second column of the matrix as a 1x4 vector.
            shz_vec4_t forward;     //!< Access the third column of the matrix as a 1x4 vector.
            shz_vec4_t pos;         //!< Access the last column of the matrix as a 1x4 vector.
        };
    };
} shz_mat4x4_t;

//! Alternate shz_mat4x4_t C typedef for those who hate POSIX style.
typedef shz_mat4x4_t shz_mat4x4;

/*! \name  Initialization
    \brief Routines for fully initializing a matrix.
   @{
*/

/*! Initializes the given matrix to the identity matrix as fast as possible.

    \warning This routine will NOT zero out NaNs properly!
    \warning This routine clobbers XMTRX.

    \sa shz_mat4x4_init_identity_safe()
*/
SHZ_INLINE void shz_mat4x4_init_identity(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Initializes the given matrix to the identity matrix, safely zeroing out NaN values.

    \warning This routine clobbers XMTRX.

    \sa shz_mat4x4_init_identity()
*/
SHZ_INLINE void shz_mat4x4_init_identity_safe(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Initializes the given matrix with all 0s for its element values.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_zero(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Initializes the given matrix with all 1s for its element values.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_one(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Initializes the given matrix with all elements assigned to the given value.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_fill(shz_mat4x4_t* mat, float value) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D translation matrix with the given coordinates.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D scaling matrix with the given dimensions.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix by \p xAngle radians over the X-axis.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_x(shz_mat4x4_t* mat, float xAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix by \p yAngle radians over the Y-axis.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_y(shz_mat4x4_t* mat, float yAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix by \p zAngle radians over the Z-axis.

    \warning This routine clobber XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_z(shz_mat4x4_t* mat, float zAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix from the intrinsic rotation created by the given Tait-Bryan X-Y-Z angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_xyz(shz_mat4x4_t* mat, float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix from the intrinsic rotation created by the given Tait-Bryan Z-Y-X angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_zyx(shz_mat4x4_t* mat, float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix from the intrinsic rotation created by the given Tait-Bryan Z-X-Y angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_zxy(shz_mat4x4_t* mat, float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix from the intrinsic rotation created by the given Tait-Bryan Y-X-Z angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_yxz(shz_mat4x4_t* mat, float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix about the given \p axis rotated by \p angle radians.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation(shz_mat4x4_t* mat, float radians, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix with its orientation given by a quaternion.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a diagonal matrix with the given 4 values.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT;

/*! Initializes the given matrix to an upper diagonal matrix whose nonzero entries have the given value.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_upper_diagonal(shz_mat4x4_t* mat, float col1, shz_vec2_t col2, shz_vec3_t col3, shz_vec4_t col4) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a lower diagonal matrix whose nonzero entries have the given value.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_lower_diagonal(shz_mat4x4_t* mat, shz_vec4_t col1, shz_vec3_t col2, shz_vec2_t col3, float col4) SHZ_NOEXCEPT;

/*! Initializes the given matrix to be the symmetric skew of the given 3D vector components.

    \note
    This can be useful for batching the cross-product operation against a constant vector.
    For one-offs, prefer shz_vec3_cross().

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Initializes the given matrix to be the result from taking the outer product of the two given 4D vectors.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_outer_product(shz_mat4x4_t* mat, shz_vec4_t v1, shz_vec4_t v2) SHZ_NOEXCEPT;

/*! Initializes the matrix to to a permutation matrix, which reorders the components of transformed vectors to be in WXYZ order.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_permutation_wxyz(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Initializes the matrix to to a permutation matrix, which reorders the components of transformed vectors to be in YZWX order.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_permutation_yzwx(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Initializes the given matrix to the viewport matrix with the given dimenions.

    \sa This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_screen(shz_mat4x4_t* mat, float width, float height) SHZ_NOEXCEPT;

//! @}

/*! \name Getting
    \brief Routines for getting specific values within a matrix.
    @{
*/

//! Extracts the \p row index as a 4D row vector from the given matrix.
SHZ_FORCE_INLINE shz_vec4_t shz_mat4x4_row(const shz_mat4x4_t* mat, size_t row) SHZ_NOEXCEPT;

//! Extracts the \p col index as a 4D column vector from the given matrix.
SHZ_FORCE_INLINE shz_vec4_t shz_mat4x4_col(const shz_mat4x4_t* mat, size_t col) SHZ_NOEXCEPT;

//! Extracts the top-left 3x3 of the given 4D matrix.
SHZ_INLINE void shz_mat4x4_3x3(const shz_mat4x4_t* mat4, shz_mat3x3_t* mat3) SHZ_NOEXCEPT;

//! Extracts and inverts the top-level, unscaled 3x3 of the given 4x4 matrix.
SHZ_INLINE void shz_mat4x4_3x3_inverse_unscaled(const shz_mat4x4_t* mat4, shz_mat3x3_t* invmat3) SHZ_NOEXCEPT;

//! Extracts and inverts the top-level 3x3 (which may be scaled) of the given 4x4 matrix.
SHZ_INLINE void shz_mat4x4_3x3_inverse(const shz_mat4x4_t* mat4, shz_mat3x3_t* invmat3) SHZ_NOEXCEPT;

//! Returns the determinant of the given 4x4 matrix.
SHZ_INLINE float shz_mat4x4_determinant(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

//! Returns the determinant of the given 4x4 matrix's internal top-level 3x3 matrix.
SHZ_INLINE float shz_mat4x4_3x3_determinant(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

//! @}

/*! \name Setting
    \brief Routines for setting specific values within a matrix
    @{
*/

//! Sets the values of \p mat at the given \p row to those of the 4D vector, \p vec.
SHZ_FORCE_INLINE void shz_mat4x4_set_row(shz_mat4x4_t* mat, size_t row, shz_vec4_t vec) SHZ_NOEXCEPT;

//! Sets the values of \p mat at the given \p col to those of the 4D vector, \p vec.
SHZ_FORCE_INLINE void shz_mat4x4_set_col(shz_mat4x4_t* mat, size_t col, shz_vec4_t vec) SHZ_NOEXCEPT;

//! Swaps the 4D row vectors located at \p row1 and \p row2 within \p mat.
SHZ_INLINE void shz_mat4x4_swap_rows(shz_mat4x4_t* mat, size_t row1, size_t row2) SHZ_NOEXCEPT;

//! Swaps the 4D column vectors located at \p row1 and \p row2 within \p mat.
SHZ_INLINE void shz_mat4x4_swap_cols(shz_mat4x4_t* mat, size_t col1, size_t col2) SHZ_NOEXCEPT;

//! Assigns only the 3D translation-related elements of the given matrix to the given values.
SHZ_INLINE void shz_mat4x4_set_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

//! Assigns only the 3D scale-related elements of the given matrix to the given values.
SHZ_INLINE void shz_mat4x4_set_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

//! Sets just the rotational component of the matrix to the orientation given by a quaternion, keeping the other elements in-tact.
SHZ_INLINE void shz_mat4x4_set_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;

//! Assigns only the 4 elements along the diagonal of the given matrix to the given values.
SHZ_INLINE void shz_mat4x4_set_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT;

//! @}

/*! \name  Applying
    \brief Routines for multiplying and accumulating onto the given matrix.
    @{
*/

/*! Multiplies and accumulates the \p src 4x4 matrix onto the \p dst 4x4 matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the unaligned \p src 4x4 matrix onto the \p dst 4x4 matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_unaligned(shz_mat4x4_t* dst, const float src[16]) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the transposed \p src 4x4 matrix onto the \p dst 4x4 matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_transpose(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the transposed unaligned \p src 4x4 matrix onto the \p dst 4x4 matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_transpose_unaligned(shz_mat4x4_t* dst, const float src[16]) SHZ_NOEXCEPT;

/*! Adds the given 3D vector components to the translational values of the given matrix.

    \note This routine does not use XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the scale-related elements of the given matrix by the given 3D components.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates a rotation matrix by \p xAngle radians about the X-axis onto the given matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_x(shz_mat4x4_t* mat, float xAngle) SHZ_NOEXCEPT;

/*! Multiplies and accumulates a rotation matrix by \p yAngle radians about the Y-axis onto the given matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_y(shz_mat4x4_t* mat, float yAngle) SHZ_NOEXCEPT;

/*! Multiplies and accumulates a rotation matrix by \p zAngle radians about the Z-axis onto the given matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_z(shz_mat4x4_t* mat, float zAngle) SHZ_NOEXCEPT;

/*! Rotates the given transform matrix about the X axis, then the Y axis, then the Z axis by the given angles in radians.

    Multiplies and accumulates the given matrix with the 3D rotation matrix formed from the intrinsic rotation created by the given Tait-Bryan X-Y-Z angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_xyz(shz_mat4x4_t* mat, float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

/*! Rotates the given transform matrix about the Z axis, then the Y axis, then the X axis by the given angles in radians.

    Multiplies and accumulates the given matrix with the 3D rotation matrix formed from the intrinsic rotation created by the given Tait-Bryan Z-Y-X angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_zyx(shz_mat4x4_t* mat, float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

/*! Rotates the given transform matrix about the Z axis, then the X axis, then the Y axis by the given angles in radians.

    Multiplies and accumulates the given matrix with the 3D rotation matrix formed from the intrinsic rotation created by the given Tait-Bryan Z-Y-X angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_zxy(shz_mat4x4_t* mat, float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT;

/*! Rotates the given transform matrix about the Y axis, then the X axis, then the Z axis by the given angles in radians.

    Multiplies and accumulates the given matrix with the 3D rotation matrix formed from the intrinsic rotation created by the given Tait-Bryan Y-X-Z angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_yxz(shz_mat4x4_t* mat, float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

/*! Rotates the given transform matrix about the arbitrary axis given by a 3D direction vector and angle of rotation in radians.

    Multiplies and accumulates the given matrix with the 3D rotation matrix formed from the intrinsic rotation created by the given Tait-Bryan Z-Y-X angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation(shz_mat4x4_t* mat, float angle, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the given matrix with a rotation matrix whose orientation is given by a quaternion.

    \note This routine does not use XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;

/*! Applies the 3D "lookAt" matrix constructed with the given vector components onto the given matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_lookat(shz_mat4x4_t* m, shz_vec3_t pos, shz_vec3_t target, shz_vec3_t up) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the perspective matrix constructed from the given values onto the given matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_perspective(shz_mat4x4_t* m, float fov, float aspect, float near_z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the viewport matrix created with the given components ont othe given matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_screen(shz_mat4x4_t* m, float width, float height) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the given matrix with a symmetric skew matrix formed from the given 3D vector components.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates a permutation matrix, which reorders the components of transformed vectors to be in WXYZ order.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_permutation_wxyz(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Multiplies and accumulates a permutation matrix, which reorders the components of transformed vectors to be in YZWX order.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_permutation_yzwx(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the given matrix onto itself.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_self(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

//! @}

/*! \name  GL Transformations
    \brief OpenGL-style 4x4 matrix transforms.
    @{
*/

/*! Multiplies and accumulates \p mat by a 3D translation matrix with the given components.

    \note glTranslatef() equivalent.
    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_translate(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by a 3D scaling matrix with the given components.

    \note glScalef() equivalent.
    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by a 3D rotation matrix about the X axis.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_rotate_x(shz_mat4x4_t* mat, float radians) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by a 3D rotation matrix about the Y axis.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_rotate_y(shz_mat4x4_t* mat, float radians) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by a 3D rotation matrix about the Z axis.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_rotate_z(shz_mat4x4_t* mat, float radians) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by 3D rotation matrices about the X then Y then Z axes.

    \warning This routine clobbers XMTRX.
*/
SHZ_FORCE_INLINE void shz_mat4x4_rotate_xyz(shz_mat4x4_t* mat, float xRadians, float yRadians, float zRadians) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by 3D rotation matrices about the Z then Y then X axes.

    \warning This routine clobbers XMTRX.
*/
SHZ_FORCE_INLINE void shz_mat4x4_rotate_zyx(shz_mat4x4_t* mat, float zRadians, float yRadians, float xRadians) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by 3D rotation matrices about the Z then X then Y axes.

    \warning This routine clobbers XMTRX.
*/
SHZ_FORCE_INLINE void shz_mat4x4_rotate_zxy(shz_mat4x4_t* mat, float zRadians, float xRadians, float yRadians) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by 3D rotation matrices about the Y then X then Z axes.

    \warning This routine clobbers XMTRX.
*/
SHZ_FORCE_INLINE void shz_mat4x4_rotate_yxz(shz_mat4x4_t* mat, float yRadians, float xRadians, float zRadians) SHZ_NOEXCEPT;

/*! Multiplies and accumulates \p mat by the 3D rotation matrix formed by the given axis and angle.

    \note Equivalent to glRotatef().
    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_rotate(shz_mat4x4_t* mat, float radians, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

//!@}

/*! \name  Transforming
    \brief Routines for transforming vectors and points by a matrix.
    @{
*/

/*! Multiplies two 4x4 matrices together, storing the result into a third.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_mult(shz_mat4x4_t* mat, const shz_mat4x4_t* lhs, const shz_mat4x4_t* rhs) SHZ_NOEXCEPT;

/*! Multiplies two 4x4 matrices together, with the right handed matrix being unaligned, storing the result into a third.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_mult_unaligned(shz_mat4x4_t* mat, const shz_mat4x4_t* lhs, const float rhs[16]) SHZ_NOEXCEPT;

/*! Transforms a 2D vector by a 4x4 matrix.

    This is a routine specializing in one-off transforms of a **single**
    2D vector by a **single** 4x4 matrix. It should be faster than going
    through XMTRX.

    \note
    For batch transforming multiple 2D vectors against the same 4x4 matrix,
    preload the matrix into XMTRX, then use shz_xmtrx_trans_vec2().

    \sa shz_mat4x4_transform_vec2(), shz_xmtrx_transform_vec3()
*/
SHZ_INLINE shz_vec2_t shz_mat4x4_transform_vec2(const shz_mat4x4_t* m, shz_vec2_t v) SHZ_NOEXCEPT;

/*! Transforms a 3D vector by a 4x4 matrix.

    This is a routine specializing in one-off transforms of a **single**
    3D vector (such as a normal, without a W component) by a **single**
    4x4 matrix. It should be faster than going through XMTRX.

    \note
    For batch transforming multiple 3D vectors against the same 4x4 matrix,
    preload the matrix into XMTRX, then use shz_xmtrx_trans_vec3().

    \sa shz_mat4x4_transform_vec4(), shz_xmtrx_transform_vec3()
*/
SHZ_INLINE shz_vec3_t shz_mat4x4_transform_vec3(const shz_mat4x4_t* m, shz_vec3_t v) SHZ_NOEXCEPT;

/*! Transforms a 4D vector by a 4x4 matrix.

    This is a routine specializing in one-off transforms of a *single*
    4D vector by a *single* 4x4 matrix. It should be faster than going
    through XMTRX.

    \note
    For batch transforming multiple 4D vectors against the same 4x4 matrix,
    preload the matrix into XMTRX, then use shz_xmtrx_trans_vec4().

    \sa shz_mat4x4_transform_vec3(), shz_xmtrx_transform_vec4()
*/
SHZ_INLINE shz_vec4_t shz_mat4x4_transform_vec4(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT;

/*! Transforms a 2D vector the the transpose of a 4x4 matrix.

    This is a routine specializing in one-off transforms of a **single**
    2D vector by the transpose of a **single** 4x4 matrix. It should be
    faster than going through XMTRX.

    \note
    For batch transforming multiple 2D vectors against the transpose of
    the same 4x4 matrix, preload the matrix into XMTRX, take its transpose
    with shz_xmtrx_transpose(), then use shz_xmtrx_transform_vec2().

    \sa shz_xmtrx_transpose(), shz_xmtrx_transform_vec2().
*/
SHZ_INLINE shz_vec2_t shz_mat4x4_transform_vec2_transpose(const shz_mat4x4_t* m, shz_vec2_t v) SHZ_NOEXCEPT;

/*! Transforms a 3D vector the the transpose of a 4x4 matrix.

    This is a routine specializing in one-off transforms of a **single**
    3D vector by the transpose of a **single** 4x4 matrix. It should be
    faster than going through XMTRX.

    \note
    For batch transforming multiple 3D vectors against the transpose of
    the same 4x4 matrix, preload the matrix into XMTRX, take its transpose
    with shz_xmtrx_transpose(), then use shz_xmtrx_transform_vec3().

    \sa shz_xmtrx_transpose(), shz_xmtrx_transform_vec3().
*/
SHZ_INLINE shz_vec3_t shz_mat4x4_transform_vec3_transpose(const shz_mat4x4_t* m, shz_vec3_t v) SHZ_NOEXCEPT;

/*! Transforms a 4D vector the the transpose of a 4x4 matrix.

    This is a routine specializing in one-off transforms of a **single**
    4D vector by the transpose of a **single** 4x4 matrix. It should be
    faster than going through XMTRX.

    \note
    For batch transforming multiple 4D vectors against the transpose of
    the same 4x4 matrix, preload the matrix into XMTRX, take its transpose
    with shz_xmtrx_transpose(), then use shz_xmtrx_transform_vec4().

    \sa shz_xmtrx_transpose(), shz_xmtrx_transform_vec4().
*/
SHZ_INLINE shz_vec4_t shz_mat4x4_transform_vec4_transpose(const shz_mat4x4_t* m, shz_vec4_t v) SHZ_NOEXCEPT;

/*! Transforms a 2D point by a 4x4 matrix.

    This is a routine specializing in one-off transforms of a *single*
    2D point by a *single* 4x4 matrix. It should be faster than going
    through XMTRX.

    \note
    For batch transforming multiple 2D points against the same 4x4 matrix,
    preload the matrix into XMTRX, then use shz_xmtrx_transform_point2().

    \sa shz_xmtrx_transform_point2()
*/
SHZ_FORCE_INLINE shz_vec2_t shz_mat4x4_transform_point2(const shz_mat4x4_t* mat, shz_vec2_t pt) SHZ_NOEXCEPT;

/*! Transforms a 3D point by a 4x4 matrix.

    This is a routine specializing in one-off transforms of a *single*
    3D point by a *single* 4x4 matrix. It should be faster than going
    through XMTRX.

    \note
    For batch transforming multiple 3D points against the same 4x4 matrix,
    preload the matrix into XMTRX, then use shz_xmtrx_transform_point3().

    \sa shz_xmtrx_transform_point3()
*/
SHZ_FORCE_INLINE shz_vec3_t shz_mat4x4_transform_point3(const shz_mat4x4_t* mat, shz_vec3_t pt) SHZ_NOEXCEPT;

/*! Transforms a 2D point by the transpose of a 4x4 matrix.

    This is a routine specializing in one-off transforms of a *single*
    2D point by the transpose of a a *single* 4x4 matrix. It should be
    faster than going through XMTRX.

    \note
    For batch transforming multiple 2D points against the same 4x4 matrix,
    preload the matrix into XMTRX, use shz_xmtrx_transpose(), then use
    shz_xmtrx_transform_point2().

    \sa shz_xmtrx_transpose(), shz_xmtrx_transform_point2()
*/
SHZ_FORCE_INLINE shz_vec2_t shz_mat4x4_transform_point2_transpose(const shz_mat4x4_t* mat, shz_vec2_t pt) SHZ_NOEXCEPT;

/*! Transforms a 3D point by the transpose of a 4x4 matrix.

    This is a routine specializing in one-off transforms of a *single*
    3D point by the transpose of a a *single* 4x4 matrix. It should be
    faster than going through XMTRX.

    \note
    For batch transforming multiple 3D points against the same 4x4 matrix,
    preload the matrix into XMTRX, use shz_xmtrx_transpose(), then use
    shz_xmtrx_transform_point2().

    \sa shz_xmtrx_transpose(), shz_xmtrx_transform_point3()
*/
SHZ_FORCE_INLINE shz_vec3_t shz_mat4x4_transform_point3_transpose(const shz_mat4x4_t* mat, shz_vec3_t pt) SHZ_NOEXCEPT;

//! @}

/*! \name  Miscellaneous
    \brief Other matrix-related operations and routines
    @{
*/

//! Converts the given 4x4 orientation matrix into a quaternion.
SHZ_INLINE shz_quat_t shz_mat4x4_to_quat(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Stores the transpose of \p mat within \p out.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_transpose(const shz_mat4x4_t* mat, shz_mat4x4_t* out) SHZ_NOEXCEPT;

/*! Computes the inverse of a 4x4 matrix.

    \note
    In-place inversion (mtrx == out) is not supported.

    \warning This routine clobbers XMTRX.

    \param mtrx     Pointer to the 4x4 matrix to invert.
    \param out      Pointer to the resulting inverted matrix.
*/
void shz_mat4x4_inverse(const shz_mat4x4_t* SHZ_RESTRICT mtrx, shz_mat4x4_t* SHZ_RESTRICT out) SHZ_NOEXCEPT;

/*! Computes the inverse of a 4x4 matrix in block-triangular form.

    This is a special-case faster optimization for 4x4 matrices which take the form:

        A = [ M   b ]
            [ 0   w ]

    Where A is 4x4, M is 3x3, b is 3x1, and the bottom row is (0, 0, 0, w) with
    w != 0. For this block-triangular form, det(A) = det(M) * w. Then

        inv(A) = [ inv(M)        -inv(M) * b / w ]
                 [   0                 1/w       ]
    \note
    A regular 3D transform matrix is already in this form.
    \note
    shz_mat4x4_inverse() will dynamically check whether to use this optimization.
*/
void shz_mat4x4_inverse_block_triangular(const shz_mat4x4_t* mtx, shz_mat4x4_t* out) SHZ_NOEXCEPT;

//! Returns true if the two matrices are equal, based on either absolute or relative tolerance.
SHZ_INLINE bool shz_mat4x4_equal(const shz_mat4x4_t* SHZ_RESTRICT mat1, const shz_mat4x4_t* mat2) SHZ_NOEXCEPT;

//! Returns true if the given matrix is in block-triangular form: having a bottom row in the form of `<0.0f, 0.0f, 0.0f, w>`.
SHZ_INLINE bool shz_mat4x4_is_block_triangular(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Copies the given \p src 4x4 matrix into the given \p dst 4x4 matrix.

    \warning This routine clobbers XMTRX!
*/
SHZ_INLINE void shz_mat4x4_copy(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT;

/*! Copies the given unaligned \p src 4x4 matrix into the given \p dst 4x4 matrix.

    \warning This routine clobbers XMtRX.
*/
SHZ_INLINE void shz_mat4x4_copy_unaligned(shz_mat4x4_t* dst, const float src[16]) SHZ_NOEXCEPT;

//! @}

//! \cond UNDOCUMENTED
//        Until API is complete.
typedef SHZ_ALIGNAS(8) struct shz_mat2x2 {
    union {
        float       elem[4];
        float       elem2D[2][2];
        shz_vec2_t  col[2];
    };
} shz_mat2x2_t;

typedef struct shz_mat3x3 {
    union {
        float      elem[9];
        float      elem2D[3][3];
        shz_vec3_t col[3];
        struct {
            shz_vec3_t left;
            shz_vec3_t up;
            shz_vec3_t forward;
        };
    };
} shz_mat3x3_t;

typedef struct shz_mat4x3 {
    union {
        float      elem[12];
        float      elem2D[3][4];
        shz_vec3_t col[4];
        struct {
            shz_vec3_t left;
            shz_vec3_t up;
            shz_vec3_t forward;
            shz_vec3_t pos;
        };
    };
} shz_mat4x3_t;

typedef struct shz_mat3x4 {
    union {
        float      elem[12];
        float      elem2D[4][3];
        shz_vec4_t col[3];
        struct {
            shz_vec4_t left;
            shz_vec4_t up;
            shz_vec4_t forward;
        };
    };
} shz_mat3x4_t;

/*!
    Stores the transpose of 3x3 matrix \p mat within \p out.

    mtrx: Pointer to the 3x3 matrix to transpose.
    out: Pointer to the resulting transposed matrix.

    \warning This routine clobbers XMTRX.

 */
SHZ_INLINE void shz_mat3x3_transpose(const shz_mat3x3_t* mat, shz_mat3x3_t* out) SHZ_NOEXCEPT;

/*!
    Computes the inverse of a 3x3 matrix, saves cycles by not scaling by the
    determinant, which makes sense when used for normals and lighting that are
    usually normalized later.

    mtrx: Pointer to the 3x3 matrix to invert.
    out: Pointer to the resulting inverted matrix.

    \note
    Only valid if the matrix is known to be orthonormal.

    \warning This routine clobbers XMTRX.
 */
SHZ_INLINE void shz_mat3x3_inverse_unscaled(const shz_mat3x3_t* mtrx, shz_mat3x3_t* out) SHZ_NOEXCEPT;

/*!
    Computes the inverse of a 3x3 matrix.

    mtrx: Pointer to the 3x3 matrix to invert.
    out: Pointer to the resulting inverted matrix.

    \note
    Only valid for non-singular matrices.

    \warning This routine clobbers XMTRX.
 */
SHZ_INLINE void shz_mat3x3_inverse(const shz_mat3x3_t* mtrx, shz_mat3x3_t* out) SHZ_NOEXCEPT;

SHZ_INLINE void shz_mat3x3_scale(shz_mat3x3_t* dst, const shz_mat3x3_t* src, float value) SHZ_NOEXCEPT;
SHZ_INLINE shz_vec3_t shz_mat3x3_trans_vec3(const shz_mat3x3_t* m, shz_vec3_t v) SHZ_NOEXCEPT;

//! \endcond

#include "inline/shz_matrix.inl.h"

SHZ_DECLS_END

#endif // SHZ_MATRIX_H
