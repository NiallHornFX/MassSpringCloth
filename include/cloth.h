#ifndef CLOTH_H
#define CLOTH_H

#include "particle.h"

#include <vector>

typedef float real;

class cloth
{
public:
	cloth(std::size_t Nx, std::size_t Ny);
	~cloth() = default; 

	void set_particles();
	void set_springs(real Ks, real Kss, real Kb); 

	std::vector<particle> p_list; 
	std::size_t nx, ny; 

	real* get_ptVertexPos();
	// Get Particle-Vertex Pos, and Indices.. in POD format for GL. 
};

#endif 
