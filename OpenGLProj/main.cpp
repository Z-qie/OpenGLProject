#include "Engine.h"

// settings
const GLuint SCR_WIDTH = 1200 ;
const GLuint SCR_HEIGHT = 800;
const char* SCR_TITLE = "OPENGL_PROJECT";

int main() {
    Engine engine(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE);

    // instruction: ===========================================================================
    // 1. press P to summon fish
    // 2. press F to switch camera
    // 3. use mouse to free-look
    // 4. scroll to zoom
    // 5. press ESC to end
    // 6. use WSAD to control the player movement
    engine.Execute();
}