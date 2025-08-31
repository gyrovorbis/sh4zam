/*! \file
    \brief   C++ routines for operating upon quaternions.
    \ingroup quat

    \author    Falco Girgis
    \copyright MIT License
*/

#ifndef SHZ_QUAT_HPP
#define SHZ_QUAT_HPP

#include <compare>
#include <tuple>

#include "shz_quat.h"
#include "shz_vector.hpp"

//spaceship operator
//normalize()/normalized(), conjugate()/conjugated()
//overloaded arithmetic operators
//vector transform
//

namespace shz {

    class quat: public shz_quat_t {
    public:
        constexpr static float slerp_phi_epsilon = SHZ_QUAT_SLERP_PHI_EPSILON;

        quat() noexcept = default;

        SHZ_FORCE_INLINE quat(float w, float x, float y, float z) noexcept:
            shz_quat_t({w, x, y, z}) {}

        SHZ_FORCE_INLINE quat(shz_quat_t q) noexcept:
            shz_quat_t(q) {}

        SHZ_FORCE_INLINE static quat identity() noexcept {
            return shz_quat_identity();
        }
        
        SHZ_FORCE_INLINE static quat from_angles_xyz(float x, float y, float z) noexcept {
            return shz_quat_from_angles_xyz(x, y, z);
        }

        SHZ_FORCE_INLINE static quat from_axis_angle(vec3 axis, float angle) noexcept {
            return shz_quat_from_axis_angle(axis, angle);
        }

        SHZ_FORCE_INLINE static quat from_look_axis(vec3 forward, vec3 up) noexcept {
            return shz_quat_from_look_axis(forward, up);
        }

        SHZ_FORCE_INLINE static quat from_rotated_axis(vec3 start, vec3 end) noexcept {
            return shz_quat_from_rotated_axis(start, end);
        }

        SHZ_FORCE_INLINE static quat lerp(quat q, quat p, float t) noexcept {
            return shz_quat_lerp(q, p, t);
        }

        SHZ_FORCE_INLINE static quat slerp(quat q, quat p, float t) noexcept {
            return shz_quat_slerp(q, p, t);
        }

        SHZ_FORCE_INLINE auto &&operator[](this auto&& self, size_t index) noexcept {
            return std::forward<decltype(self)>(self).e[index];
        }

        SHZ_FORCE_INLINE auto begin(this auto&& self) noexcept {
            return &self[0];
        }

        SHZ_FORCE_INLINE auto end(this auto&& self) noexcept {
            return &self[4];
        }

        friend auto operator<=>(quat lhs, quat rhs) noexcept {
            return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
        }

        friend constexpr auto operator==(quat lhs, quat rhs) noexcept {
            return std::equal(lhs.begin(), lhs.end(),
                              rhs.begin(), rhs.end());
        }

        friend constexpr auto operator<(quat lhs, quat rhs) noexcept {
            return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
        }

        SHZ_FORCE_INLINE float angle() const noexcept {
            return shz_quat_angle(*this);
        }

        SHZ_FORCE_INLINE vec3 axis() const noexcept {
            return shz_quat_axis(*this);
        }

        SHZ_FORCE_INLINE void axis_angle(shz_vec3_t* axis, float* angle) const noexcept {
            shz_quat_axis_angle(*this, axis, angle);
        }

        SHZ_FORCE_INLINE auto axis_angle() const noexcept -> std::pair<vec3, float> {
            std::pair<vec3, float> aa;
            shz_quat_axis_angle(*this, &std::get<0>(aa), &std::get<1>(aa));
            return aa;
        }

        SHZ_FORCE_INLINE float magnitude_sqr() const noexcept {
            return shz_quat_magnitude_sqr(*this);
        }

        SHZ_FORCE_INLINE float magnitude() const noexcept {
            return shz_quat_magnitude(*this);
        }

        SHZ_FORCE_INLINE float magnitude_inv() const noexcept {
            return shz_quat_magnitude_inv(*this);
        }

        SHZ_FORCE_INLINE void normalize() noexcept {
            *this = shz_quat_normalize(*this);
        }

        SHZ_FORCE_INLINE void normalize_safe() noexcept {
            *this = shz_quat_normalize_safe(*this);
        }

        SHZ_FORCE_INLINE float dot(quat other) const noexcept {
            return shz_quat_dot(*this, other);
        }

        SHZ_FORCE_INLINE void conjugate() noexcept {
            *this = shz_quat_conjugate(*this);
        }

        SHZ_FORCE_INLINE void inverse() noexcept {
            *this = shz_quat_inverse(*this);
        }

        SHZ_FORCE_INLINE quat mult(quat rhs) const noexcept {
            return shz_quat_mult(*this, rhs);
        }

        SHZ_FORCE_INLINE quat add(quat rhs) const noexcept {
            return shz_quat_add(*this, rhs);
        }

        SHZ_FORCE_INLINE quat scale(float s) const noexcept {
            return shz_quat_scale(*this, s);
        }
    };
}

#endif
