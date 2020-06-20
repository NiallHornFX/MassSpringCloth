#include "solver.h"

#include "spring.h"

solver::solver(cloth *Clothptr, real Dt)
	: Cloth(Clothptr), dt(Dt), t_step(0), mg(vec3<real>(0.0f, -4.80f, 0.0f)) 
{
	//init();
}

void solver::init()
{
	// Init Operations ..
}

void solver::step()
{
	// Sub Solve Calls \\

	// Call Springs Eval
	for (spring *const s : Cloth->springs)
	{
		s->eval_spring();
	}

	// Constraint Ops -
	//conBreak_left(300);

	// Integration - 
	integrate_ForwardEuler();

	t_step++;
}

// Sub Solver Operation Member Functions Implementations \\
// Sub Solvers - Have own Particle Loop. 

// SubSolve - Constraints \\

void solver::conBreak_left(short frame)
{
	for (particle &cur_pt : Cloth->p_list)
	{
		// Break Left Con -
		if (t_step == frame && cur_pt.state == particle::FIXED && cur_pt.idx_3d.x == 0) // Must be LHS Pinned Pt.
		{
			cur_pt.state = particle::FREE;
			cur_pt.f *= 0.2f; // Dampen.
		}
	}
}


// SubSolve - Integrators \\ 

// Forward/Explicit Euler Integration - 
void solver::integrate_ForwardEuler()
{
	// Integrate Forces on Particles
	for (std::size_t i = 0; i < Cloth->p_list.size(); ++i) // Pts 
	{
		particle &cur_pt = Cloth->p_list.at(i);
		// On Free Particles - 
		if (cur_pt.state == cur_pt.FREE)
		{
			// Air Resist - 
			vec3<real> ar = -5.0f * cur_pt.v.squared();

			// Integrate Vel and Pos Using Forward (Explict) Euler 
			cur_pt.v += dt * (cur_pt.f + mg + ar);
			//cur_pt.v *= 0.980f; // Ad-hoc drag. 
			cur_pt.p += dt * cur_pt.v;
		}
	}
}