#ifndef ENTITY
#define ENTITY

#include <memory>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "Texture.h"

class Entity
{

public:
    Entity(glm::vec3, bool hasLight, const std::string &tex = "", int frames = 0, float frameTime = 0.0f, int texSkip = 0, float aspect = 0);
    ~Entity();
    virtual void update(float fElapsedTime);
    bool hasLight() const;
    bool isDrawable() const;
    glm::vec3 getPos() const;
    glm::vec2 getLightPos() const;
    void draw(Renderer r, glm::mat4 mvp) const;

protected:
    glm::vec3 m_EntityPos;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Texture> m_Texture;

    int m_State = 0;

    bool m_Mirror = false;
    bool m_HasLight = false;
    bool m_IsDrawable = false;

    int m_Frame = 0;
    int m_Frames;
    int m_TexSkip;

    float m_AcumTime = 0;
    float m_FrameTime;
    float m_Aspect = 1;
};

#endif