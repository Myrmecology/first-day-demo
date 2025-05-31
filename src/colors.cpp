/**
 * ASCII Fire Effect Simulator
 * Color system implementation
 * 
 * Implements color pair initialization for all fire schemes
 */

#include "colors.h"
#include <ncurses.h>

/**
 * Initialize all color pairs for the fire simulator
 */
void init_fire_colors() {
    // Initialize base fire colors
    ColorImpl::init_classic_fire();
    ColorImpl::init_blue_flame();
    ColorImpl::init_ice_fire();
    ColorImpl::init_plasma();
    ColorImpl::init_rainbow();
    ColorImpl::init_matrix();
    ColorImpl::init_ui_colors();
}

namespace ColorImpl {

/**
 * Initialize classic fire color scheme (red/orange/yellow/white)
 */
void init_classic_fire() {
    init_pair(FIRE_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(FIRE_RED, COLOR_RED, COLOR_BLACK);
    init_pair(FIRE_ORANGE, COLOR_YELLOW, COLOR_BLACK);     // Closest to orange
    init_pair(FIRE_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(FIRE_WHITE, COLOR_WHITE, COLOR_BLACK);
}

/**
 * Initialize blue flame color scheme
 */
void init_blue_flame() {
    init_pair(FIRE_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(FIRE_CYAN, COLOR_CYAN, COLOR_BLACK);
    // White and black already initialized
}

/**
 * Initialize ice fire color scheme (cyan/white)
 */
void init_ice_fire() {
    // Cyan and white already initialized in other schemes
    // This scheme reuses existing color pairs
}

/**
 * Initialize plasma color scheme (magenta/cyan/white)
 */
void init_plasma() {
    init_pair(FIRE_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    // Cyan and white already initialized
}

/**
 * Initialize rainbow color scheme
 */
void init_rainbow() {
    // Uses multiple existing colors in rotation
    // All base colors already initialized
}

/**
 * Initialize matrix color scheme (green theme)
 */
void init_matrix() {
    init_pair(FIRE_GREEN, COLOR_GREEN, COLOR_BLACK);
    // Yellow already initialized for highlights
}

/**
 * Initialize UI color scheme
 */
void init_ui_colors() {
    init_pair(UI_BORDER, COLOR_CYAN, COLOR_BLACK);
    init_pair(UI_TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(UI_HIGHLIGHT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(UI_WARNING, COLOR_RED, COLOR_BLACK);
    init_pair(UI_SUCCESS, COLOR_GREEN, COLOR_BLACK);
}

} // namespace ColorImpl