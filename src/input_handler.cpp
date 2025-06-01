/**
 * ASCII Fire Effect Simulator
 * Input handling implementation - Windows Console Compatible
 * 
 * Processes keyboard input for interactive fire control
 */

#include "input_handler.h"
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

// Help text for display
static const char* help_text[] = {
    "🔥 FIRE SIMULATOR CONTROLS 🔥",
    "",
    "Movement & Wind:",
    "  W / ←  - Wind Left",
    "  E / →  - Wind Right", 
    "  R      - Reset Fire",
    "",
    "Fire Control:",
    "  + / =  - Increase Fuel",
    "  - / _  - Decrease Fuel",
    "  Space  - Pause/Resume",
    "",
    "Display:",
    "  C      - Cycle Colors",
    "  H / ?  - Toggle Help",
    "  S      - Toggle Stats",
    "  P      - Screenshot",
    "",
    "Mouse:",
    "  Click  - Ignite Fire",
    "  Drag   - Wind Direction",
    "",
    "  Q / ESC - Quit",
    "",
    "Press H or ? to hide help"
};

static const int help_line_count = sizeof(help_text) / sizeof(help_text[0]);

/**
 * Constructor
 */
InputHandler::InputHandler() 
    : help_visible(false)
    , stats_visible(false)
    , mouse_enabled(false)
    , last_key(-1)
    , repeat_count(0) {
    
    init_mouse();
}

/**
 * Destructor
 */
InputHandler::~InputHandler() {
    cleanup_mouse();
}

/**
 * Process input and return appropriate action
 */
InputResult InputHandler::process_input(int key) {
    // Handle no input
    if (key == -1) {
        return InputResult(InputAction::NONE);
    }
    
    // Check for key repeat prevention
    if (!should_process_key(key)) {
        return InputResult(InputAction::NONE);
    }
    
    // Process keyboard input
    switch (key) {
        // Quit commands
        case 'q':
        case 'Q':
        case 27:  // ESC
            return InputResult(InputAction::QUIT);
        
        // Wind controls
        case 'w':
        case 'W':
        case 'a':  // Alternative left
        case 'A':
            return InputResult(InputAction::WIND_LEFT);
            
        case 'e':
        case 'E':
        case 'd':  // Alternative right
        case 'D':
            return InputResult(InputAction::WIND_RIGHT);
        
        // Fuel controls
        case '+':
        case '=':
            return InputResult(InputAction::INCREASE_FUEL);
            
        case '-':
        case '_':
            return InputResult(InputAction::DECREASE_FUEL);
        
        // Simulation controls
        case 'r':
        case 'R':
            return InputResult(InputAction::RESET);
            
        case ' ':  // Space bar for pause/unpause
        case 32:   // ASCII code for space
            return InputResult(InputAction::TOGGLE_PAUSE);
        
        // Display controls
        case 'c':
        case 'C':
            return InputResult(InputAction::CYCLE_COLORS);
            
        case 'h':
        case 'H':
        case '?':
            help_visible = !help_visible;
            return InputResult(InputAction::TOGGLE_HELP);
            
        case 's':
        case 'S':
            stats_visible = !stats_visible;
            return InputResult(InputAction::TOGGLE_STATS);
            
        case 'p':
        case 'P':
            return InputResult(InputAction::SAVE_SCREENSHOT);
        
        default:
            return InputResult(InputAction::NONE);
    }
}

/**
 * Enable or disable mouse input
 */
void InputHandler::set_mouse_enabled(bool enable) {
    if (enable && !mouse_enabled) {
        init_mouse();
    } else if (!enable && mouse_enabled) {
        cleanup_mouse();
    }
}

/**
 * Get help text array
 */
const char* const* InputHandler::get_help_text() {
    return help_text;
}

/**
 * Get number of help lines
 */
int InputHandler::get_help_line_count() {
    return help_line_count;
}

/**
 * Process mouse input events (Windows version - simplified)
 */
InputResult InputHandler::process_mouse() {
#ifdef _WIN32
    // Mouse input not implemented for Windows console version
    return InputResult(InputAction::NONE);
#else
    // ncurses mouse handling would go here
    return InputResult(InputAction::NONE);
#endif
}

/**
 * Prevent key repeat spam
 */
bool InputHandler::should_process_key(int key) {
    if (key == last_key) {
        repeat_count++;
        // Allow some keys to repeat (movement, fuel)
        switch (key) {
            case 'w': case 'W': case 'a': case 'A':
            case 'e': case 'E': case 'd': case 'D':
            case '+': case '=':
            case '-': case '_':
                return repeat_count % 3 == 0;  // Every 3rd repeat
            default:
                return repeat_count == 1;      // Only first press
        }
    } else {
        last_key = key;
        repeat_count = 1;
        return true;
    }
}

/**
 * Initialize mouse support (Windows compatible)
 */
void InputHandler::init_mouse() {
#ifdef _WIN32
    // Mouse support not implemented for Windows console version
    mouse_enabled = false;
#else
    // ncurses mouse initialization would go here
    mouse_enabled = false;
#endif
}

/**
 * Cleanup mouse support (Windows compatible)
 */
void InputHandler::cleanup_mouse() {
#ifdef _WIN32
    // Nothing to cleanup for Windows console version
    mouse_enabled = false;
#else
    // ncurses mouse cleanup would go here
    mouse_enabled = false;
#endif
}