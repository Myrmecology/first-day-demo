/**
 * ASCII Fire Effect Simulator
 * Input handling implementation
 * 
 * Processes keyboard and mouse input for interactive fire control
 */

#include "input_handler.h"
#include <cstring>

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
    if (key == ERR) {
        return InputResult(InputAction::NONE);
    }
    
    // Check for key repeat prevention
    if (!should_process_key(key)) {
        return InputResult(InputAction::NONE);
    }
    
    // Handle mouse input
    if (key == KEY_MOUSE) {
        return process_mouse();
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
        case KEY_LEFT:
            return InputResult(InputAction::WIND_LEFT);
            
        case 'e':
        case 'E':
        case KEY_RIGHT:
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
            
        case ' ':  // Space bar
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
        
        // Arrow keys (alternative wind control)
        case KEY_UP:
            return InputResult(InputAction::INCREASE_FUEL);
            
        case KEY_DOWN:
            return InputResult(InputAction::DECREASE_FUEL);
        
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
 * Process mouse input events
 */
InputResult InputHandler::process_mouse() {
    if (!mouse_enabled) {
        return InputResult(InputAction::NONE);
    }
    
    if (getmouse(&mouse_event) == OK) {
        // Left click to ignite fire
        if (mouse_event.bstate & BUTTON1_CLICKED) {
            return InputResult(InputAction::MOUSE_CLICK, 
                             mouse_event.x, mouse_event.y);
        }
        
        // Right click for wind burst (future feature)
        if (mouse_event.bstate & BUTTON3_CLICKED) {
            // Could add wind burst at mouse position
            return InputResult(InputAction::NONE);
        }
        
        // Mouse wheel for fuel control
        if (mouse_event.bstate & BUTTON4_PRESSED) {  // Wheel up
            return InputResult(InputAction::INCREASE_FUEL);
        }
        
        if (mouse_event.bstate & BUTTON5_PRESSED) {  // Wheel down
            return InputResult(InputAction::DECREASE_FUEL);
        }
    }
    
    return InputResult(InputAction::NONE);
}

/**
 * Prevent key repeat spam
 */
bool InputHandler::should_process_key(int key) {
    if (key == last_key) {
        repeat_count++;
        // Allow some keys to repeat (movement, fuel)
        switch (key) {
            case 'w': case 'W': case KEY_LEFT:
            case 'e': case 'E': case KEY_RIGHT:
            case '+': case '=':
            case '-': case '_':
            case KEY_UP: case KEY_DOWN:
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
 * Initialize mouse support
 */
void InputHandler::init_mouse() {
    if (has_mouse()) {
        mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED | 
                  BUTTON4_PRESSED | BUTTON5_PRESSED, NULL);
        mouse_enabled = true;
    }
}

/**
 * Cleanup mouse support
 */
void InputHandler::cleanup_mouse() {
    if (mouse_enabled) {
        mousemask(0, NULL);
        mouse_enabled = false;
    }
}