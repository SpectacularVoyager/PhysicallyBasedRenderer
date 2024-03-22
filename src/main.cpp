#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
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
#include"include/PBRMaterial.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

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
#define WIDTH  1920
#define HEIGHT 1080
static GLubyte *pixels = NULL;

unsigned int captureViews(Shader& shader,const int& map,Texture2D& texture,Mesh& mesh){
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] = 
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	
	shader.use();
	shader.setInt("equirectangularMap", map);
	shader.setMat4("projection", captureProjection);
	texture.Bind(map);
	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        shader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mesh.Bind();
		mesh.Draw();
    }
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,WIDTH,HEIGHT);
	return envCubemap;
}
static void create_ppm(char *prefix, int frame_id, unsigned int width, unsigned int height,
        unsigned int color_max, unsigned int pixel_nbytes, GLubyte *pixels) {
    size_t i, j, k, cur;
    enum Constants { max_filename = 256 };
    char filename[max_filename];
    snprintf(filename, max_filename, "%s%d.ppm", prefix, frame_id);
    FILE *f = fopen(filename, "w");
    fprintf(f, "P3\n%d %d\n%d\n", width, HEIGHT, 255);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            cur = pixel_nbytes * ((height - i - 1) * width + j);
            fprintf(f, "%3d %3d %3d ", pixels[cur], pixels[cur + 1], pixels[cur + 2]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
static void createPng(char* prefix,int frame_id, unsigned int width, unsigned int height,unsigned char* pixels){
    enum Constants { max_filename = 256 };
    char filename[max_filename];
    snprintf(filename, max_filename, "%s%d.png", prefix, frame_id);
	stbi_write_png(filename,WIDTH,HEIGHT,4,pixels,WIDTH*4);
}

int main(void)
{
    pixels = (unsigned char*)malloc(4 * WIDTH * HEIGHT);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    //window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", glfwGetPrimaryMonitor(), NULL);
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

	//Anti-Aliasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);  

	glDepthFunc(GL_LEQUAL);  
	

	VertexLayout layout;
	layout.addLayout(0, 3);
	layout.addLayout(1, 2);
	layout.addLayout(2, 3);


	Shader shader("res/shaders/blin/blin.vs","res/shaders/blin/blin.fs");
	shader.compile();


	Shader hdr("res/shaders/hdr/hdr.vs","res/shaders/hdr/hdr.fs");
	hdr.compile();

	Shader shaderEnv("res/shaders/env/env.vs","res/shaders/env/env.fs");
	shaderEnv.compile();

	Mesh mesh1("res/models/public/sphere.obj",layout,shader,0);
	Mesh mesh2("res/models/private/crate.obj",layout,shader,0);
	//Mesh mesh2("res/models/public/sphere.obj",layout,shader,0);

	Mesh cube("res/models/public/cube.obj",layout,hdr,0);
	Mesh cubeMap("res/models/public/cube.obj",layout,shaderEnv,0);

	Texture2D albedo("res/models/private/rusted/albedo.png",		GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE,false,false);
	Texture2D norm("res/models/private/rusted/normal.png",			GL_RGBA,GL_RGB,GL_UNSIGNED_BYTE,false,false);
	Texture2D metallic("res/models/private/rusted/metallic.png",	GL_RGBA,GL_RED,GL_UNSIGNED_BYTE,false,false);
	Texture2D roughness("res/models/private/rusted/roughness.png",	GL_RGBA,GL_RED,GL_UNSIGNED_BYTE,false,false);

	Texture2D albedo1("res/textures/Quartz/COL.png",		GL_RGBA,GL_RGB,GL_UNSIGNED_BYTE,false,false);
	Texture2D norm1("res/textures/Quartz/NRM.png",			GL_RGBA,GL_RGB,GL_UNSIGNED_BYTE,false,false);
	Texture2D metallic1("res/textures/Quartz/METALNESS.png",	GL_RGBA,GL_RED,GL_UNSIGNED_BYTE,false,false);
	Texture2D roughness1("res/textures/Quartz/ROUGHNESS.png",	GL_RGBA,GL_RED,GL_UNSIGNED_BYTE,false,false);

	PBRMaterial pbr1("res/models/public/sphere.obj",layout,shader,0,albedo,metallic,roughness,norm);
	//PBRMaterial pbr1("res/models/private/Dinklage.obj",layout,shader,0,albedo,metallic,roughness,norm);
	PBRMaterial pbr2("res/models/public/sphere.obj",layout,shader,0,albedo1,metallic1,roughness1,norm1);
	//PBRMaterial pbr2("res/models/private/Dinklage.obj",layout,shader,0,albedo1,metallic1,roughness1,norm1);

	PBRMaterial pbr3("res/models/private/Dinklage.obj",layout,shader,0,albedo1,metallic1,roughness1,norm1);
	Texture2D hdriMap("res/HDRI/meadow_4k.hdr",GL_RGB16F,GL_RGB,GL_FLOAT,true,true);
	hdriMap.setWrapAndFilter(GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);

	



	glm::mat4 proj=glm::perspective(glm::radians(45.0f), WIDTH/((float)HEIGHT),0.1f, 20.0f);
	glm::vec3 viewPos(0.0f,0.0f,10.0f);
	glm::mat4 view=glm::translate(glm::mat4(1.0),-viewPos);
	glm::vec3 lightPos(0.0f,0.0f,10.0f);


	pbr1.transform*=glm::translate(glm::mat4(1.0f),glm::vec3(-4.0f,0.0f,0.0f));	
	pbr2.transform*=glm::translate(glm::mat4(1.0f),glm::vec3( 4.0f,0.0f,0.0f));	
	pbr3.transform*=glm::scale(glm::mat4(1.0f),glm::vec3( 2.0f,2.0f,2.0f));	
	pbr3.transform*=glm::translate(glm::mat4(1.0f),glm::vec3( 0.0f,-2.0f,0.0f));	
	float theta=0;
	cube.transform*=glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-4.0f,0.0f)),glm::vec3(2.0f));

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	unsigned int envMap=captureViews(hdr,4,hdriMap,cube);
	
	int img_count=1;
	create_ppm("out/tmp", img_count++, WIDTH, HEIGHT, 255, 4, pixels);
	
    while (!glfwWindowShouldClose(window))
    {
		shader.use();
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glm::vec3 col(1.0,0.0,0.0);

		//mesh.transform*=glm::rotate(glm::mat4(1.0f), glm::radians( 0.5f),glm::vec3(0.0,1.0,0.0));
		theta+=0.01f;	
		float r=15.0f;
		lightPos=glm::vec3(r*std::cos(theta),0.0f,r*std::sin(theta));
		//lightPos=glm::vec3(10,0.0f,0.0f);
		
		pbr1.use(proj,view,viewPos,{lightPos});
		pbr2.use(proj,view,viewPos,{lightPos});
		pbr3.use(proj,view,viewPos,{lightPos});
		hdr.use();

		hdr.setMat4("view",view);
		hdr.setMat4("proj",proj);
		hdr.setInt("equirectangularMap",4);

		//cube.Bind();
		//cube.Draw();

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP,envMap);
		shaderEnv.use();
		shaderEnv.setMat4("view",view);
		shaderEnv.setMat4("proj",proj);
		shaderEnv.setInt("equirectangularMap",6);
		//cubeMap.Bind();
		//cubeMap.Draw();
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		//create_ppm("out/tmp", img_count++, WIDTH, HEIGHT, 255, 4, pixels);
		createPng("out/tmp",img_count++,WIDTH,HEIGHT,pixels);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	shader.deleteShader();

    glfwTerminate();
    return 0;
}

