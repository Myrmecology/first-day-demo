/**
 * ASCII Fire Effect Simulator
 * User interface implementation
 * 
 * Renders all UI elements including control panels and statistics
 */

#include "ui_manager.h"
#include <ncurses.h>
#include <cstring>
#include <cstdio>
#include <algorithm>

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
    attron(COLOR_PAIR(UI_HIGHLIGHT));
    mvprintw(content_y, content_x, "🔥 FIRE SIM 🔥");
    attroff(COLOR_PAIR(UI_HIGHLIGHT));
    content_y += 2;
    
    // Pause indicator
    if (paused) {
        attron(COLOR_PAIR(UI_WARNING));
        mvprintw(content_y, content_x, "⏸️  PAUSED");
        attroff(COLOR_PAIR(UI_WARNING));
    } else {
        attron(COLOR_PAIR(UI_SUCCESS));
        mvprintw(content_y, content_x, "▶️  RUNNING");
        attroff(COLOR_PAIR(UI_SUCCESS));
    }
    content_y += 2;
    
    // Fuel level bar
    attron(COLOR_PAIR(UI_TEXT));
    mvprintw(content_y, content_x, "Fuel:");
    attroff(COLOR_PAIR(UI_TEXT));
    draw_progress_bar(content_x, content_y + 1, layout.panel_width - 4, 
                     smooth_fuel, "", FIRE_ORANGE);
    content_y += 3;
    
    // Temperature gauge
    attron(COLOR_PAIR(UI_TEXT));
    mvprintw(content_y, content_x, "Heat:");
    attroff(COLOR_PAIR(UI_TEXT));
    draw_temperature_gauge(content_x, content_y + 1, smooth_temp);
    content_y += 3;
    
    // Wind indicator
    attron(COLOR_PAIR(UI_TEXT));
    mvprintw(content_y, content_x, "Wind:");
    attroff(COLOR_PAIR(UI_TEXT));
    draw_wind_indicator(content_x, content_y + 1, stats.wind_speed);
    content_y += 3;
    
    // Color scheme
    attron(COLOR_PAIR(UI_TEXT));
    mvprintw(content_y, content_x, "Colors:");
    attroff(COLOR_PAIR(UI_TEXT));
    attron(COLOR_PAIR(UI_HIGHLIGHT));
    mvprintw(content_y + 1, content_x, "%s", get_scheme_name(current_scheme));
    attroff(COLOR_PAIR(UI_HIGHLIGHT));
    content_y += 3;
    
    // Quick stats
    attron(COLOR_PAIR(UI_TEXT));
    char fps_str[16];
    snprintf(fps_str, sizeof(fps_str), "FPS: %.1f", smooth_fps);
    mvprintw(content_y, content_x, "%s", fps_str);
    attroff(COLOR_PAIR(UI_TEXT));
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
    attron(COLOR_PAIR(UI_TEXT));
    mvprintw(content_y, 2, "Controls: W/E=Wind  +/-=Fuel  C=Colors  R=Reset  Q=Quit  H=Help");
    attroff(COLOR_PAIR(UI_TEXT));
    
    // Right side - statistics
    char stats_str[100];
    snprintf(stats_str, sizeof(stats_str), "Active: %d  Max: %d°  Particles: %d", 
             stats.active_pixels, stats.max_temp, static_cast<int>(stats.total_heat / 100));
    
    int stats_x = layout.screen_width - strlen(stats_str) - 2;
    attron(COLOR_PAIR(UI_HIGHLIGHT));
    mvprintw(content_y, stats_x, "%s", stats_str);
    attroff(COLOR_PAIR(UI_HIGHLIGHT));
}

/**
 * Render help panel
 */
