#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>
#include <map>

#include "Renderer.h"
#include "Tile.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Player.h"
#include "SoundEngine.h"

class Map
{
private:
    int m_width, m_height;
    std::string m_Map;

    glm::mat4 m_Proj, m_View;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Texture> m_Texture;

    std::map<char, bool> m_keys;

    std::vector<Tile> m_Tiles;

    Player m_Player;

    SoundEngine m_Sound;

    Tile getTile(char, int);

public:
    Map();
    ~Map();
    char getMap(int x, int y);
    void draw(Renderer r);
    void update(int);
    void keyboardDown(unsigned char key, int x, int y);
    void keyboardUp(unsigned char key, int x, int y);
};

#endif