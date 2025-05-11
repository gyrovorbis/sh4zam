#ifndef SHZ_VECTOR_HPP
#define SHZ_VECTOR_HPP

#include <compare>

#include "shz_vector.h"

namespace shz {

struct vec2: public shz_vec2_t {

    vec2() = default;

    vec2(shz_vec2_t vec2) noexcept:
        shz_vec2_t(vec2)
    {}

    float dot(vec2 rhs) const noexcept {
        return shz_vec2_dot(*this, rhs);
    }

    float magnitudeSqr(void) const noexcept {
        return shz_vec2_magnitude_sqr(*this);
    }

    float magnitude(void) const noexcept {
        return shz_vec2_magnitude(*this);
    }

    void normalize(void) noexcept {
        *this = shz_vec2_normalize(*this);
    }

    void normalizeSafe(void) noexcept {
        *this = shz_vec2_normalize_safe(*this);
    }

    float distance(vec2 other) const noexcept {
        return shz_vec2_distance(*this, other);
    }

    float cross(vec2 other) const noexcept {
        return shz_vec2_cross(*this, other);
    }

    auto operator<=>(const vec2 &rhs) const = default;

    vec2 operator+(vec2 rhs) const noexcept {
        return shz_vec2_add(*this, rhs);
    }

    vec2 operator-(vec2 rhs) const noexcept {
        return shz_vec2_sub(*this, rhs);
    }

    vec2 &operator+=(vec2 rhs) noexcept {
        *this = shz_vec2_add(*this, rhs);
        return *this;
    }

    vec2 &operator-=(vec2 rhs) noexcept {
        *this = shz_vec2_sub(*this, rhs);
        return *this;
    }

};

}

#endif // SHZ_VECTOR_HPP
