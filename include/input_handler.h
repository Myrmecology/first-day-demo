/**
 * ASCII Fire Effect Simulator
 * Input handling system
 * 
 * Handles keyboard and mouse input for fire simulation controls
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <ncurses.h>
#endif

/**
 * Available input actions
 */
enum class InputAction {
    NONE = 0,           // No action
    QUIT,               // Exit application
    WIND_LEFT,          // Apply wind to the left
    WIND_RIGHT,         // Apply wind to the right
    INCREASE_FUEL,      // Add more fuel to fire
    DECREASE_FUEL,      // Reduce fuel
    RESET,              // Reset fire simulation
    CYCLE_COLORS,       // Switch color scheme
    TOGGLE_PAUSE,       // Pause/resume simulation
    MOUSE_CLICK,        // Mouse interaction
    TOGGLE_HELP,        // Show/hide help
    SAVE_SCREENSHOT,    // Save current frame
    TOGGLE_STATS        // Show/hide detailed stats
};

/**
 * Input result containing action and additional data
 */
struct InputResult {
    InputAction action;
    int mouse_x;        // Mouse X coordinate (if applicable)
    int mouse_y;        // Mouse Y coordinate (if applicable)
    int intensity;      // Intensity value (for variable actions)
    
    InputResult(InputAction act = InputAction::NONE, 
                int mx = 0, int my = 0, int intens = 0)
        : action(act), mouse_x(mx), mouse_y(my), intensity(intens) {}
};

/**
 * Input handling class
 */
class InputHandler {
private:
    bool help_visible;
    bool stats_visible;
    
    // Mouse state (Windows compatible)
#ifdef _WIN32
    // Windows doesn't use MEVENT
    bool mouse_enabled;
#else
    MEVENT mouse_event;
    bool mouse_enabled;
#endif
    
    // Key repeat prevention
    int last_key;
    int repeat_count;
    
public:
    /**
     * Constructor
     */
    InputHandler();
    
    /**
     * Destructor
     */
    ~InputHandler();
    
    /**
     * Process a single input event
     * @param key The key code from ncurses getch()
     * @return InputResult containing the action and data
     */
    InputResult process_input(int key);
    
    /**
     * Enable/disable mouse input
     * @param enable True to enable mouse input
     */
    void set_mouse_enabled(bool enable);
    
    /**
     * Check if help is currently visible
     * @return True if help should be displayed
     */
    bool is_help_visible() const { return help_visible; }
    
    /**
     * Check if detailed stats are visible
     * @return True if stats should be displayed
     */
    bool is_stats_visible() const { return stats_visible; }
    
    /**
     * Get help text for display
     * @return Array of help strings
     */
    static const char* const* get_help_text();
    
    /**
     * Get number of help lines
     * @return Number of help text lines
     */
    static int get_help_line_count();

private:
    /**
     * Process mouse input
     * @return InputResult for mouse action
     */
    InputResult process_mouse();
    
    /**
     * Check for key repeat (prevent spam)
     * @param key Current key
     * @return True if key should be processed
     */
    bool should_process_key(int key);
    
    /**
     * Initialize mouse support
     */
    void init_mouse();
    
    /**
     * Cleanup mouse support
     */
    void cleanup_mouse();
};

#endif // INPUT_HANDLER_H