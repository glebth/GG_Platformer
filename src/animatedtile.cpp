#include "graphics.h"
#include "tiles/animatedtile.h"

AnimatedTile::AnimatedTile(std::vector<GG_Rectangle> &tilesInTileset, 
                GG_Rectangle &tileInMap, SDL_Texture* texture, int duration) 
    : Tile::Tile(tilesInTileset[0], tileInMap, texture),
    _tileToDraw(0),
    _tilesInTileset(tilesInTileset)
{
    Tile::_duration = duration;
}

void AnimatedTile::Draw(Graphics &graphics) {

    SDL_Rect dst = (_tileInMap ^ globals::SPRITE_SCALE).GetSDLRect();

    SDL_Rect src =  (_tilesInTileset[_tileToDraw]).GetSDLRect();
    dst.x -= (int)_tileOffset.x;
    dst.y -= (int)_tileOffset.y;

    graphics.BlitToWindow(_tileTexture, &src, &dst);
}

void AnimatedTile::Update(int elapsedTime, GG_Vector2 offset /* = {0,0}*/ ) {
    
    _amountOfTime += elapsedTime;

    if ( _amountOfTime >= _duration ) {

        _amountOfTime -= _duration;

        if ( _tileToDraw < _tilesInTileset.size()-1 ) {
            _tileToDraw++;
        }
        else {
            _tileToDraw = 0;
        }
    }

    Tile::Update(elapsedTime, offset);
}