#include "Board.h"
#include "Temp.h"
#include "Console.h"
#include <conio.h>
#include <assert.h>
#include <vector>

/****************************************************************************************************
Modifies the 2d vector 'moves' to be all the possible moves the vector 'pieces' can make for the
pieces which correspond to the current 'turn'. 'sim' states whether this function was called through
a simulation
****************************************************************************************************/
void getMoves(std::vector<std::vector<Point>> & moves, const std::vector<Piece*> & pieces, int turn, bool sim = false) {
    for (int pieceNum = 0; pieceNum < 32; pieceNum++) {
        if (pieces[pieceNum]->getRemoved() == false) {
            if (pieces[pieceNum]->getPlayerTeam() == turn) {
                // For every piece which has not been removed and is on the correct team
                pieces[pieceNum]->move(moves[pieceNum], sim);
            }
        }
    }
}

/****************************************************************************************************
Randomly picks a possible move from 'moves', determined by info from 'pieces' and 'turn'. 'sim' states
whether this function was called through a simulation
****************************************************************************************************/
void makeMove(std::vector<std::vector<Point>> & moves, const std::vector<Piece*> & pieces, int turn, bool sim = false) {
    // Set offset
    int offset = 0;
    if (turn == 1) {
        offset = 0;
    }
    else {
        offset = 16;
    }

    // Fill blank array
    int poss[16];
    for (int i = 0; i < 16; i++) {
        poss[i] = -1;
    }

    // Fills the array only with indexs which correspond to pieces which can make moves
    int count = 0;
    for (int i = offset; i < offset + 16; i++) { // For every piece on the corresponding team
        if (moves[i][0] != Point(-1, -1)) { // If that piece can move
            poss[count] = i; // Add its index
            count++; // Increse the number
        }
    }
    if (count < 1) {
        // Stalemate
        return;
    }

    // Randomly pick a move which can be made
    int piece = poss[rand() % count]; // Pick a random index in range which corresponds with a piece which can move
    int move = rand() % (int)moves[piece].size(); // Pick a random move the piece can make
    pieces[piece]->makeMove(moves[piece][move], sim); // Make the move

    // Make the move
    std::string string = pieces[piece]->getChar() + moves[piece][move].asString();
    conCONSOLE.write(string, 0, (turn + 3) / 2);
}

/****************************************************************************************************
Copies and simulates the board in order to play out the game for several turns, scoring moves which
fit certain critera.
    *****This function does not currently work*****
****************************************************************************************************/
void gradeMoves(int turn) {
    // Copy the pieces to the temp
    TEMP_BOARD.m_pieces.resize(32);
    for (int i = 0; i < 32; i++) {
        TEMP_BOARD.m_pieces[i] = new Piece;
        *TEMP_BOARD.m_pieces[i] = *BOARD.getPieces()[i];
    }
    // Copy the board to the temp
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            TEMP_BOARD.m_board[col][row] = BOARD.getBoard()[col][row];
        }
    }

    // Get possible moves
    std::vector<std::vector<Point>> moves;
    moves.resize(32, std::vector<Point>(1, Point{ -1,-1 }));
    getMoves(moves, TEMP_BOARD.m_pieces, turn, true);

    // AI bit
    for (int i = 0; i < 1; i++) { // 'i' represents number of iterations for the simulation to go through
        makeMove(moves, TEMP_BOARD.m_pieces, turn, true);

        // Grade moves
    }

    // Return scores
}

/****************************************************************************************************
Checks the board to determine if either of the Kings have been taken, if so return true ending the game
****************************************************************************************************/
bool checkWin() {
    if (BOARD.getPiece(15)->getRemoved() == true || BOARD.getPiece(31)->getRemoved() == true) {
        return true;
    }
    return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Primary Loop Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************************
****************************************************************************************************/
void init() {
    BOARD.writeBoard();
}

/****************************************************************************************************
****************************************************************************************************/
void mainloop() {
    int turn = 1; // -1 for black, 1 for white
    int turnNum = 0; // Number of turns into the game
    while (true) { // Game Loop
        turnNum++;
        conCONSOLE.write(turnNum, 0, 0);
        // Column is index of piece, row is possible moves
        std::vector<std::vector<Point>> moves;
        moves.resize(32, std::vector<Point>(1, Point{ -1,-1 }));

        // White turn loop
        while (turn == 1) {
            getMoves(moves, BOARD.getPieces(), turn);

            //gradeMoves(turn); ***** Does not work

            makeMove(moves, BOARD.getPieces(), turn);

            turn = -1;
        }

        // Updates UI
        BOARD.writeBoard();
        if (checkWin() == true) {
            return;
        }

        // Black turn loop
        while (turn == -1) {
            getMoves(moves, BOARD.getPieces(), turn);

            //gradeMoves(turn); ***** Does not work

            makeMove(moves, BOARD.getPieces(), turn);

            turn = 1;
        }

        // Updates UI
        BOARD.writeBoard();
        if (checkWin() == true) {
            return;
        }

        // Pauses for user input
        while (true) {
            int key = _getch();
            if (key == VK_ESCAPE) {
                return;
            }
            else if (key == ' ') {
                break;
            }
        }

    }
}

/****************************************************************************************************
****************************************************************************************************/
void shutdown() {
    conCONSOLE.write("Press Escape to quit", 5, 5);

    while (true) {
        int key = _getch();
        if (key == VK_ESCAPE) {
            return;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Function
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************************
****************************************************************************************************/
int main() {
    try {
        init();
        mainloop();
        shutdown();
    }
    catch (...) {
        MessageBox(0, "Unkown Exception", "Exception Thrown", 0);
    }
}