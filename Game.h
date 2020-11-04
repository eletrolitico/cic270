#ifndef GAME
#define GAME

#include <vector>
#include <map>

#include "Renderer.h"
#include "Player.h"
#include "SoundEngine.h"
#include "Map.h"

class Game
{

public:
    Game();
    ~Game();
    char getMap(int x, int y);
    void draw(Renderer r);
    void update(int);
    void keyboardDown(unsigned char key, int x, int y);
    void keyboardUp(unsigned char key, int x, int y);
    void reshape(int width, int height);

private:
    glm::mat4 m_Proj, m_View;
    float xScreen = 0, yScreen = 0;

    std::map<char, bool> m_keys;
    std::vector<Map *> m_Map;
    int m_CurrentMap = 0;
    int m_MapCount;
    Player m_Player;

    SoundEngine *m_Sound;

    Tile getTile(char, int);
};

#endif