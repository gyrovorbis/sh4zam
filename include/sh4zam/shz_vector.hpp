/*! \file
 *  \brief C++ Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides types and mathematical functions for representing and
 *  operating on vectors within C++.
 *
 *  \author Falco Girgis
 */
#ifndef SHZ_VECTOR_HPP
#define SHZ_VECTOR_HPP

#include <compare>

#include "shz_vector.h"
#include "shz_trig.hpp"

namespace shz {

template<typename CRTP, typename C, size_t R>
struct vecN: C {
    using            CppType = CRTP;
    using            CType   = C;

    constexpr size_t Rows    = R;
    constexpr size_t Cols    = 1;

    vecN() = default;

    SHZ_FORCE_INLINE SHZ_CONST vecN(CType other) noexcept:
        CType(other) {}

    SHZ_FORCE_INLINE SHZ_CONST static CppType lerp(vec3 start, vec3 end, float t) noexcept {
        return shz_vec_lerp(start, end, t);
    }

    SHZ_FORCE_INLINE SHZ_CONST auto &&operator[](this auto &&self, size_t index) {
        std::forward<decltype(self)>(self).e[index];
    }

    auto operator<=>(const CppType &rhs) const noexcept = default;

    SHZ_FORCE_INLINE SHZ_CONST CppType &operator+=(CppType other) noexcept {
        *this = *this + other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType &operator-=(CppType other) noexcept {
        *this = *this - other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType &operator*=(CppType other) noexcept {
        *this = *this * other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType &operator/=(CppType other) noexcept {
        *this = *this / other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType &operator*=(float other) noexcept {
        *this = *this * other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType &operator/=(float other) noexcept {
        *this = *this / other;
        return *this;
    }

    auto operator<=>(const vec2 &rhs) const noexcept = default;

    SHZ_FORCE_INLINE SHZ_CONST float dot(CppType other) const noexcept {
        return shz_vec_dot(*this, rhs);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude() const noexcept {
        return shz_vec_magnitude(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude_sqr() const noexcept {
        return shz_vec_magnitude_sqr(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude_inv() const noexcept {
        return shz_vec_magnitude_inv(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType direction() const noexcept {
        return shz_vec_normalize(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void normalize() noexcept {
        *this = shz_vec_normalize(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType direction_safe() const noexcept {
        return shz_vec_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void normalize_safe() noexcept {
        *this = shz_vec_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float distance(CppType other) const noexcept {
        return shz_vec_distance(*this, other);
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType reflect(CppType normal) const noexcept {
        return shz_vec_reflect(*this, normal);
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType project(CppType onto) const noexcept {
        return shz_vec_project(*this, onto);
    }

    SHZ_FORCE_INLINE SHZ_CONST CppType project_safe(CppType onto) const noexcept {
        return shz_vec_project_safe(*this, onto);
    }

    SHZ_FORCE_INLINE SHZ_CONST float angle_between(CppType other) const noexcept {
        return shz_vec_angle_between(*this, other);
    }

    SHZ_FORCE_INLINE SHZ_CONST auto angles() const noexcept {
        return shz_vec_angles(*this);
    }
};

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator+(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> CRTP rhs) noexcept {
    return shz_vec_add(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator-(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_sub(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator*(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_mul(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator/(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_div(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator*(vecN<CRTP, C, R> lhs, float rhs) noexcept {
    return shz_vec_scale(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator*(float lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_scale(rhs, lhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator/(vecN<CRTP, C, R> lhs, float rhs) noexcept {
    return shz_vec_scale(lhs, shz::fast_invf(rhs));
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE SHZ_CONST CRTP operator/(float lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_scale(rhs, shz_fast_invf(lhs));
}

/*! 2D Vector type
 *
 *  C++ structure for representing a 2-dimensional vector.
 *
 *  \sa vec3, vec4
 *  \todo
 *      - stringify
 *      - std::swap
 *      - swizzling
 *      - std::pair
 *      - iterators
 *      - conditional exceptions on safe stuff
 */
struct vec2: vecN<vec2, shz_vec2_t, 2> {
    vec2() = default;

    SHZ_FORCE_INLINE SHZ_CONST vec2(float v) noexcept:
        vecN({ v, v }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(float x, float y) noexcept:
        vecN({ x, y }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(vec2 vec2) noexcept:
        vecN(vec2) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(vec2 other) noexcept:
        vecN(other) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(sincos pair) noexcept:
        vecN(shz_vec2_from_sincos(pair)) {}

    SHZ_FORCE_INLINE SHZ_CONST static vec2 from_angle(float rads) noexcept {
        return shz_vec2_from_angle(rads);
    }

    SHZ_FORCE_INLINE SHZ_CONST static vec2 from_angle_deg(float deg) noexcept {
        return shz_vec2_from_angles_deg(deg);
    }

    SHZ_FORCE_INLINE SHZ_CONST float cross(vec2 other) const noexcept {
        return shz_vec2_cross(*this, other);
    }
};

struct vec3: vecN<vec3, shz_vec3_t, 3> {
    vec3() = default;

    SHZ_FORCE_INLINE SHZ_CONST vec3(float v) noexcept:
        shz_vec3_t({ v, v, v }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(float x, float y, float z) noexcept:
        shz_vec3_t({ x, y, z }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(vec2 xy, float z) noexcept:
        shz_vec3_t({ xy.x, xy.y, z }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(float x, vec2 yz) noexcept:
        shz_vec3_t({ x, yz.y, yz.z }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(sincos azimuth, sincos elevation) noexcept:
        shz_vec3_t(shz_vec3_from_sincos(azimuth, elevation)) {}

    SHZ_FORCE_INLINE SHZ_CONST static vec3 from_angles(float azimuth_rads, elevation_rads) noexcept {
        return shz_vec3_from_angles(azimuth_rads, elevation_rads);
    }

    SHZ_FORCE_INLINE SHZ_CONST static vec3 from_angles_deg(float azimuth_deg, float elevation_deg) noexcept {
        return shz_vec3_from_angles_deg(azimuth_deg, elevation_deg);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 cross(vec2 other) const noexcept {
        return shz_vec3_cross(*this, other);
    }
};

struct vec4: vecN<vec4, shz_vec4_t, 4> {
    vec4() = default;

    SHZ_FORCE_INLINE SHZ_CONST vec4(float v) noexcept:
        shz_vec4_t({ v, v, v, v }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec4(float x, float y, float z, float w) noexcept:
        shz_vec4_t({ x, y, z, w }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec4(vec2 xy, float z, float w) noexcept:
        shz_vec4_t({ xy.x, xy.y, z, w }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec4(float x, vec2 yz, float w) noexcept:
        shz_vec4_t({ x, yz.y, yz.z, w }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec4(float x, float y, vec2 zw) noexcept:
        shz_vec4_t({ x, y, zw.z, zw.w }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec4(vec2 xy, vec2 zw) noexcept:
        shz_vec4_t({ xy.x, xy.y, zw.z, zw.w }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec4(vec3 xyz, float w) noexcept:
        shz_vec4_t({ xyz.x, xyz.y, xyz.z, w }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec4(float x, vec3 yzw) noexcept:
        shz_vec4_t({ x, xyz.y, xyz.z, xyz.w }) {}
};

}

#endif // SHZ_VECTOR_HPP
