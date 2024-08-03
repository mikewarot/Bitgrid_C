#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define CELL_SIZE 20

typedef struct {
    bool input[4];  // Inputs from four rook-neighbors
    bool output[4]; // Outputs to four rook-neighbors
    uint64_t gates; // 64 bits to determine the four gates
} Cell;

void initialize_grid(Cell **grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 4; k++) {
                grid[i][j].input[k] = false;
                grid[i][j].output[k] = false;
            }
            grid[i][j].gates = 0; // Initialize gates to some default value
        }
    }
}

void random_seed_grid(Cell **grid, int rows, int cols) {
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 4; k++) {
                grid[i][j].input[k] = rand() % 2;  // Random boolean value
                grid[i][j].output[k] = rand() % 2; // Random boolean value
            }
        }
    }
}

void run_phase1(Cell **grid, int rows, int cols) {
    bool temp_inputs[rows][cols][4];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 4; k++) {
                temp_inputs[i][j][k] = grid[i][j].input[k];
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i > 0) temp_inputs[i][j][0] = grid[i-1][j].output[2]; // Top neighbor
            if (j < cols - 1) temp_inputs[i][j][1] = grid[i][j+1].output[3]; // Right neighbor
            if (i < rows - 1) temp_inputs[i][j][2] = grid[i+1][j].output[0]; // Bottom neighbor
            if (j > 0) temp_inputs[i][j][3] = grid[i][j-1].output[1]; // Left neighbor
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 4; k++) {
                grid[i][j].input[k] = temp_inputs[i][j][k];
            }
        }
    }
}

void run_phase2(Cell **grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 4; k++) {
                grid[i][j].output[k] = grid[i][j].input[k];
            }
        }
    }
}

void draw_grid(SDL_Renderer *renderer, Cell **grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            SDL_Rect cell_rect = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            if (grid[i][j].output[0]) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for active cells
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for inactive cells
            }
            SDL_RenderFillRect(renderer, &cell_rect);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    if (rows % 2 != 0 || cols % 2 != 0) {
        fprintf(stderr, "Error: Both rows and cols must be even numbers.\n");
        return 1;
    }

    Cell **grid = malloc(rows * sizeof(Cell *));
    for (int i = 0; i < rows; i++) {
        grid[i] = malloc(cols * sizeof(Cell));
    }

    initialize_grid(grid, rows, cols);
    random_seed_grid(grid, rows, cols);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("BitGrid", 100, 100, cols * CELL_SIZE, rows * CELL_SIZE, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        run_phase1(grid, rows, cols);
        run_phase2(grid, rows, cols);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_grid(renderer, grid, rows, cols);

        SDL_RenderPresent(renderer);

        SDL_Delay(100); // Delay to control the update speed
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}