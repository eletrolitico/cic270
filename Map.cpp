#include "Map.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "VertexBufferLayout.h"

irrklang::ISoundSource *jump;
Map::Map() : m_width(32), m_height(11), m_Proj(glm::ortho(0.0f, 16.0f, 0.0f, 9.0f)), m_View(glm::mat4(1))
{
    int fSize = m_height * m_width * 24;
    float *positions = new float[fSize];

    m_Map = "";
    m_Map += "................................";
    m_Map += "................................";
    m_Map += "................................";
    m_Map += ".........RTTY...................";
    m_Map += "................................";
    m_Map += "..................RTY...........";
    m_Map += "...........EGD..................";
    m_Map += "...........LUK..................";
    m_Map += "EGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGD";
    m_Map += "LUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUK";
    m_Map += "ASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSB";

    m_Texture = std::make_unique<Texture>("res/textures/Tilemap.png");
    m_Texture->Unbind();

    int texSize = 16;
    int tw = m_Texture->GetWidth(), th = m_Texture->GetHeight();

    for (int j = 0; j < th / texSize; ++j)
    {
        for (int i = 0; i < tw / texSize; ++i)
        {
            m_Tiles.push_back(Tile(i * texSize, th - j * texSize, tw, th, texSize));
        }
    }

    int index = 0;
    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            char mP = m_Map[(m_height - 1 - j) * m_width + i];
            if (mP != '.')
            {
                float *temp = getTile(mP, tw / texSize).draw(i, j);

                for (int k = 0; k < 24; ++k)
                {
                    positions[index++] = temp[k];
                }
                delete temp;
            }
        }
    }

    // Vertex array.
    m_VAO = std::make_unique<VertexArray>();

    // Vertex buffer
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, fSize);

    // Set attributes.
    VertexBufferLayout lay;
    lay.Push<float>(2);
    lay.Push<float>(2);

    m_VAO->AddBuffer(*m_VertexBuffer, lay);

    // Unbind Vertex Array Object.
    m_VAO->Unbind();

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    m_Shader = std::make_unique<Shader>("res/shaders/Tile.shader");

    delete positions;

    // Sound
    SoundEngine = irrklang::createIrrKlangDevice();
    SoundEngine->play2D("res/audio/cobblestone_village.mp3", true);
    SoundEngine->setSoundVolume(0.5);
    jump = SoundEngine->addSoundSourceFromFile("res/audio/jump.ogg", irrklang::ESM_NO_STREAMING, true);
    jump->setForcedStreamingThreshold(-1);
}

Map::~Map()
{
    SoundEngine->drop();
}

Tile Map::getTile(char c, int width)
{
    switch (c)
    {
    case 'R':
        return m_Tiles[1];
    case 'T':
        return m_Tiles[2];
    case 'Y':
        return m_Tiles[3];
    case 'E':
        return m_Tiles[width + 1];
    case 'G':
        return m_Tiles[width + 2];
    case 'D':
        return m_Tiles[width + 3];
    case 'L':
        return m_Tiles[2 * width + 1];
    case 'K':
        return m_Tiles[2 * width + 3];
    case 'U':
        return m_Tiles[2 * width + 2];
    case 'A':
        return m_Tiles[3 * width + 1];
    case 'S':
        return m_Tiles[3 * width + 2];
    case 'B':
        return m_Tiles[3 * width + 3];
    }
    return m_Tiles[0];
}

void Map::draw(Renderer r)
{
    GLCall(glClearColor(53 / 100.0f, 81 / 100.0f, 92 / 100.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_Texture->Bind();

    glm::mat4 mvp = m_Proj * m_View;
    m_Shader->Bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    r.Draw(*m_VAO, *m_Shader);
    mvp = m_Proj * m_View * glm::translate(glm::mat4(1), m_Player.m_PlayerPos);
    m_Player.draw(r, mvp);

    //glm::mat4 model = glm::translate(glm::mat4(1.0f), m_PlayerPos);
}

char Map::getMap(int x, int y)
{
    return m_Map[(m_height - 1 - y) * m_width + x];
}

float xScreen = 0, yScreen = 0;
void Map::update(int fElapsedTime)
{

    if ((m_keys[32] || m_keys['w']))
    {
        if (m_Player.m_Ground)
            SoundEngine->play2D(jump);
        m_Player.jump();
    }
    if (m_keys['d'])
        m_Player.moveRight();
    else if (m_keys['a'])
        m_Player.moveLeft();
    else
        m_Player.stop();

    m_Player.update(fElapsedTime, m_Map, m_width, m_height);

    if (m_Player.m_PlayerPos.x - xScreen > 13.4)
        xScreen = m_Player.m_PlayerPos.x - 13.4;
    if (m_Player.m_PlayerPos.x - xScreen < 1.6)
        xScreen = m_Player.m_PlayerPos.x - 1.6;

    if (xScreen < 0)
        xScreen = 0;

    yScreen = m_Player.m_PlayerPos.y - 6.5;

    if (yScreen < 0)
        yScreen = 0;

    m_View = glm::translate(glm::mat4(1), glm::vec3(-xScreen, -yScreen, 0));
}

void Map::keyboardDown(unsigned char key, int x, int y)
{
    m_keys[key] = true;
}

void Map::keyboardUp(unsigned char key, int x, int y)
{
    m_keys[key] = false;
}