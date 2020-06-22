#ifndef SPRING_H
#define SPRING_H

#include "particle.h"
#include "vec3.h"

typedef float real;

enum spring_type
{
	STRUCT_SPRING = 0, SHEAR_SPRING, BEND_SPRING
};

class spring
{
public:
	spring(particle *P0, particle *P1, real K, real d_c, spring_type sty)
		: p0(P0), p1(P1), k(K), d(d_c), st(sty)
	{
		l = (p1->p - p0->p).length(); // Rest Length.
	} 
	~spring() = default; 

	inline void eval_spring(); 

	particle *p0, *p1;
	real k, l, d; 
	spring_type st; 
};

// \\ 

void spring::eval_spring()
{
	// Spring Force k * (L - ||p0-p1||) * (p0-p1)/(||p0-q1||)
	// Damper Force = d * (p1.v - p0.v) | -d * (p0.v - p1.v)

	vec3<real> p0p1 = (p0->p - p1->p); real p0p1_l = p0p1.length();
	vec3<real> p0p1_v = (p0->v - p1->v);
	//k *= (l / p0p1_l); // Scale (k) by Edge Length. 

	vec3<real> s_force = (-k) * (l - p0p1_l) * p0p1.normalize();
	vec3<real> d_force = (-d) * p0p1_v;
	vec3<real> sd_force = s_force + d_force; //Fsd = -k(l0 - l) * (p0-p1) -d(v0 - v1)

	p0->f += sd_force, p1->f += -sd_force;
}



#endif 