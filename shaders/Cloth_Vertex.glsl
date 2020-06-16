#version 430 core 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aN;

out vec4 mpos; 
out vec4 vpos; 
out vec4 ppos; 
out vec4 vN; 

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 

void main()
{
	gl_PointSize = 5.0;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vpos = view * model * vec4(aPos, 1.0);
	mpos = model * vec4(aPos, 1.0);
	ppos = projection * view * model * vec4(aPos, 1.0); 
	vN = projection * view * model * vec4(aN, 1.0); 
}