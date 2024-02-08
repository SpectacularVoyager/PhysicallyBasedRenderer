#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec2 texCoord; // the position variable has attribute position 0
layout (location = 2) in vec3 aNorm; // the position variable has attribute position 0
  

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat3 transform_norm;

out vec3 FragPos;
out vec3 norm;
out vec2 tex;

void main()
{
    gl_Position = proj*view*model*vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
	norm=normalize(transform_norm * aNorm);
	tex=texCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
}
