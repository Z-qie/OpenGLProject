#pragma once

#include <string>
#include <vector>

#include "Shader.h"

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Material // dont acctually need many of them for this cw
{
    // Material Name
    std::string name;
    // Ambient Color
    glm::vec3 Ka;
    // Diffuse Color
    glm::vec3 Kd;
    // Specular Color
    glm::vec3 Ks;
    // Specular Exponent
    float Ns{0.f};
    // Optical Density
    float Ni{0.f};
    // Dissolve
    float d{0.f};
    // Illumination
    int illum{0};
    std::vector<Texture> textures;
};

class Mesh
{
public:
    // Default Constructor
    Mesh() {}

    // Variable Set Constructor
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
    {
        Vertices = vertices;
        Indices = indices;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        SetupMesh();
    }

    // render the mesh
    void Draw(std::shared_ptr<Shader> shader)
    {  // bind appropriate textures
        unsigned int ambientNr = 1;
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        
        shader->Use();
        // setup material as uniform
        // Ambient Color
        shader->setVec3("material.ambient", Material.Ka);
        // Diffuse Color m
        shader->setVec3("material.diffuse", Material.Kd);
        // Specular Colorm
        shader->setVec3("material.specular", Material.Ks);
        // Specular Exponent
        shader->setFloat("material.shiniess", Material.Ns);

        //std::cout << Material.Ns << std::endl;
        for (unsigned int i = 0; i < Material.textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = Material.textures[i].type;
            if (name == "texture_ambient")
                number = std::to_string(ambientNr++);
            else if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, Material.textures[i].id);
        }


        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

    // initializes all the buffer objects/arrays
    void SetupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glBindVertexArray(0);
    }

public:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    // Mesh Name
    std::string MeshName;
    // Vertex List
    std::vector<Vertex> Vertices;
    // Index List
    std::vector<unsigned int> Indices;

    // Material
    Material Material;
};
