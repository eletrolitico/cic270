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
    //P espinho com back
    //F fim do mapa
    //H Tijolo
    //W parede
    std::string tmp = "";
    tmp += "................................";
    tmp += "...............................W";
    tmp += "..............................WW";
    tmp += ".........RTTY.................WW";
    tmp += "..............................WW";
    tmp += "..................RTY.........WW";
    tmp += "...........EGD................WW";
    tmp += "...........LUK...........OOO..WW";
    tmp += "EGGGGGGGGGGGGGGGGGGGGGGGGGGGGGHF";
    tmp += "LUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU";
    tmp += "ASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";
    m_Map.push_back(new Map(tmp, 32, 11, 2, 3));
    m_Player.m_PlayerPos = {2, 3, 0};

    tmp = "";
    tmp += "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    tmp += "HHHHHWWWWWWWWWWWWWWWWWWWWWWWWHHH";
    tmp += "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWWHWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWHHWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWPPWWWWHHHWPPPPWWWWWWWWWWWWW";
    tmp += "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    tmp += "UUUHHHUUUUHHHHHHUUUUUUHHHHHHUUUU";
    tmp += "SSSSSSSSSSSSSSHHSSSSSSSSSSSSSSSS";
    m_Map.push_back(new Map(tmp, 32, 11, 2, 3));

    m_MapCount = 2;

    // Sound
    m_Sound = SoundEngine::GetInstance();
    m_Sound->loadAudio("res/audio/jump.ogg", "jump", 0.6f, false);
    m_Sound->loadAudio("res/audio/morte.ogg", "death", 0.8f, false);
    m_Sound->streamAudio("res/audio/cobblestone_village.ogg", "music", 0.1f, true);
}

Game::~Game()
{
    for (auto m : m_Map)
    {
        delete m;
    }
}

void Game::draw(Renderer r)
{
    glClearColor(53 / 100.0f, 81 / 100.0f, 92 / 100.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 mvp = m_Proj * m_View;
    m_Map[m_CurrentMap]->draw(r, mvp);
    m_Player.draw(r, mvp * glm::translate(glm::mat4(1), m_Player.m_PlayerPos));
}

void Game::update(int fElapsedTime)
{
    if (m_Player.m_State == 4)
    {
        m_CurrentMap++;
        if (m_CurrentMap < m_MapCount)
        {
            m_Player.m_PlayerPos = m_Map[m_CurrentMap]->getInitialPos();
            m_Player.m_State = 0;
        }
        else
        {
            //you win
            m_Sound->playAudio("jump");
        }
    }
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
        m_Player.m_PlayerPos = m_Map[m_CurrentMap]->getInitialPos();
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

    m_Player.update(fElapsedTime, *m_Map[m_CurrentMap]);

    if (m_Player.m_PlayerPos.x - xScreen > 7)
        xScreen = m_Player.m_PlayerPos.x - 7;
    if (m_Player.m_PlayerPos.x - xScreen < 1.6)
        xScreen = m_Player.m_PlayerPos.x - 1.6;

    if (xScreen < 0)
        xScreen = 0;

    if (xScreen > m_Map[m_CurrentMap]->m_width - 16)
        xScreen = m_Map[m_CurrentMap]->m_width - 16;

    yScreen = m_Player.m_PlayerPos.y - 6.5;

    if (yScreen < 0)
        yScreen = 0;

    if (yScreen > m_Map[m_CurrentMap]->m_height - 9)
        yScreen = m_Map[m_CurrentMap]->m_height - 9;

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