#version 410
in  vec3 vertNormal;
in  vec3 lightDir;
in  vec3 vertDir;
out vec4 fragColor;

void main() { 
	const vec4 ks = vec4(0.6, 0.6, 0.6, 0.0);
	const vec4 kd = vec4(0.6, 0.6, 0.6, 0.0); /// const means it cannot be changed just like C++
	const vec4 ka = 0.1 * kd;
	
	float diff = max(dot(vertNormal, lightDir), 0.0);
	vec3 reflection = normalize(reflect(lightDir, vertNormal));
	float spec = max(dot(vertDir, reflection), 0.0);
	if(diff > 0.0){
		spec = pow(spec,14.0);
	}
	fragColor =  ka + (diff * kd) + (spec * ks);	
} 

