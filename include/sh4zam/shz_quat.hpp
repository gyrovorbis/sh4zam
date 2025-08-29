/*! \file
    \brief C++ routines for operating upon quaternions.
    \ingroup quat
    \todo
    - to and from axis/angle
    - to and from matrix

    \author Falco Girgis
*/

#ifndef SHZ_QUAT_HPP
#define SHZ_QUAT_HPP

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
        quat() noexcept = default;

        SHZ_FORCE_INLINE quat(float w, float x, float y, float z) noexcept:
            shz_quat_t({w, x, y, z}) {}

        SHZ_FORCE_INLINE quat(shz_quat_t q) noexcept:
            shz_quat_t(q) {}

        SHZ_FORCE_INLINE static quat identity() noexcept {
            return shz_quat_identity();
        }
        
        SHZ_FORCE_INLINE static quat from_angles_xyz(float x, float y, float z) noexcept {
            return shz_quat_from_angles(x, y, z);
        }

        SHZ_FORCE_INLINE static quat from_axis_angle(vec3 axis, float angle) noexcept {
            return shz_quat_from_axis_angle(axis, angle);
        }

        SHZ_FORCE_INLINE static quat lerp(quat q, quat p, float t) noexcept {
            return shz_quat_lerp(q, p, t);
        }

        SHZ_FORCE_INLINE static quat slerp(quat q, quat p, float t) noexcept {
            return shz_quat_slerp(q, p, t);
        }

        bool operator==(quat rhs) const noexcept {
            return shz_quat_equals(*this, rhs);
        }

        SHZ_FORCE_INLINE void axis_angle(shz_vec3_t* axis, float* angle) const noexcept {
            shz_quat_to_axis_angle(*this, axis, angle);
        }

        SHZ_FORCE_INLINE auto axis_angle() const noexcept
            -> std::pair<vec3, float>
        {
            std::pair<vec3, float> ret;
            axis_angle(&std::get<0>(ret), &std::get<1>(ret));
            return ret;
        }

        SHZ_FORCE_INLINE void angles_xyz(float* x, float* y, float* z) const noexcept {
            return shz_quat_to_angles_xyz(*this, x, y, z);
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
            shz_quat_dot(*this, other);
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

    };
}

#endif