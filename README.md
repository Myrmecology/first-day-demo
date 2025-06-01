# 🐍 Premium Snake Game

A smooth, professional Snake game with flicker-free graphics and premium polish. Built in C++ with advanced double-buffering for silky-smooth gameplay.

```
    ##################################################
    #                                                #
    #    @oooo                                       #
    #                                                #
    #                          *                     #
    #                                                #
    #                                                #
    ##################################################
    
    SCORE: 50    LENGTH: 6    HIGH SCORE: 150
    WASD = Move   Q = Quit   Premium Snake Game!
```

## ✨ Premium Features

- **🎮 Flicker-Free Graphics** - Advanced double-buffering eliminates screen flicker
- **⚡ Smooth 60 FPS Rendering** - Buttery smooth animation and response
- **🎨 Professional Visuals** - Clean borders, colored snake, and polished UI
- **🎯 Precision Controls** - Responsive WASD movement with direction buffering
- **🏆 Score Tracking** - Current score, snake length, and persistent high score
- **💫 Game States** - Start screen, game over screen, and restart functionality
- **🔥 Performance Optimized** - Efficient rendering with minimal CPU usage

## 🚀 Quick Start

### Compile and Run
```bash
g++ -o snake.exe snake.cpp
./snake.exe
```

## 🎮 How to Play

- **WASD** - Move the snake (smooth directional control)
- **SPACE** - Start game / Restart after game over
- **R** - Restart game (when game over)
- **Q** - Quit

### Objective
Eat the food (*) to grow your snake and increase score. Avoid walls and your own tail!

## 🎨 Visual Design

- **Snake Head**: @ (bright green)
- **Snake Body**: o (green) 
- **Food**: * (bright red)
- **Borders**: # (cyan)
- **Smooth colors** and **professional layout**

## ⚡ Technical Excellence

- **Double Buffering** - Eliminates flicker completely
- **Frame Rate Control** - Consistent timing for smooth gameplay  
- **Input Buffering** - Prevents missed key presses
- **Memory Efficient** - Optimized rendering system
- **Windows Console API** - Direct buffer manipulation for performance

## 🏆 Game Features

- **Growing Snake** - Snake extends as you eat food
- **Smart Food Placement** - Food never spawns on snake
- **Collision Detection** - Precise wall and self-collision
- **High Score Persistence** - Tracks your best game
- **Smooth Speed** - Consistent game speed regardless of snake length

## 🛠️ Advanced Implementation

This isn't just another Snake game - it's built with **professional game development techniques**:

- **Console Buffer Management** - Direct screen buffer manipulation
- **Frame-Independent Timing** - Consistent gameplay across different systems  
- **State Machine Architecture** - Clean game state transitions
- **Optimized Rendering Pipeline** - Only updates changed screen areas
- **Professional Code Structure** - Clean, maintainable C++ code

## 🎯 Why This Version is Special

✅ **Zero Flicker** - Advanced double-buffering technology  
✅ **Smooth as Silk** - 60 FPS rendering with precision timing  
✅ **Professional Polish** - Every detail optimized for the best experience  
✅ **Rock Solid** - Thoroughly tested and debugged  
✅ **Performance Optimized** - Efficient algorithms and memory usage  
✅ **Beautiful Code** - Clean, readable, and well-documented  

## 🔧 Compilation

**Windows:**
```bash
g++ -std=c++11 -O2 -o snake.exe snake.cpp
```

**Requirements:**
- C++11 compatible compiler
- Windows (uses Windows Console API for smooth graphics)

## 🎮 Experience the Difference

This Snake game demonstrates **advanced console programming** with:
- Professional-grade rendering techniques
- Optimized game loop architecture  
- Smooth user experience design
- High-performance graphics programming

**Play it once and you'll see the difference!** 🎯

---

**Built with passion for smooth, professional gameplay** 🐍✨ Happy coding 