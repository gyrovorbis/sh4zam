/*! \file
 *  \brief C++ Vector types and operations.
 *  \ingroup vector
 *
 *  This file provides types and mathematical functions for representing and
 *  operating on vectors within C++.
 *
 *  \author    Falco Girgis
 *  \copyright MIT License
 */
#ifndef SHZ_VECTOR_HPP
#define SHZ_VECTOR_HPP

#include <compare>
#include <concepts>

#include "shz_vector.h"
#include "shz_scalar.hpp"
#include "shz_trig.hpp"

namespace shz {

template<typename CRTP, typename C, size_t R>
struct vecN: C {
    using CppType  = CRTP;
    using CType    = C;

    static constexpr size_t Rows = R;
    static constexpr size_t Cols = 1;

    vecN() = default;

    SHZ_FORCE_INLINE vecN(CType other) noexcept:
        CType(other) {}

    SHZ_FORCE_INLINE static CppType lerp(CppType start, CppType end, float t) noexcept {
        return shz_vec_lerp(start, end, t);
    }

    SHZ_FORCE_INLINE auto &&operator[](this auto&& self, size_t index) {
        return std::forward<decltype(self)>(self).e[index];
    }

    friend constexpr auto operator<=>(CppType lhs, CppType rhs) noexcept {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                      rhs.begin(), rhs.end());
    }

    friend constexpr auto operator==(CppType lhs, CppType rhs) noexcept {
        return std::equal(lhs.begin(), lhs.end(),
                          rhs.begin(), rhs.end());
    }

