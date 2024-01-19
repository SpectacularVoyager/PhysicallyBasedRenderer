#include "include/Mesh.h"
#include <glm/matrix.hpp>


Mesh::Mesh( float* vertices, int n_vert
		, unsigned int* indices, int n_ind
		,VertexLayout& layout,Shader& shader):
shader(shader),layout(layout),vertices(vertices),indices(indices),n_vert(n_vert),n_ind(n_ind)
{
	transform=glm::mat4(1.0f);
	
    glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(unsigned int),indices,GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, n_vert*layout.getStride()*sizeof(float), vertices, GL_STATIC_DRAW);

	layout.setLayout();
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}
void Mesh::Bind(){
    glBindVertexArray(VAO); 
}
void Mesh::Unbind(){
    glBindVertexArray(0); 
}

void Mesh::Draw(){
	shader.setMat4("model",transform);
	glm::mat3 norm=glm::transpose(glm::inverse(transform));
	shader.setMat3("transform_norm",norm);
	glDrawElements(GL_TRIANGLES,n_ind,GL_UNSIGNED_INT,0);
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
Mesh::Mesh(const std::string& path,VertexLayout& layout,Shader& shader,int idx):
	shader(shader),layout(layout)
{
	std::vector<Vertex> p_vertices;
	std::vector<unsigned int> p_indices;
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 
	aiMesh* mesh=scene->mMeshes[idx];

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
				);

		glm::vec3 normals(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
				);

		glm::vec2 uv(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
				);
		Vertex vertex(position,uv,normals);
		p_vertices.push_back(vertex);

    }

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			p_indices.push_back(face.mIndices[j]);
	}  
	this->n_vert=mesh->mNumVertices;
	this->n_ind=mesh->mNumFaces*3;

	this->vertices=&p_vertices[0].position[0];
	this->indices=&p_indices[0];

	transform=glm::mat4(1.0f);
	
    glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,n_ind*sizeof(unsigned int),indices,GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, n_vert*layout.getStride()*sizeof(float), vertices, GL_STATIC_DRAW);

	layout.setLayout();
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}





