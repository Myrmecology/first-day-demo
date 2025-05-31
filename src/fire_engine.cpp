/**
 * ASCII Fire Effect Simulator
 * Fire simulation engine implementation
 * 
 * Implements realistic fire physics with heat diffusion and wind effects
 */

#include "fire_engine.h"
#include <ncurses.h>
#include <algorithm>
#include <cmath>
#include <chrono>

/**
 * Constructor - Initialize fire simulation
 */
FireEngine::FireEngine(int w, int h) 
    : width(w)
    , height(h)
    , cooling_rate(0.85f)
    , wind_strength(0.0f)
    , fuel_amount(50)
    , base_heat(80)
    , wind_direction(0.0f)
    , turbulence(0.1f)
    , max_particles(200)
    , rng(std::chrono::steady_clock::now().time_since_epoch().count())
    , rand_float(0.0f, 1.0f)
    , rand_int(0, 100)
    , frame_count(0)
    , current_scheme(CLASSIC_FIRE)
    , update_counter(0) {
    
    init_grid();
    reset();
}

/**
 * Initialize heat grids
 */
void FireEngine::init_grid() {
    // Initialize heat grids
    heat_grid.resize(height);
    new_heat_grid.resize(height);
    
    for (int y = 0; y < height; y++) {
        heat_grid[y].resize(width, 0);
        new_heat_grid[y].resize(width, 0);
    }
    
    // Reserve space for particles
    particles.reserve(max_particles);
}

/**
 * Reset fire simulation
 */
void FireEngine::reset() {
    // Clear heat grids
    for (int y = 0; y < height; y++) {
        std::fill(heat_grid[y].begin(), heat_grid[y].end(), 0);
        std::fill(new_heat_grid[y].begin(), new_heat_grid[y].end(), 0);
    }
    
    // Clear particles
    particles.clear();
    
    // Reset parameters
    wind_strength = 0.0f;
    wind_direction = 0.0f;
    fuel_amount = 50;
    frame_count = 0;
    
    // Initialize base fire line
    if (height > 0) {
        int bottom_row = height - 1;
        for (int x = width / 4; x < 3 * width / 4; x++) {
            heat_grid[bottom_row][x] = base_heat + rand_int(rng) % 20;
        }
    }
}

/**
 * Main update function - advance simulation by one frame
 */
void FireEngine::update() {
    update_counter++;
    
    // Add fuel source at bottom
    add_fuel_source();
    
    // Apply wind effects
    apply_wind_effects();
    
    // Update heat diffusion
    update_heat_diffusion();
    
    // Add realistic turbulence
    add_turbulence();
    
    // Update particle system
    update_particles();
    
    // Generate new particles from hot areas
    if (update_counter % 2 == 0) {  // Every other frame
        generate_particles();
    }
    
    // Swap heat grids (double buffering)
    std::swap(heat_grid, new_heat_grid);
    
    // Update statistics
    update_statistics();
    
    frame_count++;
}

/**
 * Render fire to screen
 */
void FireEngine::render() {
    // Get screen offset (center the fire)
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int offset_x = (max_x - width) / 2;
    int offset_y = (max_y - height) / 2;
    
    // Render heat grid
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int heat = heat_grid[y][x];
            
            if (heat > 0) {
                char fire_char = get_fire_char(heat);
                int color = get_fire_color(heat, current_scheme);
                
                attron(COLOR_PAIR(color));
                mvaddch(offset_y + y, offset_x + x, fire_char);
                attroff(COLOR_PAIR(color));
            }
        }
    }
    
    // Render particles on top
    for (const auto& particle : particles) {
        if (particle.heat > 10) {
            int px = static_cast<int>(particle.x);
            int py = static_cast<int>(particle.y);
            
            if (px >= 0 && px < width && py >= 0 && py < height) {
                char p_char = (particle.heat > 50) ? '*' : '.';
                int p_color = get_fire_color(particle.heat, current_scheme);
                
                attron(COLOR_PAIR(p_color));
                mvaddch(offset_y + py, offset_x + px, p_char);
                attroff(COLOR_PAIR(p_color));
            }
        }
    }
}

/**
 * Update heat diffusion across grid
 */
void FireEngine::update_heat_diffusion() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int current_heat = heat_grid[y][x];
            
            if (current_heat <= 0) {
                new_heat_grid[y][x] = 0;
                continue;
            }
            
            // Heat rises and spreads
            float new_heat = current_heat * cooling_rate;
            
            // Heat rises (moves up)
            if (y > 0) {
                float rise_amount = current_heat * 0.3f;
                new_heat -= rise_amount;
                new_heat_grid[y - 1][x] += static_cast<int>(rise_amount);
            }
            
            // Heat spreads horizontally
            float spread = current_heat * 0.1f;
            
            if (x > 0) {
                new_heat_grid[y][x - 1] += static_cast<int>(spread);
                new_heat -= spread;
            }
            
            if (x < width - 1) {
                new_heat_grid[y][x + 1] += static_cast<int>(spread);
                new_heat -= spread;
            }
            
            // Set the new heat value
            new_heat_grid[y][x] = clamp_heat(static_cast<int>(new_heat));
        }
    }
}

/**
 * Apply wind effects to fire
 */
void FireEngine::apply_wind_effects() {
    if (std::abs(wind_strength) < 0.1f) return;
    
    // Wind affects upper portions of fire more
    for (int y = 0; y < height * 0.8f; y++) {
        for (int x = 0; x < width; x++) {
            int heat = heat_grid[y][x];
            if (heat <= 0) continue;
            
            // Calculate wind effect (stronger at top)
            float wind_factor = (height - y) / static_cast<float>(height);
            int wind_offset = static_cast<int>(wind_strength * wind_factor);
            
            if (wind_offset != 0) {
                int target_x = x + wind_offset;
                if (target_x >= 0 && target_x < width) {
                    // Move some heat horizontally
                    int moved_heat = heat * 0.2f;
                    new_heat_grid[y][target_x] += moved_heat;
                    new_heat_grid[y][x] -= moved_heat;
                }
            }
        }
    }
}

