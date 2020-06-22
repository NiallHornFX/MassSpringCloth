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
	
	//real s_t = (real)t_step / (real) 1e+04; 
	//real x_s = std::sinf(s_t * 750.0f) * 5.0f; 
	//real z_s = std::cosf(s_t+0.54 * 600.0f) * 5.0f;
	//std::cout << s_t << "\n";
	//std::cout << x_s << "  " << z_s << "\n";
	//mg.x = x_s, mg.z = x_s; 

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
	for (std::size_t i = 0; i < Cloth->p_list.size(); ++i) // Pts 
	{
		particle &cur_pt = Cloth->p_list.at(i);
		// On Free Particles - 
		if (cur_pt.state == cur_pt.FREE)
		{
			// Tst Wind Force. 
			cur_pt.f += vec3<real>(sin(cur_pt.p.x * 50.0f) * 1.0f, sin(cos(cur_pt.p.y * 40.0f) * 50.0f) * 1.0f, cos(cur_pt.p.z * 100.0f) * 1.0f);

			// Air Resist - 
			//vec3<real> ar = -5.0f * cur_pt.v.squared(); // Not correct.

			// Integrate Forces on Particles
			//cur_pt.v += dt * (cur_pt.f + mg + ar);
			cur_pt.v += dt * (cur_pt.f + mg);
			//cur_pt.v *= 0.990f; // Ad-hoc drag. 
			cur_pt.p += dt * cur_pt.v;
		}
	}
}