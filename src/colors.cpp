/**
 * ASCII Fire Effect Simulator
 * Windows Console color system implementation
 */

#include "colors.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>

// Global Windows console handles
HANDLE hConsole = nullptr;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

/**
 * Initialize Windows console for fire simulator
 */
void init_fire_colors() {
    // Get console handle
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to get console handle!" << std::endl;
        return;
    }
    
    // Get console info
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    
    // Enable virtual terminal processing for better color support (if available)
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    // Note: ENABLE_VIRTUAL_TERMINAL_PROCESSING may not be available on older Windows
    dwMode |= 0x0004;  // ENABLE_VIRTUAL_TERMINAL_PROCESSING value
    SetConsoleMode(hConsole, dwMode);
    
    // Set console title
    SetConsoleTitleA("🔥 ASCII Fire Effect Simulator 🔥");
    
    // Hide cursor
    set_cursor_visible(false);
}

/**
 * Set text color in Windows console
 */
void set_console_color(int color) {
    if (hConsole == nullptr) return;
    
    // Map our color constants to Windows console colors
    WORD windowsColor = 0;
    
    switch (color) {
        case FIRE_BLACK:
            windowsColor = 0;
            break;
        case FIRE_RED:
            windowsColor = FOREGROUND_RED;
            break;
        case FIRE_ORANGE:
            windowsColor = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case FIRE_YELLOW:
            windowsColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case FIRE_WHITE:
            windowsColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case FIRE_BLUE:
            windowsColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case FIRE_CYAN:
            windowsColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case FIRE_MAGENTA:
            windowsColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case FIRE_GREEN:
            windowsColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case UI_BORDER:
            windowsColor = FOREGROUND_BLUE | FOREGROUND_GREEN;
            break;
        case UI_TEXT:
            windowsColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case UI_HIGHLIGHT:
            windowsColor = FOREGROUND_GREEN;
            break;
        case UI_WARNING:
            windowsColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case UI_SUCCESS:
            windowsColor = FOREGROUND_GREEN;
            break;
        default:
            windowsColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
    }
    
    SetConsoleTextAttribute(hConsole, windowsColor);
}

/**
 * Get console dimensions
 */
void get_console_size(int& width, int& height) {
    if (hConsole == nullptr) {
        width = 80;
        height = 25;
        return;
    }
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

/**
 * Set cursor position
 */
void set_cursor_position(int x, int y) {
    if (hConsole == nullptr) return;
    
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

/**
 * Clear console screen
 */
void clear_console() {
    if (hConsole == nullptr) return;
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};
    
    if (GetConsoleScreenBufferInfo(hConsole, &csbi) == 0) return;
    
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    
    if (FillConsoleOutputCharacterA(hConsole, (TCHAR)' ', cellCount, homeCoords, &count) == 0) return;
    
    if (FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count) == 0) return;
    
    SetConsoleCursorPosition(hConsole, homeCoords);
}

/**
 * Hide/show console cursor
 */
void set_cursor_visible(bool visible) {
    if (hConsole == nullptr) return;
    
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

/**
 * Check for key press (non-blocking)
 */
int get_key_press() {
    if (_kbhit()) {
        return _getch();
    }
    return -1;  // No key pressed
}

#else
// Placeholder for non-Windows systems
void init_fire_colors() {
    // ncurses implementation would go here
}
#endif