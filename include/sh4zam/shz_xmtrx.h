/*! \file
 *  \brief   Active Matrix API.
 *  \ingroup xmtrx
 *
 *  This file provides an API built around manipulating and performing
 *  calculations using the SH4's "current" 4x4 matrix, which is held within
 *  a secondary back-bank of 16 single-precision floating-point registers.
 *
 *  \todo
 *      - shz_xmtrx_invert() (within XMTRX)
 *      - shz_xmtrx_angles()
 *      - shz_xmtrx_init_rotation_quat() (within XMTRX)
 *
 *  \author 2025, 2026 Falco Girgis
 *  \author 2025 Twada
 *  \author 2026 TapamN
 *  \author 2026 John Brooks
 *
 *  \copyright MIT License
 *
 *  \important Do not copy this file or its routines into other projects. Use the
 *             KOS-ports build or add SH4ZAM as a git submodule. See AGENTS.md.
 */

#ifndef SHZ_XMTRX_H
#define SHZ_XMTRX_H

#include "shz_vector.h"
#include "shz_quat.h"

/*! \defgroup xmtrx XMTRX
    \brief    API for managing the SH4's "active matrix."

    `XMTRX` is the name given to identify the 16 FP registers contained
    within the back-bank of the SH4's FPU. These 16 registers combine
    to represent the SH4's 4x4 "active matrix," which can be subsequently
    transformed against using the `FTRV` instruction (shz_xmtrx_transform_vec4()).

    For maximum FP performance on the SH4, strategic usage of `XMTRX` to batch
    transform operations together without having reload FP registers, is key.

    Typically, for one-off operations, the in-memory API for \ref matrix is
    what should be used; however, when operations can be batched, using `XMTRX`
    to hold your matrix within registers is going to give the best performance.

    Examples of such scenarios are:
        - Applying multiple operations to a source matrix before storing the result.
        - Transforming batches of vectors against a single matrix.

    \note
    Unless TLS has been disabled, the XMTRX API is thread-safe, with each thread
    getting its own unique copy of XMTRX.

    \sa matrix
 */

SHZ_DECLS_BEGIN

