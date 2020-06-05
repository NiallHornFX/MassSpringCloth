#include "particle.h"

#include "spring.h"

particle::particle(const vec3<real> &P, const vec3<std::size_t> &IDX3D, const std::size_t IDX1D, const p_state State)
	: p(P), idx_3d(IDX3D), idx_1d(IDX1D), state(State) {}

particle::~particle()
{
	// Delete Springs
	for (spring *s : springs)
	{
		delete s; 
	}
	springs.clear();
}

// Oppose to Manually doing this inline within cloth set_springs(). 
void particle::add_spring(const particle &q, const spring_type type, const real K)
{
	switch (type) {
	default: std::terminate();
	case 0: springs.push_back(new struct_spring(this, &(const_cast<particle&>(q)), K)); break;
	case 1: springs.push_back(new shear_spring(this, &(const_cast<particle&>(q)), K)); break;
	case 2: springs.push_back(new bend_spring(this, &(const_cast<particle&>(q)), K)); break;
	}
}

void particle::calc_normal(const particle &p1, const particle &p2)
{
	// P1(idx +/- 1) P2(idx +/- pt_N)
	vec3<real> u = p1.p - p; vec3<real> v = p2.p - p;
	n = vec3<real>::cross(u, v).normalize();
}

void particle::dbg_springcount()
{
	std::cout << "DBG PARTICLE " << idx_1d << " " << "[" << idx_3d.x << "," << idx_3d.y << "," << idx_3d.z << "]" << " Spring Count == " << springs.size() << "\n";
}

