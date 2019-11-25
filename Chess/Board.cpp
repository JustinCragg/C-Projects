#include "Piece.h"
#include "Board.h"
#include "Factory.h"
#include "Point.h"
#include "Console.h"
#include <string>

/****************************************************************************************************
****************************************************************************************************/
Board & Board::instance() {
    static Board theBoard;
    return theBoard;
}

/****************************************************************************************************
****************************************************************************************************/
Board::Board() {
    resetBoard();
}

/****************************************************************************************************
****************************************************************************************************/
Board::~Board() {
    for (int i = 0; i < (int)m_pieces.size(); i++) {
        delete m_pieces[i];
    }
    m_pieces.clear();
}

/****************************************************************************************************
****************************************************************************************************/
void Board::resetBoard() {
    // Clears board
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            m_board[col][row] = " ";
        }
    }
    // Ensures pieces is the correct size
    m_pieces.resize(32);

    // Creates and assigns each piece
        // *****White Pieces
    // White Pawns
    for (int i = 0; i < 8; i++) {
        m_pieces[i] = Factory::makePiece(PieceType::Pawn, 1, Point{ i,6 });
    }
    // White Rooks
    for (int i = 0; i < 2; i++) {
        m_pieces[8 + i] = Factory::makePiece(PieceType::Rook, 1, Point{ i % 2 * 7,7 });
    }
    // White Knights
    for (int i = 0; i < 2; i++) {
        m_pieces[10 + i] = Factory::makePiece(PieceType::Knight, 1, Point{ i % 2 * 5 + 1,7 });
    }
    // White Bishop
    for (int i = 0; i < 2; i++) {
        m_pieces[12 + i] = Factory::makePiece(PieceType::Bishop, 1, Point{ i % 2 * 3 + 2,7 });
    }
    // White Queen
    m_pieces[14] = Factory::makePiece(PieceType::Queen, 1, Point{ 4,7 });
    // White King
    m_pieces[15] = Factory::makePiece(PieceType::King, 1, Point{ 3,7 });
        // *****Black Pieces
    // Black Pawns
    for (int i = 0; i < 8; i++) {
        m_pieces[16 + i] = Factory::makePiece(PieceType::Pawn, -1, Point{ i,1 });
    }
    // Black Rooks
    for (int i = 0; i < 2; i++) {
        m_pieces[24 + i] = Factory::makePiece(PieceType::Rook, -1, Point{ i % 2 * 7,0 });
    }
    // Black Knights
    for (int i = 0; i < 2; i++) {
        m_pieces[26 + i] = Factory::makePiece(PieceType::Knight, -1, Point{ i % 2 * 5 +1,0 });
    }
    // Black Bishop
    for (int i = 0; i < 2; i++) {
        m_pieces[28 + i] = Factory::makePiece(PieceType::Bishop, -1, Point{ i%2 * 3 + 2,0 });
    }
    // Black Queen
    m_pieces[30] = Factory::makePiece(PieceType::Queen, -1, Point{ 4,0 });
    // Black King
    m_pieces[31] = Factory::makePiece(PieceType::King, -1, Point{ 3,0 });
}

/****************************************************************************************************
****************************************************************************************************/
void Board::writeBoard() {
    // Resets board
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            m_board[col][row] = " ";
        }
    }

    // Replaces blank characters with the characters of the pieces at their current positions
    for (int i = 0; i < 32; i++) {
        Point pos = m_pieces[i]->getPos();
        if (getPieceAtPoint(pos)->getRemoved() == false) {
            // Ensures the pieces aren't written if they have been removed
            m_board[pos.m_y][pos.m_x] = m_pieces[i]->getChar();
        }
    }

    // Adds the characters to be written to a string
    std::string string = "";
    string += "   0 1 2 3 4 5 6 7";
    string += "\n  -----------------";
    string += '\n';
    for (int col = 0; col < 8; col++) {
        string += std::to_string(col) + " |";
        for (int row = 0; row < 8; row++) {
            string += m_board[col][row];
            string += "|";
        }
        string += '\n';
        string += "  -----------------";
        string += '\n';
    }
    // Writes the string
    conCONSOLE.write(string, 30, 3);
}

/****************************************************************************************************
****************************************************************************************************/
std::vector<std::vector<std::string>> Board::getBoard() const {
    return m_board;
}

/****************************************************************************************************
****************************************************************************************************/
Piece* Board::getPiece(int index)const {
    return m_pieces[index];
}

/****************************************************************************************************
****************************************************************************************************/
Piece* Board::getPieceAtPoint(Point point)const {
    for (int i = 0; i < 32; i++) { // Max must be set to number of pieces
        if (m_pieces[i]->getPos() == point) {
            return m_pieces[i];
        }
    }
    return nullptr;
}

/****************************************************************************************************
****************************************************************************************************/
std::vector<Piece*> Board::getPieces() const {
    return m_pieces;
}

/****************************************************************************************************
****************************************************************************************************/
std::string Board::getSquare(Point pos)const {
    return m_board[pos.m_y][pos.m_x];
}

/****************************************************************************************************
****************************************************************************************************/
void Board::setSquare(Point pos, char piece) {
    m_board[pos.m_y][pos.m_x] = piece;
}