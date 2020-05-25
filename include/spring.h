#ifndef SPRING_H
#define SPRING_H

#include "particle.h"

typedef float real;

// Spring ABC.
class spring
{
public:
	spring(particle *P0, particle *P1, real K)
		: p0(P0), p1(P1), k(K) {} 
	virtual ~spring() = default; 

	virtual void eval_spring() = 0; 

	particle *p0, *p1;
	real k;
};

class struct_spring : public spring
{
	struct_spring(particle *P0, particle *P1, real K)
		: spring(P0, P1, K) {}

	virtual void eval_spring() override; 
};

#endif 