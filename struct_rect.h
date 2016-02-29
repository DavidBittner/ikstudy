#pragma once

#include "struct_coord.h"

struct Rect
{

    Rect( float x, float y, float w, float h );
    Rect();
    Rect( Coord pos, Coord size );

    Coord pos;
    Coord bnd;

};
