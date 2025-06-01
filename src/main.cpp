/**
 * ASCII Fire Effect Simulator - Windows Console Version
 * Main entry point and game loop
 * 
 * Author: Your Name
 * Created: Tonight!
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#include "fire_engine.h"
#include "ui_manager.h"
#include "input_handler.h"
#include "colors.h"

// Global cleanup flag for signal handling
volatile sig_atomic_t should_exit = 0;

/**
 * Signal handler for graceful shutdown
 */
void signal_handler(int) {
    should_exit = 1;
}

/**
 * Initialize display system
 */
bool init_display() {
    // Initialize color system
    init_fire_colors();
    
    // Check minimum console size
    int max_x, max_y;
    get_console_size(max_x, max_y);
    
    if (max_y < 25 || max_x < 80) {
        std::cerr << "🔥 Error: Console too small! Need at least 80x25, got " 
                  << max_x << "x" << max_y << std::endl;
        std::cerr << "Please resize your console window and try again." << std::endl;
        return false;
    }
    
    return true;
}

/**
 * Cleanup and restore console
 */
void cleanup_display() {
    set_cursor_visible(true);
    set_console_color(UI_TEXT);
    
    // Move cursor to bottom
    int width, height;
    get_console_size(width, height);
    set_cursor_position(0, height - 1);
}

/**
 * Display startup splash screen
 */
void show_splash() {
    clear_console();
    
    int max_x, max_y;
    get_console_size(max_x, max_y);
    
    // Center the splash text
    int start_y = max_y / 2 - 3;
    int start_x = max_x / 2 - 15;
    
    // Use fire colors for splash
    set_console_color(FIRE_RED);
    set_cursor_position(start_x, start_y);
    std::cout << "🔥 ASCII FIRE SIMULATOR 🔥";
    
    set_console_color(FIRE_YELLOW);
    set_cursor_position(start_x + 2, start_y + 1);
    std::cout << "Initializing flames...";
    
    set_console_color(FIRE_ORANGE);
    set_cursor_position(start_x + 5, start_y + 3);
    std::cout << "Press any key to ignite!";
    
    // Wait for key press or 2 seconds
    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start_time < std::chrono::seconds(2)) {
        if (get_key_press() != -1) break;
        Sleep(100);  // Windows Sleep function
    }
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
        // Show splash screen
        show_splash();
        
        // Get console dimensions
        int max_x, max_y;
        get_console_size(max_x, max_y);
        
        // Create main components
        FireEngine fire_engine(max_x - 20, max_y - 5);  // Leave space for UI
        UIManager ui_manager(max_x, max_y);
        InputHandler input_handler;
        
        // Main simulation loop
        const auto frame_duration = std::chrono::milliseconds(33); // ~30 FPS
        
        bool running = true;
        bool paused = false;
        int frame_count = 0;
        bool first_frame = true;
        
        while (running && !should_exit) {
            auto frame_start = std::chrono::high_resolution_clock::now();
            
            // Clear screen only on first frame
            if (first_frame) {
                clear_console();
                first_frame = false;
            }
            
            // Handle input
            int key = get_key_press();
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
                    fire_engine.set_color_scheme(ui_manager.get_color_scheme());
                    // Force a visual update by resetting some heat
                    for (int i = 0; i < 5; i++) {
                        fire_engine.add_fuel(1);
                        fire_engine.add_fuel(-1);
                    }
                    break;
                    
                case InputAction::TOGGLE_PAUSE:
                    paused = !paused;
                    break;
                    
                case InputAction::TOGGLE_HELP:
                    ui_manager.set_help_visible(!ui_manager.is_help_visible());
                    break;
                    
                case InputAction::TOGGLE_STATS:
                    ui_manager.set_stats_detailed(!ui_manager.is_stats_detailed());
                    break;
                    
                case InputAction::MOUSE_CLICK:
                    // Mouse not implemented for console version
                    break;
                    
                default:
                    break;
            }
            
            // Update simulation (if not paused)
            if (!paused) {
                fire_engine.update();
            }
            
            // Render fire effect (main area only)
            fire_engine.render();
            
            // Only redraw UI every few frames to reduce flicker
            if (frame_count % 3 == 0) {  // Redraw UI every 3 frames
                // Draw UI overlay
                ui_manager.render(fire_engine.get_stats(), paused);
            }
            
            frame_count++;
            
            // Frame rate limiting
            auto frame_end = std::chrono::high_resolution_clock::now();
            auto frame_time = frame_end - frame_start;
            
            if (frame_time < frame_duration) {
                auto sleep_time = frame_duration - frame_time;
                auto sleep_ms = std::chrono::duration_cast<std::chrono::milliseconds>(sleep_time);
                Sleep(static_cast<DWORD>(sleep_ms.count()));
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