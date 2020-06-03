#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec3.h"

//#include <list>
#include <vector>

extern std::size_t pt_N;
typedef float real;

class spring; 

class particle
{
public:
	enum p_state
	{
		FREE = 0,
		FIXED
	};

	particle(const vec3<real> &P, const vec3<std::size_t> &IDX, const p_state State)
		: p(P), idx(IDX), state(State) {}
	~particle() = default;

	inline void dbg_springcount(); 

	static inline vec3<real> idxToLocal(const vec3<std::size_t> &idx_s);
	static inline vec3<std::size_t> localToIdx(const vec3<real> &world_s);
	static inline std::size_t idx_2dto1d(std::size_t i, std::size_t j); 

	//std::list<spring*> springs; 
	std::vector<spring*> springs; 
	vec3<real> p, a, v, n;
	vec3<std::size_t> idx; 
	real mass;
	p_state state; 
protected:
	void inline calc_normal(const particle &p1, const particle &p2);
};

//

void particle::calc_normal(const particle &p1, const particle &p2)
{
	// P1(idx +/- 1) P2(idx +/- pt_N)
	vec3<real> u = p1.p - p; vec3<real> v = p2.p - p;
	n = vec3<real>::cross(u, v).normalize();
}

void particle::dbg_springcount()
{
	std::cout << "DBG PARTICLE " << "[" << idx.x << "," << idx.y << "," << idx.z << "]" << " Spring Count == " << springs.size() << "\n";
}

std::size_t particle::idx_2dto1d(std::size_t i, std::size_t j)
{
	return i + pt_N * j; 
}

#endif