#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords; 

void main()
{   
    FragPos = aPos;
    // I dont like to calculate a parser for tan and bitan....
  
    Normal = aNormal;  
    // TexCoords = aTexCoords * vec2(1.0, -1.0); // have to flip it cause OpenGL is a right-handed system
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}