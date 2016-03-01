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

            return PI;

        }else
        {

            return 0;

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

    printf( "%f\n", angleOfInclin( Coord( 0, 0 ), Coord( 2, -2 ) ) );

    srand( time(nullptr) );

    this->startPos = startPos; 
    this->jointLength = jointLength;
    this->jointNum = jointNum;

}

Coord circleIntersectPoint( Coord c0, double r0, Coord c1, float r1 )
{
    double a, dx, dy, d, h, rx, ry;
    Coord midPoint;

    dx = c1.x - c0.x;
    dy = c1.y - c0.y;

    d = hypot(dx,dy);

    a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;

    midPoint.x = c0.x + (dx * a/d);
    midPoint.y = c0.y + (dy * a/d);

    h = sqrt((r0*r0) - (a*a));

    rx = -dy * (h/d);
    ry = dx * (h/d);

    Coord temp = Coord( midPoint.x + rx, midPoint.y + ry ); 

    return temp;

}

void Arm::calcAngles( Coord reachPos )
{

    points.clear();

    if( distance( reachPos, startPos ) > jointLength*jointNum )
    {

        //printf( "Cannot reach.\n" );

    }

    std::vector<Coord> jointPos;
    jointPos.push_back( reachPos );

    for( int i = 0; i < jointNum; i++ )
    {

        double angle = 0.0;
        int jointsLeft = jointNum - i;

        if( round(distance( jointPos.at( jointPos.size()-1 ), startPos )) == jointLength )
        {

            angle = angleOfInclin( jointPos.at( jointPos.size()-1 ), startPos );

        }else if( distance( jointPos.at( jointPos.size()-1 ), startPos ) < jointLength )
        {

            Coord tempPos = circleIntersectPoint( jointPos.at(jointPos.size()-1), jointLength, startPos, jointLength );

            angle = angleOfInclin( jointPos.at(jointPos.size()-1), tempPos );

        }else if( distance( jointPos.at( jointPos.size()-1 ), startPos ) > jointLength )
        {

            angle = angleOfInclin( jointPos.at( jointPos.size()-1 ), startPos );

        }

        double xComp = cos( angle ) * jointLength;
        double yComp = sin( angle ) * jointLength;

        xComp += jointPos.at( jointPos.size()-1 ).x;
        yComp += jointPos.at( jointPos.size()-1 ).y;

        jointPos.push_back( Coord( xComp, yComp ) );

    }

    for( Coord a : jointPos )
    {

        points.push_back( a.x );
        points.push_back( a.y );

    }

}

void Arm::draw()
{

    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 2, GL_DOUBLE, 0, points.data() );
    glDrawArrays( GL_LINE_STRIP, 0, points.size()/2 ); 

    glDisableClientState( GL_VERTEX_ARRAY );

}
