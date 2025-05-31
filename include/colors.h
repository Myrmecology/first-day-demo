/**
 * ASCII Fire Effect Simulator
 * Color definitions and schemes
 * 
 * Defines all color pairs and palettes used in the fire simulation
 */

#ifndef COLORS_H
#define COLORS_H

#include <ncurses.h>

// Fire color pair constants
enum FireColors {
    FIRE_BLACK = 1,     // Background/cool areas
    FIRE_RED,           // Hot base fire
    FIRE_ORANGE,        // Medium intensity flames
    FIRE_YELLOW,        // High intensity flames
    FIRE_WHITE,         // Hottest core flames
    FIRE_BLUE,          // Blue flame mode
    FIRE_CYAN,          // Ice fire mode
    FIRE_MAGENTA,       // Plasma mode
    FIRE_GREEN,         // Matrix mode
    UI_BORDER,          // UI frame color
    UI_TEXT,            // UI text color
    UI_HIGHLIGHT,       // UI highlighted elements
    UI_WARNING,         // Warning/alert color
    UI_SUCCESS          // Success/good status color
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

// Heat intensity levels (for mapping temperature to colors)
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

/**
 * Initialize all color pairs for ncurses
 */
void init_fire_colors();

/**
 * Get color pair for specific heat level in current scheme
 * @param heat_level The intensity level (0-100)
 * @param scheme Current color scheme
 * @return ncurses color pair ID
 */
int get_fire_color(int heat_level, ColorScheme scheme = CLASSIC_FIRE);

/**
 * Get appropriate character for heat level
 * @param heat_level The intensity level (0-100)
 * @return ASCII character representing the heat intensity
 */
char get_fire_char(int heat_level);

/**
 * Get color scheme name for display
 * @param scheme The color scheme
 * @return Human-readable name
 */
const char* get_scheme_name(ColorScheme scheme);

/**
 * Implementation details - color pair initialization
 */
namespace ColorImpl {
    void init_classic_fire();
    void init_blue_flame();
    void init_ice_fire();
    void init_plasma();
    void init_rainbow();
    void init_matrix();
    void init_ui_colors();
}

// Inline implementations for performance-critical functions

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
            
        case RAINBOW:
            // Cycle through colors based on heat
            int color_index = (heat_level / 15) % 6;
            return FIRE_RED + color_index;
            
        case MATRIX:
            if (heat_level < 40) return FIRE_GREEN;
            return FIRE_YELLOW;
            
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
    if (heat_level < 30) return '░';        // Light flames
    if (heat_level < 50) return '▒';        // Medium flames
    if (heat_level < 70) return '▓';        // Dense flames
    if (heat_level < 90) return '█';        // Intense flames
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