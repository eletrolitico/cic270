#ifndef GAME
#define GAME

#include <vector>
#include <memory>
#include <map>

#include "Renderer.h"
#include "Player.h"
#include "SoundEngine.h"
#include "Map.h"

class Game
{
private:
    glm::mat4 m_Proj, m_View;

    std::map<char, bool> m_keys;
    std::unique_ptr<Map> m_Map;
    Player m_Player;

    SoundEngine m_Sound;

    Tile getTile(char, int);

public:
    Game();
    ~Game();
    char getMap(int x, int y);
    void draw(Renderer r);
    void update(int);
    void keyboardDown(unsigned char key, int x, int y);
    void keyboardUp(unsigned char key, int x, int y);
    void reshape(int width, int height);
};

#endif