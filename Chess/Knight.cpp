#include "Knight.h"
#include "Board.h"
#include "Temp.h"
#include "Point.h"

/****************************************************************************************************
****************************************************************************************************/
Knight::Knight(int player, Point pos) : Piece(player, pos) {
    // Character changes depending on the team
    if (player == 1) {
        m_char = 'n';
    }
    else if (player == -1) {
        m_char = 'N';
    }

    m_playerTeam = player;
    m_pos = pos;
};

/****************************************************************************************************
****************************************************************************************************/
void Knight::move(std::vector<Point> & moves, bool sim) {
    int size = 0;
    Point temp = m_pos;

    // Knights move in 'L' shaped pattern, two squares in a straight line then one square to the side
    for (int i = 0; i < 8; i++) {
        temp = m_pos;
        // Upwards pair
        if (i < 2) {
            temp.m_x += i*2 -1;
            temp.m_y += -2;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                if (sim == false) {
                    if (BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
                else {
                    if (TEMP_BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
            }
        }
        // Downwards pair
        else if (i<4) {
            temp.m_x += i%2 * 2 - 1;
            temp.m_y += 2;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                if (sim == false) {
                    if (BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
                else {
                    if (TEMP_BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
            }
        }
        // Left pair
        else if (i<6) {
            temp.m_x += -2;
            temp.m_y += i % 2 * 2 - 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                if (sim == false) {
                    if (BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
                else {
                    if (TEMP_BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
            }
        }
        // Right pair
        else {
            temp.m_x += 2;
            temp.m_y += i % 2 * 2 - 1;
            if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                if (sim == false) {
                    if (BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
                else {
                    if (TEMP_BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;
                    }
                    else {
                        // Take piece
                        if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                            // Other Team
                            size++;
                            moves.resize(size);
                            moves[size - 1] = temp;
                        }
                    }
                }
            }
        }
    }
}