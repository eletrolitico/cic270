#include "Player.h"

#include "VertexBufferLayout.h"

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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    m_Shader = std::make_unique<Shader>("res/shaders/Player.shader");
}

Player::~Player()
{
}

void Player::draw(Renderer r, glm::mat4 mvp)
{
    m_Shader->Bind();
    m_Texture->Bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    m_Shader->setUniform1i("u_State", 3 - m_State);
    m_Shader->setUniform1i("u_Frame", m_Frame);
    m_Shader->setUniform1f("u_OffsetX", ts / m_Texture->GetWidth());
    m_Shader->setUniform1f("u_OffsetY", ts / m_Texture->GetHeight());
    m_Shader->setUniform1i("u_Mirror", m_Mirror);

    r.Draw(*m_VAO, *m_Shader);
}

float accumTime = 0.0f;
void Player::update(int elapsedTime, const Map &map)
{
    float fElapsedTime = (float)elapsedTime / 1000.0f;
    float g = 50 * fElapsedTime;
    float x = m_PlayerPos.x;
    float y = m_PlayerPos.y;

    if (m_State != 3)
    {

        m_PlayerSpeed.y -= g;

        glm::vec2 mv = (float)fElapsedTime * m_PlayerSpeed;

        auto getCollide = [&](float x, float y, glm::vec2 dir) {
            return x + dir.x > 0 && map.getMap(x + dir.x, y + dir.y) == '.' && map.getMap(x + 1 + dir.x, y + 1 + dir.y) == '.' && map.getMap(x + 1 + dir.x, y + dir.y) == '.' && map.getMap(x + dir.x, y + 1 + dir.y) == '.';
        };

        if (getCollide(x, y, glm::vec2(0, mv.y)))
        {
            m_PlayerPos.y += mv.y;
            m_Ground = false;
        }
        else
        {
            if (mv.y < 0)
                m_Ground = true;
            else
                m_Ground = false;
            m_PlayerSpeed.y = 0;

            if (mv.y > 0)
                m_PlayerPos.y = floor(y) + 0.999f;
            else
                m_PlayerPos.y = floor(y);

            auto dd = map.getDanger();
            if (dd.find(map.getMap(x, (int)m_PlayerPos.y - 1)) != dd.end())
            {
                m_State = 3;
                m_PlayerSpeed = {0.0f, 0.0f};
                goto end;
            }
        }

        y = m_PlayerPos.y;

        if (getCollide(x, y, glm::vec2(mv.x, 0)))
        {
            m_PlayerPos.x += mv.x;
        }
        else
        {
            m_PlayerSpeed.x = 0;
            if (mv.x > 0)
                m_PlayerPos.x = floor(x) + 0.999f;
            else
                m_PlayerPos.x = floor(x) + 0.01f;
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
    m_Mirror = true;
}

void Player::moveRight()
{
    if (m_PlayerSpeed.x < 10)
        m_PlayerSpeed.x += 1;
    m_State = 1;
    m_Mirror = false;
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
        m_PlayerSpeed.y += sqrt(2 * 50 * 3.5);
        m_Frame = 0;
    }
}