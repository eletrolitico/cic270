#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

Game::Game() : m_Proj(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), m_View(glm::mat4(1))
{

    //R grama flutuante esquerda
    //T grama flutuante meio
    //Y grama flutuante direita
    //E grama esquerda
    //G grama meio
    //D grama direita
    //L terra esquerda
    //U terra meio
    //K terra direita
    //A fundo esquerda
    //S fundo meio
    //B fundo direita
    //O espinho
    std::string tmp;
    tmp = "";
    tmp += "................................";
    tmp += "................................";
    tmp += "................................";
    tmp += ".........RTTY...................";
    tmp += "................................";
    tmp += "..................RTY...........";
    tmp += "...........EGD..................";
    tmp += "...........LUK.............OOO..";
    tmp += "EGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGD";
    tmp += "LUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUK";
    tmp += "ASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSB";
    m_Map = std::make_unique<Map>(tmp, 32, 11, 2, 7);
    // Sound
    m_Sound = SoundEngine::GetInstance();
    m_Sound->loadAudio("res/audio/jump.ogg", "jump", 0.6f, false);
    m_Sound->loadAudio("res/audio/morte.ogg", "death", 0.6f, false);
    m_Sound->streamAudio("res/audio/cobblestone_village.ogg", "music", 0.4f, true);
}

Game::~Game()
{
}

void Game::draw(Renderer r)
{
    glClearColor(53 / 100.0f, 81 / 100.0f, 92 / 100.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 mvp = m_Proj * m_View;
    m_Map->draw(r, mvp);
    m_Player.draw(r, mvp * glm::translate(glm::mat4(1), m_Player.m_PlayerPos));
}

float xScreen = 0, yScreen = 0;
void Game::update(int fElapsedTime)
{

    // if not dead
    if (m_Player.m_State != 3)
    {
        if ((m_keys[32] || m_keys['w']))
        {
            if (m_Player.m_Ground)
                m_Sound->playAudio("jump");
            m_Player.jump();
        }
        if (m_keys['d'])
            m_Player.moveRight();
        else if (m_keys['a'])
            m_Player.moveLeft();
        else
            m_Player.stop();
    }

    static bool music = true;
    if (m_keys[13] && m_Player.m_State == 3)
    {
        m_Player.m_State = 0;
        m_Player.m_PlayerPos = m_Map->getInitialPos();
        if (!music)
        {
            music = true;
            m_Sound->playAudio("music");
        }
    }

    static int prevState = 0;
    if (m_Player.m_State == 3 && prevState != 3)
    {
        m_Sound->pauseAudio("music");
        m_Sound->playAudio("death");
        music = false;
    }
    prevState = m_Player.m_State;

    static bool keyPrev = false;
    if (m_keys['m'] && m_keys['m'] != keyPrev)
    {
        if (music)
            m_Sound->pauseAudio("music");
        else
            m_Sound->playAudio("music");
        music = !music;
    }
    keyPrev = m_keys['m'];

    
    m_Player.update(fElapsedTime, *m_Map);

    if (m_Player.m_PlayerPos.x - xScreen > 7)
        xScreen = m_Player.m_PlayerPos.x - 7;
    if (m_Player.m_PlayerPos.x - xScreen < 1.6)
        xScreen = m_Player.m_PlayerPos.x - 1.6;

    if (xScreen < 0)
        xScreen = 0;

    if (xScreen > m_Map->m_width - 16)
        xScreen = m_Map->m_width - 16;

    yScreen = m_Player.m_PlayerPos.y - 6.5;

    if (yScreen < 0)
        yScreen = 0;

    m_View = glm::translate(glm::mat4(1), glm::vec3(-xScreen, -yScreen, 0));
    m_Sound->update();
}

void Game::keyboardDown(unsigned char key, int x, int y)
{
    m_keys[key] = true;
}

void Game::keyboardUp(unsigned char key, int x, int y)
{
    m_keys[key] = false;
}

void Game::reshape(int width, int height)
{
    m_Proj = glm::ortho(0.0f, width * 9.0f / height, 0.0f, 9.0f);
}