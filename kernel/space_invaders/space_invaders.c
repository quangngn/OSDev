#include <graphic.h>
#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <system.h>
#include <time.h>

#define DRAW_TIME 1000000
#define INPUT_TIME 100000
// The speed at which the player is moving
#define MOVE_SPEED 10
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720
#define NUM_OF_ENEMIES 10
#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 40
#define BULLET_WIDTH 5
#define BULLET_HEIGHT 5
#define MIDDLE_X_COORD WINDOW_WIDTH / 2

window_t window;

triangle_t player;
fmat4x4_t t_mat;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} enemy_t;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} bullet_t;

typedef struct bullet_lst {
  bullet_t bullet;
  struct bullet_lst* next;
} bullet_lst_t;

bullet_lst_t* head = NULL;

enemy_t enemies[NUM_OF_ENEMIES];
float angle = 0;

void init_enemy() {
  // Initialize an array containing enemy objects and set the enemy positions on e screen
  for (int i = 0; i < NUM_OF_ENEMIES; i++) {
    enemies[i] = (enemy_t){.x = 25 + i * (WINDOW_WIDTH / NUM_OF_ENEMIES),
                           .y = 700,
                           .w = ENEMY_WIDTH,
                           .h = ENEMY_HEIGHT};
  }
}

void draw_enemy() {
  // Draw the enemies at the points specified in init_enemy
  for (int i = 0; i < NUM_OF_ENEMIES; i++) {
    rec2d_wh(enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h, ARGB32_RED,
             true, &window);
  }
}

void init_player() {
  // Create 3 points which will be used as triangle vertices
  point_t p0 = {.x = MIDDLE_X_COORD, .y = 30, .z = 0, .w = 1};
  point_t p1 = {.x = MIDDLE_X_COORD + 15, .y = 12, .z = 0, .w = 1};
  point_t p2 = {.x = MIDDLE_X_COORD - 15, .y = 12, .z = 0, .w = 1};
  player.p0 = p0;
  player.p1 = p1;
  player.p2 = p2;
}

void draw_player() { 
  // Draw the player, represented by a green triangle
  tri2d_t(&player, ARGB32_GREEN, true, &window); }

void move_player(char c) {
  // Read in the pressed key and move the player in the direction specified by the key
  switch (c) {
    case 'a':
      player.p0.x -= MOVE_SPEED;
      player.p1.x -= MOVE_SPEED;
      player.p2.x -= MOVE_SPEED;

      tri2d_t(&player, ARGB32_GREEN, true, &window);
      graphic_draw(&window, true);
      break;
    case 'd':
      player.p0.x += MOVE_SPEED;
      player.p1.x += MOVE_SPEED;
      player.p2.x += MOVE_SPEED;

      tri2d_t(&player, ARGB32_GREEN, true, &window);
      graphic_draw(&window, true);
      break;
    default:
      break;
  }
}

void shoot() {
  // Allocate space for a new bullet
  bullet_lst_t* new_bullet = (bullet_lst_t*)malloc(sizeof(bullet_lst_t));

  // Initialize the position of the bullet at the time of shooting
  new_bullet->bullet = (bullet_t){.x = player.p0.x,
                                  .y = player.p0.y + 10,
                                  .w = BULLET_WIDTH,
                                  .h = BULLET_HEIGHT};
  
  // Append the newly-created bullet to the front of the bullet linked list
  new_bullet->next = head;
  head = new_bullet;
}

void draw_bullet() {
  bullet_lst_t* pointer = head;
  // Draw all the bullets in the bullet linked list
  while (pointer != NULL) {
    rec2d_wh(pointer->bullet.x, pointer->bullet.y, pointer->bullet.w,
             pointer->bullet.h, ARGB32_YELLOW, true, &window);

    pointer = pointer->next;
  }
}

void update_bullet() {
  // If there is no bullet, return
  if (head == NULL) {
    return;
  }

  bullet_lst_t* current_bullet = head->next;
  bullet_lst_t* previous_bullet = head;

  // We might not be able to delete if the list has 1 bullet. But the behavior
  // is fine. We will patch in future update
  head->bullet.y += 5;

  // Increase the y-coordinate of every bullet in the linked list
  // If the y-coordinate is greater than the height of the window, delete the bullet
  while (current_bullet != NULL) {
    if (current_bullet->bullet.y > WINDOW_HEIGHT + BULLET_HEIGHT) {
      previous_bullet->next = current_bullet->next;
      free(current_bullet);
      current_bullet = previous_bullet->next;
    } else {
      current_bullet->bullet.y += 5;
      current_bullet = current_bullet->next;
      previous_bullet = previous_bullet->next;
    }
  }
}

void delete_enemy(int index) {
  // Delete enemy by setting their height and width to zero
  enemies[index].h = 0;
  enemies[index].w = 0;
}

void hit_enemy() {
  // If there is no bullet, return
  if (head == NULL) {
    return;
  }

  bullet_lst_t* current_bullet = head;

  // Iterate through all the bullets and for each bullet, check if
  //  it hits an enemy. If an enemy is hit, call delete_enemy on it.
  //  Otherwise, continue iterating.
  while (current_bullet != NULL) {
    for (int i = 0; i < NUM_OF_ENEMIES; i++) {
      if (current_bullet->bullet.x + BULLET_WIDTH >= enemies[i].x &&
          current_bullet->bullet.x <= enemies[i].x + ENEMY_WIDTH &&
          current_bullet->bullet.y >= enemies[i].y - ENEMY_HEIGHT) {
        delete_enemy(i);
        continue;
      }
    }
    current_bullet = current_bullet->next;
  }
}

void initialize_game() {
  // Initialize the game window
  window_init(&window, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, ARGB32_GRAY);

  // Initialize the enemies
  init_enemy();

  // Initialize the player
  init_player();

  // Draw the enemies and the player into the window
  graphic_draw(&window, false);
}

void _start() {
  initialize_game();

  uint64_t prev_draw_time = 0;
  uint64_t prev_input_time = 0;

 // Run this loop and for each frame, 
 // 1. Draw all the objects (enemies, player, and bullets)
 // 2. Get keyboard input and either call shoot, move_player, quit or do nothing based on the input
 // 3. Get the time elapsed between one frame to the next
  while (true) {
    if (get_time() - prev_draw_time > DRAW_TIME) {
      draw_enemy();
      draw_player();
      update_bullet();
      draw_bullet();
      hit_enemy();

      graphic_draw(&window, true);
      prev_draw_time = get_time();
    }

    // Use the keyboard input to control the cube location on xy-plane
    if (get_time() - prev_input_time > INPUT_TIME) {
      char c = peekc();
      switch (c) {
        case 'a':
          move_player('a');
          break;
        case 'd':
          move_player('d');
          break;
        case ' ':
          shoot();
          break;
        case 'q':
          exit();
          break;
        default:
          break;
      }

      prev_input_time = get_time();
    }
  }

  for (;;) {
  }
  // Press 'q' to exit the the program
  while (true) {
    char c = getc_silent(STD_IN);
    if (c == 'q')
      exit();
    else
      move_player(c);
  }

  for (;;) {
  }
}
