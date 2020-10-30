#include "Map.h"

#include "VertexBufferLayout.h"

Map::~Map()
{
}

Map::Map(std::string map, int w, int h, int iniX, int iniY) : m_width(w), m_height(h), m_Map(map), m_InitialPos({iniX, iniY, 0.0f})
{
    int fSize = m_height * m_width * 24;
    float *positions = new float[fSize];

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

    m_Shader = std::make_unique<Shader>("res/shaders/Tile.shader");

    delete positions;
}

Tile Map::getTile(char c, int width) const
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
    case 'O':
        return m_Tiles[4];
    }
    return m_Tiles[0];
}

void Map::draw(Renderer r, glm::mat4 mvp) const
{
    m_Texture->Bind();
    m_Shader->Bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    r.Draw(*m_VAO, *m_Shader);
}

char Map::getMap(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height ? m_Map[(m_height - 1 - y) * m_width + x] : '_';
}

std::set<char> Map::getDanger() const
{
    return {'O'};
}

glm::vec3 Map::getInitialPos() const
{
    return m_InitialPos;
}
