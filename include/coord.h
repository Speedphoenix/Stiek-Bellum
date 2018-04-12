#ifndef COORD_H
#define COORD_H

#include "../defines.h"

class Coord
{
    //member variables
    public:

        int x;
        int y;

    public:
        Coord();
        Coord(int _x, int _y);
        virtual ~Coord();
};


std::ostream& operator<<(std::ostream& theStream, Coord& what);
std::istream& operator>>(std::istream& theStream, Coord& what);



#endif // COORD_H
