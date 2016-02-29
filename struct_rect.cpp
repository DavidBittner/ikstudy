#include "struct_rect.h"

Rect::Rect()
{

    pos.x = 0.0f;
    pos.y = 0.0f;

    bnd.x = 0.0f;
    bnd.y = 0.0f;

}

Rect::Rect( float x, float y, float w, float h )
{

    pos.x = x;
    pos.y = y;

    bnd.x = w;
    bnd.y = h;

}

Rect::Rect( Coord pos, Coord size )
{

    this->pos = pos;
    this->bnd = size;

}
