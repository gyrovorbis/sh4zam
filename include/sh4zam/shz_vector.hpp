/*! \file
 *  \brief C++ Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides types and mathematical functions for representing and
 *  operating on vectors within C++.
 *
 *  \author    2025 Falco Girgis
 *  \copyright MIT License
 *
 *  \todo
 *      - C++ proxy class equivalent for shz_vecN_deref().
 *      - C++ better swizzling mechanism
 */

#ifndef SHZ_VECTOR_HPP
#define SHZ_VECTOR_HPP

#include <compare>
#include <concepts>

#include "shz_vector.h"
#include "shz_scalar.hpp"
#include "shz_trig.hpp"

namespace shz {

struct vec2;
struct vec3;
struct vec4;

/*! Common C++ base structure inherited by all vector types. 

    This struct template serves as the base class for all
    concrete vector types, providing:
        - Common API routines
        - Adaptor between C and C++ types
        - Convenience overloaded operators and STL iterators.

    \sa shz::vec2, shz::vec3, shz::vec4
*/
template<typename CRTP, typename C, size_t R>
struct vecN: C {
    using CppType  = CRTP;  //!< Cpp derived type
    using CType    = C;     //!< C base type

    static constexpr size_t Rows = R;   //!< Number of rows
    static constexpr size_t Cols = 1;   //!< Number of columns

    //! Default constructor, does nothing.
    vecN() = default;

    //! Converting constructor from existing C instance.
    SHZ_FORCE_INLINE vecN(CType other) noexcept:
        CType(other) {}

    //! Conversion operator for going from a layout-compatible vector type to a SH4ZAM vector type.
    SHZ_FORCE_INLINE static CppType from(const auto& raw) noexcept {
        return *reinterpret_cast<const CppType*>(&raw);
    }

    //! Conversion operator for going from a SH4ZAM vector type to another layout-compatible type.
    template<typename T>
    SHZ_FORCE_INLINE T to() const noexcept {
        return *reinterpret_cast<const T*>(this);
    }

    //! Conversion operator for accessing an existing pointer type as though it were a refrence to a SH4ZAM type.
    SHZ_FORCE_INLINE CppType& deref(const auto* raw) noexcept {
        return *const_cast<CppType*>(reinterpret_cast<const CppType*>(raw));
    }

    //! Returns the vector that is linearly interpolated between the two given vectors by the `0.0f-1.0f` factor, \p t.
    SHZ_FORCE_INLINE static CppType lerp(CppType start, CppType end, float t) noexcept {
        return shz_vec_lerp(start, end, t);
    }

#ifdef SHZ_CPP23
    //! Overloaded subscript operator -- allows for indexing vectors like an array.
    SHZ_FORCE_INLINE auto&& operator[](this auto&& self, size_t index) noexcept {
        return std::forward<decltype(self)>(self).e[index];
    }

