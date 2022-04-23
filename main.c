#define _XOPEN_SOURCE 600

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


#include "game_of_life.h"

#define MY_WHITE glColor3f(1.0, 1.0, 1.0)

bool do_run = true;
int WINDOW_W = 1000, WINDOW_H = 1000;
int effective_w, effective_h;
int square_size = 5;
int width = 1000, height = 1000;
int count = 1;
board b;
board test_board;
long long camera_x = 0, camera_y = 0;

void RenderString(int x, int y, const char *string);
void processSpecialKeys(int key, int x, int y);
void run_test();
bool if_nanoseconds_passed(long long n);
long long get_nanoseconds();
void game_logic();
void render();
void draw_board(board board1);
void draw_cell(int x, int y);
void draw_square(int x, int y, int size);
void display();
void reshape(int w, int h);
double fps_counter(int resolution_ms);
void processNormalKeys(unsigned char key, int x, int y);

int main (int argc, char * argv[]) {


    effective_w = WINDOW_W;
    effective_h = WINDOW_H - 100;
    srand(time(NULL));
    b = create_empty_board(width, height);
    test_board = create_empty_board(5, 5);
    fill_board_random(test_board);
    fill_board_random(b);
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Game Of Life");

    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

void RenderString(int x, int y, const char *string){

}

void run_test(){
    draw_board(test_board);

    //usleep(20 * 1000);
}

bool if_nanoseconds_passed(long long n){
    static long long start = 0;
    if(start == 0)
        start = get_nanoseconds();
    if((get_nanoseconds() - start) >= n){
        start = get_nanoseconds();
        return true;
    }
    return false;
}

void processNormalKeys(unsigned char key, int x, int y){
    switch (key) {
        case 27: //ESC key
            exit(0);
            break;
        case ' ':
            do_run = !do_run;
            break;
        case 'a':
            camera_x -= 5;
            break;
        case 'd':
            camera_x += 5;
            break;
        case 's':
            camera_y -= 5;
            break;
        case 'w':
            camera_y += 5;
            break;
        case 'e':
            square_size += 1;
            break;
        case 'q':
            square_size -= 1;
            break;
        case 'r':
            camera_x = 0;
            camera_y = 0;
            square_size = 5;
            reshape(1000,1000);
            break;
    }
}

void processSpecialKeys(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_F5:
            fill_board_random(b);
            break;
    }
}

long long get_nanoseconds(){
    long long res = 0;
    struct timespec t = {0};
    clock_gettime(CLOCK_BOOTTIME, &t);
    res = t.tv_sec * 1000 * 1000 * 1000 + t.tv_nsec;
    return res;
}

double fps_counter(int resolution_ms){
    static long long start = 0;
    long long diff = 0;
    static double fps = 0;
    if(start == 0)
        start = get_nanoseconds();
    if((diff = (get_nanoseconds() - start)) >= resolution_ms * 1000*1000){
        start = get_nanoseconds();
        fps = (double) ((long long)count*1000*1000*1000) / diff;
        count = 0;
    }
    return fps;
}

void game_logic(){
    draw_board(b);

    if(do_run)evolve_board(&b);
    usleep(20 * 1000);
}

void render(){
    //run_test();
    game_logic();
    if(if_nanoseconds_passed(500*1000*1000))
        printf("FPS: %.2f\n",fps_counter(500));
}
void draw_board(board board1){
    for(int y = 1; y < board1.height; y++)
        for(int x = 1; x < board1.width; x++)
            if (board1.cells[y][x]) draw_cell(x, y);
}

bool if_cell_fits_in_screen(int x, int y){
    if((x * square_size + square_size - camera_x) > effective_w) return false;
    if((y * square_size + square_size - camera_y) > effective_h) return false;
    return true;
}

void draw_cell(int x, int y){
    if(if_cell_fits_in_screen(x, y))
        draw_square(x*square_size - camera_x, y*square_size - camera_y, square_size);
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

    render();

    count++;
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h){
    glViewport(0, 0, w, h);
    WINDOW_W = w; WINDOW_H = h;
    effective_w = WINDOW_W;
    effective_h = WINDOW_H - 100;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}



