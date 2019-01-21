/*
* Words With Friends Solver
*/

#include <math.h>
#include <stdio.h>

int const BOARD_HEIGHT = 4;
int const BOARD_WIDTH = 4;

struct Coordinate {
	int x, y;
}

int cantor_pairing(int x, int y) {
	int first_term = ((x + y) * (x + y + 1)) / 2;
	return first_term + y;
}

struct Coordinate inverse_cantor_pairing(int z) {
	int w = (sqrt(8 * z + 1) - 1) / 2;
	int t = (w * w + w) / 2;
	int y = z - t;
	int x = w - y;
	struct Coordinate coord = {x, y};
	return coord;
}

int check_for_duplicates(int curr_cantor, int *seen, int max_length) {
	for (int i = 0; i < max_length; i++) {
		if (*(seen + i) == curr_cantor) {
			return 1
		}
	}
	return -1;
}

void find_combinations(char *board, int x, int y, int *seen, int next_layer, int max_length) {
	// FIX BOUNDS LATER
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1, dx <= 1; dx++) {
			if (dx == 0 && dy == 0) {
				continue;
			}
			int curr_x = x + dx;
			int curr_y = y + dy;
			int *curr_seen = NULL;
			int duplicates;

			if (seen == NULL) {
				curr_seen = (int *) malloc(sizeof(int) * max_length);
				if (curr_seen == NULL || curr_seen == 0) {
					printf("ERROR: Out of memory\n");
				}
				// Use Cantor pairing function to store coordinates (x,y) as a unique int
				*curr_seen = cantor_pairing(x, y);
			} else {
				// check to see if cantor_pairing(curr_x, curr_y) is in curr_seen
				duplicates = check_for_duplicates(cantor_pairing(curr_x, curr_y), seen, max_length);
				if (duplicates == -1) {
					curr_seen = (int *) realloc(seen, sizeof(int) * max_length);
				}
			}

			if (duplicates == -1) {
				*(curr_seen + next_layer) = cantor_pairing(curr_x, curr_y);
				// Word is len >= 3
				if (next_layer >= 2) {
					char str[max_length + 1];
					for (int i = 0; i <= next_layer; i++) {
						struct Coordinate coord = inverse_cantor_pairing(*(curr_seen + i));
						char curr_char = board[BOARD_WIDTH * coord.y + coord.x];
						str[i] = curr_char;
					}
					// Use each coordinate in curr_seen to retrieve char from *board
					// concatenate into a string
					// pass into Python checker
					// if it is an English word:
					// Y=0 -> A, Y=1 -> B, ...
					// print string: "[original X,Y (at index 0 of curr_seen)] : [string]"
				}
				if (next_layer + 1 < max_length) {
					find_combinations(board, curr_x, curr_y, curr_seen, next_layer + 1);
				}
			}

			if (curr_seen != NULL) {
				free(curr_seen);
			}
		}
	}
}

void iterate_through_grid(char *board, int max_length) {
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			find_combinations(board, x, y, NULL, 1, max_length);
		}
	}
}
