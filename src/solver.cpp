#include "solver.h"

#include "spring.h"

solver::solver(cloth *Clothptr, real Dt)
	: Cloth(Clothptr), dt(Dt), t_step(0), mg(vec3<real>(0.0f, -3.80f, 0.0f)) 
{}

void solver::init()
{
	// Init Operations ..
}

void solver::step()
{
	// Call Springs Eval
	for (spring *const s : Cloth->springs)
	{
		s->eval_spring();
	}

	// Integrate Forces on Particles
	for (std::size_t i = 0; i < Cloth->p_list.size(); ++i) // Pts 
	{
		particle &cur_pt = Cloth->p_list.at(i);

		// Break Left Con - 
		if (t_step == 300 && cur_pt.state == particle::FIXED && cur_pt.idx_3d.x == 0) // Must be LHS Pinned Pt. 
		{
			cur_pt.state = particle::FREE; 
		}

		// On Free Particles - 
		if (cur_pt.state == cur_pt.FREE)
		{
			// Integrate Vel and Pos Using Forward Euler 
			cur_pt.v += dt * (cur_pt.f + mg);
			cur_pt.v *= 0.980f; // Ad-hoc drag. 
			cur_pt.p += dt * cur_pt.v;
		}
	}

	t_step++;
}