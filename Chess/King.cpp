#include "King.h"
#include "Board.h"
#include "Temp.h"
#include "Point.h"

/****************************************************************************************************
****************************************************************************************************/
King::King(int player, Point pos) {
    // Character changes depending on the team
    if (player == 1) {
        m_char = 'k';
    }
    else if (player == -1) {
        m_char = 'K';
    }

    m_playerTeam = player;
    m_pos = pos;
}

/****************************************************************************************************
****************************************************************************************************/
void King::move(std::vector<Point> & moves, bool sim) {
    int size = 0;
    // Everything
    Point temp = m_pos;
    for (int i = 0; i < 8; i++) {
        temp.m_x += i % 3 - 1;
        temp.m_y += i / 3 - 1;
        if (temp.m_x >= 0 && temp.m_x < 8 && temp.m_y >= 0 && temp.m_y < 8 && temp != m_pos) {
            // For every square it can move to in the direction
            // If it can move there
            if (sim == false) {
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
        }
    }
}