#ifndef SOLVER_H
#define SOLVER_H

#include "cloth.h"
#include "vec3.h"

// Solver Class, to Step Simulation.

typedef float real; 

class solver
{
public:
	solver(cloth *Clothptr, real Dt);
	virtual ~solver() = default; 

	virtual void step();

protected:
	//virtual void init(); 

	cloth *Cloth;
	vec3<real> mg; 
	real dt; 
	
};

#endif 