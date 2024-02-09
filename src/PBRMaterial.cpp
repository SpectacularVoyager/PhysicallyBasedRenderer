#include"include/PBRMaterial.h"


PBRMaterial::PBRMaterial(
		Mesh& mesh,
		Texture2D& albedo,
		Texture2D& metallic,
		Texture2D& roughness,
		Texture2D& normal):
	mesh(mesh),shader(mesh.getShader()),albedo(albedo),metallic(metallic),roughness(roughness),normal(normal)

{

}

void PBRMaterial::use(const glm::mat4& proj,const glm::mat4& view,const glm::vec3& viewPos,std::vector<glm::vec3> lights){
	albedo.Bind(0);
	metallic.Bind(1);
	roughness.Bind(2);
	normal.Bind(3);

	shader.setMat4("view",view);
	shader.setMat4("proj",proj);
	shader.setVec3("lightPos",lights[0]);
	shader.setVec3("viewPos",viewPos);

	shader.setInt("albedoMap",0);
	shader.setInt("metallicMap",1);
	shader.setInt("roughnessMap",2);
	shader.setInt("normalMap",3);

	mesh.Bind();
	mesh.DrawTransform();
}
