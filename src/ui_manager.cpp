/**
 * ASCII Fire Effect Simulator
 * User interface implementation - Windows Console Only
 * 
 * Simplified version for Windows Console API
 */

#include "ui_manager.h"
#include "colors.h"
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

// Helper function for older compilers without std::clamp
template<typename T>
T clamp_ui(T value, T min_val, T max_val) {
    return std::max(min_val, std::min(value, max_val));
}

/**
 * Constructor
 */
UIManager::UIManager(int screen_w, int screen_h)
    : current_scheme(CLASSIC_FIRE)
    , help_visible(false)
    , stats_detailed(false)
    , border_effects(true)
    , smooth_fps(0.0f)
    , smooth_temp(0.0f)
    , smooth_fuel(50)
    , frame_counter(0) {
    
    layout.screen_width = screen_w;
    layout.screen_height = screen_h;
    
    calculate_layout();
    
    // Initialize animations
    animations.border_flash = 0;
    animations.fuel_bar_pulse = 0;
    animations.temp_indicator = 0;
    animations.stat_smoothing = 0.9f;
}

/**
 * Calculate UI layout based on screen dimensions
 */
void UIManager::calculate_layout() {
    layout.panel_width = 25;
    layout.panel_x = layout.screen_width - layout.panel_width;
    layout.status_height = 3;
    layout.help_width = 30;
    
    layout.fire_area_width = layout.screen_width - layout.panel_width - 2;
    layout.fire_area_height = layout.screen_height - layout.status_height - 2;
}

/**
 * Main render function
 */
void UIManager::render(const FireStats& stats, bool paused) {
    // Update animations
    update_animations();
    
    // Smooth statistics for better display
    smooth_statistics(stats);
    
    // Render main UI components
    render_control_panel(stats, paused);
    render_status_bar(stats);
    
    // Conditional rendering
    if (help_visible) {
        render_help_panel();
    }
    
    if (stats_detailed) {
        render_detailed_stats(stats);
    }
    
    frame_counter++;
}

/**
 * Render main control panel
 */
void UIManager::render_control_panel(const FireStats& stats, bool paused) {
    int panel_height = 20;
    int start_y = 2;
    
    // Draw main panel box
    draw_box(layout.panel_x, start_y, layout.panel_width, panel_height, "FIRE CONTROL");
    
    int content_x = layout.panel_x + 2;
    int content_y = start_y + 2;
    
    // Title with color
    set_console_color(UI_HIGHLIGHT);
    set_cursor_position(content_x, content_y);
    std::cout << "FIRE SIMULATOR";
    content_y += 2;
    
    // Pause indicator
    if (paused) {
        set_console_color(UI_WARNING);
        set_cursor_position(content_x, content_y);
        std::cout << "** PAUSED **";
    } else {
        set_console_color(UI_SUCCESS);
        set_cursor_position(content_x, content_y);
        std::cout << "RUNNING     ";  // Extra spaces to clear old text
    }
    content_y += 2;
    
    // Fuel level
    set_console_color(UI_TEXT);
    set_cursor_position(content_x, content_y);
    std::cout << "Fuel: " << smooth_fuel << "%";
    content_y += 2;
    
    // Temperature
    set_console_color(UI_TEXT);
    set_cursor_position(content_x, content_y);
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "Heat: %.1f", smooth_temp);
    std::cout << temp_str;
    content_y += 2;
    
    // Wind
    set_console_color(UI_TEXT);
    set_cursor_position(content_x, content_y);
    char wind_str[32];
    snprintf(wind_str, sizeof(wind_str), "Wind: %.1f", stats.wind_speed);
    std::cout << wind_str;
    content_y += 2;
    
    // Color scheme
    set_console_color(UI_TEXT);
    set_cursor_position(content_x, content_y);
    std::cout << "Colors:";
    set_console_color(UI_HIGHLIGHT);
    set_cursor_position(content_x, content_y + 1);
    std::cout << get_scheme_name(current_scheme);
    content_y += 3;
    
    // FPS
    set_console_color(UI_TEXT);
    set_cursor_position(content_x, content_y);
    char fps_str[32];
    snprintf(fps_str, sizeof(fps_str), "FPS: %.1f", smooth_fps);
    std::cout << fps_str;
}

/**
 * Render status bar at bottom
 */
