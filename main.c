#define _XOPEN_SOURCE 600

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


#include "game_of_life.h"

#define MY_COLOR_WHITE (Color3f){1.0f, 1.0f, 1.0f}
#define MY_COLOR_BLACK (Color3f){0.0f, 0.0f, 0.0f}
#define MY_COLOR_DARK_GRAY (Color3f){0.3f, 0.3f, 0.3f}
#define MY_COLOR_GRAY (Color3f){0.5f, 0.5f, 0.5f}
#define MY_COLOR_LIGHT_GRAY (Color3f){0.7f, 0.7f, 0.7f}
#define MY_COLOR_RED (Color3f){1.0f, 0.0f, 0.0f}

typedef struct Color3f_{
    float r,g,b;
}Color3f;

bool do_run = true;
double FPS_G = 0;
int WINDOW_W = 1000, WINDOW_H = 1000;
int effective_w, effective_h;
int square_size = 5;
int FRAME_COUNTER = 1;
board b;
long long camera_x = 0, camera_y = 0;
int frame_delay = 20;
int mouse_x, mouse_y;
bool mlb_down, mrb_down;

void game_logic();
void render();
void display();
void reshape(int w, int h);

bool if_cell_fits_in_screen(int x, int y);
bool if_cell_is_on_board(int x, int y);
// Game information
void write_game_controls();
void write_game_state();
void write_frame_delay();
void write_camera_position();
void write_fps(double fps);
void info_area();

// Keyboard - mouse
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key, int x, int y);
void processMouseAction(int button, int state, int x, int y);
void processMouseMove(int x, int y);
void color_cells_with_mouse();

// Time counting
double fps_counter(int resolution_ms);
bool if_nanoseconds_passed(long long n);
long long get_nanoseconds();

// Shape drawers
void draw_string(int x, int y, const unsigned char *string);
void draw_board(board board1);
void draw_cell(int x, int y);
void draw_box(int x, int y, int x_side, int y_side, int thickness, Color3f color);
void draw_vertical_line(int x, int thickness, Color3f color);
void draw_horizontal_line(int y, int thickness, Color3f color);
void draw_white_square(int x, int y, int side);
void draw_square(int x, int y, int side, Color3f color);
void draw_rectangle(int x, int y, int x_side, int y_side, Color3f color);

int main (int argc, char * argv[]) {

    effective_w = WINDOW_W;
    effective_h = WINDOW_H - 100;
    srand(time(NULL));
    b = create_empty_board(WINDOW_W, WINDOW_H);
    fill_board_random(b);
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Game Of Life");

    glutMouseFunc(processMouseAction);
    glutMotionFunc(processMouseMove);
    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}


void game_logic(){
    info_area();
    draw_board(b);

    if(do_run)evolve_board(&b);
    color_cells_with_mouse();
    usleep(frame_delay * 1000);
}
void render(){
    //run_test();
    game_logic();
    if(if_nanoseconds_passed(500*1000*1000))
        FPS_G = fps_counter(500);
}
bool if_cell_fits_in_screen(int x, int y){
    if(((x - camera_x) * square_size + square_size) > effective_w) return false;
    if(((y - camera_y) * square_size + square_size) > effective_h) return false;
    return true;
}
bool if_cell_is_on_board(int x, int y){
    if( x < 1 || x > b.width-1) return false;
    if( y < 1 || y > b.height-1) return false;
    return true;
}

void write_game_controls(){
    draw_string(400, WINDOW_H - 29, (unsigned char *)"WASD: Camera");
    draw_string(400, effective_h + 10, (unsigned char *)"QE: Zoom");
    draw_string(600, WINDOW_H - 29, (unsigned char *)"ZX: Delay");
    draw_string(520, effective_h + 10, (unsigned char *)"R: Reset");
    draw_string(620, effective_h + 10, (unsigned char *)"C: Clear");
    draw_string(720, WINDOW_H - 29, (unsigned char *)"F5: Seed");
    draw_string(720, effective_h + 10, (unsigned char *)"ESC: Quit");
}
void write_game_state(){
    if(do_run){
        draw_string(250, WINDOW_H - 29, (unsigned char *)"RUNNING");
    } else{
        draw_string(250, WINDOW_H - 29, (unsigned char *)"STOPPED");
    }
}
void write_frame_delay(){
    unsigned char buff[64];
    sprintf((char *)buff, "Delay: +%dms", frame_delay);
    draw_string(250, effective_h + 10, buff);
}
void write_camera_position(){
    unsigned char buff[64];
    sprintf((char *)buff, "Camera(%lld, %lld)", camera_x, camera_y);
    draw_string(5, effective_h + 10, buff);
}
void write_fps(double fps){
    unsigned char buff[64];
    sprintf((char *)buff, "FPS: %.2f", fps);
    draw_string(5, WINDOW_H - 29, buff);
}
void info_area(){
    draw_horizontal_line(effective_h, 5, MY_COLOR_LIGHT_GRAY);
    write_fps(FPS_G);
    write_camera_position();
    write_frame_delay();
    write_game_state();
    write_game_controls();
}

