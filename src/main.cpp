/**
 * ASCII Fire Effect Simulator
 * Main entry point and game loop
 * 
 * Author: Your Name
 * Created: Tonight!
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <ncurses.h>

#include "fire_engine.h"
#include "ui_manager.h"
#include "input_handler.h"
#include "colors.h"

// Global cleanup flag for signal handling
volatile sig_atomic_t should_exit = 0;

/**
 * Signal handler for graceful shutdown
 */
void signal_handler(int signum) {
    should_exit = 1;
}

/**
 * Initialize ncurses and terminal settings
 */
bool init_display() {
    // Initialize ncurses
    initscr();
    
    // Check if terminal supports colors
    if (!has_colors()) {
        endwin();
        std::cerr << "🔥 Error: Terminal does not support colors!" << std::endl;
        return false;
    }
    
    // Configure terminal settings
    start_color();              // Enable color support
    use_default_colors();       // Use terminal's default colors
    cbreak();                   // Disable line buffering
    noecho();                   // Don't echo pressed keys
    keypad(stdscr, TRUE);       // Enable special keys
    nodelay(stdscr, TRUE);      // Non-blocking input
    curs_set(0);                // Hide cursor
    
    // Check minimum terminal size
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    if (max_y < 25 || max_x < 80) {
        endwin();
        std::cerr << "🔥 Error: Terminal too small! Need at least 80x25, got " 
                  << max_x << "x" << max_y << std::endl;
        return false;
    }
    
    return true;
}

/**
 * Cleanup and restore terminal
 */
void cleanup_display() {
    endwin();
}

/**
 * Display startup splash screen
 */
void show_splash() {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Center the splash text
    int start_y = max_y / 2 - 3;
    int start_x = max_x / 2 - 15;
    
    // Use fire colors for splash
    attron(COLOR_PAIR(FIRE_RED));
    mvprintw(start_y, start_x, "🔥 ASCII FIRE SIMULATOR 🔥");
    attroff(COLOR_PAIR(FIRE_RED));
    
    attron(COLOR_PAIR(FIRE_YELLOW));
    mvprintw(start_y + 1, start_x + 2, "Initializing flames...");
    attroff(COLOR_PAIR(FIRE_YELLOW));
    
    attron(COLOR_PAIR(FIRE_ORANGE));
    mvprintw(start_y + 3, start_x + 5, "Press any key to ignite!");
    attroff(COLOR_PAIR(FIRE_ORANGE));
    
    refresh();
    
    // Wait for key press or 2 seconds
    timeout(2000);
    getch();
    timeout(-1);
}

/**
 * Main application loop
 */
int main() {
    std::cout << "🔥 Starting ASCII Fire Effect Simulator..." << std::endl;
    
    // Set up signal handlers for graceful shutdown
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Initialize display
    if (!init_display()) {
        return 1;
    }
    
    try {
        // Initialize color schemes
        init_fire_colors();
        
        // Show splash screen
        show_splash();
        
        // Get terminal dimensions
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        
        // Create main components
        FireEngine fire_engine(max_x - 20, max_y - 5);  // Leave space for UI
        UIManager ui_manager(max_x, max_y);
        InputHandler input_handler;
        
        // Main simulation loop
        auto last_frame = std::chrono::high_resolution_clock::now();
        const auto frame_duration = std::chrono::milliseconds(33); // ~30 FPS
        
        bool running = true;
        bool paused = false;
        
        while (running && !should_exit) {
            auto frame_start = std::chrono::high_resolution_clock::now();
            
            // Handle input
            int key = getch();
            auto input_result = input_handler.process_input(key);
            
            switch (input_result.action) {
                case InputAction::QUIT:
                    running = false;
                    break;
                    
                case InputAction::WIND_LEFT:
                    fire_engine.set_wind(-2.0f);
                    break;
                    
                case InputAction::WIND_RIGHT:
                    fire_engine.set_wind(2.0f);
                    break;
                    
                case InputAction::INCREASE_FUEL:
                    fire_engine.add_fuel(10);
                    break;
                    
                case InputAction::DECREASE_FUEL:
                    fire_engine.add_fuel(-10);
                    break;
                    
                case InputAction::RESET:
                    fire_engine.reset();
                    break;
                    
                case InputAction::CYCLE_COLORS:
                    ui_manager.cycle_color_scheme();
                    break;
                    
                case InputAction::TOGGLE_PAUSE:
                    paused = !paused;
                    break;
                    
                case InputAction::MOUSE_CLICK:
                    fire_engine.ignite_at(input_result.mouse_x, input_result.mouse_y);
                    break;
                    
                default:
                    break;
            }
            
            // Update simulation (if not paused)
            if (!paused) {
                fire_engine.update();
            }
            
            // Render frame
            clear();
            
            // Draw fire effect
            fire_engine.render();
            
            // Draw UI overlay
            ui_manager.render(fire_engine.get_stats(), paused);
            
            // Show frame
            refresh();
            
            // Frame rate limiting
            auto frame_end = std::chrono::high_resolution_clock::now();
            auto frame_time = frame_end - frame_start;
            
            if (frame_time < frame_duration) {
                std::this_thread::sleep_for(frame_duration - frame_time);
            }
        }
        
    } catch (const std::exception& e) {
        cleanup_display();
        std::cerr << "🔥 Error: " << e.what() << std::endl;
        return 1;
    }
    
    // Cleanup
    cleanup_display();
    
    std::cout << "🔥 Fire extinguished! Thanks for playing!" << std::endl;
    return 0;
}