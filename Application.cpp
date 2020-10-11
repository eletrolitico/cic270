#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Renderer.h"
#include "Map.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

int win_width = 1280;
int win_height = 720;

Renderer renderer;

Map *map;

void display()
{

    map->draw(renderer);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
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
    map->keyboardDown(key, x, y);
}

void keyUp(unsigned char key, int x, int y)
{
    map->keyboardUp(key, x, y);
}

void SpecialInput(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        break;
    case GLUT_KEY_DOWN:
        break;
    }

    glutPostRedisplay();
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
    map->update(elapsedTime - lastTime);
    lastTime = elapsedTime;
    glutPostRedisplay();
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

    map = new Map();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(SpecialInput);
    glutIdleFunc(idleFunc);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glutMainLoop();
}
