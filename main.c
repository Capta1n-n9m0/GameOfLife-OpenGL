#define _XOPEN_SOURCE 600

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "game_of_life.h"

#define MY_WHITE glColor3f(1.0, 1.0, 1.0)

int square_size = 5;
int width = 200, height = 200;
int count = 1;
board b;

void step();
void draw_board(board board1);
void draw_cell(int x, int y);
void draw_square(int x, int y, int size);
void display();
void reshape(int w, int h);

int main (int argc, char * argv[]) {
    srand(time(NULL));
    b = create_empty_board(width, height);
    fill_board_random(b);
    puts("good 29");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Game Of Life");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    puts("good 38");
    glutMainLoop();

    return 0;
}

void step(){
    draw_board(b);

    evolve_board(&b);
    usleep(30 * 1000);
}
void draw_board(board board1){
    for(int y = 1; y < board1.height; y++)
        for(int x = 1; x < board1.width; x++)
            if (board1.cells[y][x]) draw_cell(x, y);
}

void draw_cell(int x, int y){
    draw_square(x*square_size, y*square_size, square_size);
}

void draw_square(int x, int y, int size){
    MY_WHITE;
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + size, y);
    glVertex2i(x + size, y + size);
    glVertex2i(x, y+size);
    glEnd();
}

void display(){
    glClear (GL_COLOR_BUFFER_BIT);

    step();
    count++;
    printf("%d\n", count);

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h){
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



