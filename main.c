/* Copyright (c) 2025 leiswatch. All Rights Reserved. */

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Game of life"
#define FPS 15
#define CELL_SIZE 20

typedef struct Board {
    bool** cells;
    uint8_t rows;
    uint8_t cols;
} t_board;

static const int directions[8][2] = {
    {0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};

void free_board(t_board* board, uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        free(board->cells[i]);
    }

    free(board->cells);
    free(board);
}

t_board* create_board(uint8_t rows, uint8_t cols) {
    t_board* board = (t_board*)malloc(sizeof(t_board));

    if (board == NULL) {
        return NULL;
    }

    bool** cells = (bool**)malloc(sizeof(bool*) * rows);

    if (cells == NULL) {
        free(board);
        return NULL;
    }

    board->cells = cells;
    board->rows = rows;
    board->cols = cols;

    for (uint8_t i = 0; i < rows; i++) {
        board->cells[i] = (bool*)calloc(cols, sizeof(bool));

        if (board->cells[i] == NULL) {
            free_board(board, i);

            return NULL;
        }
    }

    return board;
}

bool in_board(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

bool is_cell_alive(t_board* board, int x, int y) {
    bool is_alive = board->cells[x][y];
    uint8_t alive_neighbours_count = 0;
    int new_x = x;
    int new_y = y;

    for (int i = 0; i < 8; i++) {
        new_x = x + directions[i][0];
        new_y = y + directions[i][1];

        if (in_board(new_x, new_y, board->rows, board->cols) &&
            board->cells[new_x][new_y]) {
            alive_neighbours_count++;
        }
    }

    if (is_alive && alive_neighbours_count < 2) {
        is_alive = false;
    } else if (is_alive &&
               (alive_neighbours_count == 2 || alive_neighbours_count == 3)) {
        is_alive = true;
    } else if (is_alive && alive_neighbours_count > 3) {
        is_alive = false;
    } else if (!is_alive && alive_neighbours_count == 3) {
        is_alive = true;
    }

    return is_alive;
}

void update_board(t_board* board) {
    t_board* new_board = create_board(board->rows, board->cols);

    for (uint8_t i = 0; i < board->rows; i++) {
        for (uint8_t j = 0; j < board->cols; j++) {
            new_board->cells[i][j] = is_cell_alive(board, i, j);
        }
    }

    for (uint8_t i = 0; i < board->rows; i++) {
        for (uint8_t j = 0; j < board->cols; j++) {
            board->cells[i][j] = new_board->cells[i][j];
        }
    }

    free_board(new_board, board->rows);
}

void set_initial_cells(t_board* board, uint8_t** indices, uint8_t n) {
    int8_t x = 0;
    int8_t y = 0;

    for (uint8_t i = 0; i < n; i++) {
        x = indices[i][0];
        y = indices[i][1];

        if (in_board(indices[i][0], indices[i][1], board->rows, board->cols)) {
            board->cells[x][y] = true;
        }
    }
}

int main(int argc, char* argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "i")) != -1) {
        switch (opt) {
            case 'i':
                printf("optarg: %s\n", argv[optind]);
                break;
            default:
                fprintf(stderr, "Usage: %s [-i] [1,1;2,2]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    uint8_t rows = WINDOW_WIDTH / CELL_SIZE;
    uint8_t cols = WINDOW_HEIGHT / CELL_SIZE;
    t_board* board = create_board(rows, cols);

    board->cells[15][15] = 1;
    board->cells[15][16] = 1;
    board->cells[14][16] = 1;
    board->cells[15][17] = 1;
    board->cells[16][17] = 1;

    SetTargetFPS(FPS);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        for (uint8_t i = 0; i < rows; i++) {
            for (uint8_t j = 0; j < cols; j++) {
                if (board->cells[i][j]) {
                    DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE,
                                  CELL_SIZE, LIGHTGRAY);
                }
            }
        }

        for (int i = CELL_SIZE; i < WINDOW_WIDTH; i += CELL_SIZE) {
            DrawLine(0, i, WINDOW_HEIGHT, i, DARKGRAY);
            DrawLine(i, 0, i, WINDOW_HEIGHT, DARKGRAY);
        }

        update_board(board);

        EndDrawing();
    }

    CloseWindow();
    free_board(board, WINDOW_WIDTH / CELL_SIZE);

    return EXIT_SUCCESS;
}
