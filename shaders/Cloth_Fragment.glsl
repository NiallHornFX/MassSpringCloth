#version 430 core 

out vec4 frag_colour; 

// Marhsalled Rasterized Vertex Attribs
in vec4 vN; // WorldSpace Normals

// Marshalled Rasterized Vertex Vars
in vec2 uv; // LocalSpace UV Coords
in vec4 wpos; // WorldSpacePos
in vec4 vpos; // ViewSpacePos
in vec4 ppos; // ProjectionSpacePos


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
	//frag_colour = vec4(wpos.xyz, 0.5); 
	//frag_colour = vec4((length(ppos.xyz) * vec3(0.025, 0.025, 1.0)) * d, 0.5); 
	
	frag_colour = vec4(abs(vN.x), abs(vN.y), abs(vN.z), 1.0); 
	
	// Checker - 
	float freq = 25.0; float amp = 100.0; 
    float f = clamp((cos(uv.x * freq) * sin(uv.y * freq)) * amp, 0.075, 1.0);
    vec3 col = vec3(f,f,f);
	//frag_colour = vec4(uv, 0.0, 0.75); 
	//frag_colour = vec4(col, 0.75); 
	
	// Points
	//frag_colour = vec4(gl_PointCoord.xy, 0.0, 1.0); 
}
