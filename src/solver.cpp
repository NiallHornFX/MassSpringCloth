#include "solver.h"

#include "spring.h"

solver::solver(cloth *Clothptr, real Dt)
	: Cloth(Clothptr), dt(Dt), mg(vec3<real>(0.0f, -2.80f, 0.0f))
{
	//
}

void solver::init()
{
	// Init Operations ..
}

void solver::step()
{
	real d_c = 7.5f;

	// Call Springs Eval
	for (spring *const s : Cloth->springs)
	{
		s->eval_spring();
	}

	// Integrate Force on Particles
	for (std::size_t i = 0; i < Cloth->p_list.size(); ++i) // Pts 
	{
		particle &cur_pt = Cloth->p_list.at(i);

		// On Free Particles - 
		if (cur_pt.state == cur_pt.FREE)
		{
			// Dampening Force 
			vec3<real> dampforce = (-d_c) * cur_pt.v;

			// Integrate Vel and Pos Using Forward Euler 
			cur_pt.v += dt * (mg + cur_pt.f + dampforce);
			cur_pt.p += dt * cur_pt.v;
		}
	}
}