void processSpecialKeys(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_F5: // seed board with cells, 50% density
            fill_board_random(b);
            break;
    }
}
void processNormalKeys(unsigned char key, int x, int y){
    switch (key) {
        case 27: // ESC key
            exit(0);
            break;
        case ' ': // pause the game
            do_run = !do_run;
            break;
        case 'a': // move camera left 5 squares
            camera_x -= 5;
            break;
        case 'd': // move camera right 5 squares
            camera_x += 5;
            break;
        case 's': // move camera down 5 squares
            camera_y -= 5;
            break;
        case 'w': // move camera up 5 squares
            camera_y += 5;
            break;
        case 'e': // make side of a cell 1 pixel bigger
            square_size += 1;
            break;
        case 'q': // make side of a cell 1 pixel smaller, but not smaller than 1 pixel
            if(square_size != 1) square_size -= 1;
            break;
        case 'r': // reset camera position, square size, and frame delay to default
            camera_x = 0;
            camera_y = 0;
            square_size = 5;
            frame_delay = 20;
            reshape(1000,1000);
            break;
        case 'c': // kill all cells on the board (empty the board)
            clear_board(b);
            break;
        case 'z': // lower additional frame delay by 1, but not smaller than 0 ms
            if(frame_delay != 0) frame_delay -= 1;
            break;
        case 'x': // increase additional frame delay by 1
            frame_delay += 1;
            break;
    }
}
void processMouseAction(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        mlb_down = (state == GLUT_DOWN);
    }else if(button == GLUT_RIGHT_BUTTON){
        mrb_down = (state == GLUT_DOWN);
    }
}
void processMouseMove(int x, int y){
    mouse_x = x;
    mouse_y = WINDOW_H - y;
}
void color_cells_with_mouse(){
    int grid_x, grid_y;
    if(mlb_down){
        grid_x = mouse_x / square_size + camera_x;
        grid_y = mouse_y / square_size + camera_y;
        if(if_cell_is_on_board(grid_x, grid_y))
            b.cells[grid_y][grid_x] = alive;
    } else if(mrb_down){
        grid_x = mouse_x / square_size + camera_x;
        grid_y = mouse_y / square_size + camera_y;
        if(if_cell_is_on_board(grid_x, grid_y))
            b.cells[grid_y][grid_x] = dead;
    }else{
        mouse_x = -1;
        mouse_y = -1;
    }
}

double fps_counter(int resolution_ms){
    static long long start = 0;
    long long diff = 0;
    static double fps = 0;
    if(start == 0)
        start = get_nanoseconds();
    if((diff = (get_nanoseconds() - start)) >= resolution_ms * 1000*1000){
        start = get_nanoseconds();
        fps = (double) ((long long)FRAME_COUNTER * 1000 * 1000 * 1000) / diff;
        FRAME_COUNTER = 0;
    }
    return fps;
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
long long get_nanoseconds(){
    long long res = 0;
    struct timespec t = {0};
    clock_gettime(CLOCK_BOOTTIME, &t);
    res = t.tv_sec * 1000 * 1000 * 1000 + t.tv_nsec;
    return res;
}

void draw_string(int x, int y, const unsigned char *string){
    glColor3f(1,1,1);
    glRasterPos2i(x, y);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, string);
}
void draw_board(board board1){
    draw_box(0 - camera_x*square_size, 0 - camera_y*square_size, board1.width * square_size, board1.height * square_size, square_size,
             MY_COLOR_RED);
    for(int y = 1; y < board1.height-1; y++)
        for(int x = 1; x < board1.width-1; x++)
            if (board1.cells[y][x]) draw_cell(x, y);
}
void draw_cell(int x, int y){
    if(if_cell_fits_in_screen(x, y))
        draw_white_square((x-camera_x)*square_size, (y-camera_y)*square_size, square_size);
}
void draw_box(int x, int y, int x_side, int y_side, int thickness, Color3f color){
    draw_horizontal_line(y, thickness, color);
    draw_horizontal_line(y + y_side, thickness, color);
    draw_vertical_line(x, thickness, color);
    draw_vertical_line(x + x_side, thickness, color);
}
void draw_vertical_line(int x, int thickness, Color3f color){
    draw_rectangle(x, 0, thickness, WINDOW_H, color);
}
void draw_horizontal_line(int y, int thickness, Color3f color){
    draw_rectangle(0, y, WINDOW_W, thickness, color);
}
void draw_white_square(int x, int y, int side){
    draw_square(x, y, side, (Color3f){1.0f, 1.0f,1.0f});
}
void draw_square(int x, int y, int side, Color3f color){
    draw_rectangle(x, y, side, side, color);
}
void draw_rectangle(int x, int y, int x_side, int y_side, Color3f color){
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + x_side, y);
    glVertex2i(x + x_side, y+y_side);
    glVertex2i(x, y + y_side);
    glEnd();
}

void display(){
    glClear (GL_COLOR_BUFFER_BIT);
    render();
    FRAME_COUNTER++;
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



