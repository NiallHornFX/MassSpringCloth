
#include "cloth.h"
#include "vec3.h"
#include "spring.h"

cloth::cloth(std::size_t Nx, std::size_t Ny)
	: nx(Nx), ny(Ny)
{
	set_particles();
	set_springs(0.5f, 0.5f, 0.5f);
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
			if ((i == 0 && j == 0) || (i == pt_N && j == 0))
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
void cloth::set_springs(real ks, real kss, real kb)
{
	// Edges 
	for (std::size_t i = 1; i < (pt_N - 1); ++i)
	{
		std::size_t idxc;

		idxc = i + pt_N * 0; // Bottom Edge (i, 0)
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at((i + 1) + pt_N * 0), 1.0f)); // ij,i+1j
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at(i + pt_N * 1), 1.0f)); // ij,ij+1
		// Shear Spring
		if ((i + 1) % 2 == 0) {p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at((i + 1) + pt_N * 1), 1.0f));} // ij,i+1j+1
		else { p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at((i - 1) + pt_N * 1), 1.0f)); } // ij,i-1j+1
		// Bend Spring
		if ((i + 2) <= (pt_N-1)) p_list.at(idxc).springs.push_back(new bend_spring(&p_list.at(idxc), &p_list.at((i + 2) + pt_N * 0), 1.0f)); //ij,i+2j

		idxc = i + pt_N * (pt_N - 1); // Top Edge (i, pt_N-1) 
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at((i + 1) + pt_N * (pt_N - 1)), 1.0f)); // ij,i+1j
		// Shear Spring
		// Bend Spring
		if ((i + 2) <= (pt_N-1)) p_list.at(idxc).springs.push_back(new bend_spring(&p_list.at(idxc), &p_list.at((i + 2) + pt_N * (pt_N - 1)), 1.0f)); //ij,i+2j

		idxc = 0 + pt_N * i; // Left Edge (0, i)
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at(1 + pt_N * i), 1.0f)); // ij,i+1j
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at(0 + pt_N * i + 1), 1.0f)); // ij,ij+1
		// Shear Spring
		if ((i + 1) % 2 == 0){p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at(1 + pt_N * (i + 1)), 1.0f));} // ij,i+1j+1
		else { p_list.at(idxc).springs.push_back(new shear_spring(&p_list.at(idxc), &p_list.at(1 + pt_N * (i - 1)), 1.0f)); }  // ij,i+1j-1
		// Bend Spring
		if ((i + 2) <= (pt_N-1)) p_list.at(idxc).springs.push_back(new bend_spring(&p_list.at(idxc), &p_list.at(0 + pt_N * (i + 2)), 1.0f)); //ij,i+2j

		idxc = pt_N + pt_N * i; // Right Edge (pt_N-1, i)
		// Struct Spring
		p_list.at(idxc).springs.push_back(new struct_spring(&p_list.at(idxc), &p_list.at((pt_N - 1) + pt_N * i + 1), 1.0f)); // ij,i+1j
		// Shear Spring Bend Spring
	}

	// Inners
	for (std::size_t j = 1; j < (pt_N - 1); ++j)
	{
		for (std::size_t i = 1; i < (pt_N - 1); ++i)
		{
			std::size_t idx_c = i + pt_N * j;
			// Struct Spring
			p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at((i + 1) + pt_N * j), 1.0f)); // ij,i+1j
			p_list.at(idx_c).springs.push_back(new struct_spring(&p_list.at(idx_c), &p_list.at(i + pt_N * (j + 1)), 1.0f)); // ij,ij+1
			// Shear Spring
			if ((i + 1) % 2 == 0){p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at((i + 1) + pt_N * (j + 1)), 1.0f));} // ij,i+1j+1
			else { p_list.at(idx_c).springs.push_back(new shear_spring(&p_list.at(idx_c), &p_list.at((i - 1) + pt_N * (j + 1)), 1.0f)); } // ij,i-1j+1

			// Bend Spring // Cant offset by 2 (if 3 --> 5, OOB ...)
			/// FIX ME
			//p_list.at(idx_c).springs.push_back(new bend_spring(&p_list.at(idx_c), &p_list.at((i + 2) + pt_N * j), 1.0f)); //ij,i+2j
			//p_list.at(idx_c).springs.push_back(new bend_spring(&p_list.at(idx_c), &p_list.at(i + pt_N * (j + 2)), 1.0f)); //ij,ij+2


		}
	}

	// Corners 
	// Top Left (0,0)
	std::size_t c_idx = 0 + pt_N * 0;
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at(1 + pt_N * 0), 1.0f)); // ij,i+1j
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at(0 + pt_N * 1), 1.0f)); // ij,ij+1
	p_list.at(c_idx).springs.push_back(new shear_spring(&p_list.at(c_idx), &p_list.at(1 + pt_N * 1), 1.0f)); // ij,i+1j+1
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at(2 + pt_N * 0), 1.0f)); //ij,i+2j
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at(0 + pt_N * 2), 1.0f)); //ij,ij+2
	// Top Right (pt_N-1, 0)
	c_idx = pt_N + pt_N * 0;
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at((pt_N - 1) + pt_N * 1), 1.0f)); // ij,ij+1
	p_list.at(c_idx).springs.push_back(new shear_spring(&p_list.at(c_idx), &p_list.at((pt_N - 2) + pt_N * 1), 1.0f)); // ij,i-1,j+1
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at((pt_N - 1) + pt_N * 2), 1.0f)); //ij,ij+2
	// Bottom Left (0, pt_N-1) 
	c_idx = 0 + pt_N * (pt_N-1);
	p_list.at(c_idx).springs.push_back(new struct_spring(&p_list.at(c_idx), &p_list.at(1 + pt_N * (pt_N - 1)), 1.0f));
	p_list.at(c_idx).springs.push_back(new bend_spring(&p_list.at(c_idx), &p_list.at(2 + pt_N * (pt_N - 1)), 1.0f)); //ij,i+2j
	// Bottom Right (pt_N-1, pt_N-1) 
	//c_idx = pt_N + pt_N * pt_N;
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
		//std::cout << "DEBUG Vertex = " << i << "[" << p_list[i].p.x << "," << p_list[i].p.y << "," << p_list[i].p.z << "]\n";
		//std::cout << "DEBUG V = " << i << "[" << p_list[i].v.x << "," << p_list[i].v.y << "," << p_list[i].v.z << "]\n";
	}
	///std::cout << "DEBUG END\n";
	return vpos; 
}