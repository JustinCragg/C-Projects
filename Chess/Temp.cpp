#include "Piece.h"
#include "Temp.h"
#include "Point.h"
#include "Console.h"
#include <string>

/****************************************************************************************************
****************************************************************************************************/
Temp & Temp::instance() {
    static Temp tempBoard;
    return tempBoard;
}

/****************************************************************************************************
****************************************************************************************************/
Temp::Temp() {
}

/****************************************************************************************************
****************************************************************************************************/
Temp::~Temp() {
    for (int i = 0; i < (int)m_pieces.size(); i++) {
        delete m_pieces[i];
    }
    m_pieces.clear();
}

/****************************************************************************************************
****************************************************************************************************/
void Temp::writeBoard() {
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
Piece* Temp::getPiece(int index)const {
    return m_pieces[index];
}

/****************************************************************************************************
****************************************************************************************************/
Piece *Temp::getPieceAtPoint(Point point)const {
    for (int i = 0; i < 32; i++) { // Max must be set to number of pieces
        if (m_pieces[i]->getPos() == point) {
            return m_pieces[i];
        }
    }
    return nullptr;
}

/****************************************************************************************************
****************************************************************************************************/
std::string Temp::getSquare(Point pos)const {
    return m_board[pos.m_y][pos.m_x];
}

/****************************************************************************************************
****************************************************************************************************/
void Temp::setSquare(Point pos, char piece) {
    m_board[pos.m_y][pos.m_x] = piece;
}