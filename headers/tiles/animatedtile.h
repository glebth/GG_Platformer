#ifndef ANIMATEDTILE_H
#define ANIMATEDTILE_H

#include <vector>

#include "tiles/tile.h"

class AnimatedTile : public Tile {
public:
    AnimatedTile() {};
    AnimatedTile(std::vector<GG_Rectangle> &tilesInTileset, GG_Rectangle &tileInMap, SDL_Texture* texture, int duration);

    void Draw(Graphics& graphics);

    void Update(int elapsedTime, GG_Vector2 offset = {0, 0});
private:
    int _amountOfTime = 0;
    bool _isNotDone = false;
    size_t _tileToDraw; // na kakom frame seychas

    std::vector<GG_Rectangle> _tilesInTileset;
    //std::vector< std::pair<GG_Rectangle, int> > _tileInTileset;
};

struct AnimatedTileInfo {
    int tilesetFirstGID;
    int startTileId;
    std::vector<int> TileIds;
    int durationInfo;
};

#endif