#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include<glm/glm.hpp>

class Shader{
		std::string vss;
		std::string fss;

		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;
	public:
		Shader(const std::string& vs,const std::string& fs);
		void compile();
		void use();
		void deleteShader();
		unsigned int getId();

		void setVec3(const char* name,const glm::vec3& vec);
		void setMat4(const char* name,const glm::mat4& vec);
		void setMat3(const char* name,const glm::mat3& vec);
		void setFloat(const char* name,const float& vec);
		void setInt(const char* name,const int& vec);
		unsigned int getLocation(const char* name);
};
