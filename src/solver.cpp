#include "solver.h"
#include "spring.h"

solver::solver(cloth *Clothptr, real Dt)
	: Cloth(Clothptr), dt(Dt), mg(vec3<real>(0.0f, -1.8f, 0.0f))
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

		for (std::size_t j = 0; j < cur_pt.springs.size(); j++) // pt,Springs
		{
			// Cumlate CurPts Spring's Force
			cumlspringforce += cur_pt.springs.at(j)->eval_spring();  
		}
		
		if (cur_pt.state == cur_pt.FREE)
		{
			cumlforce = cumlspringforce + mg;

			// Integrate Vel and Pos. 
			cur_pt.v += dt * cumlforce;
			cur_pt.p += dt * cur_pt.v;
		}
	}
}