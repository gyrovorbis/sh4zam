/*! \file
 *  \brief   Active Matrix API.
 *  \ingroup xmtrx
 *
 *  This file provides an API built around manipulating and performing
 *  calculations using the SH4's "current" 4x4 matrix, which is held within
 *  a secondary back-bank of 16 single-precision floating-point registers.
 *
 *  \todo
 *      - shz_xmtrx_xxx_fft()
 *      - shz_xmtrx_invert()
 *      - shz_xmtrx_position()
 *      - shz_xmtrx_size()
 *      - shz_xmtrx_angles()
 *      - shz_xmtrx_ortho()
 *      - shz_xmtrx_frustum()
 *      - shz_xmtrx_store_transpose_unaligned_4x4()
 *      - shz_xmtrx_init_rotation_quat()
 *
 *  \author 2025 Falco Girgis
 *  \author 2025 Twada
 *
 *  \copyright MIT License
 */

#ifndef SHZ_XMTRX_H
#define SHZ_XMTRX_H

#include "shz_fpscr.h"
#include "shz_vector.h"
#include "shz_quat.h"

/*! \defgroup xmtrx XMTRX
    \brief    API for managing the SH4's "active matrix."

    `XMTRX` is the name given to identify the 16 FP registers contained
    within the back-bank of the SH4's FPU. These 16 registers combine
    to represent the SH4's 4x4 "active matrix," which can be subsequently
    transformed against using the `FTRV` instruction (shz_xmtrx_trans_vec4()).

    For maximum FP performance on the SH4, strategic usage of `XMTRX` to batch
    transform operations together without having reload FP registers, is key.

    Typically, for one-off operations, the in-memory API for \ref matrix is
    what should be used; however, when operations can be batched, using `XMTRX`
    to hold your matrix within registers is going to give the best performance.

    Examples of such scenarios are:
        - Applying multiple operations to a source matrix before storing the result.
        - Transforming batches of vectors against a single matrix.

    \sa matrix
 */

SHZ_DECLS_BEGIN

//! Registers comprising XMTRX, in the FPU back-bank.
typedef enum shz_xmtrx_reg {
    SHZ_XMTRX_XF0,  //!< FP register `xf0`.
    SHZ_XMTRX_XF1,  //!< FP register `xf1`.
    SHZ_XMTRX_XF2,  //!< FP register `xf2`.
    SHZ_XMTRX_XF3,  //!< FP register `xf3`.
    SHZ_XMTRX_XF4,  //!< FP register `xf4`.
    SHZ_XMTRX_XF5,  //!< FP register `xf5`.
    SHZ_XMTRX_XF6,  //!< FP register `xf6`.
    SHZ_XMTRX_XF7,  //!< FP register `xf7`.
    SHZ_XMTRX_XF8,  //!< FP register `xf8`.
    SHZ_XMTRX_XF9,  //!< FP register `xf9`.
    SHZ_XMTRX_XF10, //!< FP register `xf10`.
    SHZ_XMTRX_XF11, //!< FP register `xf11`.
    SHZ_XMTRX_XF12, //!< FP register `xf12`.
    SHZ_XMTRX_XF13, //!< FP register `xf13`.
    SHZ_XMTRX_XF14, //!< FP register `xf14`.
    SHZ_XMTRX_XF15  //!< FP register `xf15`.
} shz_xmtrx_reg_t;

/*! \name  Accessors
    \brief Setting and retrieving individual XMTRX register values.
    @{
*/

//! Returns the floating-point value held within the given XMTRX register.
SHZ_INLINE float shz_xmtrx_read(shz_xmtrx_reg_t xf) SHZ_NOEXCEPT;

//! Sets the floating-point value held within the given XMTRX register to \p value.
SHZ_INLINE void shz_xmtrx_write(shz_xmtrx_reg_t xf, float value) SHZ_NOEXCEPT;

//! Returns the values at the the given row \p index, as a 4D vector.
SHZ_INLINE shz_vec4_t shz_xmtrx_read_row(unsigned int index) SHZ_NOEXCEPT;

