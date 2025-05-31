/**
 * ASCII Fire Effect Simulator
 * User interface manager
 * 
 * Handles rendering of UI elements, controls, and statistics
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "colors.h"
#include "fire_engine.h"
#include "input_handler.h"

/**
 * UI layout information
 */
struct UILayout {
    int screen_width;       // Total screen width
    int screen_height;      // Total screen height
    int fire_area_width;    // Fire simulation area width
    int fire_area_height;   // Fire simulation area height
    int panel_width;        // Control panel width
    int panel_x;            // Control panel X position
    int status_height;      // Status bar height
    int help_width;         // Help panel width
};

/**
 * Animation states for UI elements
 */
struct UIAnimations {
    int border_flash;       // Border flash counter
    int fuel_bar_pulse;     // Fuel bar pulse animation
    int temp_indicator;     // Temperature indicator animation
    float stat_smoothing;   // Statistics smoothing factor
};

/**
 * User interface manager class
 */
class UIManager {
private:
    UILayout layout;
    UIAnimations animations;
    ColorScheme current_scheme;
    
    // Display states
    bool help_visible;
    bool stats_detailed;
    bool border_effects;
    
    // Smoothed statistics for display
    float smooth_fps;
    float smooth_temp;
    int smooth_fuel;
    
    // Animation counters
    int frame_counter;
    
public:
    /**
     * Constructor
     * @param screen_w Screen width
     * @param screen_h Screen height
     */
    UIManager(int screen_w, int screen_h);
    
    /**
     * Render complete UI overlay
     * @param stats Current fire simulation statistics
     * @param paused Whether simulation is paused
     */
    void render(const FireStats& stats, bool paused);
    
    /**
     * Cycle to next color scheme
     */
    void cycle_color_scheme();
    
    /**
     * Set whether help should be visible
     * @param visible True to show help
     */
    void set_help_visible(bool visible);
    
    /**
     * Set whether detailed stats should be shown
     * @param detailed True for detailed statistics
     */
    void set_stats_detailed(bool detailed);
    
    /**
     * Check if help is currently visible
     * @return True if help should be displayed
     */
    bool is_help_visible() const { return help_visible; }
    
    /**
     * Check if detailed stats are visible
     * @return True if stats should be displayed
     */
    bool is_stats_detailed() const { return stats_detailed; }
    
    /**
     * Update UI animations
     */
    void update_animations();
    
    /**
     * Get current color scheme
     * @return Current ColorScheme
     */
    ColorScheme get_color_scheme() const { return current_scheme; }

private:
    /**
     * Calculate UI layout based on screen size
     */
    void calculate_layout();
    
    /**
     * Render main control panel
     * @param stats Current simulation statistics
     * @param paused Whether simulation is paused
     */
    void render_control_panel(const FireStats& stats, bool paused);
    
    /**
     * Render status bar at bottom
     * @param stats Current simulation statistics
     */
    void render_status_bar(const FireStats& stats);
    
    /**
     * Render help panel
     */
    void render_help_panel();
    
    /**
     * Render detailed statistics
     * @param stats Current simulation statistics
     */
    void render_detailed_stats(const FireStats& stats);
    
    /**
     * Draw a bordered box
     * @param x X position
     * @param y Y position
     * @param width Box width
     * @param height Box height
     * @param title Optional title for box
     */
    void draw_box(int x, int y, int width, int height, const char* title = nullptr);
    
    /**
     * Draw a progress bar
     * @param x X position
     * @param y Y position
     * @param width Bar width
     * @param value Current value (0-100)
     * @param label Bar label
     * @param color Color scheme for bar
     */
    void draw_progress_bar(int x, int y, int width, int value, 
                          const char* label, int color);
    
    /**
     * Draw temperature indicator
     * @param x X position
     * @param y Y position
     * @param temperature Current temperature
     */
    void draw_temperature_gauge(int x, int y, float temperature);
    
    /**
     * Draw wind indicator
     * @param x X position
     * @param y Y position
     * @param wind_speed Current wind speed
     */
    void draw_wind_indicator(int x, int y, float wind_speed);
    
    /**
     * Draw animated fire border
     * @param x X position
     * @param y Y position
     * @param width Border width
     * @param height Border height
     */
    void draw_fire_border(int x, int y, int width, int height);
    
    /**
     * Smooth statistics for display
     * @param stats Current raw statistics
     */
    void smooth_statistics(const FireStats& stats);
    
    /**
     * Get color for UI element based on current scheme
     * @param element UI element type
     * @return Color pair ID
     */
    int get_ui_color(int element) const;
    
    /**
     * Center text in given width
     * @param text Text to center
     * @param width Available width
     * @return X offset for centering
     */
    int center_text(const char* text, int width) const;
    
    /**
     * Format number with units
     * @param value Numeric value
     * @param buffer Output buffer
     * @param size Buffer size
     * @param suffix Unit suffix
     */
    void format_number(float value, char* buffer, size_t size, const char* suffix) const;
};

#endif // UI_MANAGER_H