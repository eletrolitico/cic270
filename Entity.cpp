#include "Entity.h"

#include <glm/gtx/transform.hpp>
#include "VertexBufferLayout.h"

Entity::Entity(glm::vec3 pos, bool hasLight, const std::string &tex, int frames, float frameTime, int texSkip, float aspect) : m_EntityPos(pos), m_HasLight(hasLight),
                                                                                                                               m_Frames(frames), m_TexSkip(texSkip), m_FrameTime(frameTime), m_Aspect(aspect)
{
    if (tex != "")
    {
        m_IsDrawable = true;
        //Texture
        m_Texture = std::make_unique<Texture>(tex);
        int tw = m_Texture->GetWidth();

        float positions[] = {
            0.0f,
            0.0f,
            0,
            0,

            aspect,
            0.0f,
            m_TexSkip / (float)tw,
            0,

            0.0f,
            1.0f,
            0,
            1,

            aspect,
            0.0f,
            m_TexSkip / (float)tw,
            0,

            0.0f,
            1.0f,
            0,
            1,

            aspect,
            1.0f,
            m_TexSkip / (float)tw,
            1,
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

        m_Shader = std::make_unique<Shader>("res/shaders/Entity.shader");
    }
    else
    {
        m_IsDrawable = false;
    }
}

Entity::~Entity()
{
}

void Entity::draw(Renderer r, glm::mat4 mvp) const
{
    m_Shader->Bind();
    m_Texture->Bind();
    m_Shader->setUniformMat4f("u_MVP", mvp * glm::translate(glm::mat4(1), m_EntityPos));

    m_Shader->setUniform1i("u_Frame", m_Frame);
    m_Shader->setUniform1f("u_OffsetX", (float)m_TexSkip / m_Texture->GetWidth());
    m_Shader->setUniform1i("u_Mirror", m_Mirror);

    r.Draw(*m_VAO, *m_Shader);
}

void Entity::update(float fElapsedTime)
{
    m_AcumTime += fElapsedTime;

    if (m_AcumTime > m_FrameTime)
    {
        m_AcumTime = 0;
        m_Frame++;
    }

    if (m_Frame > m_Frames)
        m_Frame = 0;
}

glm::vec3 Entity::getPos() const
{
    return glm::vec3(m_EntityPos);
}

glm::vec2 Entity::getLightPos() const
{
    return glm::vec2(m_EntityPos.x + m_Aspect / 2.0f, m_EntityPos.y + 0.5f);
}

bool Entity::hasLight() const
{
    return m_HasLight;
}
bool Entity::isDrawable() const
{
    return m_IsDrawable;
}
