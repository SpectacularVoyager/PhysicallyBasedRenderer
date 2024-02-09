#include"stb_image.h"
#include<GL/glew.h>
#include <GL/gl.h>
#include<GLFW/glfw3.h>
#include<iostream>
#pragma once

class Texture2D{
	private:
		unsigned int rendererID;
		int width, height, nrChannels;
	public:
		//source is for source data type RGBA for transparency RGB for non tranparency
		Texture2D(const char* path,unsigned int source,unsigned int dest,unsigned int type,bool flip,bool floating);
		Texture2D(int SCR_WIDTH,int SCR_HEIGHT);
		Texture2D(int SCR_WIDTH,int SCR_HEIGHT,unsigned int typeSource,unsigned int typeDestination);
		void Bind(int slot);
		void UnBind();
		void Free();
		void setProperty(unsigned int property,unsigned int value);
		unsigned int getID();

		void setWrapAndFilter(unsigned int WX,
				unsigned int WY,
				unsigned int FX,
				unsigned int FY
				);
		
};
