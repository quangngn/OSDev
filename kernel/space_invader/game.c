#include "game.h"

window_t window;

void draw_enemy(){
    for (int i = 0; i < NUM_OF_ENEMY; i++){
        rec2d_wh(i * (WINDOW_WIDTH/NUM_OF_ENEMY), 700, ENEMY_WIDTH, ENEMY_HEIGHT, ARGB32_RED, true, &window);
    }
    
}

void draw_player(){
    point_t p0 = {.x = MIDDLE_X_COORD, .y = 30, .z = 0, .w = 1};
    point_t p1 = {.x = MIDDLE_X_COORD + 15, .y = 12, .z = 0, .w = 1};
    point_t p2 = {.x = MIDDLE_X_COORD - 15, .y = 12, .z = 0, .w = 1};

    triangle_t player = {.p0 = p0, .p1 = p1, .p2 = p2};
    
    tri2d_t(&player, ARGB32_GREEN, true, &window);
}

void start_game(){
    window_init(&window, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, ARGB32_WHITE);

    draw_enemy();
    draw_player();
    graphic_draw(&window, false);
}