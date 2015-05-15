
// create camera
// window pointer
// then make primitive
// cube and triangle derive from primitve


// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>

#include <map>

#include "app.h"

#include "shaderprogram.h"

#include "texture.h"

static std::map<int, int> keyboard;

App::App(int width, int height, const char* name) : 
   m_WindowWidth(width)
 , m_WindowHeight(height)
 , m_WindowName(name)
 , m_prevTime(0) 
 , m_currTime(0)
{}

App::~App() { }

bool App::Initialise() {

    keyboard[GLFW_KEY_W] = GLFW_RELEASE;
    keyboard[GLFW_KEY_S] = GLFW_RELEASE;
    keyboard[GLFW_KEY_A] = GLFW_RELEASE;
    keyboard[GLFW_KEY_D] = GLFW_RELEASE;
    keyboard[GLFW_KEY_Q] = GLFW_RELEASE;
    keyboard[GLFW_KEY_E] = GLFW_RELEASE;

    if (!glfwInit()) {
        std::cerr << "Failed to initialise the GLFW." << std::endl;
        return false;
    }

    // create the window using GLFW
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x AA
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    this->window = glfwCreateWindow(
        this->m_WindowWidth,
        this->m_WindowHeight,
        this->m_WindowName.c_str(),
        glfwGetPrimaryMonitor(),
        NULL
    );
    if (!this->window) {
        std::cerr << "Failed to initialise the GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(this->window, App::KeyCallback);
    glfwSetErrorCallback(App::ErrorCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // setup OpenGL specific configuration
    std::cerr << "Using " << glGetString(GL_VERSION) << std::endl;
    // TODO work out what `glewExperimental = true;` does
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialise GLEW." << std::endl;
        return false;
    }
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LESS);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );

    // create our shaders
    
    ShaderProgram::Builder shaderProgramBuilder;
    //shaderProgramBuilder.buildShader("shaders/shader.vert", GL_VERTEX_SHADER);
    shaderProgramBuilder.buildShader("shaders/shader.frag", GL_FRAGMENT_SHADER);
    m_simpleShader = shaderProgramBuilder.getResult();

    if (m_simpleShader == nullptr) {
        std::cerr << "Shaders could not be loaded." << std::endl;
        return false;
    }

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(this->window, &width, &height);


    m_texture = new Texture("pal.png");

    return true;
}

void App::CleanUp() {
    delete this->m_simpleShader;
    glfwDestroyWindow(window);
    glfwTerminate();
}

float dt = 0.0f;
float cx = 0.0f;
float cy = 0.0f;
float scale = 1.0f;

bool App::Update() {
    if (glfwWindowShouldClose(this->window)) {
        return false;
    }   

    m_prevTime = m_currTime;
    m_currTime = glfwGetTime();
    dt = m_currTime - m_prevTime;

    if (keyboard[GLFW_KEY_W] == GLFW_PRESS) {
        cy -= scale * dt;
    }
    if (keyboard[GLFW_KEY_S] == GLFW_PRESS) {
        cy += scale * dt;
    }
    if (keyboard[GLFW_KEY_A] == GLFW_PRESS) {
        cx += scale * dt;
    }
    if (keyboard[GLFW_KEY_D] == GLFW_PRESS) {
        cx -= scale * dt;
    }
    if (keyboard[GLFW_KEY_Q] == GLFW_PRESS) {
        scale += scale * 1.2 * dt;
    }
    if (keyboard[GLFW_KEY_E] == GLFW_PRESS) {
        scale -= scale * 0.8 * dt;
    }

    glfwPollEvents();
    return true;
}



void App::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_simpleShader->use();
    GLuint timeId = glGetUniformLocation(m_simpleShader->id(), "time");
    glUniform1f(timeId, glfwGetTime());

    int samplerId = glGetUniformLocation(m_simpleShader->id(), "tex");
    glUniform1i(samplerId, 0);
    m_texture->bind();

    int centerId = glGetUniformLocation(m_simpleShader->id(), "center");
    glUniform2f(centerId, cx, cy);

    int scaleId = glGetUniformLocation(m_simpleShader->id(), "scale");
    glUniform1f(scaleId, scale);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(-1, -1);
    glTexCoord2f(1, 0);
    glVertex2f(1, -1);
    glTexCoord2f(1, 1);
    glVertex2f(1, 1);
    glTexCoord2f(0, 1);
    glVertex2f(-1, 1);
    glEnd(); 

    glfwSwapBuffers(window);
}

void App::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (action == GLFW_REPEAT)
        return;

    keyboard[key] = action;

}

void App::ErrorCallback(int error, const char* desc) {
    std::cerr << "error occured (" << error << "): " << desc << std::endl;
}
