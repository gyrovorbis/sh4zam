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

    SHZ_FORCE_INLINE SHZ_CONST auto &&operator[](this auto&& self, size_t index) noexcept {
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

    SHZ_FORCE_INLINE SHZ_CONST void normalize(void) noexcept {
        *this = shz_vec2_normalize(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void direction_safe() const noexcept {
        return shz_vec2_normalize_safe(*this);
    }

    SHZ_FORCE_INLINE SHZ_CONST void normalize_safe(void) noexcept {
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

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator+(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_add(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator-(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_sub(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator*(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_mul(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator/(vec2 lhs, vec2 rhs) noexcept {
    return shz_vec2_div(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator*(vec2 lhs, float rhs) noexcept {
    return shz_vec2_scale(lhs, rhs);
}

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator*(float lhs, vec2 rhs) noexcept {
    return shz_vec2_scale(rhs, lhs);
}

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator/(vec2 lhs, float rhs) noexcept {
    return shz_vec2_scale(lhs, shz::fast_invf(rhs));
}

SHZ_FORCE_INLINE SHZ_CONST
vec2 operator/(float lhs, vec2 rhs) noexcept {
    return shz_vec2_scale(rhs, shz_fast_invf(lhs));
}

}

#endif // SHZ_VECTOR_HPP
