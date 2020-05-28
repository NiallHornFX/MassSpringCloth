#version 430 core 

in vec4 opos; 
out vec4 frag_colour; 

void main()
{
	frag_colour = vec4(opos.xyz * 10.0f, 1.0); 
}
