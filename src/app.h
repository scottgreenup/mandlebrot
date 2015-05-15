#ifndef _APP_HPP_
#define _APP_HPP_

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "texture.h"

class App {
public:
    App(int width, int height, const char* name);
    ~App();

    bool Initialise();
    void CleanUp();

    bool Update();
    void Render();

    static void KeyCallback(struct GLFWwindow* window, int key, int scancode, int action, int mods);
    static void ErrorCallback(int error, const char* desc);

private:

    Texture* m_texture;

    struct GLFWwindow* window;
    int m_WindowWidth;
    int m_WindowHeight;
    std::string m_WindowName;

    class ShaderProgram* m_simpleShader;

    float m_prevTime;
    float m_currTime;

};

#endif
