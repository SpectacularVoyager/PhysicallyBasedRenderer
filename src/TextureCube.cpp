#include "include/TextureCube.h"
#include <vector>

void loadFaces(const std::vector<std::string> str){
	int width,height,nrChannels;
	for(int i=0;i<str.size();i++){
    const void *data = stbi_load(str[i].c_str(), &width, &height, &nrChannels, 0);
	glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
	}
}

TextureCube::TextureCube(){
	glGenTextures(1,&texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP,texture_id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
TextureCube::TextureCube(std::vector<std::string> str):TextureCube(){
	loadFaces(str);
}
