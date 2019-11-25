#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include <conio.h>
#include <assert.h>
#include <Windows.h>

const int BOARD_SIZE = 3;

HANDLE g_hConsole = NULL;

/**********************************************************************************************
***********************************************************************************************/

// Function which changes the visibility of the console cursor
void showCursor(bool visible) {
    // Get the Current Console Info (cci)
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(g_hConsole, &cci);

    // Set the cursor visibility
    if (visible == true) {
        cci.bVisible = true;
    }
    else if (visible == false) {
        cci.bVisible = false;
    }
    SetConsoleCursorInfo(g_hConsole, &cci);
}

// Base console write funtion
void write(std::string text, int x, int y) {
    // Writes a string onto the console
    DWORD dwBytesWritten = 0;
    COORD pos = { (short)x, (short)y };
    WriteConsoleOutputCharacter(g_hConsole, text.c_str(), text.length(), pos, &dwBytesWritten);
}

void write(int text, int x, int y) {
    // Passes an int converted to a string to the write function
    write(std::to_string(text), x, y);
}

void write(float text, int x, int y) {
    // Passes a float converted to a string to the write function
    write(std::to_string(text), x, y);
}

// Clears and resets the console screen
void clearScreen() {
    // Clears the console of any text
    if (!g_hConsole) {
        return;
    }

    COORD coordScreen = { 0, 0 };    // Home for the cursor 
    DWORD charsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD conSize;

    // Get the number of character cells in the current buffer. 
    if (!GetConsoleScreenBufferInfo(g_hConsole, &csbi)) {
        return;
    }

    // Fill the entire screen with blanks.
    conSize = csbi.dwSize.X * csbi.dwSize.Y;
    if (!FillConsoleOutputCharacter(g_hConsole, (TCHAR) ' ', conSize, coordScreen, &charsWritten)) {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(g_hConsole, &csbi)) {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(g_hConsole, csbi.wAttributes, conSize, coordScreen, &charsWritten)) {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(g_hConsole, coordScreen);
}

/**********************************************************************************************
***********************************************************************************************/

// Handles the end game state
void endGame(int state) {
    // Function which displays the win state of the previous game
    if (state == -1) {
        // AI Won
        write("The AI won!", 10, 10);
    }
    else if (state == 1) {
        // Draw
        write("It was a draw!", 10, 10);
    }
    else if (state == 2) {
        // Player Won
        write("The Player won!", 10, 10);
    }
    else {
        // This should not be happening
        write("This should not be happening", 10, 10);
    }
    write("Press any key to continue...", 10, 11);
    int key = _getch();
}

// Holds the game w/l/d
struct State {
    int win = 0;
    int loss = 0;
    int draw = 0;
};

// Binary File Reading for State struct only
State binRead(State placeHolder) {
    State state{ 0,0,0 };
    // Opens the file
    std::ifstream fin("data.dat", std::ios::in | std::ios::binary);

    if (fin.good()) {
        fin.seekg(0, std::ios::beg);
        fin.read((char*)&state, sizeof(State));
    }
    assert(fin.good());

    // Close the file
    fin.close();
    return state;
}

// Binary File Writing for State struct only
void binWrite(State state) {
    // Opens the file creates it if it doesn't exist
    std::ifstream fin("data.dat");

    if (fin.good()) {
        std::ofstream fout("data.dat", std::ios::out | std::ios::binary | std::ios::_Nocreate);
    }
    else {
        std::ofstream fout("data.dat", std::ios::out | std::ios::binary);
    }
    fin.close();

    std::ofstream fout("data.dat", std::ios::out | std::ios::binary | std::ios::_Nocreate);
    if (fout.good()) { // If the file opens with no problem
        fout.seekp(0, std::ios::beg);
        fout.write((char*)& state, sizeof(State)); // Write state to the data where the state is the size of State
    }
    assert(fout.good());
    // Close the file
    fout.close();
}

// Binary File Reading for string only
std::string binRead(std::string placeHolder) {
    char* buffer = nullptr;
    // Opens the file
    std::ifstream fin("data.dat", std::ios::in | std::ios::binary);

    if (fin.good()) {
        int length = 0;
        fin.seekg(sizeof(State), std::ios::beg);
        fin.read((char*)&length, sizeof(int));

        // String
        buffer = new char[length + 1];
        fin.read(buffer, length);
        buffer[length] = 0;
    }
    assert(fin.good());

    // Close the file
    fin.close();

    std::string output = "";
    if (buffer != 0) {
        output = buffer;
        delete[] buffer;
    }
    return output;
}

// Binary File Writing for string only
void binWrite(std::string string) {
    // Opens the file creates it if it doesn't exist
    std::ofstream fout("data.dat", std::ios::out | std::ios::binary | std::ios::_Nocreate);
    if (fout.good()) { // If the file opens with no problem
        // Length
        int length = string.size();
        fout.seekp(sizeof(State), std::ios::beg);
        fout.write((char*)&length, sizeof(int)); // Set state to the data at the position of sizeof(State)

        // String
        // If the cursor is not at the end of the document
        fout.write(string.c_str(), length); // Set state to the data at the position of sizeof(State)
    }
    assert(fout.good());
    // Close the file
    fout.close();
}

void resetSave() {
    std::string name = "";
    binWrite(State{ 0,0,0 }); // Make sure state is reset
    clearScreen();
    write("Welcome, what is your name?", 5, 5); // Ask for name
    while (true) {
        write(name, 5, 6); // Write input characters
        char input = _getch();
        if (input == VK_RETURN && name != "") {
            break;
        }
        else if (isalpha(input)) {
            name += input;
        }
    }
    binWrite(name); // Save name
}

/**********************************************************************************************
***********************************************************************************************/

class Board {
    // Class for handling the Tic Tac Toe board
private:
    std::string tiles[BOARD_SIZE][BOARD_SIZE];
    bool playerTurn = true;
public:
    void clearBoard() { // Clears the screen and rewrites the board
        clearScreen();
        std::string str;
        for (int y = 0; y < BOARD_SIZE; y++) {
            str = "";
            for (int x = 0; x < BOARD_SIZE - 1; x++) {
                str += " |";
            }
            str += " ";
            // Writes the vertical lines of the board
            write(str, 0, y*2);

            if (y == BOARD_SIZE - 1) {
                break;
            }

            str = "";
            for (int i = 0; i < BOARD_SIZE*2-1; i++) {
                str += "-";
            }
            // Writes the horizontal lines of the board
            write(str, 0, y*2+1);
        }
        
        // Clears the stored tiles
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                tiles[y][x] = " ";
            }
        }
    }

    // Writes the tiles onto the board
    void writeBoard()const { 
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                write(tiles[y][x], x % BOARD_SIZE *2, y % BOARD_SIZE*2);
            }
        }
    }

    // Returns the state of the game
    int gameState()const {
        // -1:AI Win, 0:Nothing, 1:Draw, 2:Player Win
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (x == 0 && y == 0) { // Diagonal left to right
                    if (tiles[0][0] == tiles[1][1] && tiles[0][0] == tiles[2][2]) {
                        if (tiles[y][x] == "X") {
                            return 2; // Player Win
                        }
                        else if (tiles[y][x] == "O") {
                            return -1; // Ai Win
                        }
                    }
                }
                else if (x == 2 && y==0) { // Diagonal right to left
                    if (tiles[0][2] == tiles[1][1] && tiles[0][2] == tiles[2][0]) {
                        if (tiles[y][x] == "X") {
                            return 2; // Player Win
                        }
                        else if (tiles[y][x] == "O") {
                            return -1; // Ai Win
                        }
                    }
                }
                if (y == 0) { // Vertical
                    if (tiles[y][x] == tiles[y+1][x] && tiles[y][x] == tiles[y+2][x]) {
                        if (tiles[y][x] == "X") {
                            return 2; // Player Win
                        }
                        else if (tiles[y][x] == "O") {
                            return -1; // Ai Win
                        }
                    }
                }
                if (x==0) { // Horizontal
                    if (tiles[y][x] == tiles[y][x+1] && tiles[y][x] == tiles[y][x+2]) {
                        if (tiles[y][x] == "X") {
                            return 2; // Player Win
                        }
                        else if (tiles[y][x] == "O") {
                            return -1; // Ai Win
                        }
                    }
                }
            }
        }

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) { // Checking if it is a draw
                if (tiles[y][x] == " ") { //  On a blank space continue
                    return 0;
                }
            }
        }
        return 1; // Draw
    }

    // Get the value of a specific tile
    std::string getTile(int x, int y)const {
        return tiles[y][x];
    }

    // Gets whether it is the player's turn
    bool getPlayerTurn()const {
        return playerTurn;
    }

    // Flips the turn
    void endTurn() {
        if (playerTurn == true) {
            playerTurn = false;
        }
        else {
            playerTurn = true;
        }
    }

    // Assigns the player's character to the given position
    bool assignTile(int x, int y, std::string player) {
        if (tiles[y][x] == " ") { // Confirms that the tile is blank
            tiles[y][x] = player;
            return true;
        }
        else {
            return false;
        }
    }
};

