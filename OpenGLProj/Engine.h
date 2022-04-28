#pragma once

#include "Camera.h"
#include "Scene.h"
#include "WindowSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"

//#define FRAME_RATE

class Engine {
public:
    Engine(GLuint w, GLuint h, const char* title);

	// Load resouces and run update loop
	void Execute();

private:
	void ConnectWindowWithInput() const;
	void Shutdown() const;

private:
	Camera m_camera;
	Scene m_scene; // we dont have multiple scene, so I use the base scene only for this coursework
	WindowSystem m_windowSystem; // single window configuration
	RenderSystem m_renderer;   // single renderer
	
};