#include <iostream>
#ifdef WIN32
#include <debugapi.h>
#else
#include <signal.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Renderer.h"
#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

int win_width = 1920;
int win_height = 1080;

Renderer renderer;

Game *game;

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length​, const GLchar *message, const void *userParam​)
{
    std::cout << source << type << id << severity << message << std::endl;
#ifdef WIN32
    DebugBreak();
#else
    raise(SIGTRAP);
#endif
}

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (action == GLFW_PRESS)
        game->keyboardDown(key);

    if (action == GLFW_RELEASE)
        game->keyboardUp(key);
}

static void resize_callback(GLFWwindow *win, int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
    GLFWwindow *window;

    glfwSetErrorCallback(error_callback);

    if (glfwInit() != GLFW_TRUE)
        exit(EXIT_FAILURE);

    std::cout << "Initialized GLFW" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(win_width, win_height, "fps: -", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    std::cout << "Created window" << std::endl;

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetWindowAspectRatio(window, 16, 9);
    glfwSetWindowSizeCallback(window, resize_callback);

    glewExperimental = GL_TRUE;

    auto err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Glew init error " << err << ": " << glewGetErrorString(err) << std::endl;
        if (err != 4)
        {
            exit(1);
        }
    }

    if (GLEW_KHR_debug)
    {
        std::cout << "Debug ext available! Enabled!" << std::endl;
        glEnable(GL_KHR_debug);
        glDebugMessageCallback(message_callback, NULL);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    game = new Game();

    int frame = 0;
    double elapsedTime, timebase = 0, lastTime = 0;
    while (!glfwWindowShouldClose(window))
    {

        frame++;
        elapsedTime = glfwGetTime();

        if (elapsedTime - timebase > 1)
        {
            float fps = frame / (elapsedTime - timebase);
            timebase = elapsedTime;
            frame = 0;
            char msg[30];
            sprintf(msg, "fps: %.3f", fps);
            glfwSetWindowTitle(window, msg);
        }

        game->update(elapsedTime - lastTime);
        lastTime = elapsedTime;

        game->draw(renderer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    return 0;
}