class PlayerAI {
    // Class which handles the AI
private:
    int score[BOARD_SIZE][BOARD_SIZE];
public:
    // Function for handling the AI's turn
    void move(Board & board) {
        int x, y;
        simulate(board, x, y); // Returns the statistically best next move for the AI

        while (true) {
            if (board.assignTile(x, y, "O")) {
                return;
            }
        }
    }

    // Resets the AI's score array
    void resetScore() {
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                score[y][x] = 0;
            }
        }
    }

    // Monte Carlo AI system
    void simulate(const Board & original, int & oX, int & oY) {
        Board sim = original;
        resetScore();
        int vic = 0;
        std::string player = "O";

        for (int i = 0; i < 500; i++) { // Number of iterations
            sim = original;
            vic = 0;
            player = "O";

            while (vic == 0) { // Game Loop
                while (true) { // Turn Loop
                    if (sim.assignTile(rand() % BOARD_SIZE, rand() % BOARD_SIZE, player)) { // Pick a random tile
                        break; // If it is empty continue
                    }
                }
                vic = sim.gameState(); // Gets what the game's state is
                if (vic == -1) { // 'O' Win
                    for (int y = 0; y < BOARD_SIZE; y++) {
                        for (int x = 0; x < BOARD_SIZE; x++) {
                            if (sim.getTile(x, y) == "O") { // Tiles which the winning player had are scored high
                                score[y][x] += 2;
                            }
                            else if (sim.getTile(x, y) == "X") { // Tiles which the losing player had are scored low
                                score[y][x] -= 1;
                            }
                        }
                    }
                }
                else if (vic == 2) { // 'X' Win
                    for (int y = 0; y < BOARD_SIZE; y++) {
                        for (int x = 0; x < BOARD_SIZE; x++) {
                            if (sim.getTile(x, y) == "X") { // Tiles which the winning player had are scored high
                                score[y][x] += 3;
                            }
                            else if (sim.getTile(x, y) == "O") { // Tiles which the losing player had are scored low
                                score[y][x] += 0;
                            }
                        }
                    }
                }
                else if (vic == 1) { // Draw
                    for (int y = 0; y < BOARD_SIZE; y++) {
                        for (int x = 0; x < BOARD_SIZE; x++) {
                            if (sim.getTile(x, y) != " ") {
                                score[y][x] += 2; // All tiles are scored moderately
                            }
                        }
                    }
                }
                // Swaps Turn
                if (player == "X") {
                    player = "O";
                }
                else if (player == "O") {
                    player = "X";
                }
            }
        }

        int hx, hy;
        int max;
        sim = original;
        // Gets the highest scoring blank tile
        while (true) {
            hx = 0;
            hy = 0;
            max = 0;
            for (int ty = 0; ty < BOARD_SIZE; ty++) {
                for (int tx = 0; tx < BOARD_SIZE; tx++) {
                    if (score[ty][tx] > max) {
                        hx = tx;
                        hy = ty;
                        max = score[hy][hx];
                    }
                }
            }
            if (sim.assignTile(hx, hy, "O")) { // Ensures the tile is blank
                break;
            }
            else {
                score[hy][hx] = 0;
            }
        }
        oX = hx;
        oY = hy;
        return;
    }
};

