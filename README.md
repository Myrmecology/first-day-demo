# 🔥 ASCII Fire Effect Simulator

A mesmerizing real-time fire simulation rendered entirely in ASCII characters with interactive controls and stunning visual effects.

```
    (  )   (   )  )
     ) (   )  (  (
     ( )  (    ) )
     _____________
    <_____________> ___
    |             |/ _ \
    |               | | |
    |               |_| |
 ___|_______________|_|_|___
|_____________________________|
```

## ✨ Features

- **Real-time Fire Simulation** - Dynamic heat diffusion algorithm creates realistic flame behavior
- **Interactive Wind Control** - Bend flames with directional wind simulation
- **Multiple Color Schemes** - Classic fire, blue flames, rainbow mode, and more
- **Fuel Management** - Increase/decrease fire intensity in real-time
- **Smooth Animations** - 30+ FPS rendering with fluid character transitions
- **Intuitive UI** - Clean control panel with real-time statistics
- **Mouse Interaction** - Click to ignite new fire sources
- **Cross-platform** - Works on Linux, macOS, and Windows

## 🚀 Quick Start

### Prerequisites

- C++ compiler (GCC 7+ or Clang 10+)
- ncurses library
- Make utility

### Installation

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential libncurses5-dev libncursesw5-dev
```

**macOS:**
```bash
brew install ncurses
```

**Windows (WSL recommended):**
```bash
sudo apt-get install build-essential libncurses5-dev
```

### Build & Run

```bash
# Clone the repository
git clone https://github.com/Myrmecology/first-day-demo#.git
cd first-day-demo

# Compile the project
make

# Run the fire simulator
./fire_simulator

# Or compile and run in one step
make run
```

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W` | Wind Left - Bend flames leftward |
| `E` | Wind Right - Bend flames rightward |
| `+` | Increase Fuel - Make fire bigger |
| `-` | Decrease Fuel - Reduce fire size |
| `R` | Reset - Clear and restart simulation |
| `C` | Cycle Colors - Switch between color schemes |
| `Space` | Pause/Resume simulation |
| `Q` | Quit application |

**Mouse Controls:**
- Left Click: Ignite new fire source at cursor position
- Right Click: Add wind burst at cursor location

## 🏗️ Project Structure

```
first-day-demo/
├── src/
│   ├── main.cpp           # Application entry point
│   ├── fire_engine.cpp    # Core fire simulation logic
│   ├── ui_manager.cpp     # User interface rendering
│   └── input_handler.cpp  # Input processing
├── include/
│   ├── fire_engine.h      # Fire engine declarations
│   ├── ui_manager.h       # UI component headers
│   ├── input_handler.h    # Input handling interface
│   └── colors.h           # Color scheme definitions
├── assets/
│   └── fire_config.txt    # Configuration parameters
└── Makefile               # Build configuration
```

## 🔧 Configuration

Customize fire behavior by editing `assets/fire_config.txt`:

```ini
# Fire simulation parameters
FIRE_HEIGHT=25
FIRE_WIDTH=80
COOLING_RATE=0.85
WIND_STRENGTH=2.0
DEFAULT_FUEL=50
ANIMATION_SPEED=30
```

## 🎨 Color Schemes

1. **Classic Fire** - Traditional red/orange/yellow flames
2. **Blue Flame** - Cool blue fire effect
3. **Rainbow** - Psychedelic multi-color flames
4. **Plasma** - Purple/pink plasma effect
5. **Ice Fire** - Cyan/white cold flames

## 🧪 Testing

```bash
# Run with debug information
make debug
./fire_simulator

# Run performance test
make test

# Memory leak check (requires valgrind)
make memcheck
```

## 🚀 Performance

- **CPU Usage:** ~5-15% on modern systems
- **Memory:** ~2-5MB RAM usage
- **Terminal:** Optimized for 80x25 minimum size
- **Refresh Rate:** 30-60 FPS depending on terminal

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 Development Notes

This project demonstrates:
- Real-time simulation algorithms
- Terminal-based graphics programming
- Event-driven input handling
- Cross-platform C++ development
- Memory-efficient rendering techniques

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by classic ASCII art and terminal graphics
- Built with love for the command line interface
- Thanks to the ncurses library developers

---

**Made with 🔥 and C++** | Happy coding
