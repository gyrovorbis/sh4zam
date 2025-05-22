#ifndef SHZ_MATRIX_HPP
#define SHZ_MATRIX_HPP

#include "shz_matrix.h"
#include "shz_vector.hpp"

namespace shz {
    template<typename M>
    concept matrix = 
    requires(M m) {

    };

    template<typename CRTP>
    class matrix_mxn {

    };


    struct matrix_2x2: shz_matrix_2x2 {
        vec2 row(size_t idx) const noexcept;
        vec2 &col(size_t idx) noexcept;

        void load() const noexcept;
        void load_transpose() const noexcept;
        void store() noexcept;
        void store_transpose();
        void copy(const matrix2_2 &other) noexcept;

        void transpose();
        void invert();
        void invert_full();
        float determinant();

        void set_identity() noexcept;
        void set_diagonal(vec2 vec) noexcept;
        void set_scale(vec2 vec) noexcept;
        void set_rotation_z(float radians) noexcept;

        void scale(vec2 vec);
        void rotate(float rads);

        vec2 get_scale() const noexcept;
        float get_rotation() const noexcept;
        bool is_identity() const noexcept;

        float normal_error() const noexcept;
        float orthogonal_error() const noexcept;
        float identity_error() const noexcept;

        void apply_scale(vec2 vec) noexcept;
        void apply_rotation_z(float radians) noexcept;

        void transform(const vec2 *in, vec2* out, size_t count) const noexcept;
        void transform_inverse(const vec2 *in, vec2* out, size_t count) const noexcept;

        auto mdspan(size_t... extents) const noexcept;
    };
}

#endif
