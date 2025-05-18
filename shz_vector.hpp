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
struct vec2: shz_vec2_t {
    vec2() = default;

    SHZ_FORCE_INLINE SHZ_CONST vec2(float v) noexcept:
        shz_vec2_t({ v, v }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(float x, float y) noexcept:
        shz_vec2_t({ x, y }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(shz_vec2_t vec2) noexcept:
        shz_vec2_t(vec2) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(vec2 other) noexcept:
        shz_vec2_t(other) {}

    SHZ_FORCE_INLINE SHZ_CONST vec2(sincos pair) noexcept:
        shz_vec2_t(shz_vec2_from_sincos(pair)) {}

    SHZ_FORCE_INLINE SHZ_CONST static vec2 from_angle(float radians) noexcept {
        return shz_vec2_from_angle(radians);
    }

    SHZ_FORCE_INLINE SHZ_CONST static vec2 from_angle_deg(float degrees) noexcept {
        return shz_vec2_from_angle_deg(degrees);
    }

    auto operator<=>(const vec2 &rhs) const = default;

    SHZ_FORCE_INLINE SHZ_CONST auto &&operator[](this auto &&self, size_t index) noexcept {
        return std::forward<decltype(self)>(self).e[index];
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 &operator+=(vec2 other) noexcept {
        *this = *this + other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 &operator-=(vec2 other) noexcept {
        *this = *this - other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 &operator*=(vec2 other) noexcept {
        *this = *this * other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 &operator/=(vec2 other) noexcept {
        *this = *this / other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 &operator*=(float other) noexcept {
        *this = *this * other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 &operator/=(float other) noexcept {
        *this = *this / other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST float dot(vec2 rhs) const noexcept {
        return shz_vec2_dot(*this, rhs);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude() const noexcept {
        return shz_vec2_magnitude(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude_sqr() const noexcept {
        return shz_vec2_magnitude_sqr(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude_inv() const noexcept {
        return shz_vec2_magnitude_inv(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void direction() const noexcept {
        return shz_vec2_normalize(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void normalize() noexcept {
        *this = shz_vec2_normalize(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void direction_safe() const noexcept {
        return shz_vec2_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void normalize_safe() noexcept {
        *this = shz_vec2_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float distance(vec2 other) const noexcept {
        return shz_vec2_distance(*this, other);
    }

    SHZ_FORCE_INLINE SHZ_CONST float cross(vec2 other) const noexcept {
        return shz_vec2_cross(*this, other);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 lerp(vec2 end, float t) const noexcept {
        return shz_vec2_lerp(*this, end, t);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 reflect(vec2 normal) const noexcept {
        return shz_vec2_reflect(*this, normal);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 project(vec2 onto) const noexcept {
        return shz_vec2_project(*this, onto);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 project_safe(vec2 onto) const noexcept {
        return shz_vec2_project_safe(*this, onto);
    }

    SHZ_FORCE_INLINE SHZ_CONST float angle() const noexcept {
        return shz_vec2_angle(*this);
    }

    SHZ_INLINE SHZ_CONST float angle_between(vec2 other) const noexcept {
        return shz_vec2_angle_between(*this, other);
    }
};

SHZ_FORCE_INLINE SHZ_CONST vec2 operator+(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_add(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec2 operator-(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_sub(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec2 operator*(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_mul(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec2 operator/(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_div(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec2 operator*(vec2 lhs, float rhs) noexcept {
    return shz_vec2_scale(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec2 operator*(float lhs, vec2 rhs) noexcept {
    return shz_vec2_scale(rhs, lhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec2 operator/(vec2 lhs, float rhs) noexcept {
    return shz_vec2_scale(lhs, shz::fast_invf(rhs));
}

SHZ_FORCE_INLINE SHZ_CONST vec2 operator/(float lhs, vec2 rhs) noexcept {
    return shz_vec2_scale(rhs, shz_fast_invf(lhs));
}

struct vec3: shz_vec3_t {
    vec3() = default;

    SHZ_FORCE_INLINE SHZ_CONST vec3(float v) noexcept:
        shz_vec3_t({ v, v, v }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(float x, float y, float z) noexcept:
        shz_vec3_t({ x, y, z }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(shz_vec2_t xy, float z) noexcept:
        shz_vec3_t({ xy.x, xy.y, z }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(float x, shz_vec2_t yz) noexcept:
        shz_vec3_t({ x, yz.y, yz.z }) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(shz_vec3_t other) noexcept:
        shz_vec3_t(other) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(vec3 other) noexcept:
        shz_vec3_t(other) {}

    SHZ_FORCE_INLINE SHZ_CONST vec3(sincos azimuth, sincos elevation) noexcept:
        shz_vec3_t(shz_vec3_from_sincos(azimuth, elevation)) {}

    SHZ_FORCE_INLINE SHZ_CONST static vec3 from_angles(float azimuth_rads, elevation_rads) noexcept {
        return shz_vec3_from_angles(azimuth_rads, elevation_rads);
    }

    SHZ_FORCE_INLINE SHZ_CONST static vec3 from_angles_deg(float azimuth_deg, float elevation_deg) noexcept {
        return shz_vec3_from_angles_deg(azimuth_deg, elevation_deg);
    }

    auto operator<=>(const vec3& rhs) const = default;

    SHZ_FORCE_INLINE SHZ_CONST auto &&operator[](this auto &&self, size_t index) noexcept {
        return std::forward<decltype(self)>(self).e[index];
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 &operator+=(vec3 other) noexcept {
        *this = *this + other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 &operator-=(vec3 other) noexcept {
        *this = *this - other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 &operator*=(vec3 other) noexcept {
        *this = *this * other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 &operator/=(vec3 other) noexcept {
        *this = *this / other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 &operator*=(float other) noexcept {
        *this = *this * other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 &operator/=(float other) noexcept {
        *this = *this / other;
        return *this;
    }

    SHZ_FORCE_INLINE SHZ_CONST float dot(vec3 rhs) const noexcept {
        return shz_vec3_dot(*this, rhs);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude() const noexcept {
        return shz_vec3_magnitude(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude_sqr() const noexcept {
        return shz_vec3_magnitude_sqr(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float magnitude_inv() const noexcept {
        return shz_vec3_magnitude_inv(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void direction() const noexcept {
        return shz_vec3_normalize(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void normalize() noexcept {
        *this = shz_vec3_normalize(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void direction_safe() const noexcept {
        return shz_vec3_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void normalize_safe() noexcept {
        *this = shz_vec3_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST float distance(vec3 other) const noexcept {
        return shz_vec3_distance(*this, other);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 cross(vec2 other) const noexcept {
        return shz_vec3_cross(*this, other);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 lerp(vec3 end, float t) const noexcept {
        return shz_vec3_lerp(*this, end, t);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec2 reflect(vec3 normal) const noexcept {
        return shz_vec3_reflect(*this, normal);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 project(vec3 onto) const noexcept {
        return shz_vec3_project(*this, onto);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 project_safe(vec3 onto) const noexcept {
        return shz_vec3_project_safe(*this, onto);
    }

    SHZ_FORCE_INLINE SHZ_CONST vec3 angles() const noexcept {
        return shz_vec3_angles(*this);
    }

    SHZ_INLINE SHZ_CONST float angle_between(vec3 other) const noexcept {
        return shz_vec3_angle_between(*this, other);
    }
};

SHZ_FORCE_INLINE SHZ_CONST vec3 operator+(vec3 lhs, vec3 rhs) noexcept {
    return shz_vec3_add(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec3 operator-(vec3 lhs, vec3 rhs) noexcept {
    return shz_vec3_sub(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec3 operator*(vec3 lhs, vec3 rhs) noexcept {
    return shz_vec3_mul(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec3 operator/(vec3 lhs, vec3 rhs) noexcept {
    return shz_vec3_div(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec3 operator*(vec3 lhs, float rhs) noexcept {
    return shz_vec3_scale(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec3 operator*(float lhs, vec3 rhs) noexcept {
    return shz_vec3_scale(rhs, lhs);
}

SHZ_FORCE_INLINE SHZ_CONST vec3 operator/(vec3 lhs, float rhs) noexcept {
    return shz_vec3_scale(lhs, shz::fast_invf(rhs));
}

SHZ_FORCE_INLINE SHZ_CONST vec3 operator/(float lhs, vec3 rhs) noexcept {
    return shz_vec3_scale(rhs, shz_fast_invf(lhs));
}

}

#endif // SHZ_VECTOR_HPP
