#pragma once
#include <Windows.h>
#include <vector>

// The Console class handles writing to the console as well handling the console's window
// The class writes to the console, with a char, float, int or std::string
// The class is used by any class which is required to write to the screen
// The class is a singleton and uncopiable

#define conCONSOLE con::Console::instance()

namespace con {
    class Console {
    public:
        // Function returns the singeton instance of the 'Console' class
        static Console & instance();
        // Destructor for the 'Console' class
        ~Console();

        // Clears the screen, removing all characters and placing the cursor at (0,0)
        void clearScreen();

        // Set the dimensiions for the the window's width and height
        void setConsoleDimensions(int width, int height);

        // Sets the window's title
        void setConsoleTitle(std::string title);

        // Shows or hides the blinking cursor
        void showCursor(bool visible);

        // Splits a string into a vector of strings, by the delimiter
        std::vector<std::string> split(const std::string & s, char delimiter);

        // --------------------------------------------------------------------------------------------------
        // Write Functions
        // --------------------------------------------------------------------------------------------------

        // Writes a char to the console at position
        void write(char text, int x, int y);
        // Writes a float to the console at position
        void write(float text, int x, int y);
        // Writes an int to the console at position
        void write(int text, int x, int y);
        // Writes a string to the console at position
        void write(std::string text, int x, int y);

    private:

        // --------------------------------------------------------------------------------------------------
        // Member Variables
        // --------------------------------------------------------------------------------------------------

        HANDLE m_hConsole = NULL;

        // Constructor takes the title for the window as well as a width and height for dimensions
        Console(const std::string & title, int width, int height);
    };
} // Namespace con