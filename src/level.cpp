#include <sstream>
#include <algorithm>

#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "utils/utils.h"
#include "sprites/player.h"
#include "sprites/enemy.h"
#include "sprites/enemylight.h"


Level::Level() {;}

Level::Level(std::string pathMap, Graphics &graphics) {
    _levelMapTiles.clear();
    _levelTilesets.clear();
    _levelColliders.clear();
    _levelAnimatedTiles.clear();
    _levelNpc.clear();

    _width = 0; _height = 0;
    _tileWidht = 0; _tileHeight = 0;

    _playerSpawnPoint = {0, 0};

    _offset = {0, 0};

    LoadLevel(pathMap, graphics);
}

void Level::SetPlayer(Player *player) {
    _player = player;
}

void Level::LoadLevel(std::string pathMap, Graphics &graphics) {
    XMLDocument doc;
    if (doc.LoadFile(pathMap.c_str())) {
        SDL_Log("Cant open file %s XML: %s", pathMap.c_str(), doc.ErrorStr());
    }

    XMLElement* mapNode = LoadMap(&doc, _width, _height, _tileWidht, _tileHeight);

    LoadTilesets(mapNode, this->_levelTilesets, graphics, _levelAnimatedTiles, _levelATInfo);

    LoadData(mapNode, this->_levelTilesets, _width, _height, _tileWidht, _tileHeight, 
        this->_levelMapTiles, _levelATInfo, _levelAnimatedTiles);

    LoadObjects(mapNode, graphics, _levelColliders, _playerSpawnPoint, 
    _levelSlopes, _levelDoors, _levelNpc, _levelEnemy);

}

void Level::Draw(Graphics &graphics) {
    for (size_t i = 0; i < _levelMapTiles.size(); i++) {
        _levelMapTiles[i].Draw(graphics);
    }
    for (size_t i = 0; i < _levelAnimatedTiles.size(); i++) {
        _levelAnimatedTiles[i].Draw(graphics);
    }

    for (size_t i = 0; i < _levelNpc.size(); i++ ) {

        std::vector<GG_Rectangle> collisionRectangles;
        if ((collisionRectangles = CollidedRects(_levelNpc[i].GetBoundingbox())).size() > 0) {
                _levelNpc[i].HandleCollision(collisionRectangles);    
        }
        collisionRectangles.clear();
    }
    for (size_t i = 0; i < _levelNpc.size(); i++ ) {
        _levelNpc[i].Draw(graphics);
    }
}

void Level::Update(float elpasedTime) {

    if ( _player->GetBoundingbox().GetLeft() > (float)globals::WINDOW_WIDTH / 2.0f &&
     _player->GetBoundingbox().GetLeft() < _width * _tileWidht * globals::SPRITE_SCALE - 
                                                (float)globals::WINDOW_WIDTH / 2.0f ) 
    {
        _offset.x = _player->GetBoundingbox().GetLeft() - (float)globals::WINDOW_WIDTH / 2.0f;
        //SDL_Log("offsetx %f", _offset.x);
    }

    //Вертикальные оффсеты
    // if ( _player->GetBoundingbox().GetTop() > (float)globals::WINDOW_HEIGHT / 2.0f &&
    //  _player->GetBoundingbox().GetTop() < _height * _tileHeight * globals::SPRITE_SCALE - 
    //                                             (float)globals::WINDOW_HEIGHT / 2.0f ) 
    // {
    //     _offset.y = _player->GetBoundingbox().GetTop() - (float)globals::WINDOW_HEIGHT / 2.0f;
    //     //SDL_Log("offsetx %f", _offset.x);
    // }

    for (size_t i = 0; i < _levelMapTiles.size(); i++) {
        _levelMapTiles[i].Update(elpasedTime, _offset);
    }
    for (size_t i = 0; i < _levelAnimatedTiles.size(); i++) {
        _levelAnimatedTiles[i].Update(elpasedTime, _offset);
    }

    for (size_t i = 0; i < _levelNpc.size(); i++ ) {
        _levelNpc[i].Update(elpasedTime, _offset);
    }
    
    for (size_t i = 0; i < _levelEnemy.size(); i++) {
        _levelEnemy[i]->Update(elpasedTime, _offset);
    }
}

 std::vector<GG_Rectangle> Level::CollidedRects(const GG_Rectangle &otherRect) {
    std::vector<GG_Rectangle> collidedRects;

    // ОПРЕДЕЛИТЬ ПОЛОЖЕНИЕ otherRect и сканировать не все а только то что в округе

    for (size_t i = 0; i < _levelColliders.size(); i++) {
        if ( _levelColliders[i].IsColliding(otherRect) ) {
            collidedRects.push_back(_levelColliders[i]);
        }
    }

    return collidedRects;
 }

 std::vector<Slope> Level::CollidedSlopes(const GG_Rectangle &otherRectangle) {
    std::vector<Slope> collidedSlopes;

    for (size_t i = 0; i < _levelSlopes.size(); i++) {
        if ( _levelSlopes[i].IsCollides(otherRectangle) ) {
            collidedSlopes.push_back(_levelSlopes[i]);
        }
    }

    return collidedSlopes;
 }

 std::vector<Door> Level::CollidedDoors(const GG_Rectangle &otherRectangle) {
    
    std::vector<Door> collidedDoors;
    
    for (size_t i = 0; i < _levelDoors.size(); i++) {

        if ( _levelDoors[i].IsColliding(otherRectangle) ) {
            collidedDoors.push_back(_levelDoors[i]);
        }
    }

    return collidedDoors;
 }

