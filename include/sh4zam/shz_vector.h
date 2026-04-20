/*! \file
 *  \brief   Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides types and mathematical routines for representing and
 *  operating on vectors.
 *
 *  \author 2025, 2026 Falco Girgis
 *  \author 2025 Paul Cercueil
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

//! Alternate typedef for the shz_vec2 struct for those who hate POSIX-style.
typedef shz_vec2_t shz_vec2;

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

//! Alternate typedef for the shz_vec3 struct for those who hate POSIX-style.
typedef shz_vec3_t shz_vec3;

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


//! Alternate typedef for the shz_vec4 struct for those who hate POSIX-style.
typedef shz_vec4_t shz_vec4;

/*! \name  Initializers
    \brief Component-based initialization routines.
    @{
*/

//! Returns a 2D vector with the given \p x, and \p y coordinates.
SHZ_INLINE shz_vec2_t shz_vec2_init(float x, float y) SHZ_NOEXCEPT;

//! Returns a 3D vector with the given \p x, \p y, and \p z coordinates.
SHZ_INLINE shz_vec3_t shz_vec3_init(float x, float y, float z) SHZ_NOEXCEPT;

//! Returns a 4D vector with the given \p x, \p y, \p z, and \p w coordinates.
SHZ_INLINE shz_vec4_t shz_vec4_init(float x, float y, float z, float w) SHZ_NOEXCEPT;

//! Returns a 2D vector with the value of each component equal to \p v.
SHZ_INLINE shz_vec2_t shz_vec2_fill(float v) SHZ_NOEXCEPT;

//! Returns a 3D vector with the value of each compoonent equal to \p v.
SHZ_INLINE shz_vec3_t shz_vec3_fill(float v) SHZ_NOEXCEPT;

//! Returns a 4D vector with the value of each component equal to \p v.
SHZ_INLINE shz_vec4_t shz_vec4_fill(float v) SHZ_NOEXCEPT;

//! @}

/*! \name  Component-wise Operations
    \brief Routines which apply to each vector component.
    @{
*/

