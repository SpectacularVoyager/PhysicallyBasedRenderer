#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include<iostream>
#include"include/Shader.h"
#include"include/Texture.h"
#include"include/VertexLayout.h"
#include"include/Mesh.h"
#include <assimp/Importer.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* msg, const void* data);
float vertices[] = {
     1.0f,  1.0f, 0.0f,1.0f,1.0f,
     1.0f, -1.0f, 0.0f,1.0f,0.0f,
    -1.0f, -1.0f, 0.0f,0.0f,0.0f,
    -1.0f,  1.0f, 0.0f,0.0f,1.0f,
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  
#define WIDTH  600
#define HEIGHT 600

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	GLenum err = glewInit();


    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMessageCallback, NULL);
    glEnable(GL_DEPTH_TEST);



	Assimp::Importer importer;




	Shader shader("res/shaders/test.vs","res/shaders/test.fs");
	shader.compile();
	VertexLayout layout;
	layout.addLayout(0, 3);
	layout.addLayout(1, 2);
	Texture2D tex("res/textures/checkers.png",GL_RGB,false);
	tex.Bind(1);


	VertexLayout layout2;
	layout2.addLayout(0, 3);
	layout2.addLayout(1, 2);
	layout2.addLayout(2, 3);
	Mesh mesh1(vertices,4,indices,6,layout,shader);
	Mesh mesh2(vertices,4,indices,6,layout,shader);
	Mesh mesh3("res/models/Dinklage.obj",layout2,shader,0);
	mesh1.transform*=glm::translate(mesh1.transform,glm::vec3(-2.0,0.0,0.0));
	mesh2.transform*=glm::translate(mesh2.transform,glm::vec3( 2.0,0.0,0.0));


	glm::mat4 proj=glm::perspective(glm::radians(45.0f), WIDTH/((float)HEIGHT),0.1f, 20.0f);
	glm::vec3 viewPos(0.0f,2.0f,4.0f);
	glm::mat4 view=glm::translate(glm::mat4(1.0),-viewPos);
	glm::vec3 lightPos(4.0f,3.0f,4.0f);


	shader.use();
	float theta=0;

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
    while (!glfwWindowShouldClose(window))
    {
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glm::vec3 col(1.0,0.0,0.0);

		mesh1.transform*=glm::rotate(glm::mat4(1.0f), glm::radians(-0.1f),glm::vec3(0.0,1.0,0.0));
		mesh2.transform*=glm::rotate(glm::mat4(1.0f), glm::radians( 0.1f),glm::vec3(0.0,1.0,0.0));
		//mesh3.transform*=glm::rotate(glm::mat4(1.0f), glm::radians( 0.5f),glm::vec3(0.0,1.0,0.0));
		theta+=0.01f;	
		float r=4.0f;
		lightPos=glm::vec3(r*std::cos(theta),3.0f,r*std::sin(theta));
		
		shader.setMat4("view",view);
		shader.setMat4("proj",proj);
		shader.setVec3("lightPos",lightPos);
		shader.setVec3("viewPos",viewPos);

		shader.setVec3("light.position",lightPos);
		shader.setVec3("light.color",glm::vec3(4.0f));

		shader.setVec3("material.diffuse",glm::vec3(1.0f, 0.5f, 0.31f));
		shader.setVec3("material.specular",glm::vec3(0.1f));
		shader.setFloat("material.shininess",32.0f);
		shader.setInt("tex1",1);
		//mesh1.Bind();
		//mesh1.Draw();

		//mesh2.Bind();
		//mesh2.Draw();

		mesh3.Bind();
		mesh3.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	shader.deleteShader();

    glfwTerminate();
    return 0;
}

