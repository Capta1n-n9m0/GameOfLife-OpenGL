#ifndef OPENGL1_GAME_OF_LIFE_H
#define OPENGL1_GAME_OF_LIFE_H

typedef enum cell_{
    dead, alive
}cell;

typedef struct board_{
    int width, height;
    cell **cells;
}board;

board create_empty_board(int width, int height);
void free_board(board b);
void copy_cells(board to, board from);
void fill_board_random(board b);
int count_alive_neighbours(board b, int x, int y);
void set_perimeter_dead(board b);
cell evolve_cell(board b, int x, int y);
void evolve_board(board *b);
void clear_board(board b);

#endif //OPENGL1_GAME_OF_LIFE_H
