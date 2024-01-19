#include"include/Texture.h"

Texture2D::Texture2D(const char* path,unsigned int source,bool flip){
	glGenTextures(1,&rendererID);
	glBindTexture(GL_TEXTURE_2D,rendererID);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	setProperty(GL_TEXTURE_WRAP_S, GL_REPEAT);	
	setProperty(GL_TEXTURE_WRAP_T, GL_REPEAT);
	setProperty(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setProperty(GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    stbi_set_flip_vertically_on_load(flip); 
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, source, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}
Texture2D::Texture2D(int SCR_WIDTH,int SCR_HEIGHT){
	glGenTextures(1,&rendererID);
	glBindTexture(GL_TEXTURE_2D,rendererID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	setProperty(GL_TEXTURE_WRAP_S, GL_REPEAT);	
	setProperty(GL_TEXTURE_WRAP_T, GL_REPEAT);
	setProperty(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setProperty(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
Texture2D::Texture2D(int SCR_WIDTH,int SCR_HEIGHT,unsigned int typeSource,unsigned int typeDest){
	glGenTextures(1,&rendererID);
	glBindTexture(GL_TEXTURE_2D,rendererID);

    glTexImage2D(GL_TEXTURE_2D, 0, typeSource , SCR_WIDTH, SCR_HEIGHT, 0, typeDest, GL_UNSIGNED_BYTE, NULL);

	setProperty(GL_TEXTURE_WRAP_S, GL_REPEAT);	
	setProperty(GL_TEXTURE_WRAP_T, GL_REPEAT);
	setProperty(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setProperty(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void Texture2D::setProperty(unsigned int property,unsigned int value){
	glTexParameteri(GL_TEXTURE_2D, property, value);
}
void Texture2D::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);	
	glBindTexture(GL_TEXTURE_2D, rendererID);
}
unsigned int Texture2D::getID(){return rendererID;}
