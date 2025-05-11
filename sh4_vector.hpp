#ifndef SH4_VECTOR_HPP
#define SH4_VECTOR_HPP

#include "sh4_vector.h"

namespace sh4 {

struct vec2: public sh4_vec2_t {

    vec2() = default;

    vec2(sh4_vec2_t vec2) noexcept:
        sh4_vec2_t(vec2)
    {}

    float dot(vec2 rhs) const noexcept {
        return sh4_vec2_dot(*this, rhs);
    }

    float magnitudeSqr(void) const noexcept {
        return sh4_vec2_magnitude_sqr(*this);
    }

    float magnitude(void) const noexcept {
        return sh4_vec2_magnitude(*this);
    }

    void normalize(void) noexcept {
        *this = sh4_vec2_normalize(*this);
    }

    void normalizeSafe(void) noexcept {
        *this = sh4_vec2_normalize_safe(*this);
    }

    float distance(vec2 other) const noexcept {
        return sh4_vec2_distance(*this, other);
    }

    float cross(vec2 other) const noexcept {
        return sh4_vec2_cross(*this, other);
    }

    vec2 operator+(vec2 rhs) const noexcept {
        return sh4_vec2_add(*this, rhs);
    }

    vec2 operator-(vec2 rhs) const noexcept {
        return sh4_vec2_sub(*this, rhs);
    }

    vec2 &operator+=(vec2 rhs) noexcept {
        *this = sh4_vec2_add(*this, rhs);
        return *this;
    }

    vec2 &operator-=(vec2 rhs) noexcept {
        *this = sh4_vec2_sub(*this, rhs);
        return *this;
    }

};

}

#endif // SH4_VECTOR_HPP
