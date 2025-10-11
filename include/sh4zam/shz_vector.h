/*! \file
 *  \brief Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides types and mathematical routines for representing and
 *  operating on vectors.
 *
 *  \todo
 *      - shz_vec_rotate(): for one-off rotations
 *
 *  \author Falco Girgis
 *  \author Paul Cercueil
 *
 *  \copyright MIT License
 */

#ifndef SHZ_VECTOR_H
#define SHZ_VECTOR_H

#include <math.h>
#include <float.h>

#include "shz_scalar.h"
#include "shz_trig.h"

/** \defgroup vector Vector
 *  \brief           API for vector math.
 *
 *  The Vector API provides structures representing commonly-used vector types
 *  as well as routines implementing various mathematical operations on them.
 */

SHZ_DECLS_BEGIN

/*! 2D Vector type
 *
 *  Structure for holding coordinates of a 2-dimensional vector.
 *
 * \sa shz_vec3_t, shz_vec4_t
 */
typedef struct shz_vec2 {
    union {
        float e[2];  //!< <X, Y> coordinates as an array
        struct {
            float x; //!< X coordinate
            float y; //!< Y coordinate
        };
    };
} shz_vec2_t;

/*! 3D Vector type
 *
 *  Structure for holding coordinates of a 3-dimensional vector.
 *
 * \sa shz_vec2_t, shz_vec4_t
 */
typedef struct shz_vec3 {
    union {
        float e[3];              //!< <X, Y, Z> coordinates as an array
        struct {
            union {
                struct {
                    float x;     //!< X coordinate
                    float y;     //!< Y coordinate
                };
                shz_vec2_t xy;   //!< Inner 2D vector containing <X, Y> coords
            };
            float z;             //!< Z coordinate
        };
    };
} shz_vec3_t;

/*! 4D Vector type
 *
 *  Structure for holding coordinates of a 4-dimensional vector.
 *
 *  \sa shz_vec2_t, shz_vec3_t
 */
typedef struct shz_vec4 {
    union {
        float e[4];                 //!< <X, Y, Z, W> coordinates as an array.
        struct {
            union {
                struct {
                    float x;        //!< X coordinate
                    float y;        //!< Y coordinate
                    float z;        //!< Z coordinate
                };
                shz_vec3_t xyz;     //!< <X, Y, Z> coordinates as a 3D vector
            };
            float w;                //!< W coordinate
        };
        struct {
            shz_vec2_t xy;          //!< <X, Y> coordinates as a 2D vector
            shz_vec2_t zw;          //!< <Z, W> coordinates as a 2D vector
        };
    };
} shz_vec4_t;

/*! \name  Initializers
    \brief Component-based initialization routines.
    @{
*/

//! Returns a 2D vector with the given \p x, and \p y coordinates.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_init(float x, float y) SHZ_NOEXCEPT;

//! Returns a 3D vector with the given \p x, \p y, and \p z coordinates.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_init(float x, float y, float z) SHZ_NOEXCEPT;

//! Returns a 4D vector with the given \p x, \p y, \p z, and \p w coordinates.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_init(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! @}

/*! \name   Arithmetic
    \brief  Routines for basic vector arithmetic operations.
    @{
*/

//! Returns a 2D vector whose components are the sums of the given vectors' components.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the sums of the given vectors' components.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the sums of the given vectors' components.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are equal to the values of \p vec1 minus \p vec2.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are equal to the values of \p vec1 minus \p vec2.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are equal to the values of \p vec1 minus \p vec2.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose component values are those of \p vec1 times \p vec2.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose component values are those of \p vec1 times \p vec2.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_mul(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose component values are those of \p vec1 times \p vec2.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_mul(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose component values are those of \p vec1 divided by \p vec2.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose component values are those of \p vec1 divided by \p vec2.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_div(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose component values are those of \p vec1 divided by \p vec2.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_div(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose component values are those of the given vector multiplied by a factor.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_scale(shz_vec2_t vec, float factor) SHZ_NOEXCEPT;

//! Returns a 3D vector whose component values are those of the given vector multiplied by a factor.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_scale(shz_vec3_t vec, float factor) SHZ_NOEXCEPT;

//! Returns a 4D vector whose component values are those of the given vector multiplied by a factor.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_scale(shz_vec4_t vec, float factor) SHZ_NOEXCEPT;

//! @}

/*! \name  Magnitude
    \brief Math routines for vector length and normalization.
    @{
*/

