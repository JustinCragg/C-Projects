#pragma once
#include "Piece.h"
#include "Point.h"
#include <vector>

// The King class consists of overridden functions of the parent class 'Piece'
// The class handles calculating the moves the piece can make - 'move'

class King : public Piece {
public:
    // Constructors
    King() = default;
    King(int player, Point pos);

    // Logic for determining where the Bishop can move, modifies 'moves'
    virtual void move(std::vector<Point> & moves, bool sim = false);
};