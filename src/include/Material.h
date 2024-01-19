#include"Shader.h"

class Material{
	Shader& shader;
	Material(Shader& shader);
	void setInt(const char* name,unsigned int x);
	void setVec3(const char* name,glm::vec3 vec);
	void setMat3(const char* name,glm::mat3 vec);
	void setMat4(const char* name,glm::mat4 vec);
};
