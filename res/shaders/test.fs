#version 330 core
out vec4 FragColor;
  
in vec3 norm; // the input variable from the vertex shader (same name and same type)  
in vec2 tex; // the input variable from the vertex shader (same name and same type)  
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
	vec3 color;
};
uniform Light light;
uniform Material material;
uniform sampler2D tex1;
void main()
{

	vec4 col=texture(tex1,tex);
	vec3 lightDir=normalize(light.position-FragPos);
	float distance=length(light.position-FragPos);
	lightDir=normalize(lightDir);
	vec3 diffuse=max(0.0,dot(norm,lightDir))*light.color*col.xyz;
	//vec3 diffuse=max(0.0,dot(norm,lightDir))*light.color*material.diffuse;

	vec3 viewDir=-normalize(FragPos-viewPos);
	vec3 reflectDir=normalize(reflect(-lightDir,norm));
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)*material.specular;
	vec3 res=(specular+diffuse)/(distance);
    FragColor =vec4(res,1.0);
    //FragColor =col;
} 
