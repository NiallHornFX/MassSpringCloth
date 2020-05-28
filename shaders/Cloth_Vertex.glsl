#version 430 core 

layout (location = 0) in vec3 aPos;
out vec4 opos; 

uniform matrices
{
	mat4 model; 
	mat4 view; 
	mat4 projection; 
};


void main()
{
	//gl_Position = projection * view * model * vec4(aPos, 1.0);	
	gl_Position = view * model * vec4(aPos, 1.0);	
	opos = vec4(aPos,1.0); 
}