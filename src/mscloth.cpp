
#include "particle.h"

#include <iostream>
#include <vector>

std::size_t pt_N = 4; // NxN Size. 

void set_particles(std::vector<particle> *P_list)
{
	real r = 1.0f / pt_N;
	for (std::size_t j = 0; j < pt_N; ++j)
	{
		for (std::size_t i = 0; i < pt_N; ++i)
		{
			vec3<real> P((float)i * r, (float)j * r, 0.0f);
			P_list->emplace_back(particle(P, vec3<std::size_t>(i, j, 0)));
		}
	}
}

int main(int argc, char *argv[])
{
	std::vector<particle> pts; pts.reserve(pt_N * pt_N);
	// Springs. 

	set_particles(&pts);

	return 0; 
}