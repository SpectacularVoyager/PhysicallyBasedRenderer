#include"include/Shader.h"
#include <cstdio>
#include <fstream>
#include<iostream>


void checkShaderErrors(unsigned int shader,int line){
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER COMPILATION FAILED AT LINE:"<<line<<"\n" << infoLog << std::endl;
	}
}
void readFile(const std::string& file,std::string& str){
	std::ifstream stream(file);
	std::string buffer;
	while(getline(stream,buffer)){
		str+=buffer+"\n";
	}
	stream.close();
}

Shader::Shader(const std::string& vs,const std::string& fs)
{
	readFile(vs,vss); 
	readFile(fs,fss); 
	//std::cout<<vss;
	//std::cout<<fss;
}
void Shader::compile(){
	const char* vss_ptr=vss.c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vss_ptr, NULL);
    glCompileShader(vertexShader);
	int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	const char* fss_ptr=fss.c_str();
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fss_ptr, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
   shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){
	glUseProgram(shaderProgram);
}
void Shader::deleteShader(){
    glDeleteProgram(shaderProgram);
}

unsigned int Shader::getId(){
	return shaderProgram;
}
void Shader::setVec3(const char* name,const glm::vec3& vec){
	unsigned int loc=getLocation(name);
	glUniform3fv(loc,1,(float*)&vec);
}
void Shader::setMat4(const char* name,const glm::mat4& vec){
	unsigned int loc=getLocation(name);
	glUniformMatrix4fv(loc,1,GL_FALSE,(float*)&vec);
}

void Shader::setFloat(const char* name,const float& vec){
	unsigned int loc=getLocation(name);
	glUniform1f(loc,vec);
}

void Shader::setMat3(const char* name,const glm::mat3& vec){
	unsigned int loc=getLocation(name);
	glUniformMatrix3fv(loc,1,GL_FALSE,(float*)&vec);
}
void Shader::setInt(const char* name,const int& vec){
	unsigned int loc=getLocation(name);
	glUniform1i(loc,vec);
}
unsigned int Shader::getLocation(const char* name){
	return glGetUniformLocation(shaderProgram,name);
}

