#pragma once
#include "Piece.h"
#include "Point.h"
#include <vector>

// The Rook class consists of overridden functions of the parent class 'Piece'
// The class handles calculating the moves the piece can make - 'move'

class Rook : public Piece {
public:
    // Constructors
    Rook() = default;
    Rook(int player, Point pos);

    // Logic for determining where the Bishop can move, modifies 'moves'
    virtual void move(std::vector<Point> & moves, bool sim = false);
};