#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>

#include "class_arm.h"

const double PI = 3.14159;

double degcos( float a )
{

    return cos( a*0.0174533 );

}

double degsin( float a )
{

    return sin( a*0.0174533 );

}

double distance( Coord a, Coord b )
{

    return sqrt( pow( ( a.x + b.x ), 2 ) + pow( ( a.y + b.y ), 2 ) ); 

}

void DrawCircle( Coord mid, double r )
{

    glPushAttrib( GL_CURRENT_BIT );

    glColor3f( 0.2f, 0.2f, 0.2f );

    glBegin( GL_LINE_LOOP );
    for( double i = 0; i < 2*PI; i+=0.1 )
    {

        glVertex2f( mid.x+cos( i )*r, mid.y + sin( i )*r );

    }
    glEnd();

    glPopAttrib();

}

double normalizeAngle( double ang )
{

    while( ang < 0 )
    {

        ang+=(2*PI);

    }

    while( ang > (2*PI) )
    {

        ang-=(2*PI);

    }

    return ang;

}

int getQuad( Coord a )
{

    if( a.x > 0 && a.y > 0 )
    {

        return 1;

    }else if( a.x < 0 && a.y > 0 )
    {

        return 2;

    }else if( a.x < 0 && a.y < 0 )
    {

        return 3;

    }else if( a.x > 0 && a.y < 0 )
    {

        return 4;

    }

    return -1;

}

double angleOfInclin( Coord a, Coord b )
{

    double dx = b.x - a.x, dy = b.y - a.y;

    if( dx == 0 )
    {

        if( dy > 0 )
        {
    
            return PI/2;

        }else
        {
    
            return 3*PI/2;

        }

    }
    if( dy == 0 )
    {

        if( dx > 0 )
        {

            return 0;

        }else
        {

            return PI;

        }

    }

    double retVal = 0.0;

    if( getQuad( Coord( dx, dy ) ) == 1 )
    {

        retVal = atan( dy/dx );

    }else if( getQuad( Coord( dx, dy ) ) == 2 || getQuad( Coord( dx, dy ) ) == 3 )
    {

        retVal = atan( dy/dx ) + PI;

    }else if( getQuad( Coord( dx, dy ) ) == 4 )
    {

        retVal = atan( dy/dx ) + 2*PI;

    }

    return retVal;

}

Arm::Arm( Coord startPos, int jointNum, double jointLength )
{

    srand( time(nullptr) );

    this->startPos = startPos; 
    this->jointLength = jointLength;
    this->jointNum = jointNum;

}

Coord *circleIntersectPoint( Coord c0, double r0, Coord c1, double r1, Coord *defLoc )
{
    
    double a, dx, dy, d, h, rx, ry;
    Coord midPoint;

    dx = c1.x - c0.x;
    dy = c1.y - c0.y;

    d = hypot(dx,dy);
    
    if( d > r0+r1 )
    {

        return defLoc;

    }

    a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;

    midPoint.x = c0.x + (dx * a/d);
    midPoint.y = c0.y + (dy * a/d);

    h = sqrt((r0*r0) - (a*a));

    rx = -dy * (h/d);
    ry = dx * (h/d);

    return new Coord( midPoint.x + rx, midPoint.y + ry );

}

void Arm::calcAngles( Coord reachPos )
{

    points.clear();

    std::vector<Coord> jointPos;
    jointPos.push_back( reachPos );

    for( int i = 0; i < jointNum; i++ )
    {

        double angle = 0.0;
        int jointsLeft = jointNum - i;
        Coord newestPoint = jointPos.at(jointPos.size()-1);

        Coord *temp;

        if( jointsLeft-1 == 0 )
        {

            temp = &startPos;

        }else
        {
        
            temp = circleIntersectPoint( newestPoint, jointLength, startPos, jointLength*(jointsLeft-1), &startPos );

        }
        angle = angleOfInclin( newestPoint, *temp );

        double xComp = cos( angle ) * jointLength;
        double yComp = sin( angle ) * jointLength;

        xComp += jointPos.at( jointPos.size()-1 ).x;
        yComp += jointPos.at( jointPos.size()-1 ).y;

        jointPos.push_back( Coord( xComp, yComp ) );

    }

    double dx = startPos.x-jointPos.at( jointPos.size()-1 ).x;
    double dy = startPos.y-jointPos.at( jointPos.size()-1 ).y;

    for( Coord a : jointPos )
    {

        points.push_back( a.x+dx );
        points.push_back( a.y+dy );

    }

}

void Arm::draw()
{

    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 2, GL_DOUBLE, 0, points.data() );
    glDrawArrays( GL_LINE_STRIP, 0, points.size()/2 ); 

    glDisableClientState( GL_VERTEX_ARRAY );

}
