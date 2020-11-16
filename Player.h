#ifndef PLAYER
#define PLAYER

#include <memory>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Map.h"

class Player
{
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Texture> m_Texture;
    bool m_Mirror = false;

    int m_Frame = 0;

public:
    glm::vec3 m_PlayerPos;
    glm::vec2 m_PlayerSpeed;
    bool m_Ground = false;
    // 0=idle 1=walking 2=jumping, 3 = dead, 4 = next stage
    int m_State = 0;

    Player();
    ~Player();
    void update(float fElapsedTime, const Map &map);
    void moveLeft();
    void moveRight();
    void stop();
    void jump();
    void draw(Renderer r, glm::mat4 mvp);
};

#endif