std::vector<Npc *> Level::CollidedNpc(const GG_Rectangle &otherRectangle) {

    std::vector<Npc *> collidedNpc;
    
    for (size_t i = 0; i < _levelNpc.size(); i++) {

        if ( _levelNpc[i].IsColliding(otherRectangle) ) {
            collidedNpc.push_back(&_levelNpc[i]);
        }
    }

    return collidedNpc;

}

//--------Help functions --------

XMLElement* LoadMap(XMLDocument* doc , int &width, int &height, int &tileWidth, int &tileHeight) {
    XMLElement* mapNode = doc->FirstChildElement("map");

    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");

    tileWidth = mapNode->IntAttribute("tilewidth");
    tileHeight = mapNode->IntAttribute("tileheight");

    return mapNode;
}

void LoadTilesets(XMLElement* mapNode,
                         std::vector<Tileset> &mapTilesets,
                         Graphics &graphics, std::vector<AnimatedTile> &levelAT, 
                         std::vector<AnimatedTileInfo> &levelATInfo) {
    
    XMLElement* pTileset = mapNode->FirstChildElement("tileset");
    
    while (pTileset != NULL)
    {
        int firstGid = -1;
        firstGid = pTileset->IntAttribute("firstgid");

        //---Tileset file Dostaem kartinku s tileset
        const char* sourceTilesetAdr = pTileset->Attribute("source"); //get address: ../tilesets/some.tsx

        std::stringstream ssTilesetAddr;
        for (int i = (int)strlen(sourceTilesetAdr) - 1; sourceTilesetAdr[i] != '/'; i-- ) {
            ssTilesetAddr << sourceTilesetAdr[i]; // Zdes shli s konca ctobi poluchit nazvanie
        }
        std::string strTilesetAddr = ssTilesetAddr.str();
            
        std::reverse(strTilesetAddr.begin(), strTilesetAddr.end());
        ssTilesetAddr.str("");
        ssTilesetAddr << "data/tilesets/" << strTilesetAddr;

        XMLDocument tileSetDoc; // tileset file also XML
        tileSetDoc.LoadFile(ssTilesetAddr.str().c_str());
        XMLElement* tilesetFileTilesetNode = tileSetDoc.FirstChildElement("tileset");

        XMLElement* pSourceImageTileset = tilesetFileTilesetNode->FirstChildElement("image");
        const char* sourceImageTilesetName = pSourceImageTileset->Attribute("source");
        std::stringstream ss;
        ss << "data/tilesets/" << sourceImageTilesetName; // eto adres kartinku
        SDL_Texture* tilesetTexture = graphics.LoadTexture(ss.str()); //eto uzhe textura
        //----Tileset file

        int tileWidth = tilesetFileTilesetNode->IntAttribute("tilewidth");
        int tileHeight = tilesetFileTilesetNode->IntAttribute("tileheight");

        Tileset newTileset(firstGid, tileWidth, tileHeight, tilesetTexture);
        mapTilesets.push_back(newTileset);

        //Load Animated Tiles
        LoadAnimatedTiles(tilesetFileTilesetNode, levelATInfo, firstGid);
        

        pTileset = pTileset->NextSiblingElement("tileset");
    }
}

