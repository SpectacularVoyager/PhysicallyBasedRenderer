#version 330 core

in vec3 norm; // the input variable from the vertex shader (same name and same type)  
in vec2 tex; // the input variable from the vertex shader (same name and same type)  
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

uniform sampler2D albedoMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;

const float PI = 3.14159265359;


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0*cosTheta;
    //return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnel(vec3 N,vec3 H, vec3 F0)
{
	float cosTheta=dot(N,H);
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


void main()
{
	//FragColor=vec4(norm.xyz,0.0);
	vec3 diffuse=pow(texture(albedoMap,tex).xyz,vec3(2.2));
	float metallic=texture(metallicMap,tex).x;
	float roughness=texture(roughnessMap,tex).x;
	vec3 normals=texture(normalMap,tex).xyz;
	vec3 V = normalize(viewPos - FragPos);
	vec3 N = normalize(norm);
	vec3 F0=vec3(0.04);
	F0=mix(F0,diffuse,metallic);


	vec3 res = vec3(0.0);



	//vec3 F = fresnelSchlick(max(dot(H, V), 0.0), vec3(0.0));
	vec3 L = normalize(lightPos - FragPos);
	float distance    = length(lightPos - FragPos);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance=attenuation*vec3(100.0);

	vec3 H = normalize(L + V);

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);   
	float G   = GeometrySmith(N, V, L, roughness);      
	vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 numerator    = NDF * G * F; 
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;	  
	float NdotL = max(dot(N, L), 0.0);                
	res+=(kD*diffuse/PI+specular)*NdotL*radiance;
	
	res=pow(res,vec3(1.0/2.2));
	FragColor=vec4(res,1.0);

} 
