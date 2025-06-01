/**
 * Console Buffer System - Eliminates Flickering
 * Double-buffered console rendering for smooth animation
 */

#ifndef CONSOLE_BUFFER_H
#define CONSOLE_BUFFER_H

#ifdef _WIN32
#include <windows.h>
#include <vector>

/**
 * Console cell containing character and color
 */
struct ConsoleCell {
    char character;
    int color;
    bool dirty;  // Needs redrawing
    
    ConsoleCell() : character(' '), color(7), dirty(true) {}
    ConsoleCell(char c, int col) : character(c), color(col), dirty(true) {}
};

/**
 * Double-buffered console rendering system
 */
class ConsoleBuffer {
private:
    int width, height;
    std::vector<std::vector<ConsoleCell>> buffer;
    std::vector<std::vector<ConsoleCell>> last_buffer;
    HANDLE hConsole;
    bool initialized;
    
public:
    /**
     * Initialize console buffer
     */
    ConsoleBuffer();
    
    /**
     * Destructor
     */
    ~ConsoleBuffer();
    
    /**
     * Initialize the buffer system
     */
    bool init();
    
    /**
     * Set character at position
     */
    void set_char(int x, int y, char c, int color);
    
    /**
     * Clear entire buffer
     */
    void clear();
    
    /**
     * Clear specific area
     */
    void clear_area(int x, int y, int w, int h);
    
    /**
     * Present buffer to screen (only draws changed cells)
     */
    void present();
    
    /**
     * Get buffer dimensions
     */
    void get_size(int& w, int& h) const { w = width; h = height; }
    
    /**
     * Force redraw of entire screen
     */
    void force_redraw();
    
private:
    /**
     * Update console size
     */
    void update_size();
};

// Global console buffer instance
extern ConsoleBuffer g_console;

#endif // _WIN32
#endif // CONSOLE_BUFFER_H