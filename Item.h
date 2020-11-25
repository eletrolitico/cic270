#ifndef ITEM
#define ITEM

#include "Entity.h"
#include "Map.h"

class Item : public Entity
{
public:
    Item(glm::vec3 pos, glm::vec2 iniSpeed, Map *map);
    ~Item();
    void update(float fElapsedTime);

private:
    glm::vec2 m_speed;
    Map *m_map;
};

#endif