    //! Overloaded space-ship operator, for generic lexicographical comparison of vectors.
    friend auto operator<=>(CppType lhs, CppType rhs) noexcept {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    //! Returns an iterator to the beginning of the vector -- For STL support.
    SHZ_FORCE_INLINE auto begin(this auto&& self) noexcept {
        return &self[0];
    }

    //! Returns an iterator to the end of the vector -- For STL support.
    SHZ_FORCE_INLINE auto end(this auto&& self) noexcept {
        return &self[Rows];
    }

    //! Overloaded "less-than" operator, for comparing vectors.
    friend auto operator<(CppType lhs, CppType rhs) noexcept {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }    
#endif

    //! Overloaded equality operator, for comparing vectors.
    friend auto operator==(CppType lhs, CppType rhs) noexcept {
        return shz_vec_equal(lhs, rhs);
    }

    //! Overloaded unary negation operator, returns the negated vector.
    friend CppType operator-(CppType vec) noexcept {
        return vec.neg();
    }

    //! Overloaded operator for adding and accumulating a vector onto another.
    SHZ_FORCE_INLINE CppType& operator+=(CppType other) noexcept {
        *static_cast<CppType*>(this) = *static_cast<CppType*>(this) + other;
        return *static_cast<CppType*>(this);
    }

    //! Overloaded subtraction assignment operator, subtracts a vector from the left-hand vector.
    SHZ_FORCE_INLINE CppType& operator-=(CppType other) noexcept {
        *static_cast<CppType*>(this) = *static_cast<CppType*>(this) - other;
        return *static_cast<CppType*>(this);
    }

    //! Overloaded multiplication assignment operator, multiplies and accumulates a vector onto the left-hand vector.
    SHZ_FORCE_INLINE CppType& operator*=(CppType other) noexcept {
        *static_cast<CppType*>(this) = *static_cast<CppType*>(this) * other;
        return *static_cast<CppType*>(this);
    }

    //! Overloaded division assignment operator, divides the left vector by the right, assigning the left to the result.
    SHZ_FORCE_INLINE CppType& operator/=(CppType other) noexcept {
        *static_cast<CppType*>(this) = *static_cast<CppType*>(this) / other;
        return *static_cast<CppType*>(this);
    }

    //! Overloaded multiplication assignment operator, multiplies and accumulates each vector component by the given scalar.
    SHZ_FORCE_INLINE CppType& operator*=(float other) noexcept {
        *static_cast<CppType*>(this) = *static_cast<CppType*>(this) * other;
        return *static_cast<CppType*>(this);
    }

    //! Overloaded division assignment operator, dividing and assigning each vector component by the given scalar value.
    SHZ_FORCE_INLINE CppType& operator/=(float other) noexcept {
        *static_cast<CppType*>(this) = *static_cast<CppType*>(this) / other;
        return *static_cast<CppType*>(this);
    }

    //! Swizzle oeprator which takes a compile-time list of indices as non-type template arguments for the index each element should use as its new value.
    template<unsigned... Indices>
    SHZ_FORCE_INLINE CppType swizzle() const noexcept {
        return shz_vec_swizzle(*static_cast<const CppType*>(this), Indices...);
    }

    //! Returns a new vector whose components are the absolute value of the given vector.
    SHZ_FORCE_INLINE CppType abs() const noexcept {
        return shz_vec_abs(*static_cast<const CppType*>(this));
    }

    //! Returns a new vector whose components are the negative values of the given vector.
    SHZ_FORCE_INLINE CppType neg() const noexcept {
        return shz_vec_neg(*static_cast<const CppType*>(this));
    }

    //! Returns a new vector whose components are the reciprocal values of the given vector.
    SHZ_FORCE_INLINE CppType inv() const noexcept {
        return shz_vec_inv(*static_cast<const CppType*>(this));
    }

    //! Returns the maximum value of every element within the vector.
    SHZ_FORCE_INLINE float max() const noexcept {
        return shz_vec_max(*static_cast<const CppType*>(this));
    }

    //! Returns the minimum value of every element within the vector.
    SHZ_FORCE_INLINE float min() const noexcept {
        return shz_vec_min(*static_cast<const CppType*>(this));
    }

    //! Returns a new vector whose values are the clamped components of the given vector.
    SHZ_FORCE_INLINE CppType clamp(float min, float max) const noexcept {
        return shz_vec_clamp(*static_cast<const CppType*>(this), min, max);
    }

    //! Returns the dot product of the given vector and another.
    SHZ_FORCE_INLINE float dot(CppType other) const noexcept {
        return shz_vec_dot(*static_cast<const CppType*>(this), other);
    }

    //! Returns the dot product of the given vector against two others.
    SHZ_FORCE_INLINE vec2 dot(CppType v1, CppType v2) const noexcept;

    //! Returns the dot product of the given vector against three others.
    SHZ_FORCE_INLINE vec3 dot(CppType v1, CppType v2, CppType v3) const noexcept;

    //! Returns the magnitude of the given vector.
    SHZ_FORCE_INLINE float magnitude() const noexcept {
        return shz_vec_magnitude(*static_cast<const CppType*>(this));
    }

    //! Returns the squared magnitude of the given vector.
    SHZ_FORCE_INLINE float magnitude_sqr() const noexcept {
        return shz_vec_magnitude_sqr(*static_cast<const CppType*>(this));
    }

    //! Returns the inverse magnitude of the given vector.
    SHZ_FORCE_INLINE float magnitude_inv() const noexcept {
        return shz_vec_magnitude_inv(*static_cast<const CppType*>(this));
    }

    //! Returns the direction vector resulting from normalizing the given vector.
    SHZ_FORCE_INLINE CppType direction() const noexcept {
        return shz_vec_normalize(*static_cast<const CppType*>(this));
    }

    //! Normalizes the given vector.
    SHZ_FORCE_INLINE void normalize() noexcept {
        *static_cast<CppType*>(this) = shz_vec_normalize(*static_cast<const CppType*>(this));
    }

    //! Returns the direction vector of a given vector, safely protecting against division-by-zero.
    SHZ_FORCE_INLINE CppType direction_safe() const noexcept {
        return shz_vec_normalize_safe(*static_cast<const CppType*>(this));
    }

    //! Normalizes the given vector, safely protecting against division-by-zero.
    SHZ_FORCE_INLINE void normalize_safe() noexcept {
        *static_cast<CppType*>(this) = shz_vec_normalize_safe(*static_cast<const CppType*>(this));
    }

    //! Returns the magnitude of the difference between two vectors as their distance.
    SHZ_FORCE_INLINE float distance(const CppType& other) const noexcept {
        return shz_vec_distance(*static_cast<CppType*>(this), other);
    }

    //! Returns the value of the distance between two vectors squared (faster than actual distance)
    SHZ_FORCE_INLINE float distance_sqr(const CppType& other) const noexcept {
        return shz_vec_distance_sqr(*static_cast<CppType*>(this), other);
    }

    //! Moves the given vector towards the target by the given \p maxdist.
    SHZ_FORCE_INLINE CppType move(CppType target, float maxdist) const noexcept {
        return shz_vec_move(*static_cast<const CppType*>(this), target, maxdist);
    }

    //! Returns the vector created from reflecting the given vector over the normal of a surface.
    SHZ_FORCE_INLINE CppType reflect(CppType normal) const noexcept {
        return shz_vec_reflect(*static_cast<const CppType*>(this), normal);
    }

    //! Returns the vector create from refracting the given incidence vector over the normal of a surface, using the given refraction ratio index.
    SHZ_FORCE_INLINE CppType refract(CppType normal, float eta) const noexcept {
        return shz_vec_refract(*static_cast<const CppType*>(this), normal, eta);
    }

    //! Returns the vector created from projecting the given vector onto another.
    SHZ_FORCE_INLINE CppType project(CppType onto) const noexcept {
        return shz_vec_project(*static_cast<const CppType*>(this), onto);
    }

    //! Returns the vector created from projecting the given vector onto another, safely protecting against division-by-zero.
    SHZ_FORCE_INLINE CppType project_safe(CppType onto) const noexcept {
        return shz_vec_project_safe(*static_cast<const CppType*>(this), onto);
    }

    //! Returns the angle between the given vector and another, in radians.
    SHZ_FORCE_INLINE float angle_between(CppType other) const noexcept {
        return shz_vec_angle_between(*static_cast<const CppType*>(this), other);
    }

    //! Returns the angle(s) created between the given vector axis and the +X axis, in radians.
    SHZ_FORCE_INLINE auto angles() const noexcept {
        return shz_vec_angles(*static_cast<const CppType*>(this));
    }
};

//! Overloaded addition operator, adding two vectors together and returning the result.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator+(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_add(lhs, rhs);
}

//! Overloaded subtraction operator, subtracting one vector from another, returning the result.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator-(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_sub(lhs, rhs);
}

