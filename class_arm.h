#pragma once

#include <vector>
#include "struct_coord.h"

class Arm
{

    public:
        Arm( Coord startPos, int jointNum, double jointLength ); 

        void calcAngles( Coord reachPos );
        void draw();

    private:
        std::vector<double> points;

        int jointNum;

        Coord startPos;
        double jointLength;

};
