#pragma once
#include"Mesh.h"
#include"Texture.h"
#include <glm/fwd.hpp>

class PBRMaterial{
	Shader& shader;
	Texture2D& albedo;
	Texture2D& metallic;
	Texture2D& roughness;
	Texture2D& normal;


	public:
	Mesh& mesh;
	PBRMaterial(
			Mesh& mesh,
			Texture2D& albedo,
			Texture2D& metallic,
			Texture2D& roughness,
			Texture2D& normal
			);
	void use(const glm::mat4& proj,const glm::mat4& view,const glm::vec3& viewPos,std::vector<glm::vec3> lights);
};