//! Overloaded multiplication operator, performing element-wise multiplication between two vectors, returning the resultant vector.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator*(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_mul(lhs, rhs);
}

//! Overloaded division operator, returning the resulting vector from component-wise dividing the elements of \p lhs by \p rhs.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator/(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_div(lhs, rhs);
}

//! Overloaded multiplication operator for scaling a vector by a scalar and returning the resulting vector.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator*(vecN<CRTP, C, R> lhs, float rhs) noexcept {
    return shz_vec_scale(lhs, rhs);
}

//! Reverse overloaded multiplication operator for scaling a vector by a scalar and returning the resulting vector.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator*(float lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_scale(rhs, lhs);
}

//! Overloaded division operator for component-wise dividing each element of the given vector by the given scalar.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator/(vecN<CRTP, C, R> lhs, float rhs) noexcept {
    return shz_vec_scale(lhs, shz::invf(rhs));
}

//! Reverse overloaded division operator for component-wise dividing each element of the given vector by the given scalar.
template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator/(float lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_scale(rhs, shz::invf(lhs));
}

/*! 2D Vector type
 *
 *  C++ structure for representing a 2-dimensional vector.
 *
 *  \sa shz::vecN, shz_vec2_t, shz::vec3, shz::vec4
 */
