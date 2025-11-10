# C-Coursework: Robot Arena Navigation
A C program that simulates a robot navigating through grid-based arenas to collect markers and avoid obstacles using breadth-first search (BFS) pathfinding.
Description
This program demonstrates robot navigation across five progressively complex stages:
- **Stage 1**: Robot finds a marker on the arena perimeter using wall-following
- **Stage 2**: Robot collects a perimeter marker and delivers it to the nearest corner
- **Stage 3**: Robot uses BFS pathfinding to navigate to a random marker and deliver to nearest corner
- **Stage 4**: Robot collects multiple markers in an obstacle-filled arena using optimal pathfinding
- **Stage 5**: Robot operates in a circular arena, collecting all markers while avoiding obstacles
## Features
- BFS Pathfinding: Guarantees shortest path to objectives
- Configurable Parameters: Grid size, marker count, obstacle count, animation speed
- Visual Protocol: Emits textual drawing commands consumed by `launcher` binary
- 5 Difficulty Stages: From simple wall-following to complex circular arenas

## Build and Run

gcc -O2 main.c robot.c arena.c controller.c graphics.c -o myProgram.exe
.\myProgram.exe {arena size} {stage no.} {no. of markers} {no. of obstacles} {radius of circle (stage 5)} {sleep time between movement} {window size} | .\launcher

Any arguments not given will default.
## Command-Line Arguments

| Argument | Description | Default | Range |
|----------|-------------|---------|-------|
| gridSize | Arena dimensions (NxN) | 6 | 2-150 |
| stage | Stage number to run | 5 | 1-5 |
| markers | Number of markers | 2 | 1+ |
| obstacles | Number of obstacles | 3 | 0+ |
| radius | Circle radius (stage 5 only) | 5 | 1+ |
| sleepTime | Animation delay (milliseconds) | 100 | 0-1000 |
| windowSize | Window dimensions (pixels) | 400 | 200-1200 |

### Examples

```powershell
# Stage 5: 15x15 grid, 5 markers, 8 obstacles, radius 7, fast animation (50ms)
.\myProgram.exe 15 5 5 8 7 50 | .\launcher

# Stage 3: 10x10 grid, default animation (100ms), large window (800px)
.\myProgram.exe 10 3 1 0 0 100 800 | .\launcher

# Stage 4: 20x20 grid, 10 markers, 15 obstacles, slow animation (500ms)
.\myProgram.exe 20 4 10 15 0 500 | .\launcher

# No animation (instant execution, 0ms delay)
.\myProgram.exe 10 5 2 3 5 0 | .\launcher
```

## Debugging

### Inspect Protocol Output

To see the raw textual protocol commands without launching the GUI:

```powershell
.\myProgram.exe 10 5 2 3 5 100 > out.txt
notepad out.txt
```

The output contains drawing commands like:
```
SW 410 410          # Set window size
DL 5 5 405 5        # Draw line (grid)
SC red              # Set color to red
FR 7 7 23 23        # Fill rectangle (obstacle)
SC darkgray         # Set color to dark gray
FP 3 x1 y1 x2 y2 x3 y3  # Fill polygon (robot)
```

### Debug Build with GDB

```powershell
gcc -g -O0 main.c robot.c arena.c controller.c graphics.c -o myProgram.exe
gdb myProgram.exe
```

## Project Structure

```
C-Coursework/
├── main.c              # Entry point, argument parsing, stage selection
├── robot.c             # Robot movement, drawing, marker pickup/drop
├── robot.h             # Robot API declarations
├── arena.c             # Arena initialization, grid drawing, obstacles
├── arena.h             # Arena API declarations
├── controller.c        # BFS pathfinding, navigation, stage implementations
├── controller.h        # Controller API declarations
├── graphics.c          # Graphics protocol implementation (emit commands)
├── graphics.h          # Graphics API (colors, drawing primitives)
├── launcher.exe        # Binary renderer (consumes protocol)
├── Makefile            # Build automation
└── README.md           # This file
```

## Algorithm Overview

### BFS Pathfinding
The robot uses breadth-first search to find the shortest path to markers and drop locations:

1. Enqueue starting position
2. Explore neighbors (up, down, left, right)
3. Track parent cells for path reconstruction
4. Return when objective found (guaranteed shortest path)
5. Walk back parent chain to build path array
6. Navigate by rotating and moving forward along path

### Navigation
The robot orients itself by:
1. Determining target cell direction (up/down/left/right)
2. Rotating until facing correct direction
3. Moving forward one cell
4. Repeating until destination reached

## Known Limitations

- Maximum grid size: 150x150 (defined by `GRID_SIZE` constant)
- Arena must be rectangular (stage 5 simulates circular via obstacles)
- Robot cannot move diagonally
- Single robot only (no multi-agent support)

## License

Educational project - MIT License

## Credits

Developed for C Programming coursework, demonstrating:
- Dynamic memory management
- Graph algorithms (BFS)
- Modular C architecture
- Protocol-based graphics rendering
