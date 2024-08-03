#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

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

void print_grid(Cell **grid, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			// Print the state of the cell
			printf("Cell (%d, %d): ", i, j);
			printf("Inputs: [");
			for (int k = 0; k < 4; k++) {
				printf("%d", grid[i][j].input[k]);
				if (k < 3) printf(", ");
			}
			printf("], Outputs: [");
			for (int k = 0; k < 4; k++) {
				printf("%d", grid[i][j].output[k]);
				if (k < 3) printf(", ");
			}
			printf("]\n");
		}
	}
}

void run_phase1(Cell **grid, int rows, int cols) {
	// Create a temporary grid to store the new inputs
	bool temp_inputs[rows][cols][4];

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < 4; k++) {
				temp_inputs[i][j][k] = grid[i][j].input[k];
			}
		}
	}

	// Update inputs based on neighbors' outputs
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (i > 0) temp_inputs[i][j][0] = grid[i-1][j].output[2]; // Top neighbor
			if (j < cols - 1) temp_inputs[i][j][1] = grid[i][j+1].output[3]; // Right neighbor
			if (i < rows - 1) temp_inputs[i][j][2] = grid[i+1][j].output[0]; // Bottom neighbor
			if (j > 0) temp_inputs[i][j][3] = grid[i][j-1].output[1]; // Left neighbor
		}
	}

	// Copy the temporary inputs back to the grid
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < 4; k++) {
				grid[i][j].input[k] = temp_inputs[i][j][k];
			}
		}
	}
}

void run_phase2(Cell **grid, int rows, int cols) {
	// Update outputs based on inputs and gates
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < 4; k++) {
				// Example logic: output is the same as input
				grid[i][j].output[k] = grid[i][j].input[k];
			}
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

	printf("Initial grid:\n");
	print_grid(grid, rows, cols);

	printf("Randomly seeding the grid...\n");
	random_seed_grid(grid, rows, cols);
	print_grid(grid, rows, cols);

	// Run your phases here
	run_phase1(grid, rows, cols);
	printf("After phase 1:\n");
	print_grid(grid, rows, cols);

	run_phase2(grid, rows, cols);
	printf("After phase 2:\n");
	print_grid(grid, rows, cols);

	for (int i = 0; i < rows; i++) {
		free(grid[i]);
	}
	free(grid);

	return 0;
}