struct vec2: vecN<vec2, shz_vec2_t, 2> {
    // Inherit parent constructors and operators.
    using vecN::vecN;

    // Unhide inherited overloaded dot product methods.
    using vecN::dot;

    //! Default constructor: does nothing.
    vec2() = default;

    //! Single-value constructor: sets both components equal to \p v.
    SHZ_FORCE_INLINE vec2(float v) noexcept:
        vecN(shz_vec2_fill(v)) {}

    //! Constructs a vec2 with the given values as components.
    SHZ_FORCE_INLINE vec2(float x, float y) noexcept:
        vecN(shz_vec2_init(x, y)) {}

    //! Constructs a vec2 from the given angle of rotation from the +X axis.
    SHZ_FORCE_INLINE vec2(sincos pair) noexcept:
        vecN(shz_vec2_from_sincos(pair)) {}

    //! Constructs a vec2 from the given angle of rotation from the +X axis, in radians.
    SHZ_FORCE_INLINE static vec2 from_angle(float rads) noexcept {
        return shz_vec2_from_angle(rads);
    }

    //! Constructs a vec2 from the given angle of rotation from the +X axis, in degrees.
    SHZ_FORCE_INLINE static vec2 from_angle_deg(float deg) noexcept {
        return shz_vec2_from_angle_deg(deg);
    }

    //! C++ wrapper for shz_vec2_cross().
    SHZ_FORCE_INLINE float cross(vec2 other) const noexcept {
        return shz_vec2_cross(*this, other);
    }

    //! C++ wrapper for shz_vec2_rotate().
    SHZ_FORCE_INLINE vec2 rotate(float radians) const noexcept {
        return shz_vec2_rotate(*this, radians);
    }
};

//! C++ alias for vec2 for those who like POSIX-style.
using vec2_t = vec2;

/*! 3D Vector type
 *
 *  C++ structure for representing a 3-dimensional vector.
 *
 *  \sa shz::vecN, shz_vec2_t, shz::vec2, shz::vec4
 */
struct vec3: vecN<vec3, shz_vec3_t, 3> {
    // Inherit parent constructors and operators.
    using vecN::vecN;

    // Unhide inherited overloaded dot product methods.
    using vecN::dot;

    //! Default constructor: does nothing
    vec3() = default;

    //! C constructor: constructs a C++ vec3 from a C shz_vec3_t.
    SHZ_FORCE_INLINE vec3(shz_vec3_t other) noexcept:
        vecN(other) {}

    //! Single-value constructor: initializes all components to \p v.
    SHZ_FORCE_INLINE vec3(float v) noexcept:
        vecN(shz_vec3_fill(v)) {}

    //! Value constructor: initializes each component to its given value.
    SHZ_FORCE_INLINE vec3(float x, float y, float z) noexcept:
        vecN(shz_vec3_init(x, y, z)) {}

