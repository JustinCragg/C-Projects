#include "Pawn.h"
#include "Board.h"
#include "Temp.h"
#include "Point.h"
#include <vector>

/****************************************************************************************************
****************************************************************************************************/
Pawn::Pawn(int player, Point pos) : Piece(player, pos) {
    // Charater changes depending on the team
    if (player == 1) {
        m_char = 'p';
    }
    else if (player == -1) {
        m_char = 'P';
    }
    
    m_playerTeam = player;
    m_pos = pos;
}

/****************************************************************************************************
****************************************************************************************************/
void Pawn::move(std::vector<Point> & moves, bool sim) {
    int size = 0;
    // One Forward Move
    Point temp = m_pos;
    temp.m_y += -1 * m_playerTeam;
    if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
        if (sim == false){
            if (BOARD.getSquare(temp) == " ") {
                // Valid Move
                size++;
                moves.resize(size);
                moves[size - 1] = temp;

                // Two Forward Move
                temp = m_pos;
                temp.m_y += -2 * m_playerTeam;
                if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                    if (m_moved == false && BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;;
                    }
                }
            }
        }
        else {
            if (TEMP_BOARD.getSquare(temp) == " ") {
                // Valid Move
                size++;
                moves.resize(size);
                moves[size - 1] = temp;

                // Two Forward Move
                temp = m_pos;
                temp.m_y += -2 * m_playerTeam;
                if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
                    if (m_moved == false && TEMP_BOARD.getSquare(temp) == " ") {
                        // Valid Move
                        size++;
                        moves.resize(size);
                        moves[size - 1] = temp;;
                    }
                }
            }
        }
    }

    // Left Diagonal Attack
    temp = m_pos;
    temp.m_x += -1;
    temp.m_y += -1 * m_playerTeam;
    if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
        if (sim == false) {
            if (BOARD.getSquare(temp) != " ") {
                if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                    // Valid Attack
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
            }
        }
        else {
            if (TEMP_BOARD.getSquare(temp) != " ") {
                if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                    // Valid Attack
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
            }
        }
    }

    // Right Diagonal Attack
    temp = m_pos;
    temp.m_x += 1;
    temp.m_y += -1 * m_playerTeam;
    if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8) {
        if (sim == false) {
            if (BOARD.getSquare(temp) != " ") {
                if (BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                    // Valid Attack
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
            }
        }
        else {
            if (TEMP_BOARD.getSquare(temp) != " ") {
                if (TEMP_BOARD.getPieceAtPoint(temp)->getPlayerTeam() != m_playerTeam) {
                    // Valid Attack
                    size++;
                    moves.resize(size);
                    moves[size - 1] = temp;
                }
            }
        }
    }
}