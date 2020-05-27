
#include "cloth.h"

#include <iostream>
#include <vector>

// Do OGL Setup and Draw in here for now. 

std::size_t pt_N = 4; // NxN Size. 

int main(int argc, char *argv[])
{
	cloth Cloth(pt_N, pt_N);
	real *tst = Cloth.get_ptVertexPos();

	return 0; 
}