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
 *      - shz_xmtrx_xxx_outer_product()
 *      - shz_xmtrx_invert()
 *      - shz_xmtrx_position()
 *      - shz_xmtrx_size()
 *      - shz_xmtrx_angles()
 *      - shz_xmtrx_ortho()
 *      - shz_xmtrx_frustum()
 *
 *  \author Falco Girgis
 *  \author Twada
 *
 *  \copyright MIT License
 */

#ifndef SHZ_XMTRX_H
#define SHZ_XMTRX_H

#include "shz_fpscr.h"
#include "shz_vector.h"
#include "shz_quat.h"

/*! \defgroup xmtrx
 *  \brief    Active Matrix
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
SHZ_INLINE float shz_xmtrx_read_reg(shz_xmtrx_reg_t xf) SHZ_NOEXCEPT;

//! Sets the floating-point value held within the given XMTRX register to \p value.
SHZ_INLINE void shz_xmtrx_write_reg(shz_xmtrx_reg_t xf, float value) SHZ_NOEXCEPT;

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

SHZ_INLINE void shz_xmtrx_load_4x3(const shz_mat4x3_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_load_cols_3x4(const shz_vec4_t* c1,
                                        const shz_vec4_t* c2,
                                        const shz_vec4_t* c3) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_load_rows_3x4(const shz_vec4_t* r1,
                                        const shz_vec4_t* r2,
                                        const shz_vec4_t* r3) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_load_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_load_transpose_3x3(const float* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_load_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT;

//! @}

/*! \name  Storing
    \brief Routines for saving XMTRX contents to memory. 
    @{
*/

SHZ_INLINE void shz_xmtrx_store_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_store_unaligned_4x4(float matrix[16]) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_store_transpose_4x4(shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_store_3x4(shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_store_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_store_transpose_3x3(shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_store_2x2(shz_mat2x2_t* matrix) SHZ_NOEXCEPT;

//! @}

/*! \name  Initialization
    \brief Routines used to initialize the entirety of XMTRX.
    @{
*/

//! Initializes XMTRX to be a 4D identity matrix.
SHZ_INLINE void shz_xmtrx_init_identity(void) SHZ_NOEXCEPT;

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

/* Tait-Bryan angles, (extrinsic rotation notation) */


SHZ_INLINE void shz_xmtrx_init_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

// Same as yaw, pitch, roll
SHZ_INLINE void shz_xmtrx_init_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_init_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_init_rotation(shz_vec3_t axis, float angle) SHZ_NOEXCEPT;

//! Initializes XMTRX to be a diagonal matrix with the given diagonal values.
SHZ_INLINE void shz_xmtrx_init_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_init_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;


//! @}

/*! \name  Apply Operation
    \brief Updates only relevant values of XMTRX based on the given transform.
    @{
*/

SHZ_INLINE void shz_xmtrx_apply_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_4x4(const shz_mat4x4_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_3x4(const shz_mat3x4_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_transpose_3x3(const shz_mat3x3_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_unaligned_4x4(const float matrix[16]) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_2x2(const shz_mat2x2_t* matrix) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_translation(float x, float y, float z) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_scale(float x, float y, float z) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation_x(float x) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation_y(float y) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation_z(float z) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation_axis(float angle, float x, float y, float z) SHZ_NOEXCEPT;

void shz_xmtrx_apply_rotation_quat(shz_quat_t quat) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation_xyz(float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation_zyx(float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation_yxz(float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_rotation(shz_vec3_t axis, float angle) SHZ_NOEXCEPT;

SHZ_INLINE void shz_xmtrx_apply_lookat(float *position_3f, float *target_3f, float *up_3f) SHZ_NOEXCEPT;

// ****************************************************************
// void shz_xmtrx_apply_perspective(float f, float a, float nz)
// ****************************************************************
//  fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
// -----------+-----------+-----------+-----------
// cot(f)/a   | 0.0f      | 0.0f      | 0.0f
// 	0.0f      | cot(f)    | 0.0f      | 0.0f
// 	0.0f      | 0.0f      | 0.0f      | nz
// 	0.0f      | 0.0f      | -1.0f     | 0.0f
// ****************************************************************
SHZ_INLINE void shz_xmtrx_apply_perspective(float fov, float aspect, float near_z) SHZ_NOEXCEPT;

// ****************************************************************
// shz_xmtrx_apply_screen(float w, float h)
// ****************************************************************
//  fr[n + 0] | fr[n + 4] | fr[n + 8] | fr[n + 12]
// -----------+-----------+-----------+-----------
//	w*0.5f    | 0.0f      | 0.0f      | w*0.5f
// 	0.0f      | -h*0.5f   | 0.0f      | h*0.5f
// 	0.0f      | 0.0f      | 1.0f      | 0.0f
//  0.0f      | 0.0f      | 0.0f      | 1.0f
// ****************************************************************
SHZ_INLINE void shz_xmtrx_apply_screen(float width, float height) SHZ_NOEXCEPT;

//! @}

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

/*! Loads XMTRX with the result of applying \p matrix2 onto \p matrix1, storing the result.

    This operation is equivalent to:
        shz_xmtrx_load_4x4(matrix1);
        shz_xmtrx_apply_4x4(matrix2);
        shz_xmtrx_store_4x4(out);

    However, it has been optimized and pipelined for performing the loads,
    multiplies, and stores in parallel.

    \sa shz_xmtrx_load_apply()
*/
SHZ_INLINE void shz_xmtrx_load_apply_store_4x4(shz_mat4x4_t* out,
                                               const shz_mat4x4_t* matrix1,
                                               const shz_mat4x4_t* matrix2) SHZ_NOEXCEPT;

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

/*! \name  Modifiers
    \brief Specialized routines that modify only the values of related XMTRX components.
    @{
*/

//! Adds the values of a 3D symmetric skew matrix constructed from the given components to XMTRX.
SHZ_INLINE void shz_xmtrx_add_symmetric_skew(float x, float y, float z) SHZ_NOEXCEPT;

//! Adds the values of a 4D diagonal matrix constructed from the given components to XMTRX.
SHZ_INLINE void shz_xmtrx_add_diagonal(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! @}

/*! \name  Miscellaneous
    \brief Random operations and conversions on XMTRX.
    @{
*/

//! Transposes the elements within XMTRX, in-place.
SHZ_INLINE void shz_xmtrx_transpose(void) SHZ_NOEXCEPT;

//! Constructs a quaternion from the 3D rotation matrix within XMTRX.
shz_quat_t shz_xmtrx_to_quat(void) SHZ_NOEXCEPT;

//! Returns the determinant of XMTRX.
float shz_xmtrx_determinant(void) SHZ_NOEXCEPT;

//! @}

#include "shz_xmtrx.inl"

SHZ_DECLS_END

#endif // SHZ_XMTRX_H