//! Returns a 2D vector whose components are the absolute values of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_abs(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the absolute values of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_abs(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the absolute values of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_abs(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the negative values of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_neg(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the negative values of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_neg(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the negative values of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_neg(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the 2D vector whose components have been inverted or reciprocated.
SHZ_INLINE shz_vec2_t shz_vec2_inv(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the 3D vector whose components have been inverted or reciprocated.
SHZ_INLINE shz_vec3_t shz_vec3_inv(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the 4D vector whose components have been inverted or reciprocated.
SHZ_INLINE shz_vec4_t shz_vec4_inv(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the maximum value of both of the given vector's components.
SHZ_INLINE float shz_vec2_max(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the maximum value of the given vector's 3 components.
SHZ_INLINE float shz_vec3_max(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the maximum value of the given vector's 4 componetns.
SHZ_INLINE float shz_vec4_max(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Retuns the minimum value of both of the given vector's components.
SHZ_INLINE float shz_vec2_min(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the minimum value of the given vector's 3 components.
SHZ_INLINE float shz_vec3_min(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the minimum value of the given vector's 4 components.
SHZ_INLINE float shz_vec4_min(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Clamps the values of the given 2D \p vec between \p min and \p max, returning a new vector.
SHZ_INLINE shz_vec2_t shz_vec2_clamp(shz_vec2_t vec, float min, float max) SHZ_NOEXCEPT;

//! Clamps the values of the given 3D \p vec between \p min and \p max, returning a new vector.
SHZ_INLINE shz_vec3_t shz_vec3_clamp(shz_vec3_t vec, float min, float max) SHZ_NOEXCEPT;

//! Clamps the values of the given 4D \p vec between \p min and \p max, returning a new vector.
SHZ_INLINE shz_vec4_t shz_vec4_clamp(shz_vec4_t vec, float min, float max) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the floor of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_floor(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the floor of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_floor(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the floor of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_floor(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the ceil of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_ceil(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the ceil of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_ceil(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the ceil of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_ceil(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the rounded values of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_round(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the rounded values of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_round(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the rounded values of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_round(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the fractional parts of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_fract(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the fractional parts of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_fract(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the fractional parts of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_fract(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the signs (-1, 0, or 1) of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_sign(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the signs (-1, 0, or 1) of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_sign(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the signs (-1, 0, or 1) of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_sign(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are saturated (clamped to [0, 1]) values of the given vector's components.
SHZ_INLINE shz_vec2_t shz_vec2_saturate(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are saturated (clamped to [0, 1]) values of the given vector's components.
SHZ_INLINE shz_vec3_t shz_vec3_saturate(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are saturated (clamped to [0, 1]) values of the given vector's components.
SHZ_INLINE shz_vec4_t shz_vec4_saturate(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the pairwise minimums of the two given vectors' components.
SHZ_INLINE shz_vec2_t shz_vec2_minv(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the pairwise minimums of the two given vectors' components.
SHZ_INLINE shz_vec3_t shz_vec3_minv(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the pairwise minimums of the two given vectors' components.
SHZ_INLINE shz_vec4_t shz_vec4_minv(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are the pairwise maximums of the two given vectors' components.
SHZ_INLINE shz_vec2_t shz_vec2_maxv(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the pairwise maximums of the two given vectors' components.
SHZ_INLINE shz_vec3_t shz_vec3_maxv(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the pairwise maximums of the two given vectors' components.
SHZ_INLINE shz_vec4_t shz_vec4_maxv(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT;

//! Returns true if the values of each element within the two 2D vectors are approximately equal based on relative or absolute tolerance.
SHZ_INLINE bool shz_vec2_equal(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT;

//! Returns true if the values of each element within the two 3D vectors are approximately equal based on relative or absolute tolerance.
SHZ_INLINE bool shz_vec3_equal(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT;

//! Returns true if the values of each element within the two 4D vectors are approximately equal based on relative or absolute tolerance.
SHZ_INLINE bool shz_vec4_equal(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT;

//! For each component: returns 0.0f if vec[i] < edge[i], otherwise 1.0f
SHZ_INLINE shz_vec2_t shz_vec2_stepv( shz_vec2_t vec, shz_vec2_t edge) SHZ_NOEXCEPT;

//! For each component: returns 0.0f if vec[i] < edge[i], otherwise 1.0f
SHZ_INLINE shz_vec3_t shz_vec3_stepv(shz_vec3_t vec, shz_vec3_t edge) SHZ_NOEXCEPT;

//! For each component: returns 0.0f if vec[i] < edge[i], otherwise 1.0f
SHZ_INLINE shz_vec4_t shz_vec4_stepv(shz_vec4_t vec, shz_vec4_t edge) SHZ_NOEXCEPT;

//! For each component: returns 0.0f if vec[i] < edge, otherwise 1.0f
SHZ_INLINE shz_vec2_t shz_vec2_step(shz_vec2_t vec, float edge) SHZ_NOEXCEPT;

//! For each component: returns 0.0f if vec[i] < edge, otherwise 1.0f
SHZ_INLINE shz_vec3_t shz_vec3_step(shz_vec3_t vec, float edge) SHZ_NOEXCEPT;

//! For each component: returns 0.0f if vec[i] < edge, otherwise 1.0f
SHZ_INLINE shz_vec4_t shz_vec4_step(shz_vec4_t vec, float edge) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between.
SHZ_INLINE shz_vec2_t shz_vec2_smoothstep(shz_vec2_t vec, float edge0, float edge1) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between
SHZ_INLINE shz_vec3_t shz_vec3_smoothstep(shz_vec3_t vec, float edge0, float edge1) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between
SHZ_INLINE shz_vec4_t shz_vec4_smoothstep(shz_vec4_t vec, float edge0, float edge1) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0[i], 1.0f at/above edge1[i], smoothly varying in-between.
SHZ_INLINE shz_vec2_t shz_vec2_smoothstepv(shz_vec2_t vec, shz_vec2_t edge0, shz_vec2_t edge1) SHZ_NOEXCEPT;

//! For each component i: returns 0.0f at/below edge0[i], 1.0f at/above edge1[i], smoothly varying in-between.
SHZ_INLINE shz_vec3_t shz_vec3_smoothstepv(shz_vec3_t vec, shz_vec3_t edge0, shz_vec3_t edge1) SHZ_NOEXCEPT;

//! For each component i: returns 0.0f at/below edge0[i], 1.0f at/above edge1[i], smoothly varying in-between.
SHZ_INLINE shz_vec4_t shz_vec4_smoothstepv(shz_vec4_t vec, shz_vec4_t edge0, shz_vec4_t edge1) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between. Accepts inverse edges.
SHZ_INLINE shz_vec2_t shz_vec2_smoothstep_safe(shz_vec2_t vec, float edge0, float edge1) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between. Accepts inverse edges.
SHZ_INLINE shz_vec3_t shz_vec3_smoothstep_safe(shz_vec3_t vec, float edge0, float edge1) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0, 1.0f at/above edge1, smoothly varying in-between. Accepts inverse edges.
SHZ_INLINE shz_vec4_t shz_vec4_smoothstep_safe(shz_vec4_t vec, float edge0, float edge1) SHZ_NOEXCEPT;

//! For each component: returns 0.0f at/below edge0[i], 1.0f at/above edge1[i], smoothly varying in-between. Accepts inverse edges.
SHZ_INLINE shz_vec2_t shz_vec2_smoothstepv_safe(shz_vec2_t vec, shz_vec2_t edge0, shz_vec2_t edge1) SHZ_NOEXCEPT;

//! For each component i: returns 0.0f at/below edge0[i], 1.0f at/above edge1[i], smoothly varying in-between. Accepts inverse edges.
SHZ_INLINE shz_vec3_t shz_vec3_smoothstepv_safe(shz_vec3_t vec, shz_vec3_t edge0, shz_vec3_t edge1) SHZ_NOEXCEPT;

//! For each component i: returns 0.0f at/below edge0[i], 1.0f at/above edge1[i], smoothly varying in-between. Accepts inverse edges.
SHZ_INLINE shz_vec4_t shz_vec4_smoothstepv_safe(shz_vec4_t vec, shz_vec4_t edge0, shz_vec4_t edge1) SHZ_NOEXCEPT;


//! @}

/*! \name   Arithmetic
    \brief  Routines for basic vector arithmetic operations.
    @{
*/

//! Returns a 2D vector whose components are the sums of the given vectors' components.
SHZ_INLINE shz_vec2_t shz_vec2_add(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are the sums of the given vectors' components.
SHZ_INLINE shz_vec3_t shz_vec3_add(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are the sums of the given vectors' components.
SHZ_INLINE shz_vec4_t shz_vec4_add(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose components are equal to the values of \p vec1 minus \p vec2.
SHZ_INLINE shz_vec2_t shz_vec2_sub(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose components are equal to the values of \p vec1 minus \p vec2.
SHZ_INLINE shz_vec3_t shz_vec3_sub(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose components are equal to the values of \p vec1 minus \p vec2.
SHZ_INLINE shz_vec4_t shz_vec4_sub(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose component values are those of \p vec1 times \p vec2.
SHZ_INLINE shz_vec2_t shz_vec2_mul(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose component values are those of \p vec1 times \p vec2.
SHZ_INLINE shz_vec3_t shz_vec3_mul(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose component values are those of \p vec1 times \p vec2.
SHZ_INLINE shz_vec4_t shz_vec4_mul(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose component values are those of \p vec1 divided by \p vec2.
SHZ_INLINE shz_vec2_t shz_vec2_div(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns a 3D vector whose component values are those of \p vec1 divided by \p vec2.
SHZ_INLINE shz_vec3_t shz_vec3_div(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns a 4D vector whose component values are those of \p vec1 divided by \p vec2.
SHZ_INLINE shz_vec4_t shz_vec4_div(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns a 2D vector whose component values are those of the given vector multiplied by a factor.
SHZ_INLINE shz_vec2_t shz_vec2_scale(shz_vec2_t vec, float factor) SHZ_NOEXCEPT;

//! Returns a 3D vector whose component values are those of the given vector multiplied by a factor.
SHZ_INLINE shz_vec3_t shz_vec3_scale(shz_vec3_t vec, float factor) SHZ_NOEXCEPT;

//! Returns a 4D vector whose component values are those of the given vector multiplied by a factor.
SHZ_INLINE shz_vec4_t shz_vec4_scale(shz_vec4_t vec, float factor) SHZ_NOEXCEPT;

//! @}

/*! \name  Magnitude
    \brief Math routines for vector length and normalization.
    @{
*/

//! Returns the squared magnitude of the given 2D vector.
SHZ_INLINE float shz_vec2_magnitude_sqr(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the squared magnitude of the given 4D vector.
SHZ_INLINE float shz_vec4_magnitude_sqr(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the squared magnitude of the given 3D vector.
SHZ_INLINE float shz_vec3_magnitude_sqr(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the magnitude of the given 2D vector.
SHZ_INLINE float shz_vec2_magnitude(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the magnitude of the given 3D vector.
SHZ_INLINE float shz_vec3_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the magnitude of the given 4D vector.
SHZ_INLINE float shz_vec4_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns the inverse magnitude of the given 2D vector.
SHZ_INLINE float shz_vec2_magnitude_inv(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the inverse magnitude of the given 3D vector.
SHZ_INLINE float shz_vec3_magnitude_inv(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the inverse magnitude of the given 4D vector.
SHZ_INLINE float shz_vec4_magnitude_inv(shz_vec4_t vec) SHZ_NOEXCEPT;

//! Returns a normalized unit vector from the given 2D vector.
SHZ_INLINE shz_vec2_t shz_vec2_normalize(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns a normalized unit vector from the given 3D vector.
SHZ_INLINE shz_vec3_t shz_vec3_normalize(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns a normalized unit vector from the given 4D vector.
SHZ_INLINE shz_vec4_t shz_vec4_normalize(shz_vec4_t vec) SHZ_NOEXCEPT;

/*! SAFELY returns a normalized unit vector from the given 2D vector.

    If the vector's magnitude is not `> 0.0f`, safely returns a zero vector.
*/
SHZ_INLINE shz_vec2_t shz_vec2_normalize_safe(shz_vec2_t vec) SHZ_NOEXCEPT;

/*! SAFELY returns a normalized unit vector from the given 3D vector.

    If the vector's magnitude is not `> 0.0f`, safely returns a zero vector.
*/
SHZ_INLINE shz_vec3_t shz_vec3_normalize_safe(shz_vec3_t vec) SHZ_NOEXCEPT;

/*! SAFELY returns a normalized unit vector from the given 4D vector.

    If the vector's magnitude is not `> 0.0f`, safely returns a zero vector.
*/
SHZ_INLINE shz_vec4_t shz_vec4_normalize_safe(shz_vec4_t vec) SHZ_NOEXCEPT;

//! @}

/*! \name  Binary Operations
    \brief Linear algebra operations performed with multiple vectors.
    @{
*/

//! Returns the dot product between the two given 2D vectors.
SHZ_INLINE float shz_vec2_dot(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the two dot products taken between the 2D vector \p l and 2D vectors \p r1 and \p r2.
SHZ_INLINE shz_vec2_t shz_vec2_dot2(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2) SHZ_NOEXCEPT;

//! Returns the three dot products taken between the 2D vector \p l and 2D vectors \p r1, \p r2, and \p r3.
SHZ_INLINE shz_vec3_t shz_vec2_dot3(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2, shz_vec2_t r3) SHZ_NOEXCEPT;

//! Returns the dot product between the two given 3D vectors.
SHZ_INLINE float shz_vec3_dot(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the two dot products taken between the 3D vector \p l and 3D vectors \p r1 and \p r2.
SHZ_INLINE shz_vec2_t shz_vec3_dot2(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2) SHZ_NOEXCEPT;

//! Returns the three dot products taken between the 3D vector \p l and 3D vectors \p r1, \p r2, and \p r3.
SHZ_INLINE shz_vec3_t shz_vec3_dot3(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT;

//! Returns the dot product between the two given 4D vectors.
SHZ_INLINE float shz_vec4_dot(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns the two dot products taken between the 4D vector \p l and 4D vectors \p r1 and \p r2.
SHZ_INLINE shz_vec2_t shz_vec4_dot2(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2) SHZ_NOEXCEPT;

//! Returns the three dot products taken between the 4D vector \p l and 4D vectors \p r1, \p r2, and \p r3.
SHZ_INLINE shz_vec3_t shz_vec4_dot3(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2, shz_vec4_t r3) SHZ_NOEXCEPT;

//! Returns the distance between the two given 2D vectors.
SHZ_INLINE float shz_vec2_distance(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the distance between the two given 3D vectors.
SHZ_INLINE float shz_vec3_distance(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the distance between the two given 4D vectors.
SHZ_INLINE float shz_vec4_distance(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns the squared-distance between the two given 2D vectors.
SHZ_INLINE float shz_vec2_distance_sqr(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the squared-distance between the two given 3D vectors.
SHZ_INLINE float shz_vec3_distance_sqr(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the squared-distance between the two given 4D vectors.
SHZ_INLINE float shz_vec4_distance_sqr(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT;

//! Returns the given 2D vector, translated towards the \p target by the given \p max_distance.
SHZ_INLINE shz_vec2_t shz_vec2_move(shz_vec2_t vec, shz_vec2_t target, float max_distance) SHZ_NOEXCEPT;

//! Returns the given 3D vector, translated towards the \p target by the given \p max_distance.
SHZ_INLINE shz_vec3_t shz_vec3_move(shz_vec3_t vec, shz_vec3_t target, float max_distance) SHZ_NOEXCEPT;

//! Returns the given 4D vector, translated towards the \p target by the given \p max_distance.
SHZ_INLINE shz_vec4_t shz_vec4_move(shz_vec4_t vec, shz_vec4_t target, float max_distance) SHZ_NOEXCEPT;

//! Returns a 2D vector that is linearly interpolated from \p a to \p b by the given `0.0f-1.0f` factor, \p t.
SHZ_INLINE shz_vec2_t shz_vec2_lerp(shz_vec2_t a, shz_vec2_t b, float t) SHZ_NOEXCEPT;

//! Returns a 3D vector that is linearly interpolated from \p a to \p b by the given `0.0f-1.0f` factor, \p t.
SHZ_INLINE shz_vec3_t shz_vec3_lerp(shz_vec3_t a, shz_vec3_t b, float t) SHZ_NOEXCEPT;

//! Returns a 4D vector that is linearly interpolated from \p a to \p b by the given `0.0f-1.0f` factor, \p t.
SHZ_INLINE shz_vec4_t shz_vec4_lerp(shz_vec4_t a, shz_vec4_t b, float t) SHZ_NOEXCEPT;

//! Reflects the given 2D \p incidence vector against a surface with the given \p normal, returning the result.
SHZ_INLINE shz_vec2_t shz_vec2_reflect(shz_vec2_t incidence, shz_vec2_t normal) SHZ_NOEXCEPT;

//! Reflects the given 3D \p incidence vector against a surface with the given \p normal, returning the result.
SHZ_INLINE shz_vec3_t shz_vec3_reflect(shz_vec3_t incidence, shz_vec3_t normal) SHZ_NOEXCEPT;

//! Reflects the given 4D \p incidence vector against a surface with the given \p normal, returning the result.
SHZ_INLINE shz_vec4_t shz_vec4_reflect(shz_vec4_t incidence, shz_vec4_t normal) SHZ_NOEXCEPT;

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
SHZ_INLINE float shz_vec2_cross(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the vector produced by taking the cross-product of the two given 3D vectors.
SHZ_INLINE shz_vec3_t shz_vec3_cross(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the resulting vector from projecting the given 2D vector along the given (unit) axis
SHZ_INLINE shz_vec2_t shz_vec2_project(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT;

//! Returns the resulting vector from projecting the given 3D vector along the given (unit) axis
SHZ_INLINE shz_vec3_t shz_vec3_project(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT;

//! Returns the resulting vector from projecting the given 4D vector along the given (unit) axis
SHZ_INLINE shz_vec4_t shz_vec4_project(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT;

/*! Returns the resulting vector from projecting the given 2D vector along the given (unit) axis

    \note
    This routine should safely return the zero vector when \p vec has a magnitude of 0.0f.
*/
SHZ_INLINE shz_vec2_t shz_vec2_project_safe(shz_vec2_t vec, shz_vec2_t onto) SHZ_NOEXCEPT;

/*! Returns the resulting vector from projecting the given 3D vector along the given (unit) axis

    \note
    This routine should safely return the zero vector when \p vec has a magnitude of 0.0f.
*/
SHZ_INLINE shz_vec3_t shz_vec3_project_safe(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT;

/*! Returns the resulting vector from projecting the given 4D vector along the given (unit) axis

    \note
    This routine should safely return the zero vector when \p vec has a magnitude of 0.0f.
*/
SHZ_INLINE shz_vec4_t shz_vec4_project_safe(shz_vec4_t vec, shz_vec4_t onto) SHZ_NOEXCEPT;

//! Returns the rejection of the given vector, \p vec, onto another vector, \p onto.
SHZ_INLINE shz_vec3_t shz_vec3_reject(shz_vec3_t vec, shz_vec3_t onto) SHZ_NOEXCEPT;

//! @}

/*! \name  Miscellaneous
    \brief Other specialized vector routines.
    @{
*/

//! Returns the 3D vector "triple product" between vector's \p a, \p b, and \p c.
SHZ_INLINE float shz_vec3_triple(shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT;

//! Returns a vector which is perpendicular to the given vector.
SHZ_INLINE shz_vec3_t shz_vec3_perp(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Computes barycentric coordinates `<u, v, w>` for point p, within the plane of the triangle with vertices \p a, \p b, and \p c.
SHZ_INLINE shz_vec3_t shz_vec3_barycenter(shz_vec3_t p, shz_vec3_t a, shz_vec3_t b, shz_vec3_t c) SHZ_NOEXCEPT;

//! Returns 2 3D vectors which are normalized and orthogonal to the two input vectors.
SHZ_INLINE void shz_vec3_orthonormalize(shz_vec3_t in1, shz_vec3_t in2, shz_vec3_t* out1, shz_vec3_t* out2) SHZ_NOEXCEPT;

//! Calculates the cubic hermite interpolation between two vectors and their tangents.
SHZ_INLINE shz_vec3_t shz_vec3_cubic_hermite(shz_vec3_t vec, shz_vec3_t tangent1, shz_vec3_t vec2, shz_vec3_t tangent2, float amounht) SHZ_NOEXCEPT;

//! @}

/*! \name  Angles
	\brief Routines for working with vectors and angles.
	@{
*/

//! Returns the angle formed between the given 2D vectors in radians.
SHZ_INLINE float shz_vec2_angle_between(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT;

//! Returns the angle formed between the given 3D vectors in radians.
SHZ_INLINE float shz_vec3_angle_between(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT;

//! Returns the angle formed between the positive X axis and the given 2D vector, in radians.
SHZ_INLINE float shz_vec2_angle(shz_vec2_t vec) SHZ_NOEXCEPT;

//! Returns the angles formed between the positive X axis and the given 3D vector, in radians.
SHZ_INLINE shz_vec3_t shz_vec3_angles(shz_vec3_t vec) SHZ_NOEXCEPT;

//! Returns the 2D unit vector representing a rotation from the positive X axis.
SHZ_INLINE shz_vec2_t shz_vec2_from_sincos(shz_sincos_t sincos) SHZ_NOEXCEPT;

//! Returns the 3D unit vector representing the given rotation angles relative to the positive X axis.
SHZ_INLINE shz_vec3_t shz_vec3_from_sincos(shz_sincos_t azimuth, shz_sincos_t elevation) SHZ_NOEXCEPT;

//! Returns the 2D unit vector representing a rotation from the positive X axis in radians.
SHZ_INLINE shz_vec2_t shz_vec2_from_angle(float radians) SHZ_NOEXCEPT;

//! Returns the 3D unit vector representing the given rotation angles relative to the positive X axis in radians.
SHZ_INLINE shz_vec3_t shz_vec3_from_angles(float azimuth, float elevation) SHZ_NOEXCEPT;

//! Returns the 2D unit vector representing a rotation from the positive X axis in degrees.
SHZ_INLINE shz_vec2_t shz_vec2_from_angle_deg(float degrees) SHZ_NOEXCEPT;

//! Returns the 3D unit vector representing the given rotation angles relative to the positive X axis in degrees.
SHZ_INLINE shz_vec3_t shz_vec3_from_angles_deg(float azimuth, float elevation) SHZ_NOEXCEPT;

//! Rotates the given 2D vector about the Z axis by the given angle in radians.
SHZ_INLINE shz_vec2_t shz_vec2_rotate(shz_vec2_t vec, float radians) SHZ_NOEXCEPT;

//! @}

/*! \name  Extending
    \brief Routines for extending vectors into other dimensions.
    @{
 */

//! Extends a 2D vector to 3D, using \p z as the value of the Z component.
SHZ_INLINE shz_vec3_t shz_vec2_vec3(shz_vec2_t vec, float z) SHZ_NOEXCEPT;

//! Extends a 2D vector to 4D, using \p z and \p w as the values of the Z and W components.
SHZ_INLINE shz_vec4_t shz_vec2_vec4(shz_vec2_t vec, float z, float w) SHZ_NOEXCEPT;

//! Extends a 3D vector to 4D, using \p w as the value of the W component.
SHZ_INLINE shz_vec4_t shz_vec3_vec4(shz_vec3_t vec, float w) SHZ_NOEXCEPT;

//! @}

/*! \name  Swizzling
    \brief Routines for swizzling the order of a vector's components.
    @{
 */

 //! Returns a 2D vector whose elements are equal to the source vector's values at the given indices.
 SHZ_INLINE shz_vec2_t shz_vec2_swizzle(shz_vec2_t vec, unsigned x_idx, unsigned y_idx) SHZ_NOEXCEPT;

 //! Returns a 3D vector whose elements are equal to the source vector's values at the given indices.
 SHZ_INLINE shz_vec3_t shz_vec3_swizzle(shz_vec3_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx) SHZ_NOEXCEPT;

 //! Returns a new 2D vector whose elements are equal to the source vector's values at the given indices.
 SHZ_INLINE shz_vec4_t shz_vec4_swizzle(shz_vec4_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx, unsigned w_idx) SHZ_NOEXCEPT;

 //! @}

SHZ_DECLS_END

/*! \name Adapters
    \brief Macros for converting between SH4ZAM and other compatible formats.
    @{
*/

//! Dereferences the given pointer to a sequence of 2 floats as a shz_vec2_t.
#define shz_vec2_deref(ptr) (*((SHZ_ALIASING shz_vec2_t*)(ptr)))

//! Dereferences the given pointer to a sequence of 3 floats as a shz_vec3_t.
#define shz_vec3_deref(ptr) (*((SHZ_ALIASING shz_vec3_t*)(ptr)))

//! Dereferences the given pointer to a sequence of 4 floats as a shz_vec4_t.
#define shz_vec4_deref(ptr) (*((SHZ_ALIASING shz_vec4_t*)(ptr)))

//! Converts the given \p value or expression to the equivalent 2D SH4ZAM vector value.
#define shz_vec2_from(value) SHZ_CONVERT(shz_vec2_t, value)

//! Converts the given \p value or expression to the equivalent 3D SH4ZAM vector value.
#define shz_vec3_from(value) SHZ_CONVERT(shz_vec3_t, value)

//! Converts the given \p value or expression to the equivalent 4D SH4ZAM vector value.
#define shz_vec4_from(value) SHZ_CONVERT(shz_vec4_t, value)

//! Converts the given 2D \p vector into a value of the given \p type.
#define shz_vec2_to(type, vector) SHZ_CONVERT(type, vector)

//! Converts the given 3D \p vector into a value of the given \p type.
#define shz_vec3_to(type, vector) SHZ_CONVERT(type, vector)

//! Converts the given 4D \p vector into a value of the given \p type.
#define shz_vec4_to(type, vector) SHZ_CONVERT(type, vector)

//! @}

/*! \name  Type-Generic Routines
    \brief Generalized vector routines for C and C++.
    @{
*/

#ifndef __cplusplus

    //! C type-generic vector absolute value.
#   define shz_vec_abs(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_abs, \
                 shz_vec3_t: shz_vec3_abs, \
                 shz_vec4_t: shz_vec4_abs)(vec)

    //! C type-generic vector negation.
#   define shz_vec_neg(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_neg, \
                 shz_vec3_t: shz_vec3_neg, \
                 shz_vec4_t: shz_vec4_neg)(vec)

    //! C type-generic vector inversion.
#   define shz_vec_inv(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_inv, \
                 shz_vec3_t: shz_vec3_inv, \
                 shz_vec4_t: shz_vec4_inv)(vec)

    //! C type-generic vector maximum value.
#   define shz_vec_max(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_max, \
                 shz_vec3_t: shz_vec3_max, \
                 shz_vec4_t: shz_vec4_max)(vec)

    //! C type-generic vector minimum value.
#   define shz_vec_min(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_min, \
                 shz_vec3_t: shz_vec3_min, \
                 shz_vec4_t: shz_vec4_min)(vec)

    //! C type-generic vector minimum value.
#   define shz_vec_clamp(vec, min, max) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_clamp, \
                 shz_vec3_t: shz_vec3_clamp, \
                 shz_vec4_t: shz_vec4_clamp)(vec, min, max)

    //! C type-generic vector equals.
#   define shz_vec_equal(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_equal, \
                 shz_vec3_t: shz_vec3_equal, \
                 shz_vec4_t: shz_vec4_equal)(vec1, vec2)

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
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_mul, \
                 shz_vec3_t: shz_vec3_mul, \
                 shz_vec4_t: shz_vec4_mul)(vec1, vec2)

    //! C type-generic vector division.
#   define shz_vec_div(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_div, \
                 shz_vec3_t: shz_vec3_div, \
                 shz_vec4_t: shz_vec4_div)(vec1, vec2)

    //! C type-generic vector scaling.
#   define shz_vec_scale(vec, factor) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_scale, \
                 shz_vec3_t: shz_vec3_scale, \
                 shz_vec4_t: shz_vec4_scale)(vec, factor)

    //! C type-generic vector dot product.
#   define shz_vec_dot(vec1, vec2) \
        _Generic((vec1), \
                 shz_vec2_t: shz_vec2_dot, \
                 shz_vec3_t: shz_vec3_dot, \
                 shz_vec4_t: shz_vec4_dot)(vec1, vec2)

    //! C type-generic vector chained double dot product.
#   define shz_vec_dot2(l, r1, r2) \
        _Generic((l), \
                 shz_vec2_t: shz_vec2_dot2, \
                 shz_vec3_t: shz_vec3_dot2, \
                 shz_vec4_t: shz_vec4_dot2)(l, r1, r2)

    //! C type-generic vector chained triple dot product.
#   define shz_vec_dot3(l, r1, r2, r3) \
        _Generic((l), \
                 shz_vec2_t: shz_vec2_dot3, \
                 shz_vec3_t: shz_vec3_dot3, \
                 shz_vec4_t: shz_vec4_dot3)(l, r1, r2, r3)

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

    //! C type-generic vector move.
#   define shz_vec_move(vec, target, maxdist) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_move, \
                 shz_vec3_t: shz_vec3_move, \
                 shz_vec4_t: shz_vec4_move)(vec, target, maxdist)

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

    //! C type-generic vector swizzling.
#   define shz_vec_swizzle(vec, ...) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_swizzle, \
                 shz_vec3_t: shz_vec3_swizzle, \
                 shz_vec4_t: shz_vec4_swizzle)(vec, __VA_ARGS__)

    //! C type-generic component-wise floor.
#   define shz_vec_floor(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_floor, \
                 shz_vec3_t: shz_vec3_floor, \
                 shz_vec4_t: shz_vec4_floor)(vec)

    //! C type-generic component-wise ceil.
#   define shz_vec_ceil(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_ceil, \
                 shz_vec3_t: shz_vec3_ceil, \
                 shz_vec4_t: shz_vec4_ceil)(vec)

    //! C type-generic component-wise round.
#   define shz_vec_round(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_round, \
                 shz_vec3_t: shz_vec3_round, \
                 shz_vec4_t: shz_vec4_round)(vec)

    //! C type-generic component-wise fract.
#   define shz_vec_fract(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_fract, \
                 shz_vec3_t: shz_vec3_fract, \
                 shz_vec4_t: shz_vec4_fract)(vec)

    //! C type-generic component-wise sign.
#   define shz_vec_sign(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_sign, \
                 shz_vec3_t: shz_vec3_sign, \
                 shz_vec4_t: shz_vec4_sign)(vec)

    //! C type-generic component-wise saturate.
#   define shz_vec_saturate(vec) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_saturate, \
                 shz_vec3_t: shz_vec3_saturate, \
                 shz_vec4_t: shz_vec4_saturate)(vec)

    //! C type-generic pairwise minimum.
#   define shz_vec_minv(a, b) \
        _Generic((a), \
                 shz_vec2_t: shz_vec2_minv, \
                 shz_vec3_t: shz_vec3_minv, \
                 shz_vec4_t: shz_vec4_minv)(a, b)

    //! C type-generic pairwise maximum.
#   define shz_vec_maxv(a, b) \
        _Generic((a), \
                 shz_vec2_t: shz_vec2_maxv, \
                 shz_vec3_t: shz_vec3_maxv, \
                 shz_vec4_t: shz_vec4_maxv)(a, b)

    //! C type-generic component-wise step
#   define shz_vec_stepv(vec, edge) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_stepv, \
                 shz_vec3_t: shz_vec3_stepv, \
                 shz_vec4_t: shz_vec4_stepv)(vec, edge)

    //! C type-generic step
#   define shz_vec_step(vec, edge) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_step, \
                 shz_vec3_t: shz_vec3_step, \
                 shz_vec4_t: shz_vec4_step)(vec, edge)

    //! C type-generic component-wise smoothstep
#   define shz_vec_smoothstepv(vec, edge0, edge1) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_smoothstepv, \
                 shz_vec3_t: shz_vec3_smoothstepv, \
                 shz_vec4_t: shz_vec4_smoothstepv)(vec, edge0, edge1)

    //! C type-generic smoothstep
#   define shz_vec_smoothstep(vec, edge0, edge1) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_smoothstep, \
                 shz_vec3_t: shz_vec3_smoothstep, \
                 shz_vec4_t: shz_vec4_smoothstep)(vec, edge0, edge1)

    //! C type-generic component-wise smoothstep_safe
#   define shz_vec_smoothstepv_safe(vec, edge0, edge1) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_smoothstepv_safe, \
                 shz_vec3_t: shz_vec3_smoothstepv_safe, \
                 shz_vec4_t: shz_vec4_smoothstepv_safe)(vec, edge0, edge1)

    //! C type-generic smoothstep_safe
#   define shz_vec_smoothstep_safe(vec, edge0, edge1) \
        _Generic((vec), \
                 shz_vec2_t: shz_vec2_smoothstep_safe, \
                 shz_vec3_t: shz_vec3_smoothstep_safe, \
                 shz_vec4_t: shz_vec4_smoothstep_safe)(vec, edge0, edge1)

#else // C++ generics (because it's too dumb to support _Generic()).

    //! C++ type-generic vector absolute value.
    SHZ_INLINE shz_vec2_t shz_vec_abs(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_abs(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_abs(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_abs(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_abs(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_abs(vec); }

    //! C++ type-generic vector negation.
    SHZ_INLINE shz_vec2_t shz_vec_neg(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_neg(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_neg(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_neg(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_neg(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_neg(vec); }

    //! C++ type-generic vector inversion.
    SHZ_INLINE shz_vec2_t shz_vec_inv(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_inv(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_inv(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_inv(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_inv(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_inv(vec); }

    //! C++ type-generic vector maximum value.
    SHZ_INLINE float shz_vec_max(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_max(vec); }
    SHZ_INLINE float shz_vec_max(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_max(vec); }
    SHZ_INLINE float shz_vec_max(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_max(vec); }

    //! C++ type-generic vector minimum value.
    SHZ_INLINE float shz_vec_min(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_min(vec); }
    SHZ_INLINE float shz_vec_min(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_min(vec); }
    SHZ_INLINE float shz_vec_min(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_min(vec); }

    //! C++ type-generic vector clamp.
    SHZ_INLINE shz_vec2_t shz_vec_clamp(shz_vec2_t vec, float min, float max) SHZ_NOEXCEPT { return shz_vec2_clamp(vec, min, max); }
    SHZ_INLINE shz_vec3_t shz_vec_clamp(shz_vec3_t vec, float min, float max) SHZ_NOEXCEPT { return shz_vec3_clamp(vec, min, max); }
    SHZ_INLINE shz_vec4_t shz_vec_clamp(shz_vec4_t vec, float min, float max) SHZ_NOEXCEPT { return shz_vec4_clamp(vec, min, max); }

    //! C++ type-generic vector equality.
    SHZ_INLINE bool shz_vec_equal(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_equal(vec1, vec2); }
    SHZ_INLINE bool shz_vec_equal(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_equal(vec1, vec2); }
    SHZ_INLINE bool shz_vec_equal(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_equal(vec1, vec2); }

    //! C++ type-generic vector addition.
    SHZ_INLINE shz_vec2_t shz_vec_add(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_add(vec1, vec2); }
    SHZ_INLINE shz_vec3_t shz_vec_add(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_add(vec1, vec2); }
    SHZ_INLINE shz_vec4_t shz_vec_add(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_add(vec1, vec2); }

    //! C++ type-generic vector subtraction.
    SHZ_INLINE shz_vec2_t shz_vec_sub(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_sub(vec1, vec2); }
    SHZ_INLINE shz_vec3_t shz_vec_sub(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_sub(vec1, vec2); }
    SHZ_INLINE shz_vec4_t shz_vec_sub(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_sub(vec1, vec2); }

    //! C++ type-generic component-wise multiplication.
    SHZ_INLINE shz_vec2_t shz_vec_mul(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_mul(vec1, vec2); }
    SHZ_INLINE shz_vec3_t shz_vec_mul(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_mul(vec1, vec2); }
    SHZ_INLINE shz_vec4_t shz_vec_mul(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_mul(vec1, vec2); }

    //! C++ type-generic component-wise division.
    SHZ_INLINE shz_vec2_t shz_vec_div(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_div(vec1, vec2); }
    SHZ_INLINE shz_vec3_t shz_vec_div(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_div(vec1, vec2); }
    SHZ_INLINE shz_vec4_t shz_vec_div(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_div(vec1, vec2); }

    //! C++ type-generic uniform scaling.
    SHZ_INLINE shz_vec2_t shz_vec_scale(shz_vec2_t vec, float factor) SHZ_NOEXCEPT { return shz_vec2_scale(vec, factor); }
    SHZ_INLINE shz_vec3_t shz_vec_scale(shz_vec3_t vec, float factor) SHZ_NOEXCEPT { return shz_vec3_scale(vec, factor); }
    SHZ_INLINE shz_vec4_t shz_vec_scale(shz_vec4_t vec, float factor) SHZ_NOEXCEPT { return shz_vec4_scale(vec, factor); }

    //! C++ type-generic dot product.
    SHZ_INLINE float shz_vec_dot(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_dot(vec1, vec2); }
    SHZ_INLINE float shz_vec_dot(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_dot(vec1, vec2); }
    SHZ_INLINE float shz_vec_dot(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_dot(vec1, vec2); }

    //! C++ type-generic dual dot product.
    SHZ_INLINE shz_vec2_t shz_vec_dot2(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2) SHZ_NOEXCEPT { return shz_vec2_dot2(l, r1, r2); }
    SHZ_INLINE shz_vec2_t shz_vec_dot2(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2) SHZ_NOEXCEPT { return shz_vec3_dot2(l, r1, r2); }
    SHZ_INLINE shz_vec2_t shz_vec_dot2(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2) SHZ_NOEXCEPT { return shz_vec4_dot2(l, r1, r2); }

    //! C++ type-generic triple dot product.
    SHZ_INLINE shz_vec3_t shz_vec_dot3(shz_vec2_t l, shz_vec2_t r1, shz_vec2_t r2, shz_vec2_t r3) SHZ_NOEXCEPT { return shz_vec2_dot3(l, r1, r2, r3); }
    SHZ_INLINE shz_vec3_t shz_vec_dot3(shz_vec3_t l, shz_vec3_t r1, shz_vec3_t r2, shz_vec3_t r3) SHZ_NOEXCEPT { return shz_vec3_dot3(l, r1, r2, r3); }
    SHZ_INLINE shz_vec3_t shz_vec_dot3(shz_vec4_t l, shz_vec4_t r1, shz_vec4_t r2, shz_vec4_t r3) SHZ_NOEXCEPT { return shz_vec4_dot3(l, r1, r2, r3); }

    //! C++ type-generic squared magnitude.
    SHZ_INLINE float shz_vec_magnitude_sqr(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_magnitude_sqr(vec); }
    SHZ_INLINE float shz_vec_magnitude_sqr(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_magnitude_sqr(vec); }
    SHZ_INLINE float shz_vec_magnitude_sqr(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_magnitude_sqr(vec); }

    //! C++ type-generic magnitude.
    SHZ_INLINE float shz_vec_magnitude(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_magnitude(vec); }
    SHZ_INLINE float shz_vec_magnitude(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_magnitude(vec); }
    SHZ_INLINE float shz_vec_magnitude(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_magnitude(vec); }

    //! C++ type-generic inverse magnitude.
    SHZ_INLINE float shz_vec_magnitude_inv(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_magnitude_inv(vec); }
    SHZ_INLINE float shz_vec_magnitude_inv(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_magnitude_inv(vec); }
    SHZ_INLINE float shz_vec_magnitude_inv(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_magnitude_inv(vec); }

    //! C++ type-generic vector normalization.
    SHZ_INLINE shz_vec2_t shz_vec_normalize(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_normalize(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_normalize(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_normalize(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_normalize(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_normalize(vec); }

    //! C++ type-generic safe vector normalization, avoids divide-by-zero.
    SHZ_INLINE shz_vec2_t shz_vec_normalize_safe(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_normalize_safe(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_normalize_safe(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_normalize_safe(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_normalize_safe(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_normalize_safe(vec); }

    //! C++ type-generic distance between two points.
    SHZ_INLINE float shz_vec_distance(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_distance(vec1, vec2); }
    SHZ_INLINE float shz_vec_distance(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_distance(vec1, vec2); }
    SHZ_INLINE float shz_vec_distance(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_distance(vec1, vec2); }

    //! C++ type-generic squared distance between two points.
    SHZ_INLINE float shz_vec_distance_sqr(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_distance_sqr(vec1, vec2); }
    SHZ_INLINE float shz_vec_distance_sqr(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_distance_sqr(vec1, vec2); }
    SHZ_INLINE float shz_vec_distance_sqr(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_distance_sqr(vec1, vec2); }

    //! C++ type-generic moving of one vector towards another.
    SHZ_INLINE shz_vec2_t shz_vec_move(shz_vec2_t vec, shz_vec2_t target, float maxdist) SHZ_NOEXCEPT { return shz_vec2_move(vec, target, maxdist); }
    SHZ_INLINE shz_vec3_t shz_vec_move(shz_vec3_t vec, shz_vec3_t target, float maxdist) SHZ_NOEXCEPT { return shz_vec3_move(vec, target, maxdist); }
    SHZ_INLINE shz_vec4_t shz_vec_move(shz_vec4_t vec, shz_vec4_t target, float maxdist) SHZ_NOEXCEPT { return shz_vec4_move(vec, target, maxdist); }

    //! C++ type-generic linear interpolation between two vectors.
    SHZ_INLINE shz_vec2_t shz_vec_lerp(shz_vec2_t vec1, shz_vec2_t vec2, float t) SHZ_NOEXCEPT { return shz_vec2_lerp(vec1, vec2, t); }
    SHZ_INLINE shz_vec3_t shz_vec_lerp(shz_vec3_t vec1, shz_vec3_t vec2, float t) SHZ_NOEXCEPT { return shz_vec3_lerp(vec1, vec2, t); }
    SHZ_INLINE shz_vec4_t shz_vec_lerp(shz_vec4_t vec1, shz_vec4_t vec2, float t) SHZ_NOEXCEPT { return shz_vec4_lerp(vec1, vec2, t); }

    //! C++ type-generic reflection of a vector over a surface normal.
    SHZ_INLINE shz_vec2_t shz_vec_reflect(shz_vec2_t incidence, shz_vec2_t normal) SHZ_NOEXCEPT { return shz_vec2_reflect(incidence, normal); }
    SHZ_INLINE shz_vec3_t shz_vec_reflect(shz_vec3_t incidence, shz_vec3_t normal) SHZ_NOEXCEPT { return shz_vec3_reflect(incidence, normal); }
    SHZ_INLINE shz_vec4_t shz_vec_reflect(shz_vec4_t incidence, shz_vec4_t normal) SHZ_NOEXCEPT { return shz_vec4_reflect(incidence, normal); }

    //! C++ type-generic refraction of a vector over a surface normal.
    SHZ_INLINE shz_vec2_t shz_vec_refract(shz_vec2_t incidence, shz_vec2_t normal, float eta) SHZ_NOEXCEPT { return shz_vec2_refract(incidence, normal, eta); }
    SHZ_INLINE shz_vec3_t shz_vec_refract(shz_vec3_t incidence, shz_vec3_t normal, float eta) SHZ_NOEXCEPT { return shz_vec3_refract(incidence, normal, eta); }
    SHZ_INLINE shz_vec4_t shz_vec_refract(shz_vec4_t incidence, shz_vec4_t normal, float eta) SHZ_NOEXCEPT { return shz_vec4_refract(incidence, normal, eta); }

    //! C++ type-generic cross product (2D returns scalar, 3D returns vector).
    SHZ_INLINE float      shz_vec_cross(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_cross(vec1, vec2); }
    SHZ_INLINE shz_vec3_t shz_vec_cross(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_cross(vec1, vec2); }

    //! C++ type-generic projection of the first vector onto the second.
    SHZ_INLINE shz_vec2_t shz_vec_project(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_project(vec1, vec2); }
    SHZ_INLINE shz_vec3_t shz_vec_project(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_project(vec1, vec2); }
    SHZ_INLINE shz_vec4_t shz_vec_project(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_project(vec1, vec2); }

    //! C++ type-generic safe projection, avoiding division-by-zero.
    SHZ_INLINE shz_vec2_t shz_vec_project_safe(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_project_safe(vec1, vec2); }
    SHZ_INLINE shz_vec3_t shz_vec_project_safe(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_project_safe(vec1, vec2); }
    SHZ_INLINE shz_vec4_t shz_vec_project_safe(shz_vec4_t vec1, shz_vec4_t vec2) SHZ_NOEXCEPT { return shz_vec4_project_safe(vec1, vec2); }

    //! C++ type-generic angles from the +X axis (2D returns float, 3D returns vec2).
    SHZ_INLINE float      shz_vec_angles(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_angle(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_angles(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_angles(vec); }

    //! C++ type-generic angle between two vectors.
    SHZ_INLINE float shz_vec_angle_between(shz_vec2_t vec1, shz_vec2_t vec2) SHZ_NOEXCEPT { return shz_vec2_angle_between(vec1, vec2); }
    SHZ_INLINE float shz_vec_angle_between(shz_vec3_t vec1, shz_vec3_t vec2) SHZ_NOEXCEPT { return shz_vec3_angle_between(vec1, vec2); }

    //! C++ type-generic swizzle.
    SHZ_INLINE shz_vec2_t shz_vec_swizzle(shz_vec2_t vec, unsigned x_idx, unsigned y_idx) SHZ_NOEXCEPT {
        return shz_vec2_swizzle(vec, x_idx, y_idx);
    }
    SHZ_INLINE shz_vec3_t shz_vec_swizzle(shz_vec3_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx) SHZ_NOEXCEPT {
        return shz_vec3_swizzle(vec, x_idx, y_idx, z_idx);
    }
    SHZ_INLINE shz_vec4_t shz_vec_swizzle(shz_vec4_t vec, unsigned x_idx, unsigned y_idx, unsigned z_idx, unsigned w_idx) SHZ_NOEXCEPT {
        return shz_vec4_swizzle(vec, x_idx, y_idx, z_idx, w_idx);
    }

    //! C++ type-generic component-wise floor.
    SHZ_INLINE shz_vec2_t shz_vec_floor(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_floor(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_floor(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_floor(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_floor(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_floor(vec); }

    //! C++ type-generic component-wise ceil.
    SHZ_INLINE shz_vec2_t shz_vec_ceil(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_ceil(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_ceil(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_ceil(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_ceil(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_ceil(vec); }

    //! C++ type-generic component-wise round.
    SHZ_INLINE shz_vec2_t shz_vec_round(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_round(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_round(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_round(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_round(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_round(vec); }

    //! C++ type-generic component-wise fract.
    SHZ_INLINE shz_vec2_t shz_vec_fract(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_fract(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_fract(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_fract(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_fract(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_fract(vec); }

    //! C++ type-generic component-wise sign.
    SHZ_INLINE shz_vec2_t shz_vec_sign(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_sign(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_sign(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_sign(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_sign(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_sign(vec); }

    //! C++ type-generic component-wise saturate.
    SHZ_INLINE shz_vec2_t shz_vec_saturate(shz_vec2_t vec) SHZ_NOEXCEPT { return shz_vec2_saturate(vec); }
    SHZ_INLINE shz_vec3_t shz_vec_saturate(shz_vec3_t vec) SHZ_NOEXCEPT { return shz_vec3_saturate(vec); }
    SHZ_INLINE shz_vec4_t shz_vec_saturate(shz_vec4_t vec) SHZ_NOEXCEPT { return shz_vec4_saturate(vec); }

    //! C++ type-generic component-wise minimum.
    SHZ_INLINE shz_vec2_t shz_vec_minv(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT { return shz_vec2_minv(a, b); }
    SHZ_INLINE shz_vec3_t shz_vec_minv(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT { return shz_vec3_minv(a, b); }
    SHZ_INLINE shz_vec4_t shz_vec_minv(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT { return shz_vec4_minv(a, b); }

    //! C++ type-generic component-wise maximum.
    SHZ_INLINE shz_vec2_t shz_vec_maxv(shz_vec2_t a, shz_vec2_t b) SHZ_NOEXCEPT { return shz_vec2_maxv(a, b); }
    SHZ_INLINE shz_vec3_t shz_vec_maxv(shz_vec3_t a, shz_vec3_t b) SHZ_NOEXCEPT { return shz_vec3_maxv(a, b); }
    SHZ_INLINE shz_vec4_t shz_vec_maxv(shz_vec4_t a, shz_vec4_t b) SHZ_NOEXCEPT { return shz_vec4_maxv(a, b); }

    //! C++ type-generic step: 0 per component if vec[i] < edge, else 1.
    SHZ_INLINE shz_vec2_t shz_vec_step(shz_vec2_t vec, float       edge) SHZ_NOEXCEPT { return shz_vec2_step (vec, edge); }
    SHZ_INLINE shz_vec2_t shz_vec_step(shz_vec2_t vec, shz_vec2_t  edge) SHZ_NOEXCEPT { return shz_vec2_stepv(vec, edge); }
    SHZ_INLINE shz_vec3_t shz_vec_step(shz_vec3_t vec, float       edge) SHZ_NOEXCEPT { return shz_vec3_step (vec, edge); }
    SHZ_INLINE shz_vec3_t shz_vec_step(shz_vec3_t vec, shz_vec3_t  edge) SHZ_NOEXCEPT { return shz_vec3_stepv(vec, edge); }
    SHZ_INLINE shz_vec4_t shz_vec_step(shz_vec4_t vec, float       edge) SHZ_NOEXCEPT { return shz_vec4_step (vec, edge); }
    SHZ_INLINE shz_vec4_t shz_vec_step(shz_vec4_t vec, shz_vec4_t  edge) SHZ_NOEXCEPT { return shz_vec4_stepv(vec, edge); }

    //! C++ type-generic smoothstep: 0 at/below edge0, 1 at/above edge1, smooth in-between. Undefined for edge0 > edge1.
    SHZ_INLINE shz_vec2_t shz_vec_smoothstep(shz_vec2_t vec, float       edge0, float       edge1) SHZ_NOEXCEPT { return shz_vec2_smoothstep (vec, edge0, edge1); }
    SHZ_INLINE shz_vec2_t shz_vec_smoothstep(shz_vec2_t vec, shz_vec2_t  edge0, shz_vec2_t  edge1) SHZ_NOEXCEPT { return shz_vec2_smoothstepv(vec, edge0, edge1); }
    SHZ_INLINE shz_vec3_t shz_vec_smoothstep(shz_vec3_t vec, float       edge0, float       edge1) SHZ_NOEXCEPT { return shz_vec3_smoothstep (vec, edge0, edge1); }
    SHZ_INLINE shz_vec3_t shz_vec_smoothstep(shz_vec3_t vec, shz_vec3_t  edge0, shz_vec3_t  edge1) SHZ_NOEXCEPT { return shz_vec3_smoothstepv(vec, edge0, edge1); }
    SHZ_INLINE shz_vec4_t shz_vec_smoothstep(shz_vec4_t vec, float       edge0, float       edge1) SHZ_NOEXCEPT { return shz_vec4_smoothstep (vec, edge0, edge1); }
    SHZ_INLINE shz_vec4_t shz_vec_smoothstep(shz_vec4_t vec, shz_vec4_t  edge0, shz_vec4_t  edge1) SHZ_NOEXCEPT { return shz_vec4_smoothstepv(vec, edge0, edge1); }

    //! C++ type-generic smoothstep with safe edge ordering (accepts edge0 > edge1).
    SHZ_INLINE shz_vec2_t shz_vec_smoothstep_safe(shz_vec2_t vec, float       edge0, float       edge1) SHZ_NOEXCEPT { return shz_vec2_smoothstep_safe (vec, edge0, edge1); }
    SHZ_INLINE shz_vec2_t shz_vec_smoothstep_safe(shz_vec2_t vec, shz_vec2_t  edge0, shz_vec2_t  edge1) SHZ_NOEXCEPT { return shz_vec2_smoothstepv_safe(vec, edge0, edge1); }
    SHZ_INLINE shz_vec3_t shz_vec_smoothstep_safe(shz_vec3_t vec, float       edge0, float       edge1) SHZ_NOEXCEPT { return shz_vec3_smoothstep_safe (vec, edge0, edge1); }
    SHZ_INLINE shz_vec3_t shz_vec_smoothstep_safe(shz_vec3_t vec, shz_vec3_t  edge0, shz_vec3_t  edge1) SHZ_NOEXCEPT { return shz_vec3_smoothstepv_safe(vec, edge0, edge1); }
    SHZ_INLINE shz_vec4_t shz_vec_smoothstep_safe(shz_vec4_t vec, float       edge0, float       edge1) SHZ_NOEXCEPT { return shz_vec4_smoothstep_safe (vec, edge0, edge1); }
    SHZ_INLINE shz_vec4_t shz_vec_smoothstep_safe(shz_vec4_t vec, shz_vec4_t  edge0, shz_vec4_t  edge1) SHZ_NOEXCEPT { return shz_vec4_smoothstepv_safe(vec, edge0, edge1); }

#endif

#include "inline/shz_vector.inl.h"

//! @}

#endif // SHZ_VECTOR_H