    friend constexpr auto operator<(CppType lhs, CppType rhs) noexcept {
        return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                            rhs.begin(), rhs.end());
    }

    SHZ_FORCE_INLINE CppType &operator+=(this CppType &self, CppType other) noexcept {
        self = self + other;
        return self;
    }

    SHZ_FORCE_INLINE CppType &operator-=(this CppType &self, CppType other) noexcept {
        self = self - other;
        return self;
    }

    SHZ_FORCE_INLINE CppType &operator*=(this CppType &self, CppType other) noexcept {
        self = self * other;
        return self;
    }

    SHZ_FORCE_INLINE CppType &operator/=(this CppType &self, CppType other) noexcept {
        self = self / other;
        return self;
    }

    SHZ_FORCE_INLINE CppType &operator*=(this CppType &self, float other) noexcept {
        self = self * other;
        return self;
    }

    SHZ_FORCE_INLINE CppType &operator/=(this CppType &self, float other) noexcept {
        self = self / other;
        return self;
    }

    SHZ_FORCE_INLINE auto begin(this auto&& self) noexcept {
        return &self[0];
    }

    SHZ_FORCE_INLINE auto end(this auto&& self) noexcept {
        return &self[Rows];
    }

    SHZ_FORCE_INLINE float dot(this const CppType& self, CppType other) noexcept {
        return shz_vec_dot(self, other);
    }

    SHZ_FORCE_INLINE float magnitude() const noexcept {
        return shz_vec_magnitude(*this);
    }

    SHZ_FORCE_INLINE float magnitude_sqr() const noexcept {
        return shz_vec_magnitude_sqr(*this);
    }

    SHZ_FORCE_INLINE float magnitude_inv() const noexcept {
        return shz_vec_magnitude_inv(*this);
    }

    SHZ_FORCE_INLINE CppType direction() const noexcept {
        return shz_vec_normalize(*this);
    }

    SHZ_FORCE_INLINE void normalize() noexcept {
        *this = shz_vec_normalize(*this);
    }

    SHZ_FORCE_INLINE CppType direction_safe() const noexcept {
        return shz_vec_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE void normalize_safe() noexcept {
        *this = shz_vec_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE float distance(this const CppType& self, const CppType& other) noexcept {
        return shz_vec_distance(self, other);
    }

    SHZ_FORCE_INLINE float distance_sqr(this const CppType& self, const CppType& other) noexcept {
        return shz_vec_distance_sqr(self, other);
    }

    SHZ_FORCE_INLINE CppType reflect(CppType normal) const noexcept {
        return shz_vec_reflect(*this, normal);
    }

    SHZ_FORCE_INLINE CppType project(CppType onto) const noexcept {
        return shz_vec_project(*this, onto);
    }

    SHZ_FORCE_INLINE CppType project_safe(CppType onto) const noexcept {
        return shz_vec_project_safe(*this, onto);
    }

    SHZ_FORCE_INLINE float angle_between(CppType other) const noexcept {
        return shz_vec_angle_between(*this, other);
    }

    SHZ_FORCE_INLINE auto angles() const noexcept {
        return shz_vec_angles(*this);
    }
};

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator+(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_add(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator-(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_sub(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator*(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_mul(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator/(vecN<CRTP, C, R> lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_div(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator*(vecN<CRTP, C, R> lhs, float rhs) noexcept {
    return shz_vec_scale(lhs, rhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator*(float lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_scale(rhs, lhs);
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator/(vecN<CRTP, C, R> lhs, float rhs) noexcept {
    return shz_vec_scale(lhs, shz::invf(rhs));
}

template<typename CRTP, typename C, size_t R>
SHZ_FORCE_INLINE CRTP operator/(float lhs, vecN<CRTP, C, R> rhs) noexcept {
    return shz_vec_scale(rhs, shz::invf(lhs));
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
    using vecN::vecN;

    //SHZ_FORCE_INLINE vec2(shz_vec2_t other) noexcept:
    //    vecN(other) {}

    SHZ_FORCE_INLINE vec2(float v) noexcept:
        vecN({ v, v }) {}

    SHZ_FORCE_INLINE vec2(float x, float y) noexcept:
        vecN({ x, y }) {}

    SHZ_FORCE_INLINE vec2(sincos pair) noexcept:
        vecN(shz_vec2_from_sincos(pair)) {}

    SHZ_FORCE_INLINE static vec2 from_angle(float rads) noexcept {
        return shz_vec2_from_angle(rads);
    }

    SHZ_FORCE_INLINE static vec2 from_angle_deg(float deg) noexcept {
        return shz_vec2_from_angle_deg(deg);
    }

    SHZ_FORCE_INLINE float cross(vec2 other) const noexcept {
        return shz_vec2_cross(*this, other);
    }
};

struct vec3: vecN<vec3, shz_vec3_t, 3> {
    vec3() = default;

    SHZ_FORCE_INLINE vec3(shz_vec3_t other) noexcept:
        vecN(other) {}

    SHZ_FORCE_INLINE vec3(float v) noexcept:
        vecN({ v, v, v }) {}

    SHZ_FORCE_INLINE vec3(float x, float y, float z) noexcept:
        vecN({ x, y, z }) {}

    SHZ_FORCE_INLINE vec3(shz::vec2 xy, float z) noexcept:
        vecN({ xy.x, xy.y, z }) {}

    SHZ_FORCE_INLINE vec3(float x, shz::vec2 yz) noexcept:
       vecN({ x, yz.x, yz.y }) {}

    SHZ_FORCE_INLINE vec3(sincos azimuth, sincos elevation) noexcept:
        vecN(shz_vec3_from_sincos(azimuth, elevation)) {}

    SHZ_FORCE_INLINE static vec3 from_angles(float azimuth_rads, float elevation_rads) noexcept {
        return shz_vec3_from_angles(azimuth_rads, elevation_rads);
    }

    SHZ_FORCE_INLINE static vec3 from_angles_deg(float azimuth_deg, float elevation_deg) noexcept {
        return shz_vec3_from_angles_deg(azimuth_deg, elevation_deg);
    }

    SHZ_FORCE_INLINE vec3 cross(vec3 other) const noexcept {
        return shz_vec3_cross(*this, other);
    }
};

struct vec4: vecN<vec4, shz_vec4_t, 4> {
    vec4() = default;

    SHZ_FORCE_INLINE vec4(shz_vec4_t other) noexcept:
        vecN(other) {}

    SHZ_FORCE_INLINE vec4(float v) noexcept:
        vecN({ v, v, v, v }) {}

    SHZ_FORCE_INLINE vec4(float x, float y, float z, float w) noexcept:
        vecN({ x, y, z, w }) {}

    SHZ_FORCE_INLINE vec4(shz::vec2 xy, float z, float w) noexcept:
        vecN({ xy.x, xy.y, z, w }) {}

    SHZ_FORCE_INLINE vec4(float x, shz::vec2 yz, float w) noexcept:
        vecN({ x, yz.x, yz.y, w }) {}

    SHZ_FORCE_INLINE vec4(float x, float y, shz::vec2 zw) noexcept:
        vecN({ x, y, zw.x, zw.y }) {}

    SHZ_FORCE_INLINE vec4(shz::vec2 xy, shz::vec2 zw) noexcept:
        vecN({ xy.x, xy.y, zw.x, zw.y }) {}

    SHZ_FORCE_INLINE vec4(shz::vec3 xyz, float w) noexcept:
        vecN({ xyz.x, xyz.y, xyz.z, w }) {}

    SHZ_FORCE_INLINE vec4(float x, shz::vec3 yzw) noexcept:
        vecN({ x, yzw.x, yzw.y, yzw.z }) {}
};

}

#endif // SHZ_VECTOR_HPP
