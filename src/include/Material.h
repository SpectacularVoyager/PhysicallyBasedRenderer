#include"Shader.h"
#include <string>
#include <vector>
enum TYPE{
	INT,
	FLOAT,
	VEC3,
	MAT3,
	MAT4
};
struct Property{
	const std::string& name;
	const void* loc;
	TYPE type;
};
class Material{
	Shader& shader;
	Material(Shader& shader);
	std::vector<Property> properties;
	public:
	void addProperty(Property p);
	void Bind();
	void addProperty(const std::string& name,const void* vec,TYPE type);
	void addProperty(const std::string& name,const glm::vec3& vec);
	void addProperty(const std::string& name,const glm::mat4& vec);
	void addProperty(const std::string& name,const glm::mat3& vec);
	void addProperty(const std::string& name,const float& vec);
};