//! Returns the squared magnitude of the given 2D vector.
SHZ_FORCE_INLINE float shz_vec2_magnitude_sqr(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the squared magnitude of the given 4D vector.
SHZ_FORCE_INLINE float shz_vec4_magnitude_sqr(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the squared magnitude of the given 3D vector.
SHZ_FORCE_INLINE float shz_vec3_magnitude_sqr(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the magnitude of the given 2D vector.
SHZ_FORCE_INLINE float shz_vec2_magnitude(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the magnitude of the given 3D vector.
SHZ_FORCE_INLINE float shz_vec3_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the magnitude of the given 4D vector.
SHZ_FORCE_INLINE float shz_vec4_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the inverse magnitude of the given 2D vector.
SHZ_FORCE_INLINE float shz_vec2_magnitude_inv(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the inverse magnitude of the given 3D vector.
SHZ_FORCE_INLINE float shz_vec3_magnitude_inv(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the inverse magnitude of the given 4D vector.
SHZ_FORCE_INLINE float shz_vec4_magnitude_inv(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a normalized unit vector from the given 2D vector.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a normalized unit vector from the given 3D vector.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a normalized unit vector from the given 4D vector.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize(shz_vec4_t vec) SHZ_NOEXCEPT;

/*! SAFELY returns a normalized unit vector from the given 2D vector.

    If the vector's magnitude is not `> 0.0f`, safely returns a zero vector.
*/
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_normalize_safe(shz_vec2_t vec) SHZ_NOEXCEPT;

/*! SAFELY returns a normalized unit vector from the given 3D vector.

    If the vector's magnitude is not `> 0.0f`, safely returns a zero vector.
*/
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_normalize_safe(shz_vec3_t vec) SHZ_NOEXCEPT;

/*! SAFELY returns a normalized unit vector from the given 4D vector.

    If the vector's magnitude is not `> 0.0f`, safely returns a zero vector.
*/
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_normalize_safe(shz_vec4_t vec) SHZ_NOEXCEPT;

//! @}

/*! \name  Binary Operations
    \brief Linear algebra operations performed with multiple vectors.
    @{
*/

//! Returns the dot product between the two given 2D vectors.
SHZ_FORCE_INLINE float shz_vec2_dot(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the dot product between the two given 4D vectors.
SHZ_FORCE_INLINE float shz_vec4_dot(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns the dot product between the two given 3D vectors.
SHZ_FORCE_INLINE float shz_vec3_dot(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the distance between the two given 2D vectors.
SHZ_FORCE_INLINE float shz_vec2_distance(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the distance between the two given 3D vectors.
SHZ_FORCE_INLINE float shz_vec3_distance(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the distance between the two given 4D vectors.
SHZ_FORCE_INLINE float shz_vec4_distance(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns the squared-distance between the two given 2D vectors.
SHZ_FORCE_INLINE float shz_vec2_distance_sqr(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the squared-distance between the two given 3D vectors.
SHZ_FORCE_INLINE float shz_vec3_distance_sqr(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the squared-distance between the two given 4D vectors.
SHZ_FORCE_INLINE float shz_vec4_distance_sqr(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector that is linearly interpolated from \p a to \p b by the given `0.0f-1.0f` factor, \p t.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_lerp(shz_vec2_t a, shz_vec2_t b, float t) SHZ_NOEXCEPT;

//! Returns a 3D vector that is linearly interpolated from \p a to \p b by the given `0.0f-1.0f` factor, \p t.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_lerp(shz_vec3_t a, shz_vec3_t b, float t) SHZ_NOEXCEPT;

//! Returns a 4D vector that is linearly interpolated from \p a to \p b by the given `0.0f-1.0f` factor, \p t.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_lerp(shz_vec4_t a, shz_vec4_t b, float t) SHZ_NOEXCEPT;

//! Reflects the given 2D \p incidence vector against a surface with the given \p normal, returning the result.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_reflect(shz_vec2_t incidence, shz_vec2_t normal) SHZ_NOEXCEPT;

//! Reflects the given 3D \p incidence vector against a surface with the given \p normal, returning the result.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_reflect(shz_vec3_t incidence, shz_vec3_t normal) SHZ_NOEXCEPT;

//! Reflects the given 4D \p incidence vector against a surface with the given \p normal, returning the result.
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_reflect(shz_vec4_t incidence, shz_vec4_t normal) SHZ_NOEXCEPT;

//! Refracts the given 2D \p incidence vector against a surface with the given \p normal using the given refraction index ratio, \p eta.
SHZ_INLINE shz_vec2_t shz_vec2_refract(shz_vec2_t incidence, shz_vec2_t normal, float eta) SHZ_NOEXCEPT;

//! Refracts the given 3D \p incidence vector against a surface with the given \p normal using the given refraction index ratio, \p eta.
SHZ_INLINE shz_vec3_t shz_vec3_refract(shz_vec3_t incidence, shz_vec3_t normal, float eta) SHZ_NOEXCEPT;

//! Refracts the given 4D \p incidence vector against a surface with the given \p normal using the given refraction index ratio, \p eta.
SHZ_INLINE shz_vec4_t shz_vec4_refract(shz_vec4_t incidence, shz_vec4_t normal, float eta) SHZ_NOEXCEPT;

/*! Returns the cross product, as a scalar, between two 2D vectors.

    \note
    The definition of the cross-product is ambiguous in 2D space, but the geometric
    interpretation here is that the result is the magnitude of the orthogonal vector
    created from the given two along the Z-axis.
*/
SHZ_FORCE_INLINE float shz_vec2_cross(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the vector produced by taking the cross-product of the two given 3D vectors.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_cross(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the resulting vector from projecting the given 2D vector along the given (unit) axis
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT;

//! Returns the resulting vector from projecting the given 3D vector along the given (unit) axis
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT;

//! Returns the resulting vector from projecting the given 4D vector along the given (unit) axis
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT;

/*! Returns the resulting vector from projecting the given 2D vector along the given (unit) axis

    \note
    This routine should safely return the zero vector when \p vec has a magnitude of 0.0f.
*/
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_project_safe(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT;

/*! Returns the resulting vector from projecting the given 3D vector along the given (unit) axis

    \note
    This routine should safely return the zero vector when \p vec has a magnitude of 0.0f.
*/
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_project_safe(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT;

/*! Returns the resulting vector from projecting the given 4D vector along the given (unit) axis

    \note
    This routine should safely return the zero vector when \p vec has a magnitude of 0.0f.
*/
SHZ_FORCE_INLINE shz_vec4_t shz_vec4_project_safe(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT;

//! @}

/*! \name  Angles
	\brief Routines for converting vectors to angles and vice-versa.
	@{
*/

//! Returns the angle formed between the given 2D vectors in radians.
SHZ_FORCE_INLINE float shz_vec2_angle_between(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the angle formed between the given 3D vectors in radians.
SHZ_FORCE_INLINE float shz_vec3_angle_between(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the angle formed between the positive X axis and the given 2D vector, in radians.
SHZ_FORCE_INLINE float shz_vec2_angle(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the angles formed between the positive X axis and the given 3D vector, in radians.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_angles(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the 2D unit vector representing a rotation from the positive X axis.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_sincos(shz_sincos_t sincos) SHZ_NOEXCEPT;

//! Returns the 3D unit vector representing the given rotation angles relative to the positive X axis.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_sincos(shz_sincos_t azimuth, shz_sincos_t elevation) SHZ_NOEXCEPT;

//! Returns the 2D unit vector representing a rotation from the positive X axis in radians.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle(float radians) SHZ_NOEXCEPT;

//! Returns the 3D unit vector representing the given rotation angles relative to the positive X axis in radians.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles(float azimuth, float elevation) SHZ_NOEXCEPT;

//! Returns the 2D unit vector representing a rotation from the positive X axis in degrees.
SHZ_FORCE_INLINE shz_vec2_t shz_vec2_from_angle_deg(float degrees) SHZ_NOEXCEPT;

//! Returns the 3D unit vector representing the given rotation angles relative to the positive X axis in degrees.
SHZ_FORCE_INLINE shz_vec3_t shz_vec3_from_angles_deg(float azimuth, float elevation) SHZ_NOEXCEPT;

//! @}

SHZ_DECLS_END

/*! \name  Type-Generic Routines
    \brief Generalized vector routines for C and C++.
    @{
*/

#ifndef __cplusplus

    //! C type-generic vector addition.
#   define shz_vec_add(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_add, \
                 shz_vec3_t: shz_vec3_add, \
                 shz_vec4_t: shz_vec4_add)(vec1, vec2)

    //! C type-generic vector subtraction.
#   define shz_vec_sub(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_sub, \
                 shz_vec3_t: shz_vec3_sub, \
                 shz_vec4_t: shz_vec4_sub)(vec1, vec2)

    //! C type-generic vector multiplication.
#   define shz_vec_mul(vec1, vec2) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_mul, \
                 shz_vec3_t: shz_vec3_mul, \
                 shz_vec4_t: shz_vec4_mul)(vec1, vec2)

    //! C type-generic vector division.
#   define shz_vec_div(vec1, vec2) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_div, \
                 shz_vec3_t: shz_vec3_div, \
                 shz_vec4_t: shz_vec4_div)(vec1, vec2)

    //! C type-generic vector scaling.
#   define shz_vec_scale(vec, factor) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_scale, \
                 shz_vec3_t: shz_vec3_scale, \
                 shz_vec4_t: shz_vec4_div)(vec, factor)

    //! C type-generic vector dot product.
#   define shz_vec_dot(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_dot, \
                 shz_vec3_t: shz_vec3_dot, \
                 shz_vec4_t: shz_vec4_dot)(vec1, vec2)

    //! C type-generic vector squared magnitude.
#   define shz_vec_magnitude_sqr(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_magnitude_sqr, \
                 shz_vec3_t: shz_vec3_magnitude_sqr, \
                 shz_vec4_t: shz_vec4_magnitude_sqr)(vec)

    //! C type-generic vector magnitude.
#   define shz_vec_magnitude(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_magnitude, \
                 shz_vec3_t: shz_vec3_magnitude, \
                 shz_vec4_t: shz_vec4_magnitude)(vec)

    //! C type-generic vector inverse magnitude.
#   define shz_vec_magnitude_inv(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_magnitude_inv, \
                 shz_vec3_t: shz_vec3_magnitude_inv, \
                 shz_vec4_t: shz_vec4_magnitude_inv)(vec)

    //! C type-generic vector normalization.
#   define shz_vec_normalize(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_normalize, \
                 shz_vec3_t: shz_vec3_normalize, \
                 shz_vec4_t: shz_vec4_normalize)(vec)

    //! C type-generic safe vector normalization.
#   define shz_vec_normalize_safe(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_normalize_safe, \
                 shz_vec3_t: shz_vec3_normalize_safe, \
                 shz_vec4_t: shz_vec4_normalize_safe)(vec)

    //! C type-generic vector distance.
#   define shz_vec_distance(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_distance, \
                 shz_vec3_t: shz_vec3_distance, \
                 shz_vec4_t: shz_vec4_distance)(vec1, vec2)

    //! C type-generic vector squared distance.
#   define shz_vec_distance_sqr(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_distance_sqr, \
                 shz_vec3_t: shz_vec3_distance_sqr, \
                 shz_vec4_t: shz_vec4_distance_sqr)(vec1, vec2)

    //! C type-generic linear interpolation between two vectors.
#   define shz_vec_lerp(vec1, vec2, t) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_lerp, \
                 shz_vec3_t: shz_vec3_lerp, \
                 shz_vec4_t: shz_vec4_lerp)(vec1, vec2, t)

    //! C type-generic vector reflection.
#   define shz_vec_reflect(incidence, normal) \
        _Generic((incidence), \
                 shz_vec2_t: shz_vec2_reflect, \
                 shz_vec3_t: shz_vec3_reflect, \
                 shz_vec4_t: shz_vec4_reflect)(incidence, normal)

    //! C type-generic vector refraction.
#   define shz_vec_refract(incidence, normal, eta) \
        _Generic((incidence), \
                 shz_vec2_t: shz_vec2_refract, \
                 shz_vec3_t: shz_vec3_refract, \
                 shz_vec4_t: shz_vec4_refract)(incidence, normal, eta)

    //! C type-generic vector cross-product.
#   define shz_vec_cross(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_cross, \
                 shz_vec3_t: shz_vec3_cross)(vec1, vec2)

    //! C type-generic vector projection.
#   define shz_vec_project(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_project, \
                 shz_vec3_t: shz_vec3_project, \
                 shz_vec4_t: shz_vec4_project)(vec1, vec2)

    //! C type-generic safe vector projection.
#   define shz_vec_project_safe(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_project_safe, \
                 shz_vec3_t: shz_vec3_project_safe, \
                 shz_vec4_t: shz_vec4_project_safe)(vec1, vec2)

    //! C type-generic angle-from-vector extraction.
#   define shz_vec_angles(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_angle, \
                 shz_vec3_t: shz_vec3_angles)(vec)

    //! C type-generic angle between two vectors.
#   define shz_vec_angle_between(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_angle_between, \
                 shz_vec3_t: shz_vec3_angle_between)(vec1, vec2)

#else // C++ generics (because it's too dumb to support _Generic()).

#   include <concepts>

    //! C++ type-generic vector addition.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_add(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_add(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_add(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_add(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic vector subtraction.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_sub(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_sub(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_sub(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_sub(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic multiplication of each vector component.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_mul(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_mul(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_mul(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_mul(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic division of each vector component.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_div(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_div(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_div(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_div(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic uniform scaling of each vector component.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_scale(T vec, float factor) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_scale(vec, factor);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_scale(vec, factor);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_scale(vec, factor);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic vector dot product operation.
    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_dot(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_dot(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_dot(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_dot(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic squared magnitude of the given vector.
    SHZ_FORCE_INLINE float shz_vec_magnitude_sqr(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_magnitude_sqr(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_magnitude_sqr(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_magnitude_sqr(vec);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic magnitude of the given vector.
    SHZ_FORCE_INLINE float shz_vec_magnitude(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_magnitude(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_magnitude(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_magnitude(vec);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic inverse magnitude of the given vector.
    SHZ_FORCE_INLINE float shz_vec_magnitude_inv(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_magnitude_inv(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_magnitude_inv(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_magnitude_inv(vec);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic vector normalization: returns normalized vector.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_normalize(T vec) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_normalize(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_normalize(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_normalize(vec);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic safe vector normalization: returns normalized vector, avoids divide-by-zero.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_normalize_safe(T vec) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_normalize_safe(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_normalize_safe(vec);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_normalize_safe(vec);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic distance between two points.
    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_distance(T vec1, T vec2) SHZ_NOEXCEPT  {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_distance(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_distance(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_distance(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! C++ type-generic square distance between two points.
    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_distance_sqr(T vec1, T vec2) SHZ_NOEXCEPT  {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_distance_sqr(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_distance_sqr(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_distance_sqr(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! Returns the linear interpolation between the two given type-generic vectors.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_lerp(T vec1, T vec2, float t) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_lerp(vec1, vec2, t);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_lerp(vec1, vec2, t);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_lerp(vec1, vec2, t);
        else static_assert(false, "Incompatible type!");
    }

    //! Type-generic reflection of a vector over the given surface normal returned.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_reflect(T incidence, T normal) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_reflect(incidence, normal);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_reflect(incidence, normal);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_reflect(incidence, normal);
        else static_assert(false, "Incompatible type!");
    }

    //! Type-generic refraction of a vector over the given surface normal returned.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_refract(T incidence, T normal, float eta) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_refract(incidence, normal, eta);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_refract(incidence, normal, eta);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_refract(incidence, normal, eta);
        else static_assert(false, "Incompatible type!");
    }

    //! Type-generic cross product between the two given vectors returned.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_cross(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_cross(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_cross(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! Type generic projection of the first vector onto the second vector returned.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_project(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_project(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_project(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_project(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! Type generic safe projection of \p vec1 onto \p vec2 returned, avoiding division-by-zero.
    template<typename T>
    SHZ_FORCE_INLINE T shz_vec_project_safe(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_project_safe(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_project_safe(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec4_t>)
            return shz_vec4_project_safe(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }

    //! Returns the angles formed by the given type-generic vector and the +X axis direction vector.
    SHZ_FORCE_INLINE auto shz_vec_angles(auto vec) SHZ_NOEXCEPT {
        using T = decltype(vec);
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_angle(vec);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_angles(vec);
        else static_assert(false, "Incompatible type!");
    }

    //! Returns the angles formed between the given two type-generic vectors.
    template<typename T>
    SHZ_FORCE_INLINE float shz_vec_angle_between(T vec1, T vec2) SHZ_NOEXCEPT {
        if constexpr(std::convertible_to<T, shz_vec2_t>)
            return shz_vec2_angle_between(vec1, vec2);
        else if constexpr(std::convertible_to<T, shz_vec3_t>)
            return shz_vec3_angle_between(vec1, vec2);
        else static_assert(false, "Incompatible type!");
    }
#endif

#include "shz_vector.inl"

//! @}

#endif // SHZ_VECTOR_H
