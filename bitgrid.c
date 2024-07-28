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

int rows, cols;

void process(Cell **grid) {
	// Example usage: Process the grid
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[i][j].output = grid[i][j].input;
		}
	}
}

// run phase 1
void run_phase1(Cell **grid) {
	// Example usage: Run phase 1, all cells where (row + col) is even
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((i + j) % 2 == 0) {
				// gather the inputs from the other cells
				grid[i][j].input = 0;
				grid[i][j].input = grid[i][j].input || (grid[i][(j+1)%cols].output >> 2) & 1;
				grid[i][j].input = grid[i][j].input || (grid[i][(j-1+cols)%cols].output >> 0) & 4;
				grid[i][j].input = grid[i][j].input || (grid[(i+1)%rows][j].output >> 1) & 2;
				grid[i][j].input = grid[i][j].input || (grid[(i-1+rows)%rows][j].output >> 3) & 8;


				grid[i][j].output = (grid[i][j].input * 0x11) >> 2;  // invert the input, for now
			}
		}
	}
}

//run phase 2
void run_phase2(Cell **grid) {
	// Example usage: Run phase 2, all cells where (row + col) is odd
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((i + j) % 2 == 1) {
				// gather the inputs from the other cells
				grid[i][j].input = 0;
				grid[i][j].input = grid[i][j].input || (grid[i][(j+1)%cols].output >> 2) & 1;
				grid[i][j].input = grid[i][j].input || (grid[i][(j-1+cols)%cols].output >> 0) & 4;
				grid[i][j].input = grid[i][j].input || (grid[(i+1)%rows][j].output >> 1) & 2;
				grid[i][j].input = grid[i][j].input || (grid[(i-1+rows)%rows][j].output >> 3) & 8;

				grid[i][j].output = (grid[i][j].input * 0x11) >> 2;  // invert the input, for now
			}
		}
	}
}

void PrintGrid(Cell **grid)
{
    // print the grid
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d -> %d ", grid[i][j].input, grid[i][j].output);
        }
        printf("\n");
    }
}


// Main function
int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <rows> <cols>\n", argv[0]);
		return 1;
	}

	rows = atoi(argv[1]);
	cols = atoi(argv[2]);

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

    printf("Initial grid:\n");
    PrintGrid(grid);

	printf("seeding the grid...\n");
	grid[1][1].output = 0xf;
	PrintGrid(grid);

	printf("Running phase 2...\n");
	run_phase2(grid);

	printf("After phase 2:\n");
	PrintGrid(grid);



	printf("Running phase 1...\n");
	run_phase1(grid);

	printf("After phase 1:\n");
	PrintGrid(grid);

	printf("Running phase 2...\n");
	run_phase2(grid);

	printf("After phase 2:\n");
	PrintGrid(grid);


    // Free the allocated memory
	for (int i = 0; i < rows; i++) {
		free(grid[i]);
	}
	free(grid);

	return 0;
}


