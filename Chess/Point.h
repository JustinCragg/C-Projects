#pragma once
#include <string>

// The Point class is a standard 2D vector, containing two ints for the x and y
// The class handles basic operators and conversion to string
// The class is used by all objects which require a position relative to a board

class Point {
public:
    // Constructors
    Point();
    Point(int ix, int iy);

    // Returns a string representation of the point "(x, y)"
    std::string asString()const;

    // --------------------------------------------------------------------------------------------------
    // Member Variables
    // --------------------------------------------------------------------------------------------------

    int m_x = 0;
    int m_y = 0;

    // --------------------------------------------------------------------------------------------------
    // Overloaded Operators
    // --------------------------------------------------------------------------------------------------

    // Adds two Points together (x+x, y+y)
    Point operator+(const Point & p)const;

    // Subtracts two Points from each other (x-x, y-y)
    Point operator-(const Point & p)const;

    // Compares two Points together (x==x, y==y)
    bool operator==(const Point & p)const;

    // Compares two Points together (x!=x, y!=y)
    bool operator!=(const Point & p)const;
};