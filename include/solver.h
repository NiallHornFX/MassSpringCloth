#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <vec3.h>

// Solver Class, to Step Simulation.

class particle;
class spring; 

typedef float real; 

class solver
{
public:
	solver(std::vector<particle> *P_list, std::vector<spring> *S_list, real Dt);
	virtual ~solver() = default; 

	virtual void step();

protected:
	virtual void init(); 

	std::vector<particle> *p_list; 
	std::vector<spring> *s_list; 
	vec3<real> mg; 
	real dt; 
	
};

#endif 