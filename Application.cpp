#include <iostream>
#ifndef WIN32
#include <signal.h>
#endif
#include <GL/glew.h>
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Renderer.h"
#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

int win_width = 1280;
int win_height = 720;

Renderer renderer;

Game *game;

void display()
{

    game->draw(renderer);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
    game->reshape(width, height);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
    case 'q':
    case 'Q':
        exit(0);
    }
    game->keyboardDown(key, x, y);
}

void keyUp(unsigned char key, int x, int y)
{
    game->keyboardUp(key, x, y);
}

int frame = 0, elapsedTime, timebase = 0, lastTime = 0;
void idleFunc()
{
    frame++;
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);

    if (elapsedTime - timebase > 1000)
    {
        float fps = frame * 1000.0 / (elapsedTime - timebase);
        timebase = elapsedTime;
        frame = 0;
        char msg[30];
        sprintf(msg, "fps: %.3f", fps);
        glutSetWindowTitle(msg);
    }
    if (elapsedTime - lastTime > 0)
        game->update(elapsedTime - lastTime);
    lastTime = elapsedTime;
    glutPostRedisplay();
}

void messageCallback(GLenum source​, GLenum type​, GLuint id​, GLenum severity​, GLsizei length​, const GLchar *message​, const void *userParam​)
{
    std::cout << source​ << type​ << id​ << severity​ << message​ << std::endl;
#ifdef WIN32
    DebugBreak();
#else
    raise(SIGTRAP);
#endif
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow(argv[0]);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_KHR_debug);
    glDebugMessageCallback(messageCallback, NULL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    game = new Game();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idleFunc);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutMainLoop();

    return 0;
}
