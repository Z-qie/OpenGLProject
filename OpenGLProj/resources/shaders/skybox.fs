#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = pow(texture(skybox, TexCoords), vec4(2.5)); // make the brightness curve even more steep

    BrightColor = vec4(FragColor.xyz * 3.0, 1.0); // highlight the stars!!!!!!!!!!!!!!!!!!!!!!
}