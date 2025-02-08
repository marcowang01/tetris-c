# Tetris Clone
A simplified clone of Tetris, implemented in C++. This version features basic gameplay mechanics and controls, allowing you to rotate, move, and drop Tetris pieces as they fall. The difficulty of the game will also increase as more lines are cleared.

![Screenshot of Tetris in C](https://github.com/marcowang01/Tetris_in_C/blob/master/screenshot.png?raw=true)

## Getting Started

### Prerequisites

Make sure you have `cmake` installed before proceeding with the setup.

### Building and Running the Game

1. **Generate build files using `cmake`:**  
   In the root directory, run:
   ```sh
   cmake .
   ```

2. **Compile the code using `make`:**  
   Run the following command to compile:
   ```sh
   make
   ```

3. **Run the executable:**  
   From the root directory, execute the program:
   ```sh
   ./main
   ```

## Controls

- **Arrow Keys:** Rotate (Left/Right) and accelerate downward (Down) the current block.
- **Enter/Return:** Start the game.
- **Space:** Place the current block instantly.
- **'R':** Restart the game after a game over.
