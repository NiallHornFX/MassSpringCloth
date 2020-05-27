// WIP !

#include "cloth.h"
#include "display.h"

#include <iostream>
#include <vector>
#include <cstdio>

// Do OGL Setup and Draw in here for now. 

std::size_t pt_N = 4; // NxN Size. 
const int width = 800, height = 600;

int main(int argc, char *argv[])
{
	display disp(width, height, 4, 3, "MSCloth");
	cloth Cloth(pt_N, pt_N);

	//real *tst = Cloth.get_ptVertexPos();
	disp.vertex_update(Cloth.get_ptVertexPos());
	
	disp.render_loop(1000); 

	return 0; 
}