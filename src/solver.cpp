#include "solver.h"
#include "spring.h"

solver::solver(cloth *Clothptr, real Dt)
	: Cloth(Clothptr), dt(Dt), mg(vec3<real>(0.0f, -2.50f, 0.0f))
{
	//
}

void solver::init()
{
	// Init Operations ..
}

void solver::step()
{
	for (std::size_t i = 0; i < Cloth->p_list.size(); ++i) // Pts 
	{
		particle &cur_pt = Cloth->p_list.at(i);
		vec3<real> cumlspringforce(0.0f); 
		vec3<real> cumlforce(0.0f);
		real max_springforce = 2.5f; 

		// Cumlate CurPts Spring's Force
		for (std::size_t j = 0; j < cur_pt.springs.size(); j++) // pt,Springs
		{
			cumlspringforce += cur_pt.springs.at(j)->eval_spring();  
		}

		/*
		// Limit Spring Force - 
		if (cumlspringforce.length() > max_springforce)
		{
			vec3<real> vn = cumlspringforce; vn.normalize(); 
			cumlspringforce = vn *= max_springforce;
		}
		*/

		// On Free Particles - 
		if (cur_pt.state == cur_pt.FREE)
		{
			cumlforce = cumlspringforce + mg;

			// Integrate Vel and Pos Using Forward Euler 
			cur_pt.v += dt * cumlforce;
			// Vel Max and Dampen - 
			cur_pt.v *= 0.980f; 
			cur_pt.p += dt * cur_pt.v;
		}
	}
}