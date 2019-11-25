#pragma once
#include "Piece.h"
#include "Point.h"
#include <vector>

// The Temp class consists of a 2D vector of string for the board, and a vector of the pieces
// The class handles the getting and setting of different aspect of the member variables
// The class is designed to have variables from the 'Board' class copied into it and then used
// to simulate the Board
// The class is a singleton and uncopiable

#define TEMP_BOARD Temp::instance()

class Temp {
public:
    // Function returns the singeton instance of the 'Temp' class
    static Temp & instance();

    // Destructor
    ~Temp();

    // --------------------------------------------------------------------------------------------------
    // Get Functions
    // --------------------------------------------------------------------------------------------------

    // Returns the string value of the board at a certain 'pos'
    std::string getSquare(Point pos)const;

    // Returns the indiviual piece referenced by its index, associated with the class
    Piece* getPiece(int index)const;

    // Returns a reference to the Piece which is at a specific 'point'
    Piece* getPieceAtPoint(Point point)const;

    // --------------------------------------------------------------------------------------------------
    // Set Functions
    // --------------------------------------------------------------------------------------------------

    // Sets the value of a specific 'pos' on the board to the character 'piece'
    void setSquare(Point pos, char piece);

    // Writes the board to the screen using the console
    void writeBoard();

    // --------------------------------------------------------------------------------------------------
    // Member Variables
    // --------------------------------------------------------------------------------------------------

    std::vector<Piece*> m_pieces;
    std::vector<std::vector<std::string>> m_board{ 8, std::vector<std::string>(8) };

private:
    // Constructor
    Temp();
};