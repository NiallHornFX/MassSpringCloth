#version 430 core 

in vec4 opos; 
//in vec3 ocol; 

out vec4 frag_colour; 

void main()
{
	//frag_colour = vec4(ocol.xyz, 1.0); 
	//frag_colour = vec4(opos.xyz, 1.0); 
	frag_colour = vec4(gl_PointCoord.xy, 0.0, 1.0); 
}
