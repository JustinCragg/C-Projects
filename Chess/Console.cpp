#include "Console.h"
#include <Windows.h>
#include <string>
#include <sstream>
#include <vector>


namespace con {
    /****************************************************************************************************
    ****************************************************************************************************/
    Console::Console(const std::string & title, int width, int height) {
        // Creastes instance of the console
        m_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(m_hConsole);

        // Sets UI
        setConsoleTitle(title);
        setConsoleDimensions(width, height);
        
        showCursor(false);
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    Console::~Console() {
        // Closes the console correctly
        CloseHandle(m_hConsole);
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    con::Console & Console::instance() {
        // Default Console, title and dimensions
        static Console theConsole("Title", 75, 25);
        return theConsole;
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    std::vector<std::string> Console::split(const std::string & s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        // Seperates string into different strings by the char 'delimiter'
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::showCursor(bool visible) {
        // Get the Current Console Info (cci)
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(m_hConsole, &cci);

        // Set the cursor visibility
        if (visible == true) {
            cci.bVisible = true;
        }
        else if (visible == false) {
            cci.bVisible = false;
        }
        SetConsoleCursorInfo(m_hConsole, &cci);
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::write(std::string text, int x, int y) {
        // Writes a string onto the console
        DWORD dwBytesWritten = 0;
        COORD pos = { (short)x, (short)y };

        // Splits 'text' then writes each resulting string to the console
        std::vector<std::string> lines = split(text, '\n');
        for (int i = 0; i < (int)lines.size(); i++) {
            pos = { (short)x, (short)(y + i) };

            // Passed the console instance, the text to be written - as a c string, the length of the text,
            // the position and the number of bytes to be written
            bool result = WriteConsoleOutputCharacter(m_hConsole, lines[i].c_str(), lines[i].length(), pos, &dwBytesWritten);
            if (result == false) {
				// Triggers if write fails
            }
        }
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::write(int text, int x, int y) {
        // Passes an int converted to a string to the write function
        write(std::to_string(text), x, y);
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::write(float text, int x, int y) {
        // Passes a float converted to a string to the write function
        write(std::to_string(text), x, y);
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::write(char charText, int x, int y) {
        // Writes a char onto the console
        DWORD dwBytesWritten = 0;
        COORD pos = { (short)x, (short)y };

        // Passed the console instance, the text to be written - as a c string, the length of the text,
        // the position and the number of bytes to be written
        BOOL result = WriteConsoleOutputCharacter(m_hConsole, &charText, 1, pos, &dwBytesWritten);
        if (result == FALSE) {
			// Triggers if write fails
        }
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::clearScreen() {
        // Clears the console of any text
        if (!m_hConsole) {
            return;
        }

        COORD coordScreen = { 0, 0 };    // Home for the cursor 
        DWORD charsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD conSize;

        // Get the number of character cells in the current buffer. 
        if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi)) {
            return;
        }

        // Fill the entire screen with blanks.
        conSize = csbi.dwSize.X * csbi.dwSize.Y;
        if (!FillConsoleOutputCharacter(m_hConsole, (TCHAR) ' ', conSize, coordScreen, &charsWritten)) {
            return;
        }

        // Get the current text attribute.
        if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi)) {
            return;
        }

        // Set the buffer's attributes accordingly.
        if (!FillConsoleOutputAttribute(m_hConsole, csbi.wAttributes, conSize, coordScreen, &charsWritten)) {
            return;
        }

        // Put the cursor at its home coordinates.
        SetConsoleCursorPosition(m_hConsole, coordScreen);
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::setConsoleDimensions(int width = 0, int height = 0) {
        COORD maxSize = GetLargestConsoleWindowSize(m_hConsole);
        bool fullScreen = (width == 0 || height == 0);

        // Determines the values for the size of the window
        COORD windowSize;
        if (fullScreen) {
            windowSize.X = maxSize.X;
            windowSize.Y = maxSize.Y;
        }
        else {
            if (width > maxSize.X) {
                windowSize.X = maxSize.X;
            }
            else {
                windowSize.X = width;
            }
            if (height > maxSize.Y) {
                windowSize.Y = maxSize.Y;
            }
            else {
                windowSize.Y = height;
            }
        }

        // Determines the location of the window
        SetConsoleScreenBufferSize(m_hConsole, windowSize);
        SMALL_RECT displayArea = {};
        displayArea.Right = windowSize.X;
        displayArea.Bottom = windowSize.Y;
        SetConsoleWindowInfo(m_hConsole, TRUE, &displayArea);

        // Gets required instance of the console
        HWND hwnd = GetConsoleWindow();
        HDC hdc = GetDC(hwnd);

        // Sets the console dimensions
        TEXTMETRIC tm;
        GetTextMetrics(hdc, &tm);
        MoveWindow(hwnd, displayArea.Left * tm.tmMaxCharWidth, displayArea.Top * tm.tmHeight, (displayArea.Right - displayArea.Left)* tm.tmMaxCharWidth, (displayArea.Bottom - displayArea.Top)* tm.tmHeight, TRUE);

        ReleaseDC(hwnd, hdc);

        if (fullScreen) {
            // For full screen, we first adjust the size of the window so that its client area will
            // take up the full screen. This is a hack to get the window's chrome out of the way, i.e.
            // off the screen.
            HWND hwnd = GetConsoleWindow();
            RECT rc = {};
            rc.right = GetSystemMetrics(SM_CXSCREEN);
            rc.bottom = GetSystemMetrics(SM_CYSCREEN);
            AdjustWindowRect(&rc, GetWindowLong(hwnd, GWL_STYLE), FALSE);

            // Now, move the window into position with the adjusted size
            MoveWindow(hwnd, rc.left - 2, rc.top - 2, rc.right - rc.left, rc.bottom - rc.top, TRUE);
        }
    }

    /****************************************************************************************************
    ****************************************************************************************************/
    void Console::setConsoleTitle(std::string title) {
        SetConsoleTitle(title.c_str());
    }
} // Namespace con