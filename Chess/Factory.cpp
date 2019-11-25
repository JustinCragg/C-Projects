#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include "Point.h"
#include "Factory.h"
#include <assert.h>

/****************************************************************************************************
****************************************************************************************************/
Factory::Factory() {
}

/****************************************************************************************************
****************************************************************************************************/
Factory::~Factory() {
}

/****************************************************************************************************
'PieceType' enumerator is found in 'Piece.h'
****************************************************************************************************/
Piece* Factory::makePiece(PieceType type, int player, Point pos) {
    switch (type) {
    case PieceType::Pawn:
        return new Pawn(player, pos);
    case PieceType::Rook:
        return new Rook(player, pos);
    case PieceType::Knight:
        return new Knight(player, pos);
    case PieceType::Bishop:
        return new Bishop(player, pos);
    case PieceType::Queen:
        return new Queen(player, pos);
    case PieceType::King:
        return new King(player, pos);
    default:
        assert(false);
        return nullptr;
    }
}
