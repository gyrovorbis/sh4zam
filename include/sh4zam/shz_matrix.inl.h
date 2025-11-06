//! \cond INTERNAL
/*! \file
    \brief   Implementation of C Matrix API
    \ingroup matrix

    This file provides the private implementation for all inlined funtions
    declared within the Matrix C API.

    \author    Falco Girgis
    \copyright MIT License
*/

SHZ_INLINE void shz_mat4x4_init_identity(shz_mat4x4_t* mat) SHZ_NOEXCEPT {
    shz_xmtrx_init_identity();
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT {
    shz_xmtrx_init_diagonal(x, y, z, w);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_scale(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_x(shz_mat4x4_t* mat, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_x(xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_y(shz_mat4x4_t* mat, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_y(yAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_z(shz_mat4x4_t* mat, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_z(zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_xyz(shz_mat4x4_t* mat, float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_xyz(xAngle, yAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_zyx(shz_mat4x4_t* mat, float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_zyx(zAngle, yAngle, xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation_yxz(shz_mat4x4_t* mat, float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation_yxz(yAngle, xAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_rotation(shz_mat4x4_t* mat, shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    shz_xmtrx_init_rotation(axis, angle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_translation(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_init_mult(shz_mat4x4_t* mat, const shz_mat4x4_t* lhs, const shz_mat4x4_t* rhs) SHZ_NOEXCEPT {
    shz_xmtrx_load_apply_store_4x4(mat, lhs, rhs);
}

SHZ_INLINE void shz_mat4x4_init_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_init_symmetric_skew(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_set_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT;

SHZ_INLINE void shz_mat4x4_init_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT {
	shz_mat4x4_set_rotation_quat(m, q);

	m->elem2D[3][0] = 0.0f;
	m->elem2D[3][1] = 0.0f;
	m->elem2D[3][2] = 0.0f;

	m->elem2D[0][3] = 0.0f;
	m->elem2D[1][3] = 0.0f;
	m->elem2D[2][3] = 0.0f;
	m->elem2D[3][3] = 1.0f;
}

SHZ_INLINE void shz_mat4x4_set_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    mat->elem2D[0][0] = x;
    mat->elem2D[1][1] = y;
    mat->elem2D[2][2] = z;
}

SHZ_INLINE void shz_mat4x4_set_diagonal(shz_mat4x4_t* mat, float x, float y, float z, float w) SHZ_NOEXCEPT {
    shz_mat4x4_set_scale(mat, x, y, z);
    mat->elem2D[3][3] = w;
}

SHZ_INLINE void shz_mat4x4_set_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    mat->pos.x = x;
    mat->pos.y = y;
    mat->pos.z = z;
}

SHZ_INLINE void shz_mat4x4_set_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT {
	m->elem2D[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	m->elem2D[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
	m->elem2D[2][0] = 2.0f * (q.x * q.z + q.y * q.w);

	m->elem2D[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
	m->elem2D[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
	m->elem2D[2][1] = 2.0f * (q.y * q.z - q.x * q.w);

	m->elem2D[0][2] = 2.0f * (q.x * q.z - q.w * q.y);
	m->elem2D[1][2] = 2.0f * (q.y * q.z + q.w * q.x);
	m->elem2D[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
}

SHZ_INLINE void shz_mat4x4_apply(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
    shz_xmtrx_load_apply_store_4x4(dst, dst, src);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_x(shz_mat4x4_t* mat, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_x(xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_y(shz_mat4x4_t* mat, float yAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_y(yAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_z(shz_mat4x4_t* mat, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_z(zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_xyz(shz_mat4x4_t* mat, float xAngle, float yAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_xyz(xAngle, yAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_zyx(shz_mat4x4_t* mat, float zAngle, float yAngle, float xAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_zyx(zAngle, yAngle, xAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_yxz(shz_mat4x4_t* mat, float yAngle, float xAngle, float zAngle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation_yxz(yAngle, xAngle, zAngle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation(shz_mat4x4_t* mat, shz_vec3_t axis, float angle) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_rotation(axis, angle);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_rotation_quat(shz_mat4x4_t* m, shz_quat_t q) SHZ_NOEXCEPT {
	shz_mat4x4_t rot;

	shz_mat4x4_init_rotation_quat(&rot, q);
	shz_mat4x4_apply(m, &rot);
}

SHZ_INLINE void shz_mat4x4_apply_symmetric_skew(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT{
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_symmetric_skew(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_INLINE void shz_mat4x4_apply_scale(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    shz_xmtrx_load_4x4(mat);
    shz_xmtrx_apply_scale(x, y, z);
    shz_xmtrx_store_4x4(mat);
}

SHZ_FORCE_INLINE void shz_mat4x4_apply_translation(shz_mat4x4_t* mat, float x, float y, float z) SHZ_NOEXCEPT {
    mat->pos.x += x;
    mat->pos.y += y;
    mat->pos.z += z;
}

SHZ_INLINE void shz_mat4x4_copy(shz_mat4x4_t* dst, const shz_mat4x4_t* src) SHZ_NOEXCEPT {
    asm volatile(R"(
        fschg

        pref    @%[dst]
        fmov.d  @%[src]+, xd0
        fmov.d  @%[src]+, xd2
        fmov.d  @%[src]+, xd4
        fmov.d  @%[src]+, xd6

        pref    @%[src]
        add     #32, %[dst]

        fmov.d  xd6, @-%[dst]
        fmov.d  xd4, @-%[dst]
        fmov.d  xd2, @-%[dst]
        fmov.d  xd0, @-%[dst]

        add     #32, %[dst]
        pref    @%[dst]

        fmov.d  @%[src]+, xd0
        fmov.d  @%[src]+, xd2
        fmov.d  @%[src]+, xd4
        fmov.d  @%[src]+, xd6

        add     #32, %[dst]
        fmov.d  xd6, @-%[dst]
        fmov.d  xd4, @-%[dst]
        fmov.d  xd2, @-%[dst]
        fmov.d  xd0, @-%[dst]

        fschg
    )"
    : [dst] "+&r" (dst), [src] "+&r" (src), "=m" (*dst)
    : "m" (*src));
}

SHZ_INLINE shz_vec3_t shz_mat4x4_trans_vec3(const shz_mat4x4_t* m, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec3_t out;

    register float fr0 asm("fr0") = v.x;
    register float fr1 asm("fr1") = v.y;
    register float fr2 asm("fr2") = v.z;
    register float fr3 asm("fr3") = 0.0f;

    register float fr4 asm("fr4") = m->elem2D[0][0];
    register float fr5 asm("fr5") = m->elem2D[0][1];
    register float fr6 asm("fr6") = m->elem2D[0][2];
    register float fr7 asm("fr7") = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    register float fr8  asm("fr8")  = m->elem2D[1][0];
    register float fr9  asm("fr9")  = m->elem2D[1][1];
    register float fr10 asm("fr10") = m->elem2D[1][2];
    register float fr11 asm("fr11") = 0.0f;

    asm volatile("fipr fv0, fv8"
        : "+f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));

    __atomic_thread_fence(1);

    out.x = fr7;

    __atomic_thread_fence(1);

    fr4 = m->elem2D[2][0];
    fr5 = m->elem2D[2][1];
    fr6 = m->elem2D[2][2];
    fr7 = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    out.y = fr11;
    out.z = fr7;

    return out;
}

SHZ_INLINE shz_vec4_t shz_mat4x4_trans_vec4(const shz_mat4x4_t* mat, shz_vec4_t in) SHZ_NOEXCEPT {
    SHZ_PREFETCH(mat);

    shz_vec4_t* v = &in;
    const shz_vec4_t* c[4] = {
        &mat->col[0], &mat->col[1], &mat->col[2], &mat->col[3]
    };

    asm volatile(R"(
        ! Load input vector into FV12
        fmov.s  @%[v]+, fr12
        fmov.s  @%[v]+, fr13
        fmov.s  @%[v]+, fr14
        fmov.s  @%[v]+, fr15

        ! Prefetch the second half of the matrix
        pref    @%[c2]

        ! Load first column int FV0
        fmov.s  @%[c0]+, fr0
        fmov.s  @%[c1]+, fr1
        fmov.s  @%[c2]+, fr2
        fmov.s  @%[c3]+, fr3
        ! Start loading next column
        fmov.s  @%[c0]+, fr4   ! Vector instructions need 3 cycles between
        fmov.s  @%[c1]+, fr5   ! loading arguments and using them.

        ! Calculate output vector's X component
        fipr    fv12, fv0

        ! Finish loading second column vector
        fmov.s  @%[c2]+, fr6
        fmov.s  @%[c3]+, fr7
        ! Begin loading third column vector
        fmov.s  @%[c0]+, fr8
        fmov.s  @%[c1]+, fr9

        ! Calculate output vector's Y componennt
        fipr    fv12, fv4

        ! Finish loading third column vector
        fmov.s  @%[c2]+, fr10
        add     #-16, %[v]      ! Point v back to the beginning of the input vector
        fmov.s  @%[c3]+, fr11
        fmov.s  fr3, @%[v]      ! Store output vector X component
        ! Start loading fourth column vector
        fmov.s  @%[c0]+, fr0

        ! Calculate output vector's Z component
        fipr    fv12, fv8

        ! Finish loading the fourth column vector
        fmov.s  @%[c1]+, fr1
        fmov.s  @%[c2]+, fr2
        fmov.s  @%[c3]+, fr3
        add     #4, %[v]        ! Advance output vector pointer
        fmov.s  fr7, @%[v]      ! Store output vector Y component

        ! Calculate output vector's W component
        fipr    fv12, fv0       ! FUCKING STALL - 4th column vector is still loading (3 cycle delay)

        ! Store output vector's Z component
        add     #4, %[v]        ! Advance output vector pointer
        fmov.s  fr11, @%[v]
        
        ! Store output vector's W component
        add     #4, %[v]        ! Advance output vector pointer
        fmov.s  fr3, @%[v]      ! FUCKING STALL - previous FIPR still in pipeline!
    )"
    : [v] "+r" (v), "=m" (in),
      [c0] "+r" (c[0]), [c1] "+r" (c[1]), [c2] "+r" (c[2]), [c3] "+r" (c[3])
    : "m" (in), "m" (*c[0]), "m" (*c[1]), "m" (*c[2]), "m" (*c[3])
    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7",
      "fr8", "fr9", "fr10", "fr11", "fr12", "fr13", "fr14", "fr15");

    return in;
}

SHZ_INLINE shz_quat_t shz_mat4x4_to_quat(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
	float f, s, m;

	if((f = mat->up.y + mat->left.x + mat->pos.z) >= 0.0f) {
		s = shz_inv_sqrtf(f + 1.0f);
		m = 0.5f * s;
		return shz_quat_init(
			shz_divf_fsrra(0.5f, s),
			(mat->up.z - mat->pos.y) * m,
			(mat->pos.x - mat->left.z) * m,
			(mat->left.y - mat->up.x) * m
		);
	}

	if((f = mat->left.x - mat->up.y - mat->pos.z) >= 0.0f) {
		s = shz_inv_sqrtf(f + 1.0f);
		m = 0.5f * s;
		return shz_quat_init(
			(mat->up.z - mat->pos.y) * m,
			shz_divf_fsrra(0.5f, s),
			(mat->up.x + mat->left.y) * m,
            (mat->up.x + mat->left.z) * m
		);
	}

	if ((f = mat->up.y - mat->left.x - mat->pos.z) >= 0.0f) {
		s = shz_inv_sqrtf(f + 1.0f);
		m = 0.5f * s;
		return shz_quat_init(
			(mat->pos.x - mat->left.z) * m,
			(mat->up.x - mat->left.y) * m,
			shz_divf_fsrra(0.5f, s),
			(mat->pos.y + mat->up.z) * m
		);
	}

	f = mat->pos.z - (mat->up.y + mat->left.x);
	s = shz_inv_sqrtf(f + 1.0f);
	m = 0.5f * s;
	return shz_quat_init(
		(mat->left.y - mat->up.x) * m,
		(mat->pos.x + mat->left.z) * m,
		(mat->pos.y + mat->up.z) * m,
		shz_divf_fsrra(0.5f, s)
	);
}

SHZ_INLINE float shz_mat4x4_determinant(const shz_mat4x4_t* mat) SHZ_NOEXCEPT {
	const float (*m)[4] = mat->elem2D;

	// Cache subfactors
	float s0 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float s1 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float s2 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float s3 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float s4 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float s5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

	shz_vec4_t coeff = shz_vec4_init(
		+ shz_dot8f(m[1][1], -m[1][2], m[1][3], 0.0f, s0, s1, s2, 0.0f),
		- shz_dot8f(m[1][0], -m[1][2], m[1][3], 0.0f, s0, s3, s4, 0.0f),
		+ shz_dot8f(m[1][0], -m[1][1], m[1][3], 0.0f, s1, s3, s5, 0.0f),
		- shz_dot8f(m[1][0], -m[1][1], m[1][2], 0.0f, s2, s4, s5, 0.0f)
	);

    return shz_dot8f(m[0][0],    m[0][1],    m[0][2],    m[0][3],
                     coeff.e[0], coeff.e[1], coeff.e[2], coeff.e[3]);
}

SHZ_INLINE void shz_mat4x4_transpose(const shz_mat4x4_t* mat, shz_mat4x4_t* out) SHZ_NOEXCEPT {
    shz_xmtrx_load_transpose_4x4(mat);
    shz_xmtrx_store_4x4(out);
}

SHZ_INLINE shz_vec3_t shz_mat3x3_trans_vec3(const shz_mat3x3_t* m, shz_vec3_t v) SHZ_NOEXCEPT {
    shz_vec3_t out;

    register float fr0 asm("fr0") = v.x;
    register float fr1 asm("fr1") = v.y;
    register float fr2 asm("fr2") = v.z;
    register float fr3 asm("fr3") = 0.0f;

    register float fr4 asm("fr4") = m->elem2D[0][0];
    register float fr5 asm("fr5") = m->elem2D[0][1];
    register float fr6 asm("fr6") = m->elem2D[0][2];
    register float fr7 asm("fr7") = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    register float fr8  asm("fr8")  = m->elem2D[1][0];
    register float fr9  asm("fr9")  = m->elem2D[1][1];
    register float fr10 asm("fr10") = m->elem2D[1][2];
    register float fr11 asm("fr11") = 0.0f;

    asm volatile("fipr fv0, fv8"
        : "+f" (fr11)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr8), "f" (fr9), "f" (fr10));

    __atomic_thread_fence(1);

    out.x = fr7;

    __atomic_thread_fence(1);

    fr4 = m->elem2D[2][0];
    fr5 = m->elem2D[2][1];
    fr6 = m->elem2D[2][2];
    fr7 = 0.0f;

    asm volatile("fipr fv0, fv4"
        : "+f" (fr7)
        : "f" (fr0), "f" (fr1), "f" (fr2), "f" (fr3),
          "f" (fr4), "f" (fr5), "f" (fr6));

    __atomic_thread_fence(1);

    out.y = fr11;
    out.z = fr7;

    return out;
}

//! \endcond
