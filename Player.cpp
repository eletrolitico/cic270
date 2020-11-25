#include "Player.h"

#include "VertexBufferLayout.h"

#include <glm/gtx/transform.hpp>
float ts = 32;

Player::Player() : m_PlayerPos(glm::vec3(2, 7, 0)), m_PlayerSpeed(glm::vec2(0, 0))
{
    //Texture
    m_Texture = std::make_unique<Texture>("res/textures/player_sprite.png");
    int tw = m_Texture->GetWidth(), th = m_Texture->GetHeight();

    float positions[] = {
        0,
        0,
        0 / tw,
        0 / th,

        1,
        0,
        ts / tw,
        0 / th,

        0,
        1,
        0 / tw,
        ts / th,

        1,
        0,
        ts / tw,
        0 / th,

        0,
        1,
        0 / tw,
        ts / th,

        1,
        1,
        ts / tw,
        ts / th,
    };

    int fSize = 6 * 4;

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

    m_Shader = std::make_unique<Shader>("res/shaders/Player.shader");
}

Player::~Player()
{
}

void Player::draw(Renderer r, glm::mat4 mvp)
{
    m_Shader->Bind();
    m_Texture->Bind();
    glm::mat4 trans = glm::translate(glm::vec3(-0.5f, -0.5f, 0.0f));
    glm::mat4 trans2 = glm::translate(glm::vec3(0.5f, 0.5f, 0.0f));
    mvp = mvp * trans2 * glm::rotate(glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) * trans;

    if (m_IsBig)
    {
        mvp = mvp * glm::scale(glm::vec3(1.2f, 2.0f, 0.0f));
    }
    m_Shader->setUniformMat4f("u_MVP", mvp);
    m_Shader->setUniform1i("u_State", 3 - m_State);
    m_Shader->setUniform1i("u_Frame", m_Frame);
    m_Shader->setUniform1f("u_OffsetX", ts / m_Texture->GetWidth());
    m_Shader->setUniform1f("u_OffsetY", ts / m_Texture->GetHeight());
    m_Shader->setUniform1i("u_Mirror", m_Mirror);

    r.Draw(*m_VAO, *m_Shader);
}

float accumTime = 0.0f;
void Player::update(float fElapsedTime, const Map &map)
{
    float g = 50 * fElapsedTime;
    float x = m_PlayerPos.x;
    float y = m_PlayerPos.y;
    if (map.isInverted(m_PlayerPos))
    {
        g = -g;
        m_IsInverted = true;
        m_Rotation += 500 * fElapsedTime;
    }
    else
    {
        m_IsInverted = false;
        m_Rotation -= 500 * fElapsedTime;
    }
    if (m_Rotation > 180.0f)
        m_Rotation = 180.0f;
    if (m_Rotation < 0.0f)
        m_Rotation = 0.0f;

    if (m_State != 3)
    {

        m_PlayerSpeed.y -= g;

        glm::vec2 mv = (float)fElapsedTime * m_PlayerSpeed;

        if (map.getCollide(x, y, glm::vec2(0, mv.y)))
        {
            m_PlayerPos.y += mv.y;
            m_Ground = false;
        }
        else
        {
            if ((mv.y < 0 && !m_IsInverted) || (mv.y > 0 && m_IsInverted))
                m_Ground = true;
            else
                m_Ground = false;
            m_PlayerSpeed.y = 0;

            if (mv.y > 0)
                m_PlayerPos.y = floor(y) + 0.999f;
            else
                m_PlayerPos.y = floor(y);

            auto dd = map.getDanger();
            if (dd.find(map.getMap(x, (int)m_PlayerPos.y - 1)) != dd.end() || dd.find(map.getMap(x + 1, (int)m_PlayerPos.y - 1)) != dd.end())
            {
                m_State = 3;
                m_PlayerSpeed = {0.0f, 0.0f};
                goto end;
            }
            if (map.getMap(x, (int)m_PlayerPos.y - 1) == 'F' || map.getMap(x + 1, (int)m_PlayerPos.y - 1) == 'F')
            {
                m_State = 4;
                m_PlayerSpeed = {0.0f, 0.0f};
                goto end;
            }
        }

        y = m_PlayerPos.y;

        if (map.getCollide(x, y, glm::vec2(mv.x, 0)))
        {
            m_PlayerPos.x += mv.x;
        }
        else
        {
            // o personagem tem 0.5 de largura
            m_PlayerSpeed.x = 0;
            if (mv.x > 0)
                // um pouquinho menos que 0.25
                m_PlayerPos.x = floor(x) + 0.249f;
            else
                // um pouquinho mais que 0.75
                m_PlayerPos.x = floor(x) + 0.751f;
        }

        if (!m_Ground)
            m_State = 2;
    }
end:
    accumTime += fElapsedTime;

    if (accumTime > 0.1)
    {
        accumTime = 0;
        m_Frame++;
    }

    switch (m_State)
    {
    case 0:
        if (m_Frame > 3)
            m_Frame = 0;
        break;
    case 1:
        if (m_Frame > 5)
            m_Frame = 0;
        break;
    case 2:
        if (m_Frame > 7)
            m_Frame = 0;
        break;
    case 3:
        if (m_Frame > 3)
            m_Frame = 0;
        break;
    }
}

void Player::moveLeft()
{
    if (m_PlayerSpeed.x > -10)
        m_PlayerSpeed.x -= 1;
    m_State = 1;
    m_Mirror = !m_IsInverted;
}

void Player::moveRight()
{
    if (m_PlayerSpeed.x < 10)
        m_PlayerSpeed.x += 1;
    m_State = 1;
    m_Mirror = m_IsInverted;
}

void Player::stop()
{
    if (m_PlayerSpeed.x > 0)
        m_PlayerSpeed.x -= 1;
    else
        m_PlayerSpeed.x += 1;

    if (abs(m_PlayerSpeed.x) < 5)
        m_PlayerSpeed.x = 0;
    m_State = 0;
}

void Player::jump()
{
    if (m_Ground)
    {
        m_PlayerSpeed.y += (1 - 2 * m_IsInverted) * sqrt(2 * 50 * 3.5);
        m_Frame = 0;
    }
}