    //! Constructs a vec3 from a shz::vec2 and a scalar value for its z component.
    SHZ_FORCE_INLINE vec3(shz::vec2 xy, float z) noexcept:
        vecN(shz_vec2_vec3(xy, z)) {}

    //! Constructs a vec3 from a scalar as its x component and a shz::vec2 as its Y and Z components.
    SHZ_FORCE_INLINE vec3(float x, shz::vec2 yz) noexcept:
       vecN(shz_vec3_init(x, yz.x, yz.y)) {}

    //! Returns a 3D vector which forms the given angles with the +X axis.
    SHZ_FORCE_INLINE vec3(sincos azimuth, sincos elevation) noexcept:
        vecN(shz_vec3_from_sincos(azimuth, elevation)) {}

    //! Returns 2 3D vectors which are normalized and orthogonal to the two input vectors as a std::pair<>.
    SHZ_FORCE_INLINE static auto orthonormalize(vec3 in1, vec3 in2) noexcept {
        vec3 out1, out2;
        shz_vec3_orthonormalize(in1, in2, &out1, &out2);
        return std::make_pair(out1, out2);
    }

    //! Returns 2 3D vectors which are normalized and orthogonal to the two input vectors via output pointers.
    SHZ_FORCE_INLINE static void orthonormalize(vec3 in1, vec3 in2, vec3* out1, vec3* out2) noexcept {
        shz_vec3_orthonormalize(in1, in2, out1, out2);
    }

    //! Calculates the cubic hermite interpolation between two vectors and their tangents.
    SHZ_FORCE_INLINE static vec3 cubic_hermite(vec3 v1, vec3 tangent1, vec3 v2, vec3 tangent2, float amount) noexcept {
        return shz_vec3_cubic_hermite(v1, tangent1, v2, tangent2, amount);
    }

    // Returns the inner 2D vector, <X, Y>, as a C++ vector.
    SHZ_FORCE_INLINE vec2 xy() const noexcept {
        return shz_vec3_t::xy;
    }

    //! Returns a 3D vector which forms the given angles with the +X axis, in radians.
    SHZ_FORCE_INLINE static vec3 from_angles(float azimuth_rads, float elevation_rads) noexcept {
        return shz_vec3_from_angles(azimuth_rads, elevation_rads);
    }

    //! Returns a 3D vector which forms the given angles with the +X axis, in degrees.
    SHZ_FORCE_INLINE static vec3 from_angles_deg(float azimuth_deg, float elevation_deg) noexcept {
        return shz_vec3_from_angles_deg(azimuth_deg, elevation_deg);
    }

    //! Returns a 3D vector which forms the given angles with the +X axis.
    SHZ_FORCE_INLINE vec3 cross(vec3 other) const noexcept {
        return shz_vec3_cross(*this, other);
    }

    //! Returns the 3D vector "triple product" between the given vector and vectors \p a and \p b.
    SHZ_FORCE_INLINE float triple(vec3 b, vec3 c) const noexcept {
        return shz_vec3_triple(*this, b, c);
    }

    //! Returns a 3D vector which is perpendicular to this vector.
    SHZ_FORCE_INLINE vec3 perp() const noexcept {
        return shz_vec3_perp(*this);
    }

    //! Returns the 3D reject vector of the given vector and another.
    SHZ_FORCE_INLINE vec3 reject(vec3 onto) const noexcept {
        return shz_vec3_reject(*this, onto);
    }

    //! Computes the barycentric coordinates `<u, v, w>` for the given 3D vector, within the plane of the triangle formed by the given vertices, \p a, \p b, and \p c.
    SHZ_FORCE_INLINE vec3 barycenter(vec3 a, vec3 b, vec3 c) const noexcept {
        return shz_vec3_barycenter(*this, a, b, c);
    }
};

//! C++ alias for vec3 for those who like POSIX-style.
using vec3_t = vec3;

