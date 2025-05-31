/**
 * ASCII Fire Effect Simulator
 * Windows Console Color definitions and schemes
 * 
 * Defines all color pairs and palettes using Windows Console API
 */

#ifndef COLORS_H
#define COLORS_H

#ifdef _WIN32
#include <windows.h>
#else
#include <ncurses.h>
#endif

// Fire color constants for Windows Console
enum FireColors {
    FIRE_BLACK = 0,     // Background/cool areas
    FIRE_RED = 4,       // Hot base fire (FOREGROUND_RED)
    FIRE_ORANGE = 6,    // Medium intensity flames (FOREGROUND_RED | FOREGROUND_GREEN)
    FIRE_YELLOW = 14,   // High intensity flames (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
    FIRE_WHITE = 15,    // Hottest core flames (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
    FIRE_BLUE = 9,      // Blue flame mode (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
    FIRE_CYAN = 11,     // Ice fire mode (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
    FIRE_MAGENTA = 13,  // Plasma mode (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
    FIRE_GREEN = 10,    // Matrix mode (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
    UI_BORDER = 3,      // UI frame color (FOREGROUND_BLUE | FOREGROUND_GREEN)
    UI_TEXT = 7,        // UI text color (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
    UI_HIGHLIGHT = 2,   // UI highlighted elements (FOREGROUND_GREEN)
    UI_WARNING = 12,    // Warning/alert color (FOREGROUND_RED | FOREGROUND_INTENSITY)
    UI_SUCCESS = 8      // Success/good status color (FOREGROUND_GREEN)
};

// Color scheme types
enum ColorScheme {
    CLASSIC_FIRE = 0,   // Traditional red/orange/yellow
    BLUE_FLAME,         // Cool blue flames
    ICE_FIRE,           // Cyan/white cold fire
    PLASMA,             // Purple/magenta plasma
    RAINBOW,            // Multi-color psychedelic
    MATRIX,             // Green matrix-style
    NUM_COLOR_SCHEMES   // Total number of schemes
};

// Heat intensity levels
enum HeatLevel {
    HEAT_COLD = 0,      // No fire, background
    HEAT_EMBER,         // Glowing embers
    HEAT_LOW,           // Low flames
    HEAT_MEDIUM,        // Medium flames  
    HEAT_HIGH,          // High flames
    HEAT_INTENSE,       // Intense flames
    HEAT_CORE,          // Hottest core
    NUM_HEAT_LEVELS
};

#ifdef _WIN32
// Windows Console API globals
extern HANDLE hConsole;
extern CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

/**
 * Initialize Windows console for colors and cursor control
 */
void init_fire_colors();

/**
 * Set text color in Windows console
 */
void set_console_color(int color);

/**
 * Get console dimensions
 */
void get_console_size(int& width, int& height);

/**
 * Set cursor position
 */
void set_cursor_position(int x, int y);

/**
 * Clear console screen
 */
void clear_console();

/**
 * Hide/show console cursor
 */
void set_cursor_visible(bool visible);

/**
 * Check for key press (non-blocking)
 */
int get_key_press();

#else
// ncurses compatibility functions
void init_fire_colors();
#endif

/**
 * Get color for specific heat level in current scheme
 */
int get_fire_color(int heat_level, ColorScheme scheme = CLASSIC_FIRE);

/**
 * Get appropriate character for heat level
 */
char get_fire_char(int heat_level);

/**
 * Get color scheme name for display
 */
const char* get_scheme_name(ColorScheme scheme);

/**
 * Fast heat level to color mapping
 */
inline int get_fire_color(int heat_level, ColorScheme scheme) {
    // Clamp heat level to valid range
    if (heat_level <= 0) return FIRE_BLACK;
    if (heat_level >= 100) {
        switch (scheme) {
            case CLASSIC_FIRE: return FIRE_WHITE;
            case BLUE_FLAME: return FIRE_CYAN;
            case ICE_FIRE: return FIRE_WHITE;
            case PLASMA: return FIRE_MAGENTA;
            case RAINBOW: return FIRE_YELLOW;
            case MATRIX: return FIRE_GREEN;
            default: return FIRE_WHITE;
        }
    }
    
    // Map heat level to color based on scheme
    switch (scheme) {
        case CLASSIC_FIRE:
            if (heat_level < 20) return FIRE_RED;
            if (heat_level < 40) return FIRE_ORANGE;
            if (heat_level < 70) return FIRE_YELLOW;
            return FIRE_WHITE;
            
        case BLUE_FLAME:
            if (heat_level < 30) return FIRE_BLUE;
            if (heat_level < 60) return FIRE_CYAN;
            return FIRE_WHITE;
            
        case ICE_FIRE:
            if (heat_level < 25) return FIRE_CYAN;
            if (heat_level < 50) return FIRE_WHITE;
            return FIRE_YELLOW;
            
        case PLASMA:
            if (heat_level < 30) return FIRE_MAGENTA;
            if (heat_level < 60) return FIRE_CYAN;
            return FIRE_WHITE;
            
        case RAINBOW: {
            // Cycle through colors based on heat
            int color_index = (heat_level / 15) % 6;
            return FIRE_RED + color_index;
        }
            
        case MATRIX:
            if (heat_level < 40) return FIRE_GREEN;
            return FIRE_YELLOW;
            
        case NUM_COLOR_SCHEMES:
        default:
            return FIRE_RED;
    }
}

/**
 * Fast character selection based on heat intensity
 */
inline char get_fire_char(int heat_level) {
    if (heat_level <= 0) return ' ';        // No fire
    if (heat_level < 15) return '.';        // Embers
    if (heat_level < 30) return ':';        // Light flames
    if (heat_level < 50) return '^';        // Medium flames
    if (heat_level < 70) return 'A';        // Dense flames
    if (heat_level < 90) return 'W';        // Intense flames
    return '#';                             // Core heat
}

/**
 * Get display name for color scheme
 */
inline const char* get_scheme_name(ColorScheme scheme) {
    switch (scheme) {
        case CLASSIC_FIRE: return "Classic Fire";
        case BLUE_FLAME: return "Blue Flame";
        case ICE_FIRE: return "Ice Fire";
        case PLASMA: return "Plasma";
        case RAINBOW: return "Rainbow";
        case MATRIX: return "Matrix";
        default: return "Unknown";
    }
}

#endif // COLORS_H