#include "Game.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Item.h"

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
    Map *tempmap;
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
    tempmap = new Map(tmp, 32, 11, 2, 3, 1.0);
    tempmap->addEntity(new Item(glm::vec3(13.0f, 9.0f, 0.0f), glm::vec2(5.0f, 0.0f), tempmap));
    m_Map.push_back(tempmap);
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
    tmp += "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHF";
    tmp += "UUUHHHUUUUHHHHHHUUUUUUHHHHHHUUUU";
    tmp += "SSSSSSSSSSSSSSHHSSSSSSSSSSSSSSSS";
    tempmap = new Map(tmp, 32, 11, 2, 3, 0.02);
    tempmap->addEntity(new Entity(glm::vec3(6.05f, 4.7f, 0.0f), true, "res/textures/torch.png", 17, 1.0f / 60.0f, 15, 15.0f / 50.0f));
    tempmap->addInvInterval(glm::vec2(15.0f, 20.0f));
    m_Map.push_back(tempmap);

    tmp = "";
    tmp += "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    tmp += "HHHHHWWWWWWWWWWWWWWWWWWWWWWWWHHH";
    tmp += "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWWHWWWWWWWWWWWWWWWWWW";
    tmp += "WWWWWWWWWWWWHHWWWWWWWWWWWWWWWWW.";
    tmp += "WWWWWPPWWWWHHHWPPPPWWWWWWWWWWWW.";
    tmp += "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    tmp += "UUUHHHUUUUHHHHHHUUUUUUHHHHHHUUUU";
    tmp += "SSSSSSSSSSSSSSHHSSSSSSSSSSSSSSSS";
    tempmap = new Map(tmp, 32, 11, 2, 3, 0.02);
    tempmap->addEntity(new Entity(glm::vec3(32.0f, 4.2f, 0.0f), true));
    m_Map.push_back(tempmap);

    m_MapCount = 3;

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
    glm::vec3 pp = m_Player.m_PlayerPos;

    m_Map[m_CurrentMap]->draw(r, mvp, glm::vec2(pp.x + 0.5f, pp.y + 0.5f));
    m_Player.draw(r, mvp * glm::translate(glm::mat4(1), m_Player.m_PlayerPos));
}

void Game::update(float fElapsedTime)
{
    std::vector<int> to_remove;
    int p = 0;
    std::vector<Entity *> &vec = m_Map[m_CurrentMap]->getEntities();
    for (Entity *e : vec)
    {
        Item *i = dynamic_cast<Item *>(e);
        if (i != NULL)
        {
            if (i->getPos().x < m_Player.m_PlayerPos.x + 1 &&
                i->getPos().x + 1 > m_Player.m_PlayerPos.x &&
                i->getPos().y < m_Player.m_PlayerPos.y + 1 &&
                i->getPos().y + 1 > m_Player.m_PlayerPos.y)
            {
                m_Player.setBig();
                delete e;
                to_remove.push_back(p);
            }
        }
        p++;
    }
    for (auto i = to_remove.rbegin(); i < to_remove.rend(); ++i)
    {
        vec.erase(vec.begin() + *i);
    }
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
        if ((m_keys[GLFW_KEY_SPACE] || m_keys['W']))
        {
            if (m_Player.m_Ground)
                m_Sound->playAudio("jump");
            m_Player.jump();
        }
        if (m_keys['D'])
            m_Player.moveRight();
        else if (m_keys['A'])
            m_Player.moveLeft();
        else
            m_Player.stop();
    }

    static bool music = true;
    if (m_keys[GLFW_KEY_ENTER] && m_Player.m_State == 3)
    {
        m_Player.m_State = 0;
        m_Player.m_PlayerPos = m_Map[m_CurrentMap]->getInitialPos();
        m_Player.setSmall();
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
    if (m_keys['M'] && m_keys['M'] != keyPrev)
    {
        if (music)
            m_Sound->pauseAudio("music");
        else
            m_Sound->playAudio("music");
        music = !music;
    }
    keyPrev = m_keys['M'];

    m_Player.update(fElapsedTime, *m_Map[m_CurrentMap]);

    if (m_Player.m_PlayerPos.x - xScreen > 7)
        xScreen = m_Player.m_PlayerPos.x - 7;
    if (m_Player.m_PlayerPos.x - xScreen < 1.6)
        xScreen = m_Player.m_PlayerPos.x - 1.6;

    if (xScreen < 0.01f)
        xScreen = 0.01f;

    if (xScreen > m_Map[m_CurrentMap]->m_width - 16.01f)
        xScreen = m_Map[m_CurrentMap]->m_width - 16.01f;

    yScreen = m_Player.m_PlayerPos.y - 6.5;

    if (yScreen < 0.01f)
        yScreen = 0.01f;

    if (yScreen > m_Map[m_CurrentMap]->m_height - 9.01f)
        yScreen = m_Map[m_CurrentMap]->m_height - 9.01f;

    m_View = glm::translate(glm::mat4(1), glm::vec3(-xScreen, -yScreen, 0));
    m_Sound->update();
    m_Map[m_CurrentMap]->update(fElapsedTime);
}

void Game::keyboardDown(int key)
{
    m_keys[key] = true;
}

void Game::keyboardUp(int key)
{
    m_keys[key] = false;
}
