/**
 * PERFECT SNAKE GAME
 * Smooth, professional, and flicker-free
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

using namespace std;

// Game constants
const int WIDTH = 50;
const int HEIGHT = 25;

// Game state
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

vector<Point> snake;
Point food;
int score = 0;
int high_score = 0;
char direction = 'w';
char next_direction = 'w';
bool game_over = false;
bool game_started = false;

// Console buffer for smooth rendering
#ifdef _WIN32
HANDLE hConsole;
CHAR_INFO* screen_buffer;
COORD buffer_size = {WIDTH, HEIGHT + 5};
COORD buffer_coord = {0, 0};
SMALL_RECT write_region = {0, 0, WIDTH - 1, HEIGHT + 4};
#endif

/**
 * Initialize console for smooth rendering
 */
void init_console() {
#ifdef _WIN32
    hConsole = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL
    );
    SetConsoleActiveScreenBuffer(hConsole);
    
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(hConsole, &cursor_info);
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursor_info);
    
    screen_buffer = new CHAR_INFO[buffer_size.X * buffer_size.Y];
    
    for (int i = 0; i < buffer_size.X * buffer_size.Y; i++) {
        screen_buffer[i].Char.AsciiChar = ' ';
        screen_buffer[i].Attributes = 15;
    }
#endif
}

/**
 * Set character in buffer
 */
void set_char(int x, int y, char ch, int color = 15) {
#ifdef _WIN32
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT + 5) {
        int index = y * buffer_size.X + x;
        screen_buffer[index].Char.AsciiChar = ch;
        screen_buffer[index].Attributes = color;
    }
#endif
}

/**
 * Set string in buffer
 */
void set_string(int x, int y, const string& str, int color = 15) {
    for (size_t i = 0; i < str.length() && x + i < WIDTH; i++) {
        set_char(x + i, y, str[i], color);
    }
}

/**
 * Present buffer to screen
 */
void present_screen() {
#ifdef _WIN32
    WriteConsoleOutput(hConsole, screen_buffer, buffer_size, buffer_coord, &write_region);
#endif
}

/**
 * Clear buffer
 */
void clear_buffer() {
#ifdef _WIN32
    for (int i = 0; i < buffer_size.X * buffer_size.Y; i++) {
        screen_buffer[i].Char.AsciiChar = ' ';
        screen_buffer[i].Attributes = 15;
    }
#endif
}

/**
 * Spawn food
 */
void spawn_food() {
    do {
        food.x = rand() % (WIDTH - 4) + 2;
        food.y = rand() % (HEIGHT - 4) + 2;
    } while (find(snake.begin(), snake.end(), food) != snake.end());
}

/**
 * Initialize game
 */
void init_game() {
    snake.clear();
    
    int start_x = WIDTH / 2;
    int start_y = HEIGHT / 2;
    
    snake.push_back(Point(start_x, start_y));
    snake.push_back(Point(start_x, start_y + 1));
    snake.push_back(Point(start_x, start_y + 2));
    
    spawn_food();
    
    score = 0;
    direction = 'w';
    next_direction = 'w';
    game_over = false;
    game_started = true;
}

/**
 * Handle input
 */
void handle_input() {
#ifdef _WIN32
    if (_kbhit()) {
        char key = tolower(_getch());
        
        if (!game_started) {
            if (key == ' ') {
                init_game();
            }
            return;
        }
        
        switch (key) {
            case 'w': 
                if (direction != 's') next_direction = 'w'; 
                break;
            case 's': 
                if (direction != 'w') next_direction = 's'; 
                break;
            case 'a': 
                if (direction != 'd') next_direction = 'a'; 
                break;
            case 'd': 
                if (direction != 'a') next_direction = 'd'; 
                break;
            case 'q': 
                exit(0);
                break;
            case 'r':
            case ' ':
                if (game_over) {
                    init_game();
                }
                break;
        }
    }
#endif
}

/**
 * Update game logic
 */
void update_game() {
    if (!game_started || game_over) return;
    
    direction = next_direction;
    
    Point new_head = snake[0];
    
    switch (direction) {
        case 'w': new_head.y--; break;
        case 's': new_head.y++; break;
        case 'a': new_head.x--; break;
        case 'd': new_head.x++; break;
    }
    
    if (new_head.x <= 0 || new_head.x >= WIDTH-1 || 
        new_head.y <= 0 || new_head.y >= HEIGHT-1) {
        game_over = true;
        if (score > high_score) high_score = score;
        return;
    }
    
    for (const Point& segment : snake) {
        if (new_head == segment) {
            game_over = true;
            if (score > high_score) high_score = score;
            return;
        }
    }
    
    snake.insert(snake.begin(), new_head);
    
    if (new_head == food) {
        score += 10;
        spawn_food();
    } else {
        snake.pop_back();
    }
}

/**
 * Render game
 */
void render_game() {
    clear_buffer();
    
    // Draw beautiful border
    for (int x = 0; x < WIDTH; x++) {
        set_char(x, 0, '#', 11);  // Cyan
        set_char(x, HEIGHT-1, '#', 11);
    }
    for (int y = 0; y < HEIGHT; y++) {
        set_char(0, y, '#', 11);
        set_char(WIDTH-1, y, '#', 11);
    }
    
    if (game_started && !game_over) {
        // Draw snake
        for (size_t i = 0; i < snake.size(); i++) {
            Point& segment = snake[i];
            if (i == 0) {
                set_char(segment.x, segment.y, '@', 10);  // Bright green head
            } else {
                set_char(segment.x, segment.y, 'o', 2);   // Green body
            }
        }
        
        // Draw food
        set_char(food.x, food.y, '*', 12);  // Bright red
    }
    
    // Game info
    stringstream info;
    info << "SCORE: " << score << "   LENGTH: " << snake.size() << "   HIGH SCORE: " << high_score;
    set_string(2, HEIGHT + 1, info.str(), 15);
    
    if (!game_started) {
        set_string(WIDTH/2 - 10, HEIGHT/2 - 2, "PREMIUM SNAKE GAME", 14);
        set_string(WIDTH/2 - 8, HEIGHT/2, "Press SPACE to Start", 15);
        set_string(WIDTH/2 - 10, HEIGHT/2 + 2, "WASD = Move, Q = Quit", 7);
    } else if (game_over) {
        set_string(WIDTH/2 - 5, HEIGHT/2 - 1, "GAME OVER!", 12);
        set_string(WIDTH/2 - 12, HEIGHT/2 + 1, "Press SPACE or R to restart", 15);
    } else {
        set_string(2, HEIGHT + 2, "WASD = Move   Q = Quit   Premium Snake Game!", 7);
    }
    
    present_screen();
}

/**
 * Main game loop
 */
int main() {
    srand(static_cast<unsigned int>(time(0)));
    
    cout << "Loading Premium Snake Game..." << endl;
    Sleep(500);
    
    init_console();
    
    DWORD last_update = GetTickCount();
    const DWORD frame_time = 120; // Smooth speed
    
    while (true) {
        DWORD current_time = GetTickCount();
        
        handle_input();
        
        if (current_time - last_update >= frame_time) {
            update_game();
            last_update = current_time;
        }
        
        render_game();
        Sleep(16); // 60 FPS rendering
    }
    
    return 0;
}