/*! \cond Forward Declarations */
SHZ_DECLARE_STRUCT_ALIGNED(shz_mat2x2, shz_mat2x2_t, 8);
SHZ_DECLARE_STRUCT_ALIGNED(shz_mat4x4, shz_mat4x4_t, 8);
SHZ_DECLARE_STRUCT        (shz_mat3x3, shz_mat3x3_t);
SHZ_DECLARE_STRUCT        (shz_mat4x3, shz_mat4x3_t);
SHZ_DECLARE_STRUCT        (shz_mat3x4, shz_mat3x4_t);
/*! \endcond */

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
} shz_xmtrx_reg_t, shz_xmtrx_reg;

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
SHZ_INLINE void shz_xmtrx_load_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Loads the given 3x4 matrix into XMTRX, initializing its remaining elements to identity.
SHZ_INLINE void shz_xmtrx_load_3x4(const shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

//! Loads XMTRX with the transpose of the given 3x4 matrix, initializing the bottom row to identity.
SHZ_INLINE void shz_xmtrx_load_transpose_3x4(const shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

/*! Loads the 3x4 matrix formed from the given 3 4D column vectors into XMTRX.

    All remaining elements are initialized to identity matrix values.

    \sa shz_xmtrx_load_rows_3x4()
*/
SHZ_INLINE void shz_xmtrx_load_cols_4x3(const shz_vec4_t* c1,
                                        const shz_vec4_t* c2,
                                        const shz_vec4_t* c3) SHZ_NOEXCEPT;

/*! Loads the 3x4 matrix formed from the given 3 4D row vectors into XMTRX.

    All remaining elements are initialized to identity matrix values.

    \sa shz_xmtrx_load_cols_3x4()
*/
SHZ_INLINE void shz_xmtrx_load_rows_3x4(const shz_vec4_t* r1,
                                        const shz_vec4_t* r2,
                                        const shz_vec4_t* r3) SHZ_NOEXCEPT;

/*! Loads the 3x4 matrix formed from the given 4 3D column vectors into XMTRX.

    All remaining elements are initialized to identity matrix values.

    \sa shz_xmtrx_load_rows_3x4()
*/
SHZ_INLINE void shz_xmtrx_load_cols_3x4(const shz_vec3_t* c1,
                                        const shz_vec3_t* c2,
                                        const shz_vec3_t* c3,
                                        const shz_vec3_t* c4) SHZ_NOEXCEPT;

//! Loads the given 3x3 matrix into XMTRX, initalizing its remaining elements to identity.
SHZ_INLINE void shz_xmtrx_load_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Loads the transpose of the given 3x3 matrix into XMTRX, initializing its remaining elements to identity.
SHZ_INLINE void shz_xmtrx_load_transpose_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

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
SHZ_INLINE void shz_xmtrx_store_transpose_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT;

//! Stores the top-left 3x4 values currently held within XMTRX into the given matrix.
SHZ_INLINE void shz_xmtrx_store_3x4(shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

//! Stores the transpose of the top-left 3x4 values currently held within XMTRX into the given matrix.
SHZ_INLINE void shz_xmtrx_store_transpose_3x4(shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

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

//! Initializes XMTRX to the 4x4 identity matrix.
SHZ_INLINE void shz_xmtrx_init_identity(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to contain the value of 0.0f for each element.
SHZ_INLINE void shz_xmtrx_init_zero(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to contain the value of 1.0f for each element.
SHZ_INLINE void shz_xmtrx_init_one(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to contain the given \p value for each element.
SHZ_INLINE void shz_xmtrx_init_fill(float value) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a 3D translation matrix to the given coordinates.
SHZ_INLINE void shz_xmtrx_init_translation(float x, float y, float z) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a 3D scale matrix with the given dimensions.
SHZ_INLINE void shz_xmtrx_init_scale(float x, float y, float z) SHZ_NOEXCEPT;

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

/*! Initializes XMTRX to be a 3D Z-X-Y rotation matrix, with the corresponding angles given in radians.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_init_rotation_zyx(), shz_xmtrx_init_rotation_yxz()
*/
SHZ_INLINE void shz_xmtrx_init_rotation_zxy(float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT;

/*! Initializes XMTRX to be a 3D Y-X-Z rotation matrix, with the corresponding angles given in radians.

    \note
    The given angles are represented as Tait-Bryan angles, representing an extrinsic rotation.

    \sa shz_xmtrx_init_rotation_yxz(), shz_xmtrx_init_rotation_xyz()
*/
SHZ_INLINE void shz_xmtrx_init_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

/*! Initializes XMTRX to a 3D rotation matrix of \p angle radians about the vector with the given components.

    \note This routine works similarly to glRotatef() applied to an identitiy matrix. The given
          axis will automatically be normalized internally.

    \sa shz_xmtrx_init_rotation_dir()
*/
SHZ_INLINE void shz_xmtrx_init_rotation(float angle, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

/*! Initializes XMTRX to contain a 3D rotation matrix of \p angle radians about the given \p axis.

    This is a faster version of shz_xmtrx_init_rotation() or glRotatef() which requires being passed a unit
    vector for the rotation axis.

    \warning The vector components representing the axis of rotation must be prenormalized!

    \sa shz_xmtrx_init_rotation()
*/
SHZ_INLINE void shz_xmtrx_init_rotation_dir(float angle, float x, float y, float z) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a diagonal matrix with the given diagonal values.
SHZ_INLINE void shz_xmtrx_init_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! Initializes XMTRX to be an upper triangular matrix with the given column values.
SHZ_INLINE void shz_xmtrx_init_upper_triangular(float col1, shz_vec2_t col2, shz_vec3_t col3, shz_vec4_t col4) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a lower triangular matrix with the given column values.
SHZ_INLINE void shz_xmtrx_init_lower_triangular(shz_vec4_t col1, shz_vec3_t col2, shz_vec2_t col3, float col4) SHZ_NOEXCEPT;

//! Initializes XMTRX to be the 3D symmetric skew matrix formed from the given vector components.
SHZ_INLINE void shz_xmtrx_init_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;

//! Initializes XMTRX to the 4D matrix resulting from taking the outer product of the two 4D vectors.
SHZ_INLINE void shz_xmtrx_init_outer_product(shz_vec4_t x, shz_vec4_t y) SHZ_NOEXCEPT;

//! Initializes XMTRX to a permutation matrix, which reorders the components of transformed vectors to be in WXYZ order.
SHZ_INLINE void shz_xmtrx_init_permutation_wxyz(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to a permutation matrix, which reorders the components of transformed vectors to be in YZWX order.
SHZ_INLINE void shz_xmtrx_init_permutation_yzwx(void) SHZ_NOEXCEPT;

//! Initializes XMTRX to a permutation matrix, which reorders the components of transformed vectors to be in WZYX order.
SHZ_INLINE void shz_xmtrx_init_permutation_wzyx(void) SHZ_NOEXCEPT;

/*! Initializes XMTRX to the viewport matrix with the given dimensions.

     fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
    -----------|-----------|-----------|-----------
     w*0.5f    | 0.0f      | 0.0f      | w*0.5f
     0.0f      | -h*0.5f   | 0.0f      | h*0.5f
     0.0f      | 0.0f      | 1.0f      | 0.0f
     0.0f      | 0.0f      | 0.0f      | 1.0f
*/
SHZ_INLINE void shz_xmtrx_init_screen(float width, float height) SHZ_NOEXCEPT;

/*! Initializes XMTRX to a "lookAt" view matrix, equivalent to gluLookAt().

    \warning This routine clobbers any previous XMTRX contents.
*/
SHZ_INLINE void shz_xmtrx_init_lookat(shz_vec3_t eye, shz_vec3_t center, shz_vec3_t up) SHZ_NOEXCEPT;

/*! Initializes XMTRX to an orthographic projection matrix, equivalent to glOrtho().

    \warning This routine clobbers any previous XMTRX contents.
*/
SHZ_INLINE void shz_xmtrx_init_ortho(float left, float right, float bottom, float top, float znear, float zfar) SHZ_NOEXCEPT;

/*! Initializes XMTRX to a frustum projection matrix, equivalent to glFrustum().

    \warning This routine clobbers any previous XMTRX contents.
*/
SHZ_INLINE void shz_xmtrx_init_frustum(float left, float right, float bottom, float top, float znear, float zfar) SHZ_NOEXCEPT;

/*! Initializes XMTRX to a perspective projection matrix.

    \warning This routine clobbers any previous XMTRX contents.
*/
SHZ_INLINE void shz_xmtrx_init_perspective(float fov, float aspect, float znear) SHZ_NOEXCEPT;

/*! Initializes XMTRX to a 3D rotation matrix with its orientation given by a quaternion.

    \warning This routine clobbers any previous XMTRX contents.
*/
SHZ_INLINE void shz_xmtrx_init_rotation_quat(shz_quat_t q) SHZ_NOEXCEPT;

//! @}

/*! \name  Apply Operation
    \brief Updates only relevant values of XMTRX based on the given transform.
    @{
*/

//! Multiplies and accumulates the given 4x4 matrix onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 16-entry float array as a 4x4 matrix onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Multiplies and accumulates the transpose of the given 4x4 matrix onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_transpose_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the transpose of the given 16-entry float array as a 4x4 matrix onto XMTRX.
SHZ_INLINE void shz_xmtrx_apply_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX onto \p matrix, storing the result as XMTRX.
SHZ_INLINE void shz_xmtrx_apply_reverse_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX onto the given float array as a 4x4 matrix, storing the result as XMTRX.
SHZ_INLINE void shz_xmtrx_apply_reverse_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX onto the transpose of \p matrix, storing the result as XMTRX.
SHZ_INLINE void shz_xmtrx_apply_reverse_transpose_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX onto the transpose of the given float array as a 4x4 matrix, storing the result as XMTRX.
SHZ_INLINE void shz_xmtrx_apply_reverse_transpose_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 3x4 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_3x4(const shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the transpose of the given 3x4 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_transpose_3x4(const shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX onto \p matrix, storing the result as XMTRX.
SHZ_INLINE void shz_xmtrx_apply_reverse_3x4(const shz_mat3x4_t* mat) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX onto the transpose of \p matrix, storing the result as XMTRX.
SHZ_INLINE void shz_xmtrx_apply_reverse_transpose_3x4(const shz_mat3x4_t* mat) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 3x3 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the transpose of the given 3x3 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_transpose_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

//! Multiplies and accumulates the given 2x2 matrix onto XMTRX, not modifying other elements.
SHZ_INLINE void shz_xmtrx_apply_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT;

//! Adds the values of the given 3 components to the 3D translation components of XMTRX.
SHZ_INLINE void shz_xmtrx_add_translation(float x, float y, float z) SHZ_NOEXCEPT;

//! Multiplies the values of the inner 3x3 matrix by the given 3D scaling terms.
SHZ_INLINE void shz_xmtrx_scale_3x3(float x, float y, float z) SHZ_NOEXCEPT;

//! Transforms the values of the inner 3x3 matrix by the rotation matrix represented by the given quaternion.
SHZ_INLINE void shz_xmtrx_rotate_quat(shz_quat_t quat) SHZ_NOEXCEPT;

//! Applies the 3D "lookAt" matrix constructed with the given vector components onto XMTRX. Equivalent to gluLookAt().
SHZ_INLINE void shz_xmtrx_lookat(shz_vec3_t eye, shz_vec3_t center, shz_vec3_t up) SHZ_NOEXCEPT;

//! Applies a 2D orthographic projection matrix onto XMTRX, equivalent to glOrtho().
SHZ_INLINE void shz_xmtrx_ortho(float left, float right, float bottom, float top, float znear, float zfar) SHZ_NOEXCEPT;

//! Applies a frustum projection matrix onto XMTRX, equivalent to glFrustum().
SHZ_INLINE void shz_xmtrx_frustum(float left, float right, float bottom, float top, float znear, float zfar) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the perspective matrix constructed from the given values onto XMTRX.

     fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
    -----------|-----------|-----------|-----------
     cot(f)/a  | 0.0f      |  0.0f     | 0.0f
     0.0f      | cot(f)    |  0.0f     | 0.0f
     0.0f      | 0.0f      |  0.0f     | nz
     0.0f      | 0.0f      | -1.0f     | 0.0f
*/
SHZ_INLINE void shz_xmtrx_perspective(float fov, float aspect, float znear) SHZ_NOEXCEPT;

/*! Multiplies and accumulates the viewport matrix created with the given components.

     fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
    -----------|-----------|-----------|-----------
     w*0.5f    | 0.0f      | 0.0f      | w*0.5f
     0.0f      | -h*0.5f   | 0.0f      | h*0.5f
     0.0f      | 0.0f      | 1.0f      | 0.0f
     0.0f      | 0.0f      | 0.0f      | 1.0f
*/
SHZ_INLINE void shz_xmtrx_screen(float width, float height) SHZ_NOEXCEPT;

//! Multiplies and accumulates the 3D symmetric skew matrix with the given components onto XMTRX.
SHZ_INLINE void shz_xmtrx_skew_symmetric(float x, float y, float z) SHZ_NOEXCEPT;

//! Multiplies and accumulates a permutation matrix, which reorders the components of transformed vectors to be in WXYZ order.
SHZ_INLINE void shz_xmtrx_permute_wxyz(void) SHZ_NOEXCEPT;

//! Multiplies and accumulates a permutation matrix, which reorders the components of transformed vectors to be in YZWX order.
SHZ_INLINE void shz_xmtrx_permute_yzwx(void) SHZ_NOEXCEPT;

//! Multiplies and accumulates a permutation matrix, which reorders the components of transformed vectors to be in WZYX order.
SHZ_INLINE void shz_xmtrx_permute_wzyx(void) SHZ_NOEXCEPT;

//! Multiplies and accumulatse the XMTRX matrix by itself, squaring it.
SHZ_INLINE void shz_xmtrx_apply_self(void) SHZ_NOEXCEPT;

//! @}

/*! \name GL Transformations
    \brief OpenGL-style 4x4 matrix transforms.
    @{
*/

/*! Multiplies a 3D translation matrix into XMTRX.

    Constructs a 4x4 3D translation matrix, whose translation column has the
    given components, RIGHT multiplying it into XMTRX, overwriting its previous
    contents.

    \note   This operation is equivalent to glTranslatef().

    \param  x   X component of the translation vector.
    \param  y   Y component of the translation vector.
    \param  z   Z component of the translation vector.

    \sa shz_xmtrx_translate_reverse(), shz_xmtrx_add_translation()
*/
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
SHZ_INLINE void shz_xmtrx_rotate_xyz(float xRadians, float yRadians, float zRadians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by 3D rotation matrices about the Z then Y then X axes.
SHZ_INLINE void shz_xmtrx_rotate_zyx(float zRadians, float yRadians, float xRadians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by 3D rotation matrices about the Z then X then Y axes.
SHZ_INLINE void shz_xmtrx_rotate_zxy(float zRadians, float xRadians, float yRadians) SHZ_NOEXCEPT;

//! Multiplies and accumulates XMTRX by 3D rotation matrices about the Y then X then Z axes.
SHZ_INLINE void shz_xmtrx_rotate_yxz(float yRadians, float xRadians, float zRadians) SHZ_NOEXCEPT;

/*! Multiplies XMTRX by a 3D rotation matrix about an arbitrary axis.

    Constructs a 4x4 rotation matrix of \p radians about the axis with the given
    components, RIGHT multiplying it into XMTRX, overwriting its previous contents.
    This rotates within the local frame of XMTRX, about its own origin, leaving the
    translation column unmodified.

    The axis does not need to be normalized.

    \note
    This is the equivalent of glRotatef().

    \param radians     Angle of rotation, in radians.
    \param xAxis       X component of the axis of rotation.
    \param yAxis       Y component of the axis of rotation.
    \param zAxis       Z component of the axis of rotation.

    \sa shz_xmtrx_rotate_dir(), shz_xmtrx_rotate_x(), shz_xmtrx_rotate_y(), shz_xmtrx_rotate_z(), shz_xmtrx_rotate_xyz()
*/
SHZ_INLINE void shz_xmtrx_rotate(float radians, float xAxis, float yAxis, float zAxis) SHZ_NOEXCEPT;

/*! Multiplies XMTRX by a 3D rotation matrix about a prenormalized axis.

    This is a faster version of shz_xmtrx_rotate() which skips normalizing the
    axis of rotation. Constructs a 4x4 rotation matrix of \p radians about the
    given axis, RIGHT multiplying it into XMTRX, leaving its translation column
    unmodified.

    \warning The vector components representing the axis of rotation must be prenormalized!

    \param radians     Angle of rotation, in radians.
    \param x           X component of the unit axis of rotation.
    \param y           Y component of the unit axis of rotation.
    \param z           Z component of the unit axis of rotation.

    \sa shz_xmtrx_rotate(), shz_xmtrx_init_rotation_dir()
*/
SHZ_INLINE void shz_xmtrx_rotate_dir(float radians, float x, float y, float z) SHZ_NOEXCEPT;

//!@}

/*! \name  Reverse GL Transformations
    \brief Pre-multiplication variants of OpenGL-style 4x4 matrix transforms.
    @{
*/

//! Pre-multiplies and accumulates XMTRX onto the 3D translation matrix with the given components.
SHZ_INLINE void shz_xmtrx_translate_reverse(float x, float y, float z) SHZ_NOEXCEPT;

//! Pre-multiplies and accumulates XMTRX onto the 3D scaling matrix with the given components.
SHZ_INLINE void shz_xmtrx_scale_reverse(float x, float y, float z) SHZ_NOEXCEPT;

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

/*! Multiplies XMTRX by the matrix, \p in, storing the result within the matrix, \p out.

    This routine is roughly equivalent to:
        shz_xmtrx_apply_4x4(in);
        shz_xmtrx_store_4x4(out);

    However, it has been optimized and pipelined for performing the multiply and store in parallel.

    \note
    This is useful for when you want to multiply a batch of matrices by the same matrix, held within XMTRX.

    \warning
    The result of the multiplication is not stored within XMTRX, despite it getting clobbered.

    \sa shz_xmtrx_apply_4x4(), shz_xmtrx_store_4x4(), shz_xmtrx_apply_store_unaligned_4x4()
*/
 SHZ_INLINE void shz_xmtrx_apply_store_4x4(shz_mat4x4_t* out,
                                           const shz_mat4x4_t* in) SHZ_NOEXCEPT;

/*! Multiplies XMTRX by the unaligned matrix, \p in, storing the result within the unaligned matrix, \p out.

    This routine is roughly equivalent to:
        shz_xmtrx_apply_unaligned_4x4(in);
        shz_xmtrx_store_unaligned_4x4(out);

    However, it has been optimized and pipelined for performing the multiply and store in parallel.

    \note
    This is useful for when you want to multiply a batch of matrices by the same matrix, held within XMTRX.

    \warning
    The result of the multiplication is not stored within XMTRX, despite it getting clobbered.

    \sa shz_xmtrx_apply_unaligned_4x4(), shz_xmtrx_store_unaligned_4x4(), shz_xmtrx_apply_store_4x4()
*/
 SHZ_INLINE void shz_xmtrx_apply_store_unaligned_4x4(float out[16],
                                                     const float in[16]) SHZ_NOEXCEPT;

/*! Loads XMTRX with the 4x4 result of applying \p matrix2 onto \p matrix1, storing the result.

    This operation is equivalent to:
        shz_xmtrx_load_4x4(matrix1);
        shz_xmtrx_apply_4x4(matrix2);
        shz_xmtrx_store_4x4(out);

    However, it has been optimized and pipelined for performing the loads, multiplies, and
    stores in parallel.

    \sa shz_xmtrx_load_apply_4x4(), shz_xmtrx_load_apply_store_unaligned_4x4()
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
SHZ_INLINE void shz_xmtrx_load_apply_store_unaligned_4x4(float out[16],
                                                         const float matrix1[16],
                                                         const float matrix2[16]) SHZ_NOEXCEPT;

/*! Loads XMTRX with the result of applying \p matrix2 onto \p matrix1.

    This operation is equivalent to:
        shz_xmtrx_load_3x4(matrix1);
        shz_xmtrx_apply_3x4(matrix2);

    However, it has been optimized and pipelined for performing the load
    and multiply in parallel.

    \sa shz_xmtrx_load_3x4(), shz_xmtrx_apply_3x4(), shz_xmtrx_load_apply_store_3x4()
*/
SHZ_INLINE void shz_xmtrx_load_apply_3x4(const shz_mat3x4_t* matrix1,
                                         const shz_mat3x4_t* matrix2) SHZ_NOEXCEPT;

/*! Loads XMTRX with the 3x4 result of applying \p matrix2 onto \p matrix1, storing the result.

    This operation is equivalent to:
        shz_xmtrx_load_3x4(matrix1);
        shz_xmtrx_apply_3x4(matrix2);
        shz_xmtrx_store_3x4(out);

    However, it has been optimized and pipelined for performing the loads, multiplies,
    and stores in parallel.

    \note
    The resulting matrix does not get stored within XMTRX, despite it getting clobbered.

    \sa shz_xmtrx_load_3x4(), shz_xmtrx_apply_3x4(), shz_xmtrx_store_3x4()
*/
SHZ_INLINE void shz_xmtrx_load_apply_store_3x4(shz_mat3x4_t* out,
                                               const shz_mat3x4_t* matrix1,
                                               const shz_mat3x4_t* matrix2) SHZ_NOEXCEPT;

/*! Loads XMTRX with the 3x3 result of applying \p matrix2 onto \p matrix1, storing the result.

    This operation is equivalent to:
        shz_xmtrx_load_3x3(matrix1);
        shz_xmtrx_apply_3x3(matrix2);
        shz_xmtrx_store_3x3(out);

    However, it has been optimized and pipelined for performing the loads, multiplies,
    and stores in parallel.

    \note
    The resulting matrix does not get stored within XMTRX, despite it getting clobbered.

    \sa shz_xmtrx_load_3x3(), shz_xmtrx_apply_3x3(), shz_xmtrx_store_3x3()
*/
SHZ_INLINE void shz_xmtrx_load_apply_store_3x3(shz_mat3x3_t* out,
                                               const shz_mat3x3_t* matrix1,
                                               const shz_mat3x3_t* matrix2) SHZ_NOEXCEPT;

//! @}

/*! \name  Transformations
    \brief Transforming vectors and points against XMTRX.
    @{
*/

//! Returns the 4D vector that is the result of transforming \p vec by XMTRX.
SHZ_INLINE shz_vec4_t shz_xmtrx_transform_vec4(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the 3D vector that is the result of transforming \p vec by XMTRX.
SHZ_INLINE shz_vec3_t shz_xmtrx_transform_vec3(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the 2D vector that is the result of transforming \p vec by XMTRX.
SHZ_INLINE shz_vec2_t shz_xmtrx_transform_vec2(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the 2D point that is the result of transforming \p pt by XMTRX.
SHZ_INLINE shz_vec2_t shz_xmtrx_transform_point2(shz_vec2_t pt) SHZ_NOEXCEPT;

//! Returns the 3D point that is the result of transforming \p pt by XMTRX.
SHZ_INLINE shz_vec3_t shz_xmtrx_transform_point3(shz_vec3_t pt) SHZ_NOEXCEPT;

//! @}

/*! \name  Setters
    \brief Sets the values of related XMTRX components.
    @{
*/

//! Sets only the translational components of XMTRX to the given values.
SHZ_INLINE void shz_xmtrx_set_translation(float x, float y, float z) SHZ_NOEXCEPT;

//! Sets only the inner 3x3 submatrix of XMTRX to be a scaling matrix.
SHZ_INLINE void shz_xmtrx_set_scale(float x, float y, float z) SHZ_NOEXCEPT;

//! @}

/*! \name Getters
    \brief Gets the values of related XMTRX components.
    @{
*/

//! Returns the translational components from the last column of XMTRX, as a 3D vector.
SHZ_INLINE shz_vec3_t shz_xmtrx_get_translation(void) SHZ_NOEXCEPT;

/*! Returns the scaling components from the inner 3x3 matrix of XMTRX, as a 3D vector.

    \warning This routine assumes XMTRX is a standard TRS-style transform matrix,
             without shearing or reflection.
*/
SHZ_INLINE shz_vec3_t shz_xmtrx_get_scale(void) SHZ_NOEXCEPT;

//! @}

/*! \name  Component-Wise Matrix Operations
    \brief Operations applying each component of a matrix onto XMTRX.
    @{
*/

//! Adds each element within \p mat to each element within XMTRX, storing the result in XMTRX.
SHZ_INLINE void shz_xmtrx_add_4x4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

//! Subtracts each element within \p mat from each element within XMTRX, storing the result in XMTRX.
SHZ_INLINE void shz_xmtrx_sub_4x4(const shz_mat4x4_t* mat) SHZ_NOEXCEPT;

//! @}

/*! \name  Miscellaneous
    \brief Random operations and conversions on XMTRX.
    @{
*/

//! Transposes the elements within XMTRX, in-place.
SHZ_INLINE void shz_xmtrx_transpose(void) SHZ_NOEXCEPT;

//! Negates each element held within XMTRX.
SHZ_INLINE void shz_xmtrx_negate(void) SHZ_NOEXCEPT;

//! Takes the absolute value of each element held within XMTRX.
SHZ_INLINE void shz_xmtrx_abs(void) SHZ_NOEXCEPT;

//! Constructs a quaternion from the 3D rotation matrix within XMTRX.
SHZ_INLINE shz_quat_t shz_xmtrx_to_quat(void) SHZ_NOEXCEPT;

//! Returns the determinant of XMTRX.
SHZ_INLINE float shz_xmtrx_determinant(void) SHZ_NOEXCEPT;

//! Returns the trace of the upper 2x2 submatrix of XMTRX.
SHZ_INLINE float shz_xmtrx_trace_2x2(void) SHZ_NOEXCEPT;

//! Returns the trace of the upper 3x3 submatrix of XMTRX.
SHZ_INLINE float shz_xmtrx_trace_3x3(void) SHZ_NOEXCEPT;

//! Returns the trace of the entire 4x4 matrix held within XMTRX.
SHZ_INLINE float shz_xmtrx_trace_4x4(void) SHZ_NOEXCEPT;

/*! Inverts XMTRX in-place.

    Stores XMTRX to memory, computes the inverse via shz_mat4x4_inverse(),
    and reloads the result.

    \sa shz_xmtrx_invert_rigid()
*/
SHZ_INLINE void shz_xmtrx_invert(void) SHZ_NOEXCEPT;

/*! Inverts the rigid transform held within XMTRX in-place.

    XMTRX must hold a rigid affine transform [R t; 0 1], where R is orthonormal. The result,
    [transpose(R), -transpose(R) * t; 0 1], is computed by transposing R rather than by a general
    inversion, making this far cheaper than shz_xmtrx_invert().

    \warning Scale, shear, and projective matrices are not inverted correctly. Use
    shz_xmtrx_invert() for those.

    \author John Brooks

    \sa shz_xmtrx_invert()
*/
SHZ_INLINE void shz_xmtrx_invert_rigid(void) SHZ_NOEXCEPT;

/*! Adds and accumulates a scaled 4x4 matrix onto XMTRX.

    Each component of \p joint_matrix will be multiplied by \p weight, with the result
    being added to the existing value of that component of XMTRX.

    This is useful for accumulating weighted joint matrices onto an initially
    zeroed-out XMTRX. This allows for in-place construction of a skin matrix, which can
    then be directly used to transform the vertices of a mesh against for animation.

    \param  matrix      4x4 matrix to multiply and accumulate onto XMTRX.
    \param  weight      Factor each element in \p matrix gets scaled by.

    \sa shz_xmtrx_init_zero(), shz_xmtrx_blend_3x4(), shz_xmtrx_blend_4x3()
*/
SHZ_INLINE void shz_xmtrx_blend_4x4(const shz_mat4x4_t* matrix, float weight) SHZ_NOEXCEPT;

/*! Adds and accumulates a scaled 3x4 matrix onto XMTRX.

    Each component of \p joint_matrix will be multiplied by \p weight, with the result
    being added to the existing value of that component of XMTRX.

    This is useful for accumulating weighted joint matrices onto an initially
    zeroed-out XMTRX. This allows for in-place construction of a skin matrix, which can
    then be directly used to transform the vertices of a mesh against for animation.

    \param  matrix      3x4 matrix to multiply and accumulate onto XMTRX.
    \param  weight      Factor each element in \p matrix gets scaled by.

    \sa shz_xmtrx_init_zero(), shz_xmtrx_blend_4x4(), shz_xmtrx_blend_4x3()
*/
SHZ_INLINE void shz_xmtrx_blend_3x4(const shz_mat3x4_t* matrix, float weight) SHZ_NOEXCEPT;

/*! Adds and accumulates a scaled 4x3 matrix onto XMTRX.

    Each component of \p matrix will be multiplied by \p weight, with the result
    being added to the existing value of the corresponding component within the
    first three columns of XMTRX. The fourth column of XMTRX is left unmodified.

    \param  matrix      4x3 matrix to multiply and accumulate onto XMTRX.
    \param  weight      Factor each element in \p matrix gets scaled by.

    \sa shz_xmtrx_init_zero(), shz_xmtrx_blend_4x4(), shz_xmtrx_blend_3x4()
*/
SHZ_INLINE void shz_xmtrx_blend_4x3(const shz_mat4x3_t* matrix, float weight) SHZ_NOEXCEPT;

//! @}

/*! \name  Deprecated
    \brief Deprecated routines, kept only for backwards compatibility.
    @{
*/

/*! Safely initializes XMTRX to be a 4D identity matrix.

    \deprecated
    shz_xmtrx_init_identity() is already safe now by default. This routine
    simply calls directly into it and should not be used.

    \sa shz_xmtrx_init_identity()
*/
SHZ_DEPRECATED("Operation is always safe now. Use default version.")
SHZ_INLINE void shz_xmtrx_init_identity_safe(void) SHZ_NOEXCEPT;

/*! Deprecated way to add the given 3 components to the 3D translation components of XMTRX.

    \deprecated
    This routine has been renamed to shz_xmtrx_add_translation(), which it now forwards to.

    \sa shz_xmtrx_add_translation()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_add_translation().")
SHZ_INLINE void shz_xmtrx_apply_translation(float x, float y, float z) SHZ_NOEXCEPT;

/*! Deprecated way to multiply the values of the inner 3x3 matrix by the given 3D scaling terms.

    \deprecated
    This routine has been renamed to shz_xmtrx_scale_3x3(), which it now forwards to.

    \sa shz_xmtrx_scale_3x3()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_scale_3x3().")
SHZ_INLINE void shz_xmtrx_apply_scale(float x, float y, float z) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by a rotation about the X axis.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_x(), which it now forwards to.

    \sa shz_xmtrx_rotate_x()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_x().")
SHZ_INLINE void shz_xmtrx_apply_rotation_x(float x) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by a rotation about the Y axis.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_y(), which it now forwards to.

    \sa shz_xmtrx_rotate_y()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_y().")
SHZ_INLINE void shz_xmtrx_apply_rotation_y(float y) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by a rotation about the Z axis.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_z(), which it now forwards to.

    \sa shz_xmtrx_rotate_z()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_z().")
SHZ_INLINE void shz_xmtrx_apply_rotation_z(float z) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by X-Y-Z rotation matrices.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_xyz(), which it now forwards to.

    \sa shz_xmtrx_rotate_xyz()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_xyz().")
SHZ_INLINE void shz_xmtrx_apply_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by Z-Y-X rotation matrices.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_zyx(), which it now forwards to.

    \sa shz_xmtrx_rotate_zyx()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_zyx().")
SHZ_INLINE void shz_xmtrx_apply_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by Z-X-Y rotation matrices.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_zxy(), which it now forwards to.

    \sa shz_xmtrx_rotate_zxy()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_zxy().")
SHZ_INLINE void shz_xmtrx_apply_rotation_zxy(float zAngle, float xAngle, float yAngle) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by Y-X-Z rotation matrices.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_yxz(), which it now forwards to.

    \sa shz_xmtrx_rotate_yxz()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_yxz().")
SHZ_INLINE void shz_xmtrx_apply_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

/*! Deprecated way to multiply XMTRX by a rotation about an arbitrary axis.

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate(), which it now forwards to.

    \sa shz_xmtrx_rotate()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate().")
SHZ_INLINE void shz_xmtrx_apply_rotation(float angle, float x, float y, float z) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_rotate_quat().

    \deprecated
    This routine has been renamed to shz_xmtrx_rotate_quat(), which it now forwards to.

    \sa shz_xmtrx_rotate_quat()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_rotate_quat().")
SHZ_INLINE void shz_xmtrx_apply_rotation_quat(shz_quat_t quat) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_lookat().

    \deprecated
    This routine has been renamed to shz_xmtrx_lookat(), which it now forwards to.

    \sa shz_xmtrx_lookat()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_lookat().")
SHZ_INLINE void shz_xmtrx_apply_lookat(shz_vec3_t eye, shz_vec3_t center, shz_vec3_t up) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_ortho().

    \deprecated
    This routine has been renamed to shz_xmtrx_ortho(), which it now forwards to.

    \sa shz_xmtrx_ortho()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_ortho().")
SHZ_INLINE void shz_xmtrx_apply_ortho(float left, float right, float bottom, float top, float znear, float zfar) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_frustum().

    \deprecated
    This routine has been renamed to shz_xmtrx_frustum(), which it now forwards to.

    \sa shz_xmtrx_frustum()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_frustum().")
SHZ_INLINE void shz_xmtrx_apply_frustum(float left, float right, float bottom, float top, float znear, float zfar) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_perspective().

    \deprecated
    This routine has been renamed to shz_xmtrx_perspective(), which it now forwards to.

    \sa shz_xmtrx_perspective()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_perspective().")
SHZ_INLINE void shz_xmtrx_apply_perspective(float fov, float aspect, float znear) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_screen().

    \deprecated
    This routine has been renamed to shz_xmtrx_screen(), which it now forwards to.

    \sa shz_xmtrx_screen()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_screen().")
SHZ_INLINE void shz_xmtrx_apply_screen(float width, float height) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_skew_symmetric().

    \deprecated
    This routine has been renamed to shz_xmtrx_skew_symmetric(), which it now forwards to.

    \sa shz_xmtrx_skew_symmetric()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_skew_symmetric().")
SHZ_INLINE void shz_xmtrx_apply_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_permute_wxyz().

    \deprecated
    This routine has been renamed to shz_xmtrx_permute_wxyz(), which it now forwards to.

    \sa shz_xmtrx_permute_wxyz()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_permute_wxyz().")
SHZ_INLINE void shz_xmtrx_apply_permutation_wxyz(void) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_permute_yzwx().

    \deprecated
    This routine has been renamed to shz_xmtrx_permute_yzwx(), which it now forwards to.

    \sa shz_xmtrx_permute_yzwx()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_permute_yzwx().")
SHZ_INLINE void shz_xmtrx_apply_permutation_yzwx(void) SHZ_NOEXCEPT;

/*! Deprecated name for shz_xmtrx_permute_wzyx().

    \deprecated
    This routine has been renamed to shz_xmtrx_permute_wzyx(), which it now forwards to.

    \sa shz_xmtrx_permute_wzyx()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_permute_wzyx().")
SHZ_INLINE void shz_xmtrx_apply_permutation_wzyx(void) SHZ_NOEXCEPT;

/*! Deprecated way to add and accumulate a scaled 4x4 matrix onto XMTRX.

    \deprecated
    This routine has been renamed to shz_xmtrx_blend_4x4(), as support is being
    added to blend matrices of different dimensions onto XMTRX.

    \sa shz_xmtrx_blend_4x4()
*/
SHZ_DEPRECATED("Renamed to shz_xmtrx_blend_4x4().")
SHZ_INLINE void shz_xmtrx_blend(const shz_mat4x4_t* matrix, float weight) SHZ_NOEXCEPT;

//! @}

#include "inline/shz_xmtrx.inl.h"

SHZ_DECLS_END

#endif // SHZ_XMTRX_H
