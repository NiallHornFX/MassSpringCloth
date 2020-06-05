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
	spring(particle *P0, particle *P1, real K, spring_type sty)
		: p0(P0), p1(P1), k(K), st(sty)
	{
		l = (p1->p - p0->p).length(); // Rest Length.
	} 
	~spring() = default; 

	inline void eval_spring(); 

	particle *p0, *p1;
	real k, l;
	spring_type st; 
};


// \\ 

void spring::eval_spring()
{
	// k * (L - ||p-q||) * (p-q)/(||p-q||)
	vec3<real> pq = (p0->p - p1->p); real pql = pq.length();
	vec3<real> s_force =  k * (l - pql) * pq.normalize();

	p0->f = s_force; p1->f = -s_force;
}



#endif 