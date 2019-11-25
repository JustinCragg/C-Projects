#pragma once
#include "Piece.h"
#include "Point.h"
#include <vector>


// The Board class consists of a 2D vector of string for the board, and a vector of the pieces
// The class handles the getting and setting of different aspect of the member variables
// and also resetting
// The class is a singleton and uncopiable

#define BOARD Board::instance()
class Board {
public:
    // Function returns the singeton instance of the 'Board' class
    static Board & instance();

    // Destructor for the 'Board' class
    ~Board();

    // Resets the board. Clears all variables and resets them to inital values
    // Must be called at the start of every game
    void resetBoard();

    // --------------------------------------------------------------------------------------------------
    // Get Functions
    // --------------------------------------------------------------------------------------------------

    // Returns the 'board' associated with the class
    std::vector<std::vector<std::string>> getBoard()const;
    
    // Returns the indiviual piece referenced by its index, associated with the class
    Piece* getPiece(int index)const;
    
    // Returns a reference to the Piece which is at a specific 'point'
    Piece* getPieceAtPoint(Point point)const;
    
    // Returns the 'pieces' associated with the class
    std::vector<Piece*> getPieces()const;
    
    // Returns the string value of the board at a certain 'pos'
    std::string getSquare(Point pos) const;

    // --------------------------------------------------------------------------------------------------
    // Set Functions
    // --------------------------------------------------------------------------------------------------

    // Sets the value of a specific 'pos' on the board to the character 'piece'
    void setSquare(Point pos, char piece);

    // Writes the board to the screen using the console
    void writeBoard();

private:
    // Constructor for the 'Board' class
    Board();
    
    // --------------------------------------------------------------------------------------------------
    // Member Variables
    // --------------------------------------------------------------------------------------------------

    std::vector<Piece*> m_pieces;
    std::vector<std::vector<std::string>> m_board{ 8, std::vector<std::string>(8) };
};