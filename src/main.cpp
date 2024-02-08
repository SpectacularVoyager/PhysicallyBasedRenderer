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
/**
	*float vertices[] = {
	*     1.0f,  1.0f, 0.0f,1.0f,1.0f,
	*     1.0f, -1.0f, 0.0f,1.0f,0.0f,
	*    -1.0f, -1.0f, 0.0f,0.0f,0.0f,
	*    -1.0f,  1.0f, 0.0f,0.0f,1.0f,
	*};
	*unsigned int indices[] = {  // note that we start from 0!
	*    0, 1, 3,   // first triangle
	*    1, 2, 3    // second triangle
	*};
*/ 
#define WIDTH  900
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



	VertexLayout layout;
	layout.addLayout(0, 3);
	layout.addLayout(1, 2);
	layout.addLayout(2, 3);


	Shader shader("res/shaders/blin/blin.vs","res/shaders/blin/blin.fs");
	shader.compile();

	Mesh mesh("res/models/sphere.obj",layout,shader,0);
	//Mesh mesh("res/models/Dinklage.obj",layout,shader,0);

	Texture2D tex("res/models/simpleGrass/Dry_Pebbles_Grassy_[4K]_Diffuse.jpg",GL_RGB,false);

	tex.Bind(1);



	glm::mat4 proj=glm::perspective(glm::radians(45.0f), WIDTH/((float)HEIGHT),0.1f, 20.0f);
	glm::vec3 viewPos(0.0f,0.0f,10.0f);
	glm::mat4 view=glm::translate(glm::mat4(1.0),-viewPos);
	glm::vec3 lightPos(3.0f,4.0f,4.0f);


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

		//mesh.transform*=glm::rotate(glm::mat4(1.0f), glm::radians( 0.5f),glm::vec3(0.0,1.0,0.0));
		theta+=0.01f;	
		float r=4.0f;
		lightPos=glm::vec3(r*std::cos(theta),2.0f,r*std::sin(theta));
		
		shader.setMat4("view",view);
		shader.setMat4("proj",proj);
		shader.setVec3("lightPos",lightPos);
		shader.setVec3("viewPos",viewPos);

		shader.setInt("tex1",1);

		mesh.Bind();
		mesh.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	shader.deleteShader();

    glfwTerminate();
    return 0;
}

