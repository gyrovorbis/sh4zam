/*! \file
    \brief Algorithm to speed up circle drawing.
    \ingroup misc

    \author Paul Cercueil

    \copyright MIT License
*/

#include "sh4zam/shz_vector.h"

static shz_vec4_t shz_calc_circle_matrix(shz_vec4_t a, shz_vec4_t b)
{
	return shz_vec4_init(a.x * b.x + a.y * b.z,
			     a.x * b.y + a.y * b.w,
			     a.z * b.x + a.w * b.z,
			     a.z * b.y + a.w * b.w);
}

shz_vec4_t shz_get_circle_matrix(unsigned int order, float factor)
{
	unsigned int i;
	float factor_squared = factor * factor;
	shz_vec4_t m = shz_vec4_init(1.0f, 1.0f / factor,
				     -1.0f / factor,
				     (factor_squared - 1.0f) / factor_squared);
	shz_vec4_t result = shz_vec4_init(1.0f, 0.0f, 0.0f, 1.0f);

	for (i = 0; i < order; i++)
		result = shz_calc_circle_matrix(result, m);

	return result;
}
