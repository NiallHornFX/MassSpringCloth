// WIP !

#include "cloth.h"
#include "display.h"
#include "solver.h" 

#include <iostream>
#include <vector>
#include <cstdio>

#define USE_EXTERNAL_STEP 1

// Application Logic/Solve and Render step polls within main fornow. 

// Check Dtor Exception.
// Springs Dbg !

std::size_t pt_N = 32; // NxN Size. 
real struct_c = 0.5f, shear_c = 0.5f, bend_c = 0.5f; 
const int width = 800, height = 600;

int main(int argc, char *argv[])
{
	display disp(width, height, 4, 3, "MSCloth");
	cloth Cloth(pt_N, pt_N, vec3<real>(struct_c, shear_c, bend_c));

	// Pass Cloth to solver ...
	solver solve(&Cloth, (real) 1.0f / 120.0f);
	
	#if USE_EXTERNAL_STEP == 1
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
	#else
	disp.vertex_update(Cloth.get_ptVertexPos());
	disp.render_loop(1000); 
	return 0; 
	#endif
}