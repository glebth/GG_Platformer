#include "tiles/tile.h"
#include "graphics.h"


Tile::Tile() {
    _tileInMap = GG_Rectangle();
    _tileInTileset = GG_Rectangle();
    _duration = 0;
    _tileOffset = {0, 0};

    _tileTexture = NULL;
};

Tile::Tile(GG_Rectangle &tileinTileset, GG_Rectangle &tileInMap, SDL_Texture* texture) {
    _tileInTileset = tileinTileset;
    _tileInMap = tileInMap;
    _duration = 0;
    _tileOffset = {0, 0};

    _tileTexture = texture;
}

void Tile::Draw(Graphics& graphics) {
    SDL_Rect src = _tileInTileset.GetSDLRect();
    SDL_Rect dst = (_tileInMap ^ globals::SPRITE_SCALE).GetSDLRect();
    dst.x -= (int)_tileOffset.x;
    dst.y -= (int)_tileOffset.y;

    graphics.BlitToWindow(_tileTexture, &src, &dst);
}

void Tile::Update(float elapsedTime, GG_Vector2 offset /* = {0,0} */) {
    _tileOffset = offset;
}