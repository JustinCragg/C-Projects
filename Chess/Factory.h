#pragma once
#include "Piece.h"
#include "Point.h"

// The Factory class is used to handle the creation of different pieces
// The class is used by the board when creating or resetting the pieces

class Factory {
public:
    // Constructor
    Factory();

    // Destructor
    ~Factory();

    // Returns a new Piece object, 'player' and 'pos' are constructors for the piece
    // Works for Pawn, Rook, Knight, Bishop, Queen and King
    static Piece* makePiece(PieceType type, int player, Point pos);
};