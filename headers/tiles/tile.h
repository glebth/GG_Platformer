#ifndef TILE_H
#define TILE_H

#include "globals.h"

class Graphics;

class Tile {
public:
    Tile();
    Tile(GG_Rectangle &tileinTileset, GG_Rectangle &tileInMap, SDL_Texture* texture);

    virtual void Draw(Graphics& graphics);

    void Update(float elapsedTime, GG_Vector2 offset = {0, 0});
    
protected:
    int _duration;

    GG_Vector2 _tileOffset;
    
    GG_Rectangle _tileInTileset;
    
    GG_Rectangle _tileInMap;

    SDL_Texture* _tileTexture;
};

struct Tileset {

    Tileset(int firstGid = -1, int tileWidth = 0, int tileHeight = 0, SDL_Texture* Tileset = 0) {
        _firstGid = firstGid;
        _tileWidth = tileWidth;
        _tileHeight = tileHeight;

        _tilesetTexture = Tileset;
    }

    int _firstGid;

    int _tileWidth, _tileHeight;

    SDL_Texture* _tilesetTexture;
};

#endif