void LoadAnimatedTiles(XMLElement *pTileset,
        std::vector<AnimatedTileInfo> &levelATInfo, int firstGid) {
    
    XMLElement *pTileA = pTileset->FirstChildElement("tile");
    while (pTileA) {

        AnimatedTileInfo ati;
        ati.tilesetFirstGID = firstGid;
        ati.startTileId = pTileA->IntAttribute("id") + firstGid; //pervii id v realnoi karte

        //Load animtaions
        XMLElement *pAnimation = pTileA->FirstChildElement("animation");
        while (pAnimation) {

            XMLElement *pFrame = pAnimation->FirstChildElement("frame");
            while (pFrame) {

                ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstGid);
                ati.durationInfo = pFrame->IntAttribute("duration");

                pFrame = pFrame->NextSiblingElement("frame");
            }

            pAnimation = pAnimation->NextSiblingElement("animation");
        }

        levelATInfo.push_back(ati);

        pTileA = pTileA->NextSiblingElement("tile");
    }
}

void LoadData(XMLElement* mapNode, 
    std::vector<Tileset> &lvlTilesets,
    int mapWidth, int mapHeight, 
    int mapTileWidth, int mapTileHeight,
    std::vector<Tile> &mapTiles,
    std::vector<AnimatedTileInfo> &atInfo,
    std::vector<AnimatedTile> &atS) 
{
    XMLElement* pLayer = mapNode->FirstChildElement("layer");

    while (pLayer)
    {
        XMLElement* pData = pLayer->FirstChildElement("data");
        while (pData)
        {
            const char* dataText;
            dataText = pData->GetText();


            LoadData_ParseData(dataText, lvlTilesets, mapWidth, mapHeight, 
                mapTileWidth, mapTileHeight, mapTiles, atInfo, atS);
            

            pData = pData->NextSiblingElement("data");
        }
        
        
        pLayer = pLayer->NextSiblingElement("layer");
    }
    
}



void LoadObjects(
    XMLElement* mapNode,
    Graphics &graphics,
    std::vector<GG_Rectangle> &collisionRects, 
    GG_Vector2 &playerSpawnPoint,
    std::vector<Slope> &slopesVector, 
    std::vector<Door> &lvlDoorsVector,
    std::vector<Npc> &lvlNpc,
    std::vector<Enemy *> &lvlEnemy) 
{

    XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");

    while (pObjectGroup) {
        const char* nameObjectgroup = pObjectGroup->Attribute("name");
        std::string nameGroup(nameObjectgroup);

        if (nameGroup == "collisions") {
            LoadColliders(pObjectGroup, collisionRects);
        }
        else if (nameGroup == "spawnpoints") {
            LoadSpawnpoints(pObjectGroup, playerSpawnPoint, lvlNpc, lvlEnemy, graphics);
        }
        else if (nameGroup == "slopes") {
            LoadSlopes(pObjectGroup, slopesVector);
        } 
        else if (nameGroup == "doors") {
            LoadDoors(pObjectGroup, lvlDoorsVector);
        }

        pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
    }
}

void LoadColliders(XMLElement* pObjectGroup, std::vector<GG_Rectangle> &collisionRects) {
    
    XMLElement* pElement = pObjectGroup->FirstChildElement("object");

            while(pElement) {

                float x, y, width, height = -1;
                x = pElement->FloatAttribute("x");
                y = pElement->FloatAttribute("y");
                width = pElement->FloatAttribute("width");
                height = pElement->FloatAttribute("height");
                ////SDL_Log("x== %f y == %f", x, y);

                collisionRects.push_back(GG_Rectangle(
                    std::ceil(x) * globals::SPRITE_SCALE,
                    std::ceil(y) * globals::SPRITE_SCALE,
                    std::ceil(width) * globals::SPRITE_SCALE,
                    std::ceil(height) * globals::SPRITE_SCALE
                ));
                            
                pElement = pElement->NextSiblingElement("object");
            };
}

