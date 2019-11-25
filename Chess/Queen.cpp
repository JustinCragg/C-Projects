#include "Queen.h"
#include "Board.h"
#include "Temp.h"
#include "Point.h"

/****************************************************************************************************
****************************************************************************************************/
Queen::Queen(int player, Point pos) : Piece(player, pos) {
    // Chracter changes depending on the team
    if (player == 1) {
        m_char = 'q';
    }
    else if (player == -1) {
        m_char = 'Q';
    }

    m_playerTeam = player;
    m_pos = pos;
};

/****************************************************************************************************
****************************************************************************************************/
void Queen::move(std::vector<Point> & moves, bool sim) {
    int size = 0;
    // Horizontal
    Point temp = m_pos;
    if (sim == false) {
        for (int right = 0; right < 8; right++) {
            temp.m_x += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        for (int left = 0; left < 8; left++) {
            temp.m_x += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        // Vertical
        temp = m_pos;
        for (int down = 0; down < 8; down++) {
            temp.m_y += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        for (int up = 0; up < 8; up++) {
            temp.m_y += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Left Up Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += -1;
            temp.m_y += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Right Up Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += 1;
            temp.m_y += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Left Down Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += -1;
            temp.m_y += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Right Down Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += 1;
            temp.m_y += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
    }
    else { // Simulation
        for (int right = 0; right < 8; right++) {
            temp.m_x += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        for (int left = 0; left < 8; left++) {
            temp.m_x += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        // Vertical
        temp = m_pos;
        for (int down = 0; down < 8; down++) {
            temp.m_y += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        for (int up = 0; up < 8; up++) {
            temp.m_y += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Left Up Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += -1;
            temp.m_y += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Right Up Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += 1;
            temp.m_y += -1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Left Down Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += -1;
            temp.m_y += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        temp = m_pos;
        // Right Down Diagonal
        for (int i = 0; i < 8; i++) {
            temp.m_x += 1;
            temp.m_y += 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                // For every square it can move to in the direction
                // If it can move there
                if (TEMP_BOARD.getSquare(temp) == " ") {
                    // Valid Move
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
                else {
                    // Take piece
                    if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() == m_playerTeam) {
                        // Same Team
                        break;
                    }
                    else {
                        // Other Team
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
    }
}