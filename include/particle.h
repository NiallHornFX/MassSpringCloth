#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec3.h"

extern const std::size_t pt_N;
typedef float real;

class spring; 

class particle
{
public:
	enum p_state
	{
		FREE = 0, FIXED
	};

	particle(const vec3<real> &P, const vec3<std::size_t> &IDX3D, const std::size_t IDX1D, const p_state State);
	~particle() = default; 

	vec3<real> p, v, f, n;
	vec3<real> sf, df; 
	std::size_t idx_1d; vec3<std::size_t> idx_3d;
	real mass;
	p_state state; 
protected:
	void calc_normal(const particle &p1, const particle &p2);
};


#endif