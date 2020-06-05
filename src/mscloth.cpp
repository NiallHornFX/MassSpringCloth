// Simple Mass Spring Cloth Solver 

#include "cloth.h"
#include "display.h"
#include "solver.h" 

#include <iostream>
#include <vector>
#include <cstdio>

std::size_t pt_N = 16; // NxN Size. 
const real struct_c = 50.0f, shear_c = 50.0f, bend_c = 10.0f, damp_c = 7.5f;
const int width = 800, height = 600;

int main(int argc, char *argv[])
{
	display disp(width, height, 4, 3, "MSCloth");
	cloth Cloth(pt_N, pt_N, damp_c, vec3<real>(struct_c, shear_c, bend_c));

	// Pass Cloth to solver ...
	solver solve(&Cloth, (real) 1.0f / 240.0f);
	
	// External Sim and Render Step TODO (Encap into App class)
	std::size_t step = 0;
	while (step < 1000 && !(disp.shouldClose()))
	{
		// SIM STEP 
		solve.step();

		// RENDER STEP
		disp.vertex_update(Cloth.get_ptVertexPos()); // Updt Verts
		disp.render_step();
		std::cout << "Step = " << step++ << "\n";
	}
	return 0;
}