void UIManager::render_help_panel() {
    int help_x = 2;
    int help_y = 2;
    int help_height = InputHandler::get_help_line_count() + 2;
    
    // Draw help box
    draw_box(help_x, help_y, layout.help_width, help_height, "HELP");
    
    // Render help text
    const char* const* help_lines = InputHandler::get_help_text();
    int line_count = InputHandler::get_help_line_count();
    
    for (int i = 0; i < line_count; i++) {
        int color = UI_TEXT;
        if (strstr(help_lines[i], "🔥") != nullptr) {
            color = UI_HIGHLIGHT;
        } else if (strlen(help_lines[i]) == 0) {
            continue;  // Skip empty lines
        }
        
        attron(COLOR_PAIR(color));
        mvprintw(help_y + 2 + i, help_x + 2, "%.25s", help_lines[i]);
        attroff(COLOR_PAIR(color));
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
    
    // Detailed statistics
    char buffer[50];
    
    attron(COLOR_PAIR(UI_TEXT));
    
    snprintf(buffer, sizeof(buffer), "Total Heat: %d", stats.total_heat);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    snprintf(buffer, sizeof(buffer), "Active Pixels: %d", stats.active_pixels);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    snprintf(buffer, sizeof(buffer), "Avg Temp: %.1f°", stats.average_temp);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    snprintf(buffer, sizeof(buffer), "Max Temp: %d°", stats.max_temp);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    snprintf(buffer, sizeof(buffer), "Wind Speed: %.1f", stats.wind_speed);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    snprintf(buffer, sizeof(buffer), "Fuel Level: %d%%", stats.fuel_level);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    snprintf(buffer, sizeof(buffer), "Frames: %d", stats.frames_rendered);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    snprintf(buffer, sizeof(buffer), "FPS: %.2f", stats.fps);
    mvprintw(content_y++, content_x, "%s", buffer);
    
    attroff(COLOR_PAIR(UI_TEXT));
}

/**
 * Draw a bordered box
 */
void UIManager::draw_box(int x, int y, int width, int height, const char* title) {
    int border_color = border_effects ? 
        (FIRE_ORANGE + (animations.border_flash / 10) % 3) : UI_BORDER;
    
    attron(COLOR_PAIR(border_color));
    
    // Draw corners and edges
    mvaddch(y, x, '+');
    mvaddch(y, x + width - 1, '+');
    mvaddch(y + height - 1, x, '+');
    mvaddch(y + height - 1, x + width - 1, '+');
    
    // Draw horizontal lines
    for (int i = 1; i < width - 1; i++) {
        mvaddch(y, x + i, '-');
        mvaddch(y + height - 1, x + i, '-');
    }
    
    // Draw vertical lines
    for (int i = 1; i < height - 1; i++) {
        mvaddch(y + i, x, '|');
        mvaddch(y + i, x + width - 1, '|');
    }
    
    attroff(COLOR_PAIR(border_color));
    
    // Draw title if provided
    if (title) {
        int title_x = x + center_text(title, width);
        attron(COLOR_PAIR(UI_HIGHLIGHT));
        mvprintw(y, title_x, " %s ", title);
        attroff(COLOR_PAIR(UI_HIGHLIGHT));
    }
}

/**
 * Draw progress bar
 */
void UIManager::draw_progress_bar(int x, int y, int width, int value, 
                                 const char* label, int color) {
    int filled = (value * width) / 100;
    
    // Draw bar background
    attron(COLOR_PAIR(UI_BORDER));
    for (int i = 0; i < width; i++) {
        mvaddch(y, x + i, '░');
    }
    attroff(COLOR_PAIR(UI_BORDER));
    
    // Draw filled portion
    attron(COLOR_PAIR(color));
    for (int i = 0; i < filled; i++) {
        mvaddch(y, x + i, '█');
    }
    attroff(COLOR_PAIR(color));
    
    // Draw percentage
    char percent_str[8];
    snprintf(percent_str, sizeof(percent_str), "%d%%", value);
    int percent_x = x + (width - strlen(percent_str)) / 2;
    
    attron(COLOR_PAIR(UI_TEXT));
    mvprintw(y, percent_x, "%s", percent_str);
    attroff(COLOR_PAIR(UI_TEXT));
}

/**
 * Draw temperature gauge
 */
void UIManager::draw_temperature_gauge(int x, int y, float temperature) {
    int temp_int = static_cast<int>(temperature);
    char temp_chars[] = {' ', '.', '░', '▒', '▓', '█'};
    int char_index = std::min(5, temp_int / 20);
    
    int color = get_fire_color(temp_int, current_scheme);
    
    attron(COLOR_PAIR(color));
    for (int i = 0; i < 15; i++) {
        int heat_level = (temp_int * 15) / 100;
        char c = (i < heat_level) ? temp_chars[std::min(5, i / 3 + 1)] : ' ';
        mvaddch(y, x + i, c);
    }
    attroff(COLOR_PAIR(color));
    
    // Temperature value
    char temp_str[16];
    snprintf(temp_str, sizeof(temp_str), "%.1f°", temperature);
    attron(COLOR_PAIR(UI_TEXT));
    mvprintw(y, x + 16, "%s", temp_str);
    attroff(COLOR_PAIR(UI_TEXT));
}

/**
 * Draw wind indicator
 */
void UIManager::draw_wind_indicator(int x, int y, float wind_speed) {
    char wind_chars[20];
    memset(wind_chars, ' ', sizeof(wind_chars));
    wind_chars[19] = '\0';
    
    int center = 9;
    int wind_pos = center + static_cast<int>(wind_speed * 2);
    wind_pos = std::clamp(wind_pos, 0, 18);
    
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
    attron(COLOR_PAIR(color));
    mvprintw(y, x, "%.19s", wind_chars);
    attroff(COLOR_PAIR(color));
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
    smooth_fuel = smooth_fuel * alpha + stats.fuel_level * (1.0f - alpha);
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
    // Map UI elements to fire colors based on current scheme
    switch (current_scheme) {
        case BLUE_FLAME:
            return (element == UI_HIGHLIGHT) ? FIRE_CYAN : element;
        case ICE_FIRE:
            return (element == UI_HIGHLIGHT) ? FIRE_WHITE : element;
        case PLASMA:
            return (element == UI_HIGHLIGHT) ? FIRE_MAGENTA : element;
        default:
            return element;
    }
}