void UIManager::render_status_bar(const FireStats& stats) {
    int status_y = layout.screen_height - layout.status_height;
    
    // Draw status box
    draw_box(0, status_y, layout.screen_width, layout.status_height, nullptr);
    
    int content_y = status_y + 1;
    
    // Left side - controls hint
    set_console_color(UI_TEXT);
    set_cursor_position(2, content_y);
    std::cout << "Controls: W/E=Wind  +/-=Fuel  C=Colors  R=Reset  Q=Quit  H=Help";
    
    // Right side - statistics
    char stats_str[100];
    snprintf(stats_str, sizeof(stats_str), "Active: %d  Max: %d  Heat: %d", 
             stats.active_pixels, stats.max_temp, stats.total_heat / 100);
    
    int stats_x = layout.screen_width - strlen(stats_str) - 2;
    set_console_color(UI_HIGHLIGHT);
    set_cursor_position(stats_x, content_y);
    std::cout << stats_str;
}

/**
 * Render help panel
 */
void UIManager::render_help_panel() {
    int help_x = 2;
    int help_y = 2;
    int help_height = 15;
    
    // Draw help box
    draw_box(help_x, help_y, layout.help_width, help_height, "HELP");
    
    // Simple help text
    const char* help_lines[] = {
        "FIRE SIMULATOR CONTROLS",
        "",
        "W/E - Wind Left/Right",
        "+/- - Fuel Up/Down", 
        "C   - Change Colors",
        "R   - Reset Fire",
        "H   - Toggle Help",
        "S   - Toggle Stats",
        "Q   - Quit",
        "",
        "Press H to hide help"
    };
    
    int line_count = sizeof(help_lines) / sizeof(help_lines[0]);
    
    for (int i = 0; i < line_count && i < help_height - 3; i++) {
        if (strlen(help_lines[i]) == 0) continue;
        
        int color = (strstr(help_lines[i], "FIRE") != nullptr) ? UI_HIGHLIGHT : UI_TEXT;
        set_console_color(color);
        set_cursor_position(help_x + 2, help_y + 2 + i);
        std::cout << help_lines[i];
    }
}

/**
 * Render detailed statistics
 */
void UIManager::render_detailed_stats(const FireStats& stats) {
    int stats_x = layout.screen_width / 2 - 15;
    int stats_y = 2;
    int stats_width = 30;
    int stats_height = 12;
    
    // Draw stats box
    draw_box(stats_x, stats_y, stats_width, stats_height, "DETAILED STATS");
    
    int content_x = stats_x + 2;
    int content_y = stats_y + 2;
    
    set_console_color(UI_TEXT);
    
    char buffer[50];
    
    snprintf(buffer, sizeof(buffer), "Total Heat: %d", stats.total_heat);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
    
    snprintf(buffer, sizeof(buffer), "Active Pixels: %d", stats.active_pixels);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
    
    snprintf(buffer, sizeof(buffer), "Avg Temp: %.1f", stats.average_temp);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
    
    snprintf(buffer, sizeof(buffer), "Max Temp: %d", stats.max_temp);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
    
    snprintf(buffer, sizeof(buffer), "Wind Speed: %.1f", stats.wind_speed);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
    
    snprintf(buffer, sizeof(buffer), "Fuel Level: %d%%", stats.fuel_level);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
    
    snprintf(buffer, sizeof(buffer), "Frames: %d", stats.frames_rendered);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
    
    snprintf(buffer, sizeof(buffer), "FPS: %.2f", stats.fps);
    set_cursor_position(content_x, content_y++);
    std::cout << buffer;
}

/**
 * Draw a bordered box
 */
void UIManager::draw_box(int x, int y, int width, int height, const char* title) {
    int border_color = border_effects ? 
        (FIRE_ORANGE + (animations.border_flash / 10) % 3) : UI_BORDER;
    
    set_console_color(border_color);
    
    // Draw corners and edges
    set_cursor_position(x, y);
    std::cout << '+';
    set_cursor_position(x + width - 1, y);
    std::cout << '+';
    set_cursor_position(x, y + height - 1);
    std::cout << '+';
    set_cursor_position(x + width - 1, y + height - 1);
    std::cout << '+';
    
    // Draw horizontal lines
    for (int i = 1; i < width - 1; i++) {
        set_cursor_position(x + i, y);
        std::cout << '-';
        set_cursor_position(x + i, y + height - 1);
        std::cout << '-';
    }
    
    // Draw vertical lines
    for (int i = 1; i < height - 1; i++) {
        set_cursor_position(x, y + i);
        std::cout << '|';
        set_cursor_position(x + width - 1, y + i);
        std::cout << '|';
    }
    
    // Draw title if provided
    if (title) {
        int title_x = x + center_text(title, width);
        set_console_color(UI_HIGHLIGHT);
        set_cursor_position(title_x, y);
        std::cout << " " << title << " ";
    }
}

