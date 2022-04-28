#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

// UBO for view and projection 
layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

// uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords; 
out float LightStrength;

void main()
{   
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    // I didnt to calculate a parser for tangent and bitan....so this code is no need for this cw
    // mat3 normalMatrix = transpose(inverse(mat3(aInstanceMatrix)));
    // vec3 T = normalize(normalMatrix * aTangent);
    // vec3 N = normalize(normalMatrix * aNormal);
    // T = normalize(T - dot(T, N) * N);
    // vec3 B = cross(N, T);
    // mat3 TBN = transpose(mat3(T, B, N));   

    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;  
    // TexCoords = aTexCoords;
    TexCoords = aTexCoords * vec2(1.0, -1.0); // have to flip it cause OpenGL is a weird right-handed system
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);

    // control the blooming level by each tile's z position ******************************************************************
    // I've tried a lot of params
    // Height = pow(aInstanceMatrix[3].y, 30.0);
    // LightStrength = aInstanceMatrix[3].y / 6.0;
    LightStrength = pow(aInstanceMatrix[3].y, 3.0) * 0.2;
}