#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec3.h"

extern std::size_t pt_N;
typedef float real;


class particle
{
public:
	particle(vec3<real> P, vec3<std::size_t> IDX)
		: p(P), idx(IDX) {}

	vec3<real> p, a, v, n;
	vec3<std::size_t> idx; 
	real mass;

	static inline vec3<real> idxToLocal(const vec3<std::size_t> &idx_s);
	static inline vec3<std::size_t> localToIdx(const vec3<real> &world_s);

protected:
	void calc_normal(vec3<real> up, const particle &p1, const particle &p2);
};

#endif