/**
 * Draw progress bar (simplified)
 */
void UIManager::draw_progress_bar(int x, int y, int width, int value, 
                                 const char*, int color) {
    int filled = (value * width) / 100;
    
    set_console_color(color);
    set_cursor_position(x, y);
    
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            std::cout << '#';
        } else {
            std::cout << '.';
        }
    }
    
    // Draw percentage
    char percent_str[8];
    snprintf(percent_str, sizeof(percent_str), " %d%%", value);
    std::cout << percent_str;
}

/**
 * Draw temperature gauge (simplified)
 */
void UIManager::draw_temperature_gauge(int x, int y, float temperature) {
    int temp_int = static_cast<int>(temperature);
    int color = get_fire_color(temp_int, current_scheme);
    
    set_console_color(color);
    set_cursor_position(x, y);
    
    for (int i = 0; i < 15; i++) {
        int heat_level = (temp_int * 15) / 100;
        char c = (i < heat_level) ? '#' : '.';
        std::cout << c;
    }
    
    // Temperature value
    char temp_str[16];
    snprintf(temp_str, sizeof(temp_str), " %.1f", temperature);
    set_console_color(UI_TEXT);
    std::cout << temp_str;
}

/**
 * Draw wind indicator (simplified)
 */
void UIManager::draw_wind_indicator(int x, int y, float wind_speed) {
    char wind_chars[20];
    memset(wind_chars, ' ', sizeof(wind_chars));
    wind_chars[19] = '\0';
    
    int center = 9;
    int wind_pos = center + static_cast<int>(wind_speed * 2);
    wind_pos = clamp_ui(wind_pos, 0, 18);
    
    // Draw wind direction
    if (wind_speed < -0.1f) {
        strcpy(wind_chars + std::max(0, wind_pos - 2), "<<<");
        wind_chars[center] = '|';
    } else if (wind_speed > 0.1f) {
        strcpy(wind_chars + std::min(16, wind_pos), ">>>");
        wind_chars[center] = '|';
    } else {
        wind_chars[center] = '|';
    }
    
    int color = (std::abs(wind_speed) > 1.0f) ? UI_WARNING : UI_TEXT;
    set_console_color(color);
    set_cursor_position(x, y);
    std::cout << wind_chars;
}

/**
 * Update UI animations
 */
void UIManager::update_animations() {
    animations.border_flash = (animations.border_flash + 1) % 30;
    animations.fuel_bar_pulse = (animations.fuel_bar_pulse + 1) % 20;
    animations.temp_indicator = (animations.temp_indicator + 1) % 40;
}

/**
 * Smooth statistics for display
 */
void UIManager::smooth_statistics(const FireStats& stats) {
    float alpha = animations.stat_smoothing;
    
    smooth_fps = smooth_fps * alpha + stats.fps * (1.0f - alpha);
    smooth_temp = smooth_temp * alpha + stats.average_temp * (1.0f - alpha);
    smooth_fuel = static_cast<int>(smooth_fuel * alpha + stats.fuel_level * (1.0f - alpha));
}

/**
 * Cycle color scheme
 */
void UIManager::cycle_color_scheme() {
    current_scheme = static_cast<ColorScheme>((current_scheme + 1) % NUM_COLOR_SCHEMES);
}

/**
 * Set help visibility
 */
void UIManager::set_help_visible(bool visible) {
    help_visible = visible;
}

/**
 * Set detailed stats visibility
 */
void UIManager::set_stats_detailed(bool detailed) {
    stats_detailed = detailed;
}

/**
 * Center text calculation
 */
int UIManager::center_text(const char* text, int width) const {
    int text_len = strlen(text);
    return std::max(0, (width - text_len) / 2);
}

/**
 * Get UI color based on current scheme
 */
int UIManager::get_ui_color(int element) const {
    return element;
}