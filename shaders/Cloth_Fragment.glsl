#version 430 core 

out vec4 frag_colour; 

// Marhsalled Vertex Attribs
in vec4 vN; 

// Marshalled Vertex Vars
in vec4 wpos; 
in vec4 vpos; 
in vec4 ppos; 


uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 

void main()
{
	vec3 light = vec3(1.5, 2.0, -0.5); 
	vec3 LN = normalize(light - (vpos).xyz); 
	
	//vec3 vd = -vpos.xyz; 
	//vec3 rd = reflect(-LN, N); 
	
	float d = max(dot(vN.xyz, LN), 0.0); 
	//float s = pow(max(dot(vd, rd), 0.0), 32); 
	//s *= 1.0; 
	
	 
	// Tris 
	frag_colour = vec4(wpos.xyz, 0.5); 
	//frag_colour = vec4((length(ppos.xyz) * vec3(0.025, 0.025, 1.0)) * d, 0.5); 
	
	//frag_colour = vec4(vN.xyz, 1.0); 
	
	// Points
	//frag_colour = vec4(gl_PointCoord.xy, 0.0, 1.0); 
}
