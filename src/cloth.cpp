
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
	//r *= 0.25f; 

	for (std::size_t j = 0; j < pt_N; ++j)
	{
		for (std::size_t i = 0; i < pt_N; ++i)
		{
			vec3<real> P((float)i * r, (float)j * r, 0.0f);
			//if ((i == 0 && j == (pt_N - 1)) || (i == (pt_N - 1) && j == (pt_N - 1))) // Bound to Top Left + Right. 
			//if (j == (pt_N - 1))
			if (i == 0 || i == (pt_N - 1))
			{
				p_list.emplace_back(particle(P, vec3<std::size_t>(i, j, 0), particle::FIXED));
			}
			else
			{
				p_list.emplace_back(particle(P, vec3<std::size_t>(i, j, 0), particle::FREE));
			}
		}
	}
}

// Edge, Inners, Corners | Indexing i + N * j. !Note i,j (0-(N-1)). 
void cloth::set_springs(const vec3<real> &sCoeff)
{
	/*
	// Edges 
	for (std::size_t i = 1; i < (pt_N - 1); ++i)
	{
		std::size_t idxc;

		idxc = i + pt_N * 0; // Bottom Edge (i, 0)
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at((i + 1) + pt_N * 0), sCoeff.x)); // ij,i+1j
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at(i + pt_N * 1), sCoeff.x)); // ij,ij+1
		// Shear Spring
		if ((i + 1) % 2 == 0) {p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at((i + 1) + pt_N * 1), sCoeff.y));} // ij,i+1j+1
		else { p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at((i - 1) + pt_N * 1), sCoeff.y)); } // ij,i-1j+1
		// Bend Spring
		if ((i + 2) <= (pt_N-1)) p_list.at(idxc).springs.push_back(new bend_spring(&p_list.at(idxc), &p_list.at((i + 2) + pt_N * 0), sCoeff.z)); //ij,i+2j

		idxc = i + pt_N * (pt_N - 1); // Top Edge (i, pt_N-1) 
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at((i + 1) + pt_N * (pt_N - 1)), sCoeff.x)); // ij,i+1j
		// Shear Spring
		// Bend Spring
		if ((i + 2) <= (pt_N-1)) p_list.at(idxc).springs.push_back(new bend_spring(&p_list.at(idxc), &p_list.at((i + 2) + pt_N * (pt_N - 1)), sCoeff.z)); //ij,i+2j

		idxc = 0 + pt_N * i; // Left Edge (0, i)
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at(1 + pt_N * i), sCoeff.x)); // ij,i+1j
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at(0 + pt_N * i + 1), sCoeff.x)); // ij,ij+1
		// Shear Spring
		if ((i + 1) % 2 == 0){p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at(1 + pt_N * (i + 1)), sCoeff.y));} // ij,i+1j+1
		else { p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at(1 + pt_N * (i - 1)), sCoeff.y)); }  // ij,i+1j-1
		// Bend Spring
		if ((i + 2) <= (pt_N-1)) p_list.at(idxc).springs.push_back(new bend_spring(&p_list.at(idxc), &p_list.at(0 + pt_N * (i + 2)), sCoeff.z)); //ij,i+2j

		idxc = pt_N + pt_N * i; // Right Edge (pt_N-1, i)
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at((pt_N - 1) + pt_N * i + 1), sCoeff.x)); // ij,i+1j
		// Shear Spring Bend Spring
	}

	// Inners
	for (std::size_t j = 1; j < (pt_N - 1); ++j)
	{
		for (std::size_t i = 1; i < (pt_N - 1); ++i)
		{
			std::size_t idx_c = i + pt_N * j;
			// Struct Spring
			p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at((i + 1) + pt_N * j), sCoeff.x)); // ij,i+1j
			p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(i + pt_N * (j + 1)), sCoeff.x)); // ij,ij+1
			// Shear Spring
			if ((i + 1) % 2 == 0){p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at((i + 1) + pt_N * (j + 1)), sCoeff.y));} // ij,i+1j+1
			else { p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at((i - 1) + pt_N * (j + 1)), sCoeff.y)); } // ij,i-1j+1

			// Bend Spring // Cant offset by 2 (if 3 --> 5, OOB ...)
			/// FIX ME
			//p_list.at(idx_c).springs.push_back(new bend_spring(&p_list.at(idx_c), &p_list.at((i + 2) + pt_N * j),  sCoeff.z)); //ij,i+2j
			//p_list.at(idx_c).springs.push_back(new bend_spring(&p_list.at(idx_c), &p_list.at(i + pt_N * (j + 2)),  sCoeff.z)); //ij,ij+2


		}
	}

	// Corners 
	// Bottom Left (0,0)
	std::size_t c_idx = 0 + pt_N * 0;
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at(1 + pt_N * 0), sCoeff.x)); // ij,i+1j
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at(0 + pt_N * 1), sCoeff.x)); // ij,ij+1
	p_list.at(c_idx).springs.push_back(new shear_spring(&p_list.at(c_idx), &p_list.at(1 + pt_N * 1), sCoeff.y)); // ij,i+1j+1
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at(2 + pt_N * 0), sCoeff.z)); //ij,i+2j
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at(0 + pt_N * 2), sCoeff.z)); //ij,ij+2
	// Bottom Right (pt_N-1, 0)
	c_idx = pt_N + pt_N * 0;
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at((pt_N - 1) + pt_N * 1), sCoeff.x)); // ij,ij+1
	p_list.at(c_idx).springs.push_back(new shear_spring(&p_list.at(c_idx), &p_list.at((pt_N - 2) + pt_N * 1), sCoeff.y)); // ij,i-1,j+1
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at((pt_N - 1) + pt_N * 2), sCoeff.z)); //ij,ij+2
	// Top Left (0, pt_N-1) 
	c_idx = 0 + pt_N * (pt_N-1);
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at(1 + pt_N * (pt_N - 1)), sCoeff.x));
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at(2 + pt_N * (pt_N - 1)), sCoeff.z)); //ij,i+2j
	// Top Right (pt_N-1, pt_N-1) 
	//c_idx = pt_N + pt_N * pt_N;
	*/

	for (std::size_t j = 0; j < pt_N; ++j)
	{
		for (std::size_t i = 0; i < (pt_N - 1); ++i)
		{
			std::size_t idx_c = i + pt_N * j, /* Cur Pt */ NN = (pt_N * pt_N) - 1; /* Max 1D Index */

			// Struct Spring
			std::size_t idx_ii = (i + 1) + pt_N * j,  idx_jj = (i + pt_N * (j + 1)); // 2D Struct Indices. 
			if (idx_ii > NN) continue; else p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(idx_ii), sCoeff.x)); // ij,i+1j
			if (idx_jj > NN) continue; else p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(idx_jj), sCoeff.x)); // ij,ij+1

			// Shear Spring
			std::size_t idx_iijj = (i+1) + pt_N * (j+1), idx_ijj = ((i-1) + pt_N * (j+1)); // 2D Shear Indices. 
			if (idx_iijj > NN) continue; else p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at(idx_iijj), sCoeff.y)); // ij,i+1j+1
			if (idx_ijj < 0 || idx_ijj > NN) continue; else p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at(idx_ijj), sCoeff.y)); // ij,i-1j+1
			
			// Bend Spring 
			std::size_t idx_iij = (i + 2) + pt_N * j;  idx_ijj = i + pt_N * (j + 2); // 2D Shear Indices. 
			if (idx_iij > NN) continue; else p_list.at(idx_c).springs.push_back(new bend_spring(&p_list.at(idx_c), &p_list.at(idx_iij), sCoeff.z));
			if (idx_ijj > NN) continue; else p_list.at(idx_c).springs.push_back(new bend_spring(&p_list.at(idx_c), &p_list.at(idx_ijj), sCoeff.z));
		}
	}

}

real* cloth::get_ptVertexPos()
{
	// P.x , P.y, P.z
	real *vpos = new real[p_list.size() * 3]; // MemLeak new Array each call ... 
	//std::cout << "DEBUG BEGIN\n";
	for (std::size_t i = 0, j = 2; i < p_list.size(); i++, j+=3)
	{
		vpos[j-2] = p_list[i].p.x;
		vpos[j-1] = p_list[i].p.y;
		vpos[j] = p_list[i].p.z;

		//p_list[i].dbg_springcount();
		//std::cout << "DEBUG Vertex = " << i << "[" << p_list[i].p.x << "," << p_list[i].p.y << "," << p_list[i].p.z << "]\n";
		//std::cout << "DEBUG V = " << i << "[" << p_list[i].v.x << "," << p_list[i].v.y << "," << p_list[i].v.z << "]\n";
	}
	///std::cout << "DEBUG END\n";
	return vpos; 
}