#ifndef SPRING_H
#define SPRING_H

#include "particle.h"
#include "vec3.h"

typedef float real;

// Spring ABC.
class spring
{
public:
	spring(particle *P0, particle *P1, real K)
		: p0(P0), p1(P1), k(K) 
	{
		l = (p1->p - p0->p).length(); // Rest Length.
	} 
	virtual ~spring() = default; 

	virtual inline vec3<real> eval_spring() = 0; 

	particle *p0, *p1;
	real k, l;
};

class struct_spring : public spring
{
public:
	struct_spring(particle *P0, particle *P1, real K)
		: spring(P0, P1, K) {}

	virtual inline vec3<real> eval_spring() override;
};

class shear_spring : public spring
{
public:
	shear_spring(particle *P0, particle *P1, real K)
		: spring(P0, P1, K) {}

	virtual inline vec3<real> eval_spring() override;
};

class bend_spring : public spring
{
public:
	bend_spring(particle *P0, particle *P1, real K)
		: spring(P0, P1, K) {}

	virtual inline vec3<real> eval_spring() override;
};



// DEF \\ 

// Structual Spring - 
vec3<real> struct_spring::eval_spring()
{
	// k * (L - ||p-q||) * (p-q)/(||p-q||)
	vec3<real> pq = (p0->p - p1->p); 
	return vec3<real>( (k * (l - pq.length()) * pq.normalize()) );
}

// Shear Spring - 
vec3<real> shear_spring::eval_spring()
{
	// k * (L - ||p-q||) * (p-q)/(||p-q||)
	vec3<real> pq = (p0->p - p1->p);
	return vec3<real>((k * (l - pq.length()) * pq.normalize()));
}


// Bend Spring - 
vec3<real> bend_spring::eval_spring()
{
	// k * (L - ||p-q||) * (p-q)/(||p-q||)
	vec3<real> pq = (p0->p - p1->p);
	return vec3<real>((k * (l - pq.length()) * pq.normalize()));
}


#endif 