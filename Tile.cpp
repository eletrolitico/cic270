#include "Tile.h"

Tile::Tile(int texX, int texY, int texW, int texH, int tileSize) : m_TexX(texX), m_TexY(texY), m_TexW(texW), m_TexH(texH), m_TileSize(tileSize)
{
}

Tile::~Tile()
{
}

float *Tile::draw(int x, int y)
{
    float *res = new float[4 * 6];
    res[0] = x;
    res[1] = y + 1;
    res[2] = (float)(m_TexX) / m_TexW;
    res[3] = (float)(m_TexY) / m_TexH;

    res[4] = x + 1;
    res[5] = y + 1;
    res[6] = (float)(m_TexX + m_TileSize) / m_TexW;
    res[7] = (float)(m_TexY) / m_TexH;

    res[8] = x;
    res[9] = y;
    res[10] = (float)(m_TexX) / m_TexW;
    res[11] = (float)(m_TexY - m_TileSize) / m_TexH;

    res[12] = x + 1;
    res[13] = y + 1;
    res[14] = (float)(m_TexX + m_TileSize) / m_TexW;
    res[15] = (float)(m_TexY) / m_TexH;

    res[16] = x;
    res[17] = y;
    res[18] = (float)(m_TexX) / m_TexW;
    res[19] = (float)(m_TexY - m_TileSize) / m_TexH;

    res[20] = x + 1;
    res[21] = y;
    res[22] = (float)(m_TexX + m_TileSize) / m_TexW;
    res[23] = (float)(m_TexY - m_TileSize) / m_TexH;

    return res;
}