#include "Item.h"

Item::Item(glm::vec3 pos, glm::vec2 iniSpeed, Map *map) : Entity(pos, false, "res/textures/banana.png", 0, 0, 32, 1), m_map(map)
{
    m_speed = iniSpeed;
}

void Item::update(float fElapsedTime)
{
    float g = 50 * fElapsedTime;
    float x = m_EntityPos.x;
    float y = m_EntityPos.y;

    m_speed.y -= g;

    glm::vec2 mv = (float)fElapsedTime * m_speed;

    if (m_map->getCollide(x, y, glm::vec2(0, mv.y)))
    {
        m_EntityPos.y += mv.y;
    }
    else
    {
        m_speed.y = 0;

        if (mv.y > 0)
            m_EntityPos.y = floor(y) + 0.999f;
        else
            m_EntityPos.y = floor(y);
    }

    y = m_EntityPos.y;

    if (m_map->getCollide(x, y, glm::vec2(mv.x, 0)))
    {
        m_EntityPos.x += mv.x;
    }
    else
    {
        // o personagem tem 0.5 de largura
        m_speed.x = -m_speed.x;
        if (mv.x > 0)
            // um pouquinho menos que 0.25
            m_EntityPos.x = floor(x) + 0.249f;
        else
            // um pouquinho mais que 0.75
            m_EntityPos.x = floor(x) + 0.751f;
    }
}