void LoadSpawnpoints(XMLElement* pObjectGroup, GG_Vector2 &spawnPoint, 
    std::vector<Npc> &lvlNpc, std::vector<Enemy *> &lvlEnemy, Graphics &graphics) {

    XMLElement* pElement = pObjectGroup->FirstChildElement("object");

    while(pElement) {

        float x, y = -1;
        x = pElement->FloatAttribute("x");
        y = pElement->FloatAttribute("y");
        const char* nameSpawn = pElement->Attribute("name");

        std::string nameSp(nameSpawn);

        if (nameSp == "player") {
            spawnPoint = GG_Vector2(ceil(x), ceil(y)) * globals::SPRITE_SCALE;
        }
        if (nameSp == "pasha") {
            LoadNpc(pElement, x, y, nameSp, lvlNpc, graphics);
        }
        if (nameSp == "enemy") {
            LoadNpc(pElement, x, y, nameSp, lvlNpc, graphics);
        }

        pElement = pElement->NextSiblingElement("object");
    }
}

void LoadNpc(XMLElement* pElement, float x, float y, std::string nameNpc, 
    std::vector<Npc> &lvlNpc, Graphics &graphics) {

    GG_Vector2 npcPoint = GG_Vector2(ceil(x), ceil(y)) * globals::SPRITE_SCALE;

    XMLElement *pProperties = pElement->FirstChildElement("properties");
    pProperties = pProperties->FirstChildElement("property");

    std::string npcDscrp = "";
    std::string npcTextureFile = "";
    float npcHText = 0;
    float npcWText = 0;
    float npcXText = 0;
    float npcYText = 0;
    std::string npcFacing = "";

    struct animation
    {
        std::vector<int> frames;
        int startX;
        int startY;
        const char* animName;
        int margin = 0;
    } npcAnimation;
    std::vector<animation> npcAnimations;
    
    while (pProperties) {

        const char* charNameProp = pProperties->Attribute("name");
        std::string propName(charNameProp);

        if (propName == "file") {
            const char* npcFilePath = pProperties->Attribute("value");

            XMLDocument npcFile;
            if (npcFile.LoadFile(npcFilePath)) {
                SDL_Log("Cant open file %s XML: %s", npcFilePath, npcFile.ErrorStr());
            }

            XMLElement* pNpc = npcFile.FirstChildElement("npc");
            XMLElement* pNpcProperties = pNpc->FirstChildElement("properties");
            pNpcProperties = pNpcProperties->FirstChildElement("property");

            while(pNpcProperties) {

                const char* charNpcNameProp = pNpcProperties->Attribute("name");
                std::string propNpcName(charNpcNameProp);

                if (propNpcName == "H_TEXT") {
                    npcHText = pNpcProperties->FloatAttribute("value");
                }

                if (propNpcName == "NPC_FILE") {
                    const char* npcFilechar = pNpcProperties->Attribute("value");
                    npcTextureFile = std::string(npcFilechar);
                }

                if (propNpcName == "W_TEXT") {
                    npcWText = pNpcProperties->FloatAttribute("value");
                }

                if (propNpcName == "X_TEXT") {
                    npcXText = pNpcProperties->FloatAttribute("value");
                }

                if (propNpcName == "Y_TEXT") {
                    npcYText = pNpcProperties->FloatAttribute("value");
                }

                if (propNpcName == "description") {
                    const char* npcDescription = pNpcProperties->Attribute("value"); // odna stroka

                    if (npcDescription == NULL) { // neskolko strok
                        npcDescription = pNpcProperties->GetText();
                    }

                    npcDscrp = std::string(npcDescription);
                }

                if (propNpcName == "facing") {

                    const char *facing = pNpcProperties->Attribute("value");
                    npcFacing = std::string(facing);
                }
                
                pNpcProperties = pNpcProperties->NextSiblingElement("property");
            }

            XMLElement* pNpcAnimations = pNpc->FirstChildElement("animations");
            pNpcAnimations = pNpcAnimations->FirstChildElement("animation");

            while(pNpcAnimations) {

                const char* charAnimName = pNpcAnimations->Attribute("name");
                int frame = pNpcAnimations->IntAttribute("frames");
                std::vector<int> npcFrames;

                if (frame == 0) {
                    const char* frames = pNpcAnimations->GetText(); // вида {0, 1, 0, 2}

                    std::string framesString(frames);
                    std::vector<std::string> framesStringVector;
                    Utils::Split(framesString, framesStringVector, ',');

                    for (size_t i = 0; i < framesStringVector.size(); i++) {
                        npcFrames.push_back(stoi(framesStringVector[i]));
                    }  
                }
                else {
                    for (int i = 0; i < frame; i++) {
                        npcFrames.push_back(i);
                    }
                }

                npcAnimation.frames = npcFrames;
                npcAnimation.animName = charAnimName;
                npcAnimation.startX = pNpcAnimations->IntAttribute("startX");
                npcAnimation.startY = pNpcAnimations->IntAttribute("startY");
                npcAnimation.margin = pNpcAnimations->IntAttribute("frameMargin");

                npcAnimations.push_back(npcAnimation);

                pNpcAnimations = pNpcAnimations->NextSiblingElement("animation");
            }
        }
        pProperties = pProperties->NextSiblingElement("property");
    }

    SpriteDir facingDir = (npcFacing == "LEFT") ? LEFT : RIGHT;

    Npc newNpc(nameNpc, npcDscrp, graphics, npcTextureFile, npcXText, npcYText, npcWText,
        npcHText, npcPoint.x, npcPoint.y, globals::NPC_ANIM_SPEED, 
        facingDir, 1.0f, true, npcAnimations[0].animName);

    for (size_t i = 0; i < npcAnimations.size(); i++) {
        newNpc.LoadAnimations(npcAnimations[i].frames, npcAnimations[i].startX, 
            npcAnimations[i].startY, npcAnimations[i].animName, npcAnimations[i].margin);
    }

    lvlNpc.push_back(newNpc);
}

