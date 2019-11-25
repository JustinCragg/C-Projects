#pragma once
#include "Point.h"
#include <assert.h>
#include <vector>

// The Piece class is a base class for generic pieces, it contains a character representing itself
// An int represeting its team, a Point for its position and a bool showing whether it has been remmoved
// from the board
// The class handles movement of the piece as well as getting and setting the member variables
// The class is used as a parent for all other Pieces

enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

class Piece {
public:
    // Constructors
    Piece();
    Piece(bool player, Point pos);

    // Function which handles moving the piece to a 'destination', and the taking of pieces at the
    // destination if neccesary
    virtual void makeMove(Point destination, bool sim = false);

    // Function is overridden by child classes, determines what moves the piece can make, modifies 'moves'
    virtual void move(std::vector<Point> & moves, bool sim = false);

    // Sets 'pos' off the board and sets 'removed'
    void remove();

    // --------------------------------------------------------------------------------------------------
    // Get Functions
    // --------------------------------------------------------------------------------------------------

    // Returns the char representing the piece
    std::string getChar()const;

    // Returns the position of the piece
    Point getPos()const;

    // Returns which team the piece is on, -1/black, 1/white
    int getPlayerTeam()const;

    // Returns whether the piece has been removed from play
    bool getRemoved()const;
protected:

    // --------------------------------------------------------------------------------------------------
    // Member Variables
    // --------------------------------------------------------------------------------------------------

    // Representation of piece
    std::string m_char = "*";
    // Team the piece is on
    int m_playerTeam;
    // Position of piece
    Point m_pos = { 0, 0 };
    // Whether the piece has been removed from play
    bool m_removed = false;
};