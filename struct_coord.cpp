#include "struct_coord.h"

Coord::Coord()
{

    x = 0.0f;
    y = 0.0f;

}

bool Coord::operator== ( Coord b )
{

    return (( this->x == b.x ) && ( this->y == b.y ));

}
