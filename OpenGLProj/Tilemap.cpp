
#include <cmath>
#include "Tilemap.h"
#include "RenderSystem.h"
#include "InputSystem.h"

Tilemap::Tilemap() {
    // load source
    m_pShader = std::make_shared<Shader>("resources/shaders/tile.vs", "resources/shaders/tile.fs");
    m_pModel = std::make_shared<Model>("resources/objects/tile/tile.obj");

    m_modelMatrices = new glm::mat4[m_mapSizeX * m_mapSizeY];
    m_curRippleHeights = new float[m_mapSizeX * m_mapSizeY];
    m_preRippleHeights = new float[m_mapSizeX * m_mapSizeY];

    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float heightOffset = 1.0f;

    for (size_t i = 0; i < m_mapSizeX; i++)
    {
        for (size_t j = 0; j < m_mapSizeY; j++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            float x = i * m_tileSize;
            //float y = heightOffset * ((rand() % 50) / 50.0f);
            float y = 0.f;
            float z = j * m_tileSize;

            model = glm::translate(model, glm::vec3(x, y, z));
            model = glm::scale(model, glm::vec3(0.01f, 0.02f, 0.01f));

            // add to list of matrices
            m_modelMatrices[i + j * m_mapSizeX] = model;
            m_curRippleHeights[i + j * m_mapSizeX] = 0;
        }
    }

    // configure instanced array
    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_mapSizeX * m_mapSizeY * sizeof(glm::mat4), &m_modelMatrices[0], GL_DYNAMIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    for (unsigned int i = 0; i < m_pModel->LoadedMeshes.size(); i++)
    {
        unsigned int VAO = m_pModel->LoadedMeshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void Tilemap::Update(const float deltatime) {
    m_curTime = glfwGetTime();
    if (m_nextRippleTime - m_curTime <= 0) {
        Ripple(deltatime);
        m_nextRippleTime = m_curTime + m_rippleInterval;
    }

    Entity::Update(deltatime);
}

void Tilemap::Ripple(float deltatime) {

    int centerTileX = GetMapXAt((int)m_PlayerTransform->GetGlobalPosition().x);
    int centerTileY = GetMapYAt((int)m_PlayerTransform->GetGlobalPosition().z);
    ////std::cout << centerTileX << ", " << centerTileY << std::endl;
    //  controlled height
    float heightOffset = 4.f;

    if (IsInBound(centerTileX, centerTileY)) {
        for (int i = 0; i < m_mapSizeX; i++)
        {
            for (int j = 0; j < m_mapSizeY; j++)
            {
                int offsetX = i - centerTileX;
                int offsetY = j - centerTileY;
                int sqrDist = offsetX * offsetX + offsetY * offsetY;
                if (sqrDist <= 25)
                    m_modelMatrices[i + j * m_mapSizeX][3][1] = heightOffset;
            }
        }
    }

    // keep recording last frame
    for (int i = 0; i < m_mapSizeX; i++)
    {
        for (int j = 0; j < m_mapSizeY; j++)
        {
            m_preRippleHeights[i + j * m_mapSizeX] = m_curRippleHeights[i + j * m_mapSizeX];
            m_curRippleHeights[i + j * m_mapSizeX] = m_modelMatrices[i + j * m_mapSizeX][3][1];
        }
    }

    // update ripple
    for (int i = 0; i < m_mapSizeX; i++)
    {
        for (int j = 0; j < m_mapSizeY; j++)
        {
            // random animation 
            //model[3][1] = heightOffset * ((rand() % 50) / 50.0f);
            float p11 = m_preRippleHeights[i + j * m_mapSizeX];

            float d = 0.0f;

            if (j - 1 >= 0) {
                int down = i + (j - 1) * m_mapSizeX;
                float p10 = m_curRippleHeights[down]; // down
                d += p10;
            }

            if (j + 1 < m_mapSizeY) {
                int up = i + (j + 1) * m_mapSizeX;
                float p01 = m_curRippleHeights[up]; // up
                d += p01;
            }

            if (i + 1 < m_mapSizeX) {
                int right = i + 1 + j * m_mapSizeX;
                float p21 = m_curRippleHeights[right];  // right
                d += p21;
            }

            if (i - 1 >= 0) {
                int left = i - 1 + j * m_mapSizeX;
                float p12 = m_curRippleHeights[left];  // left
                d += p12;
            }

            d = d / 2.0f - p11;
            d *= 0.99f;

            m_modelMatrices[i + j * m_mapSizeX][3][1] = d;
        }
    }

    if (IsInBound(centerTileX, centerTileY)) {
        for (int i = 0; i < m_mapSizeX; i++)
        {
            for (int j = 0; j < m_mapSizeY; j++)
            {
                int offsetX = i - centerTileX;
                int offsetY = j - centerTileY;
                int sqrDist = offsetX * offsetX + offsetY * offsetY;
                if (sqrDist <= 16)
                    m_modelMatrices[i + j * m_mapSizeX][3][1] = heightOffset;
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_mapSizeX * m_mapSizeY * sizeof(glm::mat4), &m_modelMatrices[0], GL_DYNAMIC_DRAW);

}

void Tilemap::DrawSelfAndChild(const Camera& camera)
{
    if (m_pShader) { // set up shader
        m_pShader->Use();

        // UBO VP
        GLuint matricesIndex = glGetUniformBlockIndex(m_pShader->ID, "Matrices");
        glUniformBlockBinding(m_pShader->ID, matricesIndex, (GLuint)UboIndex::UBOMatrices);
        // UBO light
        GLuint DirLightIndex = glGetUniformBlockIndex(m_pShader->ID, "DirLightUBO");
        glUniformBlockBinding(m_pShader->ID, DirLightIndex, (GLuint)UboIndex::UBODirLight);
        // UBO Point Light
        GLuint PointLightIndex = glGetUniformBlockIndex(m_pShader->ID, "PointLightsUBO");
        glUniformBlockBinding(m_pShader->ID, PointLightIndex, (GLuint)UboIndex::UBOPointLights);

        //m_pShader->setMat4("model", m_transform.GetModelMatrix());
        m_pShader->setVec3("viewPos", camera.Position);
        m_pShader->setInt("blinn", true);
    }

    if (m_pModel) {
        //m_pModel->Draw(m_pShader);
          //std::cout << Material.Ns << std::endl;
        unsigned int ambientNr = 1;
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < m_pModel->LoadedMeshes[0].Material.textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = m_pModel->LoadedMeshes[0].Material.textures[i].type;
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
            glUniform1i(glGetUniformLocation(m_pShader->ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, m_pModel->LoadedMeshes[0].Material.textures[i].id);
        }

        for (unsigned int i = 0; i < m_pModel->LoadedMeshes.size(); i++)
        {
            glBindVertexArray(m_pModel->LoadedMeshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(m_pModel->LoadedMeshes[i].Indices.size()), GL_UNSIGNED_INT, 0, m_mapSizeX * m_mapSizeY);
            glBindVertexArray(0);
        }
    }

    for (auto&& child : children)
    {
        child->DrawSelfAndChild(camera);
    }
}

bool Tilemap::IsInBound(int x, int y) const {
    if (x >= 0 && x < m_mapSizeX && y >= 0 && y < m_mapSizeY)
        return true;
    return false;
}
// Using the base tiles Position, work out which tile the point is over
int Tilemap::GetMapXAt(int curX) const
{
    return (curX - m_baseOffsetX) / m_tileSize;
}

// Using the base tiles Position, work out which tile the point is over
int Tilemap::GetMapYAt(int curY) const
{
    return (curY - m_baseOffsetY) / m_tileSize;
}