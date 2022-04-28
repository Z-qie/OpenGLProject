#pragma once

#include "Model.h"
#include "Skybox.h"
#include "Hdr.h"

#include <unordered_map>
#include <vector>

class Camera;
class Scene;

enum class UboIndex {
	UBOMatrices = 0,
	UBODirLight,
	UBOPointLights,
	UBOSpotLight
};
/***********************************************************************************/
class RenderSystem {
public:
	RenderSystem(GLuint w, GLuint h);
	void Init(const Scene& scene);
	void Update(const Camera& camera, const Scene& scene);
	void Shutdown() const;// I have changed resouces as shared pointer for convienence
	void Render(const Camera& camera, const Scene& scene);


private:
	// Sets the default state required for rendering
	void SetDefaultState();

	// Uniform buffer for projection and view matrix
	GLuint m_uboMatrices{ 0 };
	// Uniform buffer for dir light
	GLuint m_uboIndexDirLight{ 0 };
	// Uniform buffer for point light
	GLuint m_uboIndexPointLights{ 0 };
	// Uniform buffer for spot light
	GLuint m_uboIndexSpotLight{ 0 };

	// Projection matrix
	glm::mat4 m_projMatrix, m_lightSpaceMatrix;

	// skybox 
	Skybox m_skybox;
	// Post-processing (bloom) with HDR
	Hdr m_HDR;

	GLuint m_width{0};
	GLuint m_height{0};

};