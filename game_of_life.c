#include "game_of_life.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

board create_empty_board(int width, int height){
    width += 2;height += 2;
    board res = {0};
    res.width = width;
    res.height = height;
    res.cells = calloc(height, sizeof(cell*));
    if(res.cells == NULL) error("calloc");
    for(int i = 0; i < height; i++){
        res.cells[i] = calloc(width, sizeof(cell));
        if(res.cells[i] == NULL) error("calloc");
    }
    return res;
}
void free_board(board b){
    for(int i = 0; i < b.height; i++){
        free(b.cells[i]);
    }
    free(b.cells);
}
void copy_cells(board to, board from){
    assert(to.width == from.width);
    assert(to.height == from.height);
    for(int y = 1; y < to.height-1; y++)
        for(int x = 1; x < to.width-1; x++)
            to.cells[y][x] = from.cells[y][x];
}
void fill_board_random(board b){
    for(int y = 1; y < b.height-1; y++)
        for(int x = 1; x < b.width-1; x++)
            b.cells[y][x] = rand()%2;
}
int count_alive_neighbours(board b, int x, int y){
    int count = 0;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(i == 0 && j == 0) continue;
            if(b.cells[y+i][x+j]) count++;
        }
    }
    return count;
}
void set_perimeter_dead(board b){
    for(int i = 0; i < b.width; i++){
        b.cells[0][i] = dead;
        b.cells[b.height-1][i] = dead;
    }
    for(int i = 0; i < b.height; i++){
        b.cells[i][0] = dead;
        b.cells[i][b.width-1] =dead;
    }
}
cell evolve_cell(board b, int x, int y){
    int n = count_alive_neighbours(b, x, y);
    if(b.cells[y][x] == alive &&(n == 2 || n == 3)) return alive;
    if(b.cells[y][x] == dead &&(n == 3)) return alive;
    return dead;
}
void evolve_board(board *b){
    board temp;
    temp = create_empty_board(b->width-2, b->height-2);

    for(int y = 1; y < b->height-1; y++)
        for(int x = 1; x < b->width-1; x++)
                temp.cells[y][x] = evolve_cell(*b, x, y);
    set_perimeter_dead(temp);
    copy_cells(*b, temp);
    free_board(temp);
}
void clear_board(board b){
    for(int y = 0; y < b.height; y++){
        for(int x = 0; x < b.width; x++){
            b.cells[y][x] = dead;
        }
    }
}