void LoadSlopes(XMLElement* pObjectGroup, std::vector<Slope> &slopesVector) {

    XMLElement* pElement = pObjectGroup->FirstChildElement("object");

    while( pElement ) {

        std::vector<GG_Vector2> slopePoints;

        float x1 = pElement->FloatAttribute("x");
        float y1 = pElement->FloatAttribute("y");
        GG_Vector2 p1 = { x1, y1 }; //std::ceil?????

        XMLElement *pPolyline = pElement->FirstChildElement("polyline");

        if (pPolyline) {
            std::vector<std::string> relativePoints;

            const char* points = pPolyline->Attribute("points");
            std::string strPoints(points);
            //SDL_Log("%s", points);

            Utils::Split( strPoints, relativePoints, ' '); //razbil na point'i

            for(size_t i = 0; i < relativePoints.size(); i++) {
                std::vector<std::string> st;
                Utils::Split( relativePoints[i], st, ',');

                slopePoints.push_back( GG_Vector2(std::stof(st[0]), std::stof(st[1])) );
            }

            //Seichas v slopePoints polyline points="..." - относительные от p1 координаты
        }

        //Sdelat slopes iz otdelnix pointov
        for (size_t i = 0; i < slopePoints.size() - 1; i++) {
            slopesVector.push_back( Slope( 
                GG_Vector2( p1.x + slopePoints[i].x, p1.y + slopePoints[i].y ) * globals::SPRITE_SCALE,
                GG_Vector2( p1.x + slopePoints[i+1].x, p1.y + slopePoints[i+1].y) * globals::SPRITE_SCALE
            ));
        }


        pElement = pElement->NextSiblingElement("object");
    }
}

void LoadDoors(XMLElement* pObjectGroup, std::vector<Door> &doorsVector) {
    
    XMLElement* pElement = pObjectGroup->FirstChildElement("object");

    while ( pElement ) {

        float x = pElement->FloatAttribute("x");
        float y = pElement->FloatAttribute("y");
        float w = pElement->FloatAttribute("width");
        float h = pElement->FloatAttribute("height");

        GG_Rectangle doorRect = GG_Rectangle(x, y, w, h) ^ globals::SPRITE_SCALE;

        XMLElement *pProperties = pElement->FirstChildElement("properties");

        while (pProperties) {

            XMLElement *pProperty = pProperties->FirstChildElement("property");

            while (pProperty) {

                const char* nameObjectgroup = pProperty->Attribute("name");
                std::string propName(nameObjectgroup);

                if (propName == "destination") {

                    const char* destination = pProperty->Attribute("value");
                    std::string doorDest(destination);

                    Door newDoor(doorRect, "./data/maps/" + doorDest + ".tmx");

                    //SDL_Log("door %s", destination);

                    doorsVector.push_back(newDoor);
                }

                pProperty = pProperty->NextSiblingElement("property");
            }
            pProperties = pProperties->NextSiblingElement("properties");
        }
        pElement = pElement->NextSiblingElement("object");
    }
    
}


