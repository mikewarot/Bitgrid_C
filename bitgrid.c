#include <stdio.h>
#include <stdlib.h>

// Define a cell structure
typedef struct {
	unsigned int input : 4;
	unsigned int output : 4;
	unsigned int lookup0 : 16;
	unsigned int lookup1 : 16;
	unsigned int lookup2 : 16;
	unsigned int lookup3 : 16;
} Cell;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <rows> <cols>\n", argv[0]);
		return 1;
	}

	int rows = atoi(argv[1]);
	int cols = atoi(argv[2]);

	if (rows <= 0 || cols <= 0 || rows % 2 != 0 || cols % 2 != 0) {
		fprintf(stderr, "Error: rows and cols must be positive, even, integers.\n");
		return 1;
	}

	// Allocate memory for the 2D array of Cells
	Cell **grid = malloc(rows * sizeof(Cell *));
	for (int i = 0; i < rows; i++) {
		grid[i] = malloc(cols * sizeof(Cell));
	}

	// Example usage: Initialize the grid with some values
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[i][j].input = 0;
			grid[i][j].output = 0;
			grid[i][j].lookup0 = 0;
			grid[i][j].lookup1 = 0;
			grid[i][j].lookup2 = 0;
			grid[i][j].lookup3 = 0;
		}
	}

	printf("2D array of size %d x %d created.\n", rows, cols);

	printf("Size of Cell: %lu\n", sizeof(Cell));
	printf("Size of grid: %lu bytes\n", sizeof(Cell) * rows * cols);

	// Free the allocated memory
	for (int i = 0; i < rows; i++) {
		free(grid[i]);
	}
	free(grid);

	return 0;
}