/**
 * Add fuel source at bottom
 */
void FireEngine::add_fuel_source() {
    if (fuel_amount <= 0 || height == 0) return;
    
    int bottom_row = height - 1;
    int fuel_width = std::max(1, width / 3);
    int start_x = (width - fuel_width) / 2;
    
    for (int x = start_x; x < start_x + fuel_width; x++) {
        if (x >= 0 && x < width) {
            // Add randomized fuel
            int fuel_heat = (fuel_amount * 0.8f) + (rand_int(rng) % (fuel_amount / 2));
            heat_grid[bottom_row][x] = std::max(heat_grid[bottom_row][x], fuel_heat);
        }
    }
}

/**
 * Add realistic turbulence
 */
void FireEngine::add_turbulence() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (heat_grid[y][x] > 20) {
                // Add small random variations
                float turb = (rand_float(rng) - 0.5f) * turbulence * 20.0f;
                new_heat_grid[y][x] += static_cast<int>(turb);
                new_heat_grid[y][x] = clamp_heat(new_heat_grid[y][x]);
            }
        }
    }
}

/**
 * Update particle system
 */
void FireEngine::update_particles() {
    // Update existing particles
    for (auto it = particles.begin(); it != particles.end();) {
        auto& p = *it;
        
        // Update position
        p.x += p.vx;
        p.y += p.vy;
        
        // Apply physics
        p.vy -= 0.1f;  // Gravity/buoyancy
        p.vx += (rand_float(rng) - 0.5f) * 0.2f;  // Random drift
        p.vx *= 0.95f;  // Air resistance
        
        // Apply wind to particles
        p.vx += wind_strength * 0.1f;
        
        // Cool down
        p.heat -= 2;
        p.life--;
        
        // Remove dead particles
        if (p.life <= 0 || p.heat <= 0 || p.y < 0) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * Generate new particles from hot areas
 */
void FireEngine::generate_particles() {
    if (particles.size() >= max_particles) return;
    
    // Find hot spots to generate particles
    for (int y = height / 2; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int heat = heat_grid[y][x];
            
            if (heat > 60 && rand_float(rng) < 0.1f) {
                // Create particle
                FireParticle particle(x, y, heat);
                particle.vx = (rand_float(rng) - 0.5f) * 2.0f;
                particle.vy = -rand_float(rng) * 3.0f;
                particle.life = 20 + rand_int(rng) % 30;
                
                particles.push_back(particle);
                
                if (particles.size() >= max_particles) break;
            }
        }
        if (particles.size() >= max_particles) break;
    }
}

/**
 * Set wind strength and direction
 */
void FireEngine::set_wind(float strength) {
    wind_strength = std::clamp(strength, -5.0f, 5.0f);
    wind_direction = (strength > 0) ? 1.0f : -1.0f;
}

/**
 * Add or remove fuel
 */
void FireEngine::add_fuel(int amount) {
    fuel_amount = std::clamp(fuel_amount + amount, 0, 100);
}

/**
 * Ignite fire at specific location
 */
void FireEngine::ignite_at(int x, int y) {
    // Convert screen coordinates to grid coordinates
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int offset_x = (max_x - width) / 2;
    int offset_y = (max_y - height) / 2;
    
    int grid_x = x - offset_x;
    int grid_y = y - offset_y;
    
    // Create explosion at location
    create_explosion(grid_x, grid_y, 80);
}

/**
 * Create explosion effect
 */
void FireEngine::create_explosion(int x, int y, int intensity) {
    int radius = 3;
    
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int distance = dx * dx + dy * dy;
            if (distance <= radius * radius) {
                int heat_value = intensity * (1.0f - distance / (float)(radius * radius));
                set_heat_at(x + dx, y + dy, heat_value);
            }
        }
    }
    
    // Add explosion particles
    for (int i = 0; i < 10; i++) {
        FireParticle particle(x, y, intensity);
        particle.vx = (rand_float(rng) - 0.5f) * 4.0f;
        particle.vy = (rand_float(rng) - 0.5f) * 4.0f;
        particle.life = 30 + rand_int(rng) % 20;
        
        if (particles.size() < max_particles) {
            particles.push_back(particle);
        }
    }
}

/**
 * Set color scheme
 */
void FireEngine::set_color_scheme(ColorScheme scheme) {
    current_scheme = scheme;
}

/**
 * Update simulation statistics
 */
void FireEngine::update_statistics() {
    stats.total_heat = 0;
    stats.active_pixels = 0;
    stats.max_temp = 0;
    
    // Calculate heat statistics
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int heat = heat_grid[y][x];
            if (heat > 0) {
                stats.total_heat += heat;
                stats.active_pixels++;
                stats.max_temp = std::max(stats.max_temp, heat);
            }
        }
    }
    
    // Calculate average temperature
    stats.average_temp = (stats.active_pixels > 0) ? 
        static_cast<float>(stats.total_heat) / stats.active_pixels : 0.0f;
    
    // Update other stats
    stats.wind_speed = wind_strength;
    stats.fuel_level = fuel_amount;
    stats.frames_rendered = frame_count;
    
    // Calculate FPS (simplified)
    static auto last_time = std::chrono::steady_clock::now();
    auto current_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time);
    
    if (duration.count() > 0) {
        stats.fps = 1000.0f / duration.count();
    }
    last_time = current_time;
}