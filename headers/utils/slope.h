#ifndef SLOPE_H
#define SLOPE_H

#include <cmath>

#include "globals.h"

using namespace std;

class Slope {
public:
    Slope() {;}
    Slope(GG_Vector2 p1, GG_Vector2 p2) {
        _p1 = p1; _p2 = p2;

        _slope = 5;

        if ( fabs(_p2.x) - fabs(_p1.x) != 0 ) {
            _slope = ( fabs(_p2.y) - fabs(_p1.y) ) / ( fabs(_p2.x) - fabs(_p1.x) );
            _b = _p1.y - _slope * _p1.x;
        }
    }

    const inline float GetSlope() const { return _slope; }
    const inline GG_Vector2 GetP1() const { return _p1; }
    const inline GG_Vector2 GetP2() const { return _p2; }
    const inline float GetB() const { return _b; }

    
    const bool IsCollides(const GG_Rectangle &other ) const {

        if ( !(//_p2 ниже и levee _p1
            ( other.GetLeft() <= _p1.x && 
            other.GetRight() >= _p2.x &&
            other.GetBottom() >= _p2.y &&
            other.GetTop() <= _p1.y) || 
            //_p2 ниже и pravee p1
            ( other.GetLeft() <= _p2.x &&
            other.GetRight() >= _p1.x &&
            other.GetBottom() >= _p2.y &&
            other.GetTop() <= _p1.y) ||
            //_p2 vishe i levee
            ( other.GetLeft() <= _p1.x && 
            other.GetRight() >= _p2.x &&
            other.GetBottom() >= _p1.y &&
            other.GetTop() <= _p2.y) ||
            //_p2 vishe i levee
            ( other.GetLeft() <= _p2.x &&
            other.GetRight() >= _p1.x &&
            other.GetBottom() >= _p1.y &&
            other.GetTop() <= _p2.y)) )
            return false;
        
        float boxY = _slope * other.GetCenterX() + _b;

        float ddy = other.GetBottom() - boxY;

        if ( ((boxY >= _p2.y && boxY <= _p1.y) || (boxY >= _p1.y && boxY <= _p2.y)) && //if between dots
            ( ddy >= 0 ) ) { // if выше линии
            return true;
        }

        return false;
    }

private:
    GG_Vector2 _p1, _p2;
    float _slope;//tangens ugla
    float _b;
};

#endif