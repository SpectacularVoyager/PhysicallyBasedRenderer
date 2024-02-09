#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include"stb_image.h"


class TextureCube{
	unsigned int texture_id;
	public:
		TextureCube();
		TextureCube(std::vector<std::string> vec);
		//~TextureCube();
};
