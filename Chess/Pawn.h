#pragma once
#include "Piece.h"
#include "Point.h"
#include <vector>

// The Pawn class consists of overridden functions of the parent class 'Piece'
// The class handles calculating the moves the piece can make - 'move'

class Pawn : public Piece {
public:
    // Constructors
    Pawn() = default;
    Pawn(int player, Point pos);

    // Logic for determining where the Bishop can move, modifies 'moves'
    virtual void move(std::vector<Point> & moves, bool sim = false);

    // --------------------------------------------------------------------------------------------------
    // Member Variables
    // --------------------------------------------------------------------------------------------------

    bool m_moved = false;
};