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
	// Spring Force k * (L - ||p-q||) * (p-q)/(||p-q||)
	vec3<real> pq = (p0->p - p1->p); real pql = pq.length();
	vec3<real> s_force =  k * (l - pql) * pq.normalize();

	// Dampener Force = d * ((qv - pv) * ((p-q)/(||p-q||))) * (p-q)/(||p-q||)
	vec3<real> pqv = (p1->v - p0->v);
	vec3<real> d_force = d * pqv.dot(pq) * pq;

	// Divde Force by Rest Face Area ?
	real a = 1.0f / l * l; 
	s_force *= a; d_force *= a; 

	p0->f += s_force; p1->f += -s_force;
	p0->f += d_force; p1->f += -d_force;
}



#endif 