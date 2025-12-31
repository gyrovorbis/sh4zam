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

    \author    2025 Falco Girgis
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

/*! \name  Initialization
    \brief Routines for fully initializing a matrix.
   @{
*/

/*! Initializes the given matrix to the identity matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_identity(shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a diagonal matrix with the given 4 values.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT;

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

/*! Initializes the given matrix to a 3D rotation matrix from the intrinsic rotation created by the given Tait-Bryan Y-X-Z angles.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation_yxz(shz_mat4x4_t* mat, float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D rotation matrix about the given \p axis rotated by \p angle radians.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_rotation(shz_mat4x4_t* mat, float radians, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

/*! Initializes the given matrix to a 3D translation matrix with the given coordinates.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

SHZ_INLINE void shz_mat4x4_init_mult(shz_mat4x4_t* mat, const shz_mat4x4_t* lhs, const shz_mat4x4_t* rhs) SHZ_NOEXCEPT;

/*! Initializes the given matrix to be the symmetric skew of the given 3D vector components.

    \note
    This can be useful for batching the cross-product operation against a constant vector.
    For one-offs, prefer shz_vec3_cross().

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_init_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

SHZ_INLINE void shz_mat4x4_set_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;

//! Initializes the given matrix to a 3D rotation matrix with its orientation given by a quaternion.
SHZ_INLINE void shz_mat4x4_init_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;

//! @}

/*! \name Setting
    \brief Routines for setting specific values within a matrix
    @{
*/

//! Assigns only the 3D scale-related elements of the given matrix to the given values.
SHZ_INLINE void shz_mat4x4_set_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

//! Assigns only the 4 elements along the diagonal of the given matrix to the given values.
SHZ_INLINE void shz_mat4x4_set_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT;

//! Assigns only the 3D translation-related elements of the given matrix to the given values.
SHZ_INLINE void shz_mat4x4_set_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

//! Sets just the rotational component of the matrix to the orientation given by a quaternion, keeping the other elements in-tact.
SHZ_INLINE void shz_mat4x4_set_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;
//! @}

/*! \name  Applying
    \brief Routines for multiplying and accumulating onto the given matrix.
    @{
*/

/*! Multiplies and accumulates the \p src 4x4 matrix onto the \p dst 4x4 matrix.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT;

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

//! Multiplies and accumulates the given matrix with a rotation matrix whose orientation is given by a quaternion.
SHZ_INLINE void shz_mat4x4_apply_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the given matrix with a symmetric skew matrix formed from the given 3D vector components.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the scale-related elements of the given matrix by the given 3D components.

    \warning This routine clobbers XMTRX.
*/
SHZ_INLINE void shz_mat4x4_apply_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

//! Adds the given 3D vector components to the translational values of the given matrix.
SHZ_INLINE void shz_mat4x4_apply_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT;

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

//! Returns the determinant of the given 4x4 matrix.
SHZ_INLINE float shz_mat4x4_determinant(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

/*! Computes the inverse of a 4x4 matrix.

    \note
    In-place inversion (mtrx == out) is not supported.

    \warning This routine clobbers XMTRX.

    \param mtrx     Pointer to the 4x4 matrix to invert.
    \param out      Pointer to the resulting inverted matrix.
*/
void shz_mat4x4_inverse(const shz_mat4x4_t* SHZ_RESTRICT mtrx, shz_mat4x4_t* SHZ_RESTRICT out) SHZ_NOEXCEPT;

/*! Copies the given \p src 4x4 matrix into the given \p dst 4x4 matrix.

    \warning
    This routine clobbers XMTRX!
*/
SHZ_INLINE void shz_mat4x4_copy(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT;

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


SHZ_INLINE shz_vec3_t shz_mat3x3_trans_vec3(const shz_mat3x3_t* m, shz_vec3_t v) SHZ_NOEXCEPT;

//! \endcond

#include "shz_matrix.inl.h"

SHZ_DECLS_END

#endif // SHZ_MATRIX_H
