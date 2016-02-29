#pragma once

struct Coord
{

    Coord();
    
    template<class T>
    Coord( T x, T y )
    {

        this->x = x;
        this->y = y;

    }

    float x, y;

    bool operator== ( Coord b );

};
