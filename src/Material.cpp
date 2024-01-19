#include"include/Material.h"

//template<typename T>void Shader::addProperty(Property<T> p);
void Material::addProperty(const std::string& name,const void* vec,TYPE type){
	properties.push_back(Property(name,vec,type));	
}
void Material::addProperty(const std::string& name,const glm::vec3& vec);
void Material::addProperty(const std::string& name,const glm::mat4& vec);
void Material::addProperty(const std::string& name,const glm::mat3& vec);
void Material::addProperty(const std::string& name,const float& vec);


void Material::Bind(){
	shader.use();
}
