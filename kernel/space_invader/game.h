#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <graphic.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 720
#define NUM_OF_ENEMY 10
#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 40
#define MIDDLE_X_COORD WINDOW_WIDTH/2

void draw_enemy();
void draw_player();
void move_player(char c);
void shoot();
void start_game();