void LoadData_ParseData(const char* dataText, std::vector<Tileset> &lvlTilesets, 
   int mapWidth, int mapHeight, 
   int mapTileWidth, int mapTileHeight,
    std::vector<Tile> &mapTiles, 
    std::vector<AnimatedTileInfo> &atInfo,
    std::vector<AnimatedTile> &atS) 
{
    std::stringstream ss(dataText);
    std::string tempStroka;
    std::string stringDataGID;
    int tileCounter = 0;

    while (!ss.eof())
    {
        ss >> tempStroka; //odna stroka iz data
        //SDL_Log("data: %s", tempStroka.c_str());

        size_t posDash = -1;     

        while ( (tempStroka != "") )
        {
            bool isDashFound = ((posDash = tempStroka.find(',')) != std::string::npos);
            
            bool isLastTileNow = isLastTile(tempStroka, isDashFound); // Vse radi poslednix tilov

            if ( !isLastTileNow && isDashFound )
                stringDataGID = tempStroka.substr(0, posDash);
            else
                stringDataGID = tempStroka;
                       

            int dataGID = std::stoi(stringDataGID);

            if (dataGID != 0) {
                Tileset tls;

                for (size_t i = 0; i < lvlTilesets.size(); i++) {
                    
                    if (lvlTilesets[i]._firstGid <= dataGID) {
                        tls = lvlTilesets[i];
                    }
                }

                if (tls._firstGid == -1) { //Ne nashli zagrushennim takoi tileset
                    tileCounter++;
                    continue;
                }
                //Get tile position on the map
                int xx, yy = 0;
                xx = (tileCounter % mapWidth) * mapTileWidth;
                yy = (tileCounter / mapWidth) * mapTileHeight;
                GG_Rectangle finalTileMapPos = {xx, yy, mapTileWidth, mapTileHeight};

                //If its tile in anim tile
                bool isAnimatedTile = false;
                AnimatedTileInfo ati;
                for (size_t i = 0; i < atInfo.size(); i++) {
                    if (atInfo[i].startTileId == dataGID) {
                        ati = atInfo[i];
                        isAnimatedTile = true;
                        break;
                    }
                }

                //Get position of the tile in tileset picture. Tiles if animated
                if (isAnimatedTile) {
                    std::vector<GG_Rectangle> tileAnimatedPosition;

                    for (size_t i = 0; i < ati.TileIds.size(); i++) {
                        tileAnimatedPosition.push_back( GetTilesetPos(tls, ati.TileIds[i]) );
                    }
                    AnimatedTile aTile(tileAnimatedPosition, finalTileMapPos, tls._tilesetTexture,
                            ati.durationInfo);

                    atS.push_back(aTile);

                }
                else {

                    GG_Rectangle finalTilesetPos = GetTilesetPos(tls, dataGID);

                    //Tile sdelat i dobavit v tileKarty
                    Tile newTile = Tile(finalTilesetPos, finalTileMapPos, tls._tilesetTexture);
                    mapTiles.push_back(newTile);

                }
            }
            tileCounter++;

            if ( isDashFound )
                tempStroka.erase(0, posDash + 1);
            else
                tempStroka = "";           
        }
    }
}

bool isLastTile(std::string tempString, bool isDashFound) noexcept(false) {
    try {
        if ( !isDashFound )
        {
            std::stoi(tempString);
            return true;
        }
        else
        {
            return false;
        }
        
    }
    catch(std::invalid_argument &e) {
        return false;
    }
}

GG_Rectangle GetTilesetPos(Tileset tls, int dataGID) {
    int tilesetWidth, tilesetHight = 0;
    SDL_QueryTexture(tls._tilesetTexture, NULL, NULL, &tilesetWidth, &tilesetHight);

    int tsxx, tsyy = 0;
    tsxx = ((dataGID - tls._firstGid) * tls._tileWidth) % tilesetWidth;
    tsxx = (tsxx < 0) ? 15*tls._tileWidth : tsxx; // esli datagid kraten 16
    tsyy = ((dataGID - tls._firstGid) * tls._tileHeight) / tilesetWidth * tls._tileHeight;

    GG_Rectangle finalTilesetPos = {tsxx, tsyy, tls._tileWidth, tls._tileHeight};

    return finalTilesetPos;
}