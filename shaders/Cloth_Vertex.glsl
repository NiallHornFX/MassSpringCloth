#version 430 core 

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aCol;

out vec4 opos; 
//out vec3 ocol; 

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 

void main()
{
	gl_PointSize = 10.0;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
	opos = projection * view * model * vec4(aPos, 1.0); 
	//ocol = aCol;
}