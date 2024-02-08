#version 330 core

in vec3 norm; // the input variable from the vertex shader (same name and same type)  
in vec2 tex; // the input variable from the vertex shader (same name and same type)  
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

uniform sampler2D tex1;
void main()
{
	FragColor=vec4(norm.xyz,0.0);
} 
