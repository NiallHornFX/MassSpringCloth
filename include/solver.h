#ifndef SOLVER_H
#define SOLVER_H

#include "cloth.h"
#include "vec3.h"

// Solver Class, to Step Simulation. Possible Derive Diffrent Integrators OR Just Implement Diffrent Integrator MFs to Base?
// Sub Solver are modualar MF/Operations makeup body of step() for a single TimeStep. (Oppose to Subclassing to Override Step MF for now).

typedef float real; 

class solver
{
public:
	solver(cloth *Clothptr, real Dt);
	virtual ~solver() = default; 

	virtual void step();

protected:
	virtual void init(); 

	// Sub Solver Operation MFs \\
	
	template <typename L>
	void for_each(const L &lam);

	// Constraint Breakage SubSolve Operations
	void conBreak_left(short frame); 

	// Integrator Implementations - 
	void integrate_ForwardEuler();


	cloth *Cloth;
	vec3<real> mg; 
	real dt; 
	std::size_t t_step;
	
};

#endif 