/**********************************************************************************************
***********************************************************************************************/

void init(Board & board) {
    // Initiates the console
    g_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(g_hConsole);

    srand((unsigned)time(NULL));
    showCursor(false);

    // ********** FILE STUFF **********
    // Check to see of there is a name
    
    std::ifstream fin("data.dat");

    if (fin.good()) {
        std::string name = binRead(std::string{});
        if (name != "") {
            // Greet
            std::string string = "Welcome, " + name;
            write(string, 5, 5);
            write("Would you like to reset your save? (y, n)", 5, 6);
            // See if they want to reset
            bool stop = false;
            while (stop == false) {
                char input = _getch();
                switch (input) {
                case 'y':
                    stop = true;
                    resetSave();
                    break;
                case 'n':
                    // Continue as normal
                    stop = true;
                    break;
                }
            }
        }
        else {
            resetSave();
        }
    }
    else {
        resetSave();
    }

    fin.close();
    board.clearBoard();
}

void mainLoop(Board & board, PlayerAI & playerAI) { // Main game loop
    bool quit = false;
    State state = binRead(State{});
    while (!quit) {
        board.writeBoard(); // Draws the board and tiles
        std::string name = binRead(std::string{});
        write(name, 1, 23);
        state = binRead(State{});
        std::string str = ("Wins: " + std::to_string(state.win) + " Losses: " + std::to_string(state.loss) + " Draws: " + std::to_string(state.draw));
        write(str, 1, 24);

        if (board.getPlayerTurn() == true) {
            int key = _getch(); // Gets the next character pressed
            switch (key) { // Tries to assign the marker to the corresponding key
            case '7':
                if (board.assignTile(0, 0, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '8':
                if (board.assignTile(1, 0, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '9':
                if (board.assignTile(2, 0, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '4':
                if (board.assignTile(0, 1, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '5':
                if (board.assignTile(1, 1, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '6':
                if (board.assignTile(2, 1, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '1':
                if (board.assignTile(0, 2, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '2':
                if (board.assignTile(1, 2, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case '3':
                if (board.assignTile(2, 2, "X") == true) {
                    board.endTurn();
                    break;
                }
                else {
                    break;
                }
            case VK_ESCAPE: // Escape to quit
                quit = true;
                return;
            }
        }
        else {
            // AI turn
            playerAI.move(board);
            board.endTurn();
        }

        if (board.gameState() != 0) { // Checks if the player has won
            board.writeBoard(); // Updates the board
            endGame(board.gameState());
            if (board.gameState() == -1) {
                state.loss++;
            }
            else if (board.gameState() == 1) {
                state.draw++;
            }
            else if (board.gameState() == 2) {
                state.win++;
            }
            binWrite(state);
            board.writeBoard();
            board.clearBoard();
        }
    }
}

void shutdown() {
    // Closes the console correctly
    CloseHandle(g_hConsole);
}

int main() {
    Board board;
    PlayerAI playerAI;
    playerAI.resetScore();

    init(board);
    mainLoop(board, playerAI);
    shutdown();
}