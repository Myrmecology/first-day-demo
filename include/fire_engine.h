/**
 * ASCII Fire Effect Simulator
 * Fire simulation engine
 * 
 * Core fire physics and rendering system
 */

#ifndef FIRE_ENGINE_H
#define FIRE_ENGINE_H

#include <vector>
#include <random>
#include "colors.h"

/**
 * Fire simulation statistics
 */
struct FireStats {
    int total_heat;         // Total heat in system
    int active_pixels;      // Number of burning pixels
    float wind_speed;       // Current wind speed
    int fuel_level;         // Current fuel level
    float average_temp;     // Average temperature
    int max_temp;           // Hottest point
    int frames_rendered;    // Total frames since start
    float fps;              // Current FPS
};

/**
 * Fire particle for advanced effects
 */
struct FireParticle {
    float x, y;             // Position
    float vx, vy;           // Velocity
    int heat;               // Heat/intensity
    int life;               // Remaining lifetime
    
    FireParticle(float px, float py, int h) 
        : x(px), y(py), vx(0), vy(0), heat(h), life(100) {}
};

/**
 * Main fire simulation engine
 */
class FireEngine {
private:
    // Grid dimensions
    int width;
    int height;
    
    // Heat grids (double buffered for smooth updates)
    std::vector<std::vector<int>> heat_grid;
    std::vector<std::vector<int>> new_heat_grid;
    
    // Simulation parameters
    float cooling_rate;     // How fast fire cools down
    float wind_strength;    // Current wind effect
    int fuel_amount;        // Amount of fuel being added
    int base_heat;          // Base heat generation
    
    // Wind and physics
    float wind_direction;   // -1.0 to 1.0 (left to right)
    float turbulence;       // Random wind variation
    
    // Particle system
    std::vector<FireParticle> particles;
    int max_particles;
    
    // Random number generation
    std::mt19937 rng;
    std::uniform_real_distribution<float> rand_float;
    std::uniform_int_distribution<int> rand_int;
    
    // Statistics
    FireStats stats;
    int frame_count;
    
    // Color scheme
    ColorScheme current_scheme;
    
    // Performance optimization
    int update_counter;     // Skip some updates for performance
    
public:
    /**
     * Constructor
     * @param w Width of fire simulation area
     * @param h Height of fire simulation area
     */
    FireEngine(int w, int h);
    
    /**
     * Destructor
     */
    ~FireEngine() = default;
    
    /**
     * Update fire simulation by one frame
     */
    void update();
    
    /**
     * Render fire to screen using ncurses
     */
    void render();
    
    /**
     * Reset fire simulation to initial state
     */
    void reset();
    
    /**
     * Set wind strength and direction
     * @param strength Wind strength (-5.0 to 5.0)
     */
    void set_wind(float strength);
    
    /**
     * Add or remove fuel from the fire
     * @param amount Amount to add (negative to remove)
     */
    void add_fuel(int amount);
    
    /**
     * Ignite fire at specific location
     * @param x X coordinate
     * @param y Y coordinate
     */
    void ignite_at(int x, int y);
    
    /**
     * Set color scheme
     * @param scheme New color scheme to use
     */
    void set_color_scheme(ColorScheme scheme);
    
    /**
     * Get current simulation statistics
     * @return FireStats structure with current data
     */
    const FireStats& get_stats() const { return stats; }
    
    /**
     * Get current color scheme
     * @return Current ColorScheme
     */
    ColorScheme get_color_scheme() const { return current_scheme; }

private:
    /**
     * Initialize heat grid and parameters
     */
    void init_grid();
    
    /**
     * Update heat diffusion across the grid
     */
    void update_heat_diffusion();
    
    /**
     * Update particle system
     */
    void update_particles();
    
    /**
     * Apply wind effects to fire
     */
    void apply_wind_effects();
    
    /**
     * Add fuel source at bottom of fire
     */
    void add_fuel_source();
    
    /**
     * Calculate and update statistics
     */
    void update_statistics();
    
    /**
     * Generate new fire particles
     */
    void generate_particles();
    
    /**
     * Clamp value to valid heat range
     * @param value Heat value to clamp
     * @return Clamped value (0-100)
     */
    int clamp_heat(int value) const;
    
    /**
     * Get heat at specific grid position (with bounds checking)
     * @param x X coordinate
     * @param y Y coordinate  
     * @return Heat value at position
     */
    int get_heat_at(int x, int y) const;
    
    /**
     * Set heat at specific grid position (with bounds checking)
     * @param x X coordinate
     * @param y Y coordinate
     * @param heat Heat value to set
     */
    void set_heat_at(int x, int y, int heat);
    
    /**
     * Add random turbulence for realistic fire movement
     */
    void add_turbulence();
    
    /**
     * Create explosion effect at location
     * @param x X coordinate
     * @param y Y coordinate
     * @param intensity Explosion intensity
     */
    void create_explosion(int x, int y, int intensity);
};

// Inline performance-critical functions

inline int FireEngine::clamp_heat(int value) const {
    return (value < 0) ? 0 : (value > 100) ? 100 : value;
}

inline int FireEngine::get_heat_at(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return 0;
    }
    return heat_grid[y][x];
}

inline void FireEngine::set_heat_at(int x, int y, int heat) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        new_heat_grid[y][x] = clamp_heat(heat);
    }
}

#endif // FIRE_ENGINE_H