/*! 4D Vector type
 *
 *  C++ structure for representing a 4-dimensional vector.
 *
 *  \sa shz::vecN, shz_vec4_t, shz::vec2, shz::vec3
 */
struct vec4: vecN<vec4, shz_vec4_t, 4> {
    // Inherit parent constructors and operators.
    using vecN::vecN;

    // Unhide inherited overloaded dot product methods.
    using vecN::dot;

    //! Default constructor: does nothing.
    vec4() = default;

    //! C Constructor: initializes a C++ shz::vec4 from a C shz_vec4_t.
    SHZ_FORCE_INLINE vec4(shz_vec4_t other) noexcept:
        vecN(other) {}

    //! Single-value constructor: initializes each element to the given value.
    SHZ_FORCE_INLINE vec4(float v) noexcept:
        vecN(shz_vec4_fill(v)) {}

    //! Value constructor: initializes each element to its corresponding parameter value.
    SHZ_FORCE_INLINE vec4(float x, float y, float z, float w) noexcept:
        vecN(shz_vec4_init(x, y, z, w)) {}

    //! Constructs a 4D vector with a 2D vector providing the X and Y coordinates and scalars providing Z and W.
    SHZ_FORCE_INLINE vec4(shz::vec2 xy, float z, float w) noexcept:
        vecN(shz_vec2_vec4(xy, z, w)) {}

    //! Constructs a 4D vector with scalars providing X and W coordinates and a 2D vector providing Y and Z.
    SHZ_FORCE_INLINE vec4(float x, shz::vec2 yz, float w) noexcept:
        vecN(shz_vec4_init(x, yz.x, yz.y, w)) {}

    //! Constructs a 4D vector with scalars providing X and Y coordinaets and a 2D vector providing Z and W.
    SHZ_FORCE_INLINE vec4(float x, float y, shz::vec2 zw) noexcept:
        vecN(shz_vec4_init(x, y, zw.x, zw.y )) {}

    //! Constructs a 4D vector from the components provided by the given pair of 2D vectors.
    SHZ_FORCE_INLINE vec4(shz::vec2 xy, shz::vec2 zw) noexcept:
        vecN(shz_vec4_init(xy.x, xy.y, zw.x, zw.y)) {}

    //! Constructs a 4D vector with the X, Y, and Z components given by a 3D vector and W given by a scalar.
    SHZ_FORCE_INLINE vec4(shz::vec3 xyz, float w) noexcept:
        vecN(shz_vec3_vec4(xyz, w)) {}

    //! Constructs a 4D vector with the X component given by a scalar and the Y, Z, and W components given by a 3D vector.
    SHZ_FORCE_INLINE vec4(float x, shz::vec3 yzw) noexcept:
        vecN(shz_vec4_init(x, yzw.x, yzw.y, yzw.z)) {}

    // Returns the inner 2D vector, <X, Y>, as a C++ vector.
    SHZ_FORCE_INLINE vec2 xy() const noexcept {
        return shz_vec4_t::xy;
    }

    // Returns the inner 2D vector, <Z, W>, as a C++ vector.
    SHZ_FORCE_INLINE vec2 zw() const noexcept {
        return shz_vec4_t::zw;
    }

    // Returns the inner 3D vector, <X, Y, Z>, as a C++ vector.
    SHZ_FORCE_INLINE vec3 xyz() const noexcept {
        return shz_vec4_t::xyz;
    }
};

//! C++ alias for vec4 for those who like POSIX-style.s
using vec4_t = vec4;

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE vec2 vecN<CRTP, C, R>::dot(CppType v1, CppType v2) const noexcept {
    return shz_vec_dot2(*static_cast<const CRTP*>(this), v1, v2);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE vec3 vecN<CRTP, C, R>::dot(CppType v1, CppType v2, CppType v3) const noexcept {
    return shz_vec_dot3(*static_cast<const CRTP*>(this), v1, v2, v3);
}

}

#endif // SHZ_VECTOR_HPP
