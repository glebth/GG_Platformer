#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "tinyxml2.h"
#include "tiles/tile.h"
#include "tiles/animatedtile.h"
#include "utils/slope.h"
#include "utils/door.h"
#include "sprites/npc.h"

using namespace tinyxml2;

class Graphics;
class Sprite;
class Player;
class Enemy;

class Level {
public:
    Level();
    Level(std::string pathMap, Graphics &graphics);

    void Draw(Graphics &graphics);
    void Update(float elpasedTime);

    std::vector<GG_Rectangle> CollidedRects(const GG_Rectangle &otherRect);
    std::vector<Slope> CollidedSlopes(const GG_Rectangle &otherRectangle);
    std::vector<Door> CollidedDoors(const GG_Rectangle &otherRectangle);
    std::vector<Npc *> CollidedNpc(const GG_Rectangle &otherRectangle);

    const inline GG_Vector2 GetPlayerSpawnpoint() const { return _playerSpawnPoint; }
    inline GG_Vector2 GetOffset() { return _offset; }

    void SetPlayer(Player *player);

private:
    int _width, _height;
    int _tileWidht, _tileHeight;

    Player *_player;
    GG_Vector2 _playerSpawnPoint;

    GG_Vector2 _offset;

    std::vector<Npc> _levelNpc;
    std::vector<Enemy *> _levelEnemy;

    void LoadLevel(std::string pathMap, Graphics &graphics);

    std::vector<Tile> _levelMapTiles;
    std::vector<Tileset> _levelTilesets;

    std::vector<AnimatedTile> _levelAnimatedTiles;
    std::vector<AnimatedTileInfo> _levelATInfo;

    std::vector<GG_Rectangle> _levelColliders;

    std::vector<Slope> _levelSlopes;

    std::vector<Door> _levelDoors;

};


XMLElement* LoadMap(XMLDocument* doc , int &width, int &height, int &tileWidth, int &tileHeight);

void LoadTilesets(XMLElement* mapNode, std::vector<Tileset> &mapTilesets, Graphics &graphics,   
    std::vector<AnimatedTile> &levelAT, std::vector<AnimatedTileInfo> &levelATInfo);

void LoadAnimatedTiles(XMLElement *pTileset, 
        std::vector<AnimatedTileInfo> &levelATInfo, int firstGid);

void LoadData_ParseData(
    const char* dataText, 
    std::vector<Tileset> &lvlTilesets, 
    int mapWidth, int mapHeight, 
    int mapTileWidth, int mapTileHeight,
    std::vector<Tile> &mapTiles,
    std::vector<AnimatedTileInfo> &atInfo,
    std::vector<AnimatedTile> &atS);

void LoadData(XMLElement* mapNode, 
    std::vector<Tileset> &lvlTilesets,
    int mapWidth, int mapHeight, 
    int mapTileWidth, int mapTileHeight,
    std::vector<Tile> &mapTiles, 
    std::vector<AnimatedTileInfo> &atInfo,
    std::vector<AnimatedTile> &atS);

void LoadObjects(
    XMLElement* mapNode,
    Graphics &graphics,
    std::vector<GG_Rectangle> &collisionRects, 
    GG_Vector2 &playerSpawnPoint,
    std::vector<Slope> &slopesVector, 
    std::vector<Door> &lvlDoorsVector,
    std::vector<Npc> &lvlNpc, 
    std::vector<Enemy *> &lvlEnemy);
void LoadColliders(XMLElement* pObjectGroup, std::vector<GG_Rectangle> &collisionRects);
void LoadSpawnpoints(XMLElement* pObjectGroup, GG_Vector2 &spawnPoint, 
    std::vector<Npc> &lvlNpc, std::vector<Enemy *> &lvlEnemy, Graphics &graphics);
void LoadSlopes(XMLElement* pObjectGroup, std::vector<Slope> &slopesVector);
void LoadDoors(XMLElement* pObjectGroup, std::vector<Door> &doorsVector);

void LoadNpc(XMLElement* pElement, float x, float y, std::string nameNpc, 
    std::vector<Npc> &lvlNpc, std::vector<Enemy *> &lvlEnemy, Graphics &graphics);

Enemy * CreateEnemy(std::string enemyClass, 
                std::string name, 
                std::string description, 
                Graphics &graphics, 
                std::string filePath, 
                float xText, float yText, 
                float wText, float hText, 
                float xMap, float yMap, 
                float animUpdate, SpriteDir facing,
                float boundingBoxScale = 1,
                bool isCollides = true,
                std::string animName = "idleLeft");


//Get tile position in tileset
GG_Rectangle GetTilesetPos(Tileset tls, int dataGID);

//Check if it was last parsed tile from map. returns true if last and valid
bool isLastTile(std::string tempString, bool isDash);

#endif