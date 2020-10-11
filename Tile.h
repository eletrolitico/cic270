#ifndef TILE
#define TILE

class Tile
{
private:
    int m_TexX, m_TexY, m_TexW, m_TexH, m_TileSize;

public:
    Tile(int texX, int texY, int texW, int texH, int tileSize);
    ~Tile();
    float *draw(int x, int y);
};

#endif