#ifndef DISPLAY_H
#define DISPLAY_H

#include <vec3.h>

typedef float real; 
typedef unsigned char byte; 
typedef unsigned int uint; 

class GLFWwindow; 

class display
{
public:
	display(std::size_t W, std::size_t H, short major, short minor, const char *Title);
	display() = delete;
	~display();

	void vertex_update(real *const vertices);

	// Internal Render Loop
	void render_loop(std::size_t step_count);

	// Get Window Externally. 
	GLFWwindow* getwin();

protected:
	// Setup
	void window_context();
	void extensions_load();
	void shader_loader(const char *vert_path, const char *frag_path);
	void vertex_setup();

	// Util 
	void shader_checkCompile(const char *type);
	void shader_checkLink();

private:
	// Window 
	std::size_t width, height;
	const char *title;
	GLFWwindow *window;

	// Context 
	const byte *render_device;
	const byte *version;
	short gl_ver_major, gl_ver_minor;

	// Buffers
	uint Cloth_VAO, Cloth_VBO, Cloth_EBO;

	// Shaders
	uint cloth_vert_shader, cloth_frag_shader;
	uint cloth_shader_prog;

	// Geo Arrays
	real   *cloth_vertices;
	uint *cloth_indices;

	// Shader Code Buffers 
	const char *cloth_vert_shader_code, *cloth_frag_shader_code;
};

#endif