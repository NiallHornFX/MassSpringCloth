
#include "cloth.h"
#include "vec3.h"
#include "spring.h"

cloth::cloth(std::size_t Nx, std::size_t Ny, const vec3<real> &sCoeff)
	: nx(Nx), ny(Ny)
{
	set_particles();
	set_springs(sCoeff);
}

void cloth::set_particles()
{
	std::size_t pt_N = nx; // Assume N*N Square
	real r = 1.0f / (pt_N-1.0f);

	for (std::size_t j = 0; j < pt_N; ++j)
	{
		for (std::size_t i = 0; i < pt_N; ++i)
		{
			vec3<real> P((float)i * r, (float)j * r, 0.0f);
			std::size_t idx_1d = i + pt_N * j; 
			if ((i == 0 && j == (pt_N - 1)) || (i == (pt_N - 1) && j == (pt_N - 1))) // Bound to Top Left + Right Corner. 
			//if (i == 0 || i == (pt_N - 1)) // Bound Left and Right Edge. 
			{
				p_list.emplace_back(P, vec3<std::size_t>(i, j, 0), idx_1d, particle::FIXED);
			}
			else
			{
				p_list.emplace_back(P, vec3<std::size_t>(i, j, 0), idx_1d, particle::FREE);
			}
		}
	}
}

// Set Particle Springs Along Each Axis (0 to N-1).
void cloth::set_springs(const vec3<real> &sCoeff)
{
	std::size_t NN = (pt_N * pt_N) - 1; // Max 1D Pt Index. 

	for (std::size_t j = 0; j < pt_N; ++j)
	{
		for (std::size_t i = 0; i < pt_N; ++i)
		{
			std::size_t idx_c = i + pt_N * j; // Cur Pt 1D IDX. 
			// Oppose to checking out of bounds on 1D Index, use Sepreate i,j 2D Components per dim. To Avoid Edge i+1,j+1 next row cases. 

			// Struct/Edge Springs 
			std::size_t idx_ii = (i + 1) + pt_N * j, idx_jj = (i + pt_N * (j + 1)); // Main 2D Struct Indices. 
			if (i + 1 <= (pt_N - 1)) p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(idx_ii), sCoeff.x)); // (i,j)|(i+1,j)
			if (j + 1 <= (pt_N - 1)) p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(idx_jj), sCoeff.x)); // (i,j)|(i,j+1)

			// Shear/Diagonal Springs
			std::size_t idx_sa = (i + 1) + pt_N * (j + 1); std::size_t idx_sb = (i - 1) + pt_N * (j + 1);
			if (i + 1 <= (pt_N - 1) && j + 1 <= (pt_N - 1)) p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at(idx_sa), sCoeff.y));
			if (i - 1 <= (pt_N - 1) && j + 1 <= (pt_N - 1)) p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at(idx_sb), sCoeff.y));
			// Each Diaglonal Shear Spring case? Can have 2 per particle if possible +/-i1,+j

			// Bend Spring
			std::size_t idx_ba = (i + 2) + pt_N * j; std::size_t idx_bb = i + pt_N * (j + 2);
			if (i+2 <= (pt_N - 1)) p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(idx_ba), sCoeff.y));
			if (j+2 <= (pt_N - 1)) p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(idx_bb), sCoeff.y));	
		}
	}
}

// Get Particlee Postions For GL Vertices - 
real* cloth::get_ptVertexPos()
{
	std::size_t w_spring = 0; 

	// P.x , P.y, P.z
	real *vpos = new real[p_list.size() * 3]; // MemLeak new Array each call ... 
	for (std::size_t i = 0, j = 2; i < p_list.size(); i++, j+=3)
	{
		//p_list[i].dbg_springcount();
		if (p_list[i].springs.size() != 0) w_spring++;

		vpos[j-2] = p_list[i].p.x;
		vpos[j-1] = p_list[i].p.y;
		vpos[j] = p_list[i].p.z;
	}
	//std::cout << "Total Pts with Springs == " << w_spring << " out of " << pt_N*pt_N << " Particles\n";
	return vpos; 
}