#ifndef MAP_H
#define MAP_H

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Renderer.h"
#include "Tile.h"
#include "VertexBuffer.h"
#include "Texture.h"

class Map
{
public:
    Map(std::string map, int w, int h, int, int);
    ~Map();
    char getMap(int x, int y) const;
    Tile getTile(char c, int width) const;
    void draw(Renderer r, glm::mat4 mvp) const;
    std::set<char> getDanger() const;
    glm::vec3 getInitialPos() const;
    bool getCollide(float x, float y, glm::vec2 dir) const;
    int m_width, m_height;

private:
    std::string m_Map;

    bool m_Transparent[256];

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Texture> m_Texture;
    std::vector<Tile> m_Tiles;

    glm::vec3 m_InitialPos;
};

#endif