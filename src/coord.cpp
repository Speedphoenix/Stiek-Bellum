#include "coord.h"


using namespace std;

Coord::Coord(int _x, int _y)
    :x(_x), y(_y)
{
    //ctor
}

Coord::Coord()
    :x(-1), y(-1)
{
    //ctor
}

Coord::~Coord()
{
    //dtor
}

ostream& operator<<(ostream& theStream, Coord& what)
{
    theStream << " " << what.x << " " << what.y << " ";

    return theStream;
}


istream& operator>>(istream& theStream, Coord& what)
{
    theStream >> what.x >> what.y;

    return theStream;
}

