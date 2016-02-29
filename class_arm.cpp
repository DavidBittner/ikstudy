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

double angleOfInclin( Coord a, Coord b )
{

    double dx = b.x - a.x, dy = b.y - a.y;

    printf( "dx:%f, dy:%f\n", dx, dy );

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

    return atan(dy/dx);

}

Arm::Arm( Coord startPos, int jointNum, double jointLength )
{

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

        if( distance( jointPos.at(i), startPos ) >= jointLength*(jointNum-i) )
        {

            double ang = angleOfInclin( startPos, jointPos.at(i) );
            double xComp = cos(ang)*jointLength;
            double yComp = sin(ang)*jointLength;

            xComp = jointPos.at(i).x - xComp;
            yComp = jointPos.at(i).y - yComp;

            jointPos.push_back( Coord( xComp, yComp ) );

        }else if( round(distance( jointPos.at(i), startPos )) == jointLength ) 
        {

            jointPos.push_back( Coord( startPos.x, startPos.y ) );

        }else
        {

            Coord tempPos = circleIntersectPoint( startPos, jointLength, jointPos.at(i), jointLength );

            double ang = angleOfInclin( tempPos, jointPos.at(i) );

            double xComp = cos(ang)*jointLength;
            double yComp = sin(ang)*jointLength;

            xComp = jointPos.at(i).x - xComp;
            yComp = jointPos.at(i).y - yComp;

            jointPos.push_back( Coord( xComp, yComp ) );

        }

    }

    if( jointPos.at(jointNum-1).x != startPos.x || jointPos.at(jointNum-1).y != startPos.y )
    {

        double dx = jointPos.at(jointNum).x - startPos.x;
        double dy = jointPos.at(jointNum).y - startPos.y;

        for( int i = 0; i < jointNum+1; i++ )
        {
            
            jointPos.at(i).x-=dx;
            jointPos.at(i).y-=dy;

        }
        
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
