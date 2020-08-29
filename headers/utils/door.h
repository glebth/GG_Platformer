#ifndef DOOR_H
#define DOOR_H

#include <string>

#include "globals.h"

class Door : public GG_Rectangle {
public:

    Door() {;}
    Door(GG_Rectangle r, std::string destination) :
        GG_Rectangle(r)
    {   
        _destionation = destination;
    }

    const inline std::string GetDestination() const { return _destionation; }
    const inline GG_Rectangle GetRect() const { return this->GetRectangle(); }

private:
    std::string _destionation;
};

#endif