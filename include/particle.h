#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec3.h"

#include <vector>

extern std::size_t pt_N;
typedef float real;

class spring; 

class particle
{
public:
	enum p_state
	{
		FREE = 0, FIXED
	};
	enum spring_type
	{
		STRUCT = 0, SHEAR, BEND
	};

	particle(const vec3<real> &P, const vec3<std::size_t> &IDX3D, const std::size_t IDX1D, const p_state State);
	~particle();

	void add_spring(const particle &q, const spring_type type, const real K);

	//static inline vec3<real> idxToLocal(const vec3<std::size_t> &idx_s);
	//static inline vec3<std::size_t> localToIdx(const vec3<real> &world_s);
	static inline std::size_t idx_2dto1d(std::size_t i, std::size_t j); 

	void dbg_springcount();

	std::vector<spring*> springs; 
	vec3<real> p, a, v, n;
	std::size_t idx_1d; vec3<std::size_t> idx_3d;
	real mass;
	p_state state; 
protected:
	void calc_normal(const particle &p1, const particle &p2);
};

//

std::size_t particle::idx_2dto1d(std::size_t i, std::size_t j)
{
	return i + pt_N * j;
}


#endif