//! Returns the values at the given column \p index, as a 4D vector.
SHZ_INLINE shz_vec4_t shz_xmtrx_read_col(unsigned int index) SHZ_NOEXCEPT;

//! Sets the values at the given row \p index to the given 4D vector.
SHZ_INLINE void shz_xmtrx_write_row(unsigned int index, shz_vec4_t vector) SHZ_NOEXCEPT;

//! Sets the values at the given column \p index to the given 4D vector.
SHZ_INLINE void shz_xmtrx_write_col(unsigned int index, shz_vec4_t vector) SHZ_NOEXCEPT;

//! Swaps the values of the rows with the given indices.
SHZ_INLINE void shz_xmtrx_swap_rows(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT;

//! Swaps the values of the columns with the given indices.
SHZ_INLINE void shz_xmtrx_swap_cols(unsigned int index1, unsigned int index2) SHZ_NOEXCEPT;

//! @}

/*! \name  Loading
    \brief Routines for loading XMTRX contents from memory.
    @{
*/

//! Loads the given 4x4 matrix as XMTRX.
SHZ_INLINE void shz_xmtrx_load_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Loads the given 4x4 matrix as XMTRX, with the 4th column for translation being loaded as the first column.
SHZ_INLINE void shz_xmtrx_load_wxyz_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Loads the given array of unaligned 16 float values as the 4x4 XMTRX matrix.
SHZ_INLINE void shz_xmtrx_load_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Sets XMTRX equal to the 4x4 matrix created from the 4 given 4D column vectors.
SHZ_INLINE void shz_xmtrx_load_cols_4x4(const shz_vec4_t* c1,
                                        const shz_vec4_t* c2,
                                        const shz_vec4_t* c3,
                                        const shz_vec4_t* c4) SHZ_NOEXCEPT;

//! Sets XMTRX equal to the 4x4 matrix created from the 4 given 4D row vectors.
SHZ_INLINE void shz_xmtrx_load_rows_4x4(const shz_vec4_t* r1,
                                        const shz_vec4_t* r2,
                                        const shz_vec4_t* r3,
                                        const shz_vec4_t* r4) SHZ_NOEXCEPT;

//! Loads XMTRX with the transpose of the given 4x4 matrix.
SHZ_INLINE void shz_xmtrx_load_transpose_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Loads XMTRX with the transpose of the 4x4 matrix created from the given unaligned array of 16 floats.
SHZ_FORCE_INLINE void shz_xmtrx_load_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Loads the given 4x3 matrix into XMTRX, initializing its remaining elements to identity.
SHZ_INLINE void shz_xmtrx_load_4x3(const shz_mat4x3_t* matrix) SHZ_NOEXCEPT;

/*! Loads the 3x4 matrix formed from the given 3 4D column vectors into XMTRX.

    All remaining elements are initialized to identity matrix values.

    \sa shz_xmtrx_load_rows_3x4()
*/
SHZ_INLINE void shz_xmtrx_load_cols_3x4(const shz_vec4_t* c1,
                                        const shz_vec4_t* c2,
                                        const shz_vec4_t* c3) SHZ_NOEXCEPT;

/*! Loads the 3x4 matrix formed from the given 3 4D row vectors into XMTRX.

    All remaining elements are initialized to identity matrix values.

    \sa shz_xmtrx_load_cols_3x4()
*/
SHZ_INLINE void shz_xmtrx_load_rows_3x4(const shz_vec4_t* r1,
                                        const shz_vec4_t* r2,
                                        const shz_vec4_t* r3) SHZ_NOEXCEPT;

//! Loads the given 3x3 matrix into XMTRX, initalizing its remaining elements to identity.
SHZ_INLINE void shz_xmtrx_load_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Loads the transpose of the given 3x3 matrix into XMTRX, initializing its remaining elements to identity.
SHZ_INLINE void shz_xmtrx_load_transpose_3x3(const float* matrix) SHZ_NOEXCEPT;

//! Loads the given 2x2 matrix into XMTRX, initializing its remaining elements to identity.
SHZ_INLINE void shz_xmtrx_load_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT;

//! @}

/*! \name  Storing
    \brief Routines for saving XMTRX contents to memory.
    @{
*/

//! Stores the current values held within XMTRX into the given 4x4 matrix.
SHZ_INLINE void shz_xmtrx_store_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Stores the current values held within XMTRX into the given unaligned 16-float array.
SHZ_INLINE void shz_xmtrx_store_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT;

//! Stores the transpose of the current values held within XMTRX into the given 4x4 matrix.
SHZ_INLINE void shz_xmtrx_store_transpose_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Stores the transpose of the the current values held within XMTRX into the given 16-element float array.
SHZ_FORCE_INLINE void shz_xmtrx_store_transpose_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT;

//! Stores the top-left 3x4 values currently held within XMTRX into the given matrix.
SHZ_INLINE void shz_xmtrx_store_3x4(shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

//! Stores the top-left 3x3 values currently held within XMTRX into the given matrix.
SHZ_INLINE void shz_xmtrx_store_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Stores the transpose of the top-left 3x3 values currently held within XMTRX into the given matrix.
SHZ_INLINE void shz_xmtrx_store_transpose_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Stores the top-left 2x2 values currently held within XMTRX into the given matrix.
SHZ_INLINE void shz_xmtrx_store_2x2(shz_mat2x2_t* matrix) SHZ_NOEXCEPT;

//! @}

/*! \name  Initialization
    \brief Routines used to initialize the entirety of XMTRX.
    @{
*/

/*! Quickly initializes XMTRX to be a 4D identity matrix.

    \warning This routine will not properly zero-out NaN values!

    \sa shz_xmtrx_init_identity_safe()
*/
SHZ_INLINE void shz_xmtrx_init_identity(void) SHZ_NOEXCEPT;

/*! Safely initializes XMTRX to be a 4D identity matrix.

    This routine is guaranteed to properly initialize XMTRX, regardless of NaN values.

    \sa shz_xmtrx_init_identity()
*/
SHZ_INLINE void shz_xmtrx_init_identity_safe(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to contain the value of 0.0f for each element.
SHZ_INLINE void shz_xmtrx_init_zero(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to contain the value of 1.0f for each element.
SHZ_INLINE void shz_xmtrx_init_one(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to contain the given \p value for each element.
SHZ_INLINE void shz_xmtrx_init_fill(float value) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a 3D translation matrix to the given coordinates.
SHZ_INLINE void shz_xmtrx_init_translation(float x, float y, float z) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a 3D scale matrix with the given dimensions.
SHZ_FORCE_INLINE void shz_xmtrx_init_scale(float x, float y, float z) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a 3D rotation matrix by \p x radians about the X axis.
SHZ_INLINE void shz_xmtrx_init_rotation_x(float x) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a 3D rotation matrix by \p y radians about the Y axis.
SHZ_INLINE void shz_xmtrx_init_rotation_y(float y) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a 3D rotation matrix by \p z radians about the Z axis.
SHZ_INLINE void shz_xmtrx_init_rotation_z(float z) SHZ_NOEXCEPT;

/*! Initializes XMTRX to be a 3D X-Y-Z rotation matrix, with the corresponding angles given in radians.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_init_rotation_zyx(), shz_xmtrx_init_rotation_yxz()
*/
SHZ_INLINE void shz_xmtrx_init_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

/*! Initializes XMTRX to be a 3D Z-Y-X rotation matrix, with the corresponding angles given in radians.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_init_rotation_xyz(), shz_xmtrx_init_rotation_yxz()
*/
SHZ_INLINE void shz_xmtrx_init_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

/*! Initializes XMTRX to be a 3D Y-X-Z rotation matrix, with the corresponding angles given in radians.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_init_rotation_yxz(), shz_xmtrx_init_rotation_xyz()
*/
SHZ_INLINE void shz_xmtrx_init_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

//! Initializes XMTRX to a 3D rotation matrix of \p angle radians about the given \p axis.
SHZ_INLINE void shz_xmtrx_init_rotation(float angle, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a diagonal matrix with the given diagonal values.
SHZ_INLINE void shz_xmtrx_init_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! Initializes XMTRX to be an upper diagonal matrix with the given column values.
SHZ_INLINE void shz_xmtrx_init_upper_diagonal(float col1, shz_vec2_t col2, shz_vec3_t col3, shz_vec4_t col4) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a lower diagonal matrix with the given column values.
SHZ_INLINE void shz_xmtrx_init_lower_diagonal(shz_vec4_t col1, shz_vec3_t col2, shz_vec2_t col3, float col4) SHZ_NOEXCEPT;

//! Initializes XMTRX to be the 3D symmetric skew matrix formed from the given vector components.
SHZ_INLINE void shz_xmtrx_init_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;

//! Initializex XMTRX to the 4D matrix resulting from taking the outer product of the two 4D vectors.
SHZ_INLINE void shz_xmtrx_init_outer_product(shz_vec4_t x, shz_vec4_t y) SHZ_NOEXCEPT;

//! @}

/*! \name  Apply Operation
    \brief Updates only relevant values of XMTRX based on the given transform.
    @{
*/

//! Multiplies and accumulates the given 4x4 matrix onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 3x4 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_3x4(const shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 3x3 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulateas the transpose of the given 3x3 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_transpose_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 16-entry float array as a 4x4 matrix onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 2x2 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT;

//! Adds the values of the given 3 components to the 3D translation components of XMTRX.
SHZ_INLINE void shz_xmtrx_apply_translation(float x, float y, float z) SHZ_NOEXCEPT;

//! Multiplies the values of the inner 3x3 matrix by the given 3D scaling terms.
SHZ_INLINE void shz_xmtrx_apply_scale(float x, float y, float z) SHZ_NOEXCEPT;

//! Transforms the values of the inner 3x3 matrix by a rotation matrix of \p x radians about the X axis.
SHZ_INLINE void shz_xmtrx_apply_rotation_x(float x) SHZ_NOEXCEPT;

//! Transforms the values of the inner 3x3 matrix by a rotation matrix of \p y radians about the Y axis.
SHZ_INLINE void shz_xmtrx_apply_rotation_y(float y) SHZ_NOEXCEPT;

//! Transforms the values of the inner 3x3 matrix by a rotation matrix of \p z radians about the Z axis.
SHZ_INLINE void shz_xmtrx_apply_rotation_z(float z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates XMTRX by a 3D X-Y-Z rotation matrix, with the corresponding angles given in radians.

    The transform is applied to the inner 3x3 values within XMTRX, preserving the translational components.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_apply_rotation_zyx(), shz_xmtrx_apply_rotation_yxz()
*/
SHZ_INLINE void shz_xmtrx_apply_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

/*! Multiplies and accumulates XMTRX by a 3D Z-Y-X rotation matrix, with the corresponding angles given in radians.

    The transform is applied to the inner 3x3 values within XMTRX, preserving the translational components.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_apply_rotation_xyz(), shz_xmtrx_apply_rotation_yxz()
*/
SHZ_INLINE void shz_xmtrx_apply_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

/*! Multiplies and accumulates XMTRX by a 3D Y-X-Z rotation matrix, with the corresponding angles given in radians.

    The transform is applied to the inner 3x3 values within XMTRX, preserving the translational components.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_apply_rotation_xyz(), shz_xmtrx_apply_rotation_zyx()
*/
SHZ_INLINE void shz_xmtrx_apply_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

//! Transforms the values of the inner 3x3 matrix by a rotation matrix of \p angle radians about the axis with the given components.
SHZ_INLINE void shz_xmtrx_apply_rotation(float angle, float x, float y, float z) SHZ_NOEXCEPT;

//! Transforms the values of the inner 3x3 matrix by the rotation matrix represented by the given quaternion.
void shz_xmtrx_apply_rotation_quat(shz_quat_t quat) SHZ_NOEXCEPT;

//! Applies the 3D "lookAt" matrix constructed with the given vector components onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_lookat(const float* position_3f, const float* target_3f, const float* up_3f) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the perspective matrix constructed from the given values onto XMTRX.

     fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
    -----------|-----------|-----------|-----------
    cot(f)/a   | 0.0f      | 0.0f      | 0.0f
     0.0f      | cot(f)    | 0.0f      | 0.0f
     0.0f      | 0.0f      | 0.0f      | nz
     0.0f      | 0.0f      | -1.0f     | 0.0f
*/
SHZ_INLINE void shz_xmtrx_apply_perspective(float fov, float aspect, float near_z) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the viewport matrix created with the given components.

     fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
    -----------|-----------|-----------|-----------
     w*0.5f    | 0.0f      | 0.0f      | w*0.5f
     0.0f      | -h*0.5f   | 0.0f      | h*0.5f
     0.0f      | 0.0f      | 1.0f      | 0.0f
     0.0f      | 0.0f      | 0.0f      | 1.0f
*/
SHZ_INLINE void shz_xmtrx_apply_screen(float width, float height) SHZ_NOEXCEPT;

//! Multiplies and accumulates the 3D symmetric skew matrix with the given components onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;

//! Multiplies and accumulates a permutation matrix, which reorders the components of transformed vectors to be in WXYZ order.
SHZ_INLINE void shz_xmtrx_apply_permutation_wxyz(void) SHZ_NOEXCEPT;

//! Multiplies and accumulates a permutation matrix, which reorders the components of transformed vectors to be in YZWX order.
SHZ_INLINE void shz_xmtrx_apply_permutation_yzwx(void) SHZ_NOEXCEPT;

//! Multiplies and accumulatse the XMTRX matrix by itself, squaring it.
SHZ_INLINE void shz_xmtrx_apply_xmtrx(void) SHZ_NOEXCEPT;

//! @}

/*! \name GL Transformations
    \brief OpenGL-style 4x4 matrix transforms.
    @{
*/

//! Multiplies and accumulates XMTRX by a 3D translation matrix with the given components (glTranslatef() equivalent).
SHZ_INLINE void shz_xmtrx_translate(float x, float y, float z) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by a 3D scaling matrix with the given components (glScalef() equivalent).
SHZ_INLINE void shz_xmtrx_scale(float x, float y, float z) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by a 3D rotation matrix about the X axis.
SHZ_INLINE void shz_xmtrx_rotate_x(float radians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by a 3D rotation matrix about the Y axis.
SHZ_INLINE void shz_xmtrx_rotate_y(float radians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by a 3D rotation matrix about the Z axis.
SHZ_INLINE void shz_xmtrx_rotate_z(float radians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by 3D rotation matrices about the X then Y then Z axes.
SHZ_FORCE_INLINE void shz_xmtrx_rotate_xyz(float xRadians, float yRadians, float zRadians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by 3D rotation matrices about the Z then Y then X axes.
SHZ_FORCE_INLINE void shz_xmtrx_rotate_zyx(float zRadians, float yRadians, float xRadians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by 3D rotation matrices about the Y then X then Z axes.
SHZ_FORCE_INLINE void shz_xmtrx_rotate_yxz(float yRadians, float xRadians, float zRadians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by the 3D rotation matrix formed by the given axis and angle (glRotatef equivalent).
SHZ_INLINE void shz_xmtrx_rotate(float radians, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

//!@}

/*! \name  Compound Operations
    \brief Multiple operations combined into one pipelined transaction.
    @{
*/

/*! Loads XMTRX with the result of applying \p matrix2 onto \p matrix1.

    This operation is equivalent to:
        shz_xmtrx_load_4x4(matrix1);
        shz_xmtrx_apply_4x4(matrix2);
    
    However, it has been optimized and pipelined for performing the load
    and multiply in parallel.

    \sa shz_xmtrx_load_4x4(), shz_xmtrx_apply_4x4(), shz_xmtrx_load_apply_store_4x4()
*/
SHZ_INLINE void shz_xmtrx_load_apply_4x4(const shz_mat4x4_t* matrix1,
                                         const shz_mat4x4_t* matrix2) SHZ_NOEXCEPT;

/*! Loads XMTRX with the result of applying unaligned \p matrix2 onto \p matrix1.

    This routine is equivalent to shz_xmtrx_load_apply_4x4(), except that the two
    operand matrices do not require 8-byte alignment and can simply be 16-element
    single-precision float arrays.

    \sa shz_xmtrx_load_apply_4x4()
*/
SHZ_INLINE void shz_xmtrx_load_apply_unaligned_4x4(const float matrix1[16],
                                                   const float matrix2[16]) SHZ_NOEXCEPT;

/*! Loads XMTRX with the result of applying \p matrix2 onto \p matrix1, storing the result.

    This operation is equivalent to:
        shz_xmtrx_load_4x4(matrix1);
        shz_xmtrx_apply_4x4(matrix2);
        shz_xmtrx_store_4x4(out);

    However, it has been optimized and pipelined for performing the loads,
    multiplies, and stores in parallel.

    \sa shz_xmtrx_load_apply(), shz_xmtrx_load_apply_store_unaligned_4x4()
*/
SHZ_INLINE void shz_xmtrx_load_apply_store_4x4(shz_mat4x4_t* out,
                                               const shz_mat4x4_t* matrix1,
                                               const shz_mat4x4_t* matrix2) SHZ_NOEXCEPT;

/*! Loads XMTRX with the result of applying unaligned \p matrix2 onto unaligned \p matrix1, storing the result.

    This routine is equivalent to shz_xmtrx_load_apply_store_4x4(), except that the three
    operand matrices do not require 8-byte alignment and can simply be 16-element
    single-precision float arrays.

    \sa shz_xmtrx_load_apply_store_4x4()
*/
void shz_xmtrx_load_apply_store_unaligned_4x4(float out[16],
                                              const float matrix1[16],
                                              const float matrix2[16]) SHZ_NOEXCEPT;

//! @}

/*! \name  Vector Transformations
    \brief Transforming vectors against XMTRX.
    @{
*/

//! Returns the 4D vector that is the result of transforming \p vec by XMTRX.
SHZ_FORCE_INLINE shz_vec4_t shz_xmtrx_transform_vec4(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the 3D vector that is the result of transforming \p vec by XMTRX.
SHZ_FORCE_INLINE shz_vec3_t shz_xmtrx_transform_vec3(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the 2D vector that is the result of transformin g\p vec by XMTRX.
SHZ_FORCE_INLINE shz_vec2_t shz_xmtrx_transform_vec2(shz_vec2_t vec) SHZ_NOEXCEPT;

//! @}

/*! \name  Setters
    \brief Sets the values of related XMTRX components.
    @{
*/

//! Sets only the translational components of XMTRX to the given values.
SHZ_FORCE_INLINE void shz_xmtrx_set_translation(float x, float y, float z) SHZ_NOEXCEPT;

//! @}

/*! \name  Miscellaneous
    \brief Random operations and conversions on XMTRX.
    @{
*/

//! Adds each element within \p mat to each element within XMTRX, storing the result in XMTRX.
SHZ_INLINE void shz_xmtrx_add_4x4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

//! Subtracts each element within \p mat from each element within XMTRX, storing the result in XMTRX.
SHZ_INLINE void shz_xmtrx_sub_4x4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

//! Adds the values of a 3D symmetric skew matrix constructed from the given components to XMTRX.
SHZ_INLINE void shz_xmtrx_add_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;

//! Adds the values of a 4D diagonal matrix constructed from the given components to XMTRX.
SHZ_INLINE void shz_xmtrx_add_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! Transposes the elements within XMTRX, in-place.
SHZ_INLINE void shz_xmtrx_transpose(void) SHZ_NOEXCEPT;

//! Negates each element held within XMTRX.
SHZ_INLINE void shz_xmtrx_negate(void) SHZ_NOEXCEPT;

//! Takes the absolute value of each element held within XMTRX.
SHZ_INLINE void shz_xmtrx_abs(void) SHZ_NOEXCEPT;

//! Constructs a quaternion from the 3D rotation matrix within XMTRX.
shz_quat_t shz_xmtrx_to_quat(void) SHZ_NOEXCEPT;

//! Returns the determinant of XMTRX.
float shz_xmtrx_determinant(void) SHZ_NOEXCEPT;

//! @}

#include "shz_xmtrx.inl.h"

SHZ_DECLS_END

#endif // SHZ_XMTRX_H
