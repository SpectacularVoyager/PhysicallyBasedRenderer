#pragma once
#include"VertexLayout.h"
#include"Shader.h"
#include <glm/fwd.hpp>
#include <vector>
#include<assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh{
		const float* vertices;
		const unsigned int* indices;
		VertexLayout& layout;
		Shader& shader;
		unsigned int VBO,VAO,EBO;
		unsigned int n_vert,n_ind;

	public:
		glm::mat4 transform;

		Mesh( float* vertices, int n_vert
				, unsigned int* indices, int n_ind
				,VertexLayout& layout,Shader& shader);

		Mesh(const std::string& path,VertexLayout& layout,int idx);
		Mesh(const std::string& path,VertexLayout& layout);

		Mesh(const std::string& path,VertexLayout& layout,Shader& shader,int idx);
		void Bind();
		void DrawTransform();
		void Unbind();
		void Draw();
		Shader& getShader(){return shader;}
		~Mesh();
};

struct Vertex{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normals;
	Vertex(glm::vec3 position,glm::vec2 uv,glm::vec3 normals){
		this->position=position;
		this->uv=uv;
		this->normals=normals;
	}
};
class VertexMesh{
		const float* vertices;
		const unsigned int* indices;
		VertexLayout& layout;
		Shader& shader;
		unsigned int VBO,VAO,EBO;
		unsigned int n_vert,n_ind;

	public:
		glm::mat4 transform;
		VertexMesh(const std::string& path,VertexLayout& layout,int idx);
		VertexMesh(const std::string& path,VertexLayout& layout);
		void Bind();
		void Unbind();
		void Draw();
		~VertexMesh();
};
