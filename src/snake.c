/**
 * snake.c - Snake movement and collision logic
 */

#include "snake.h"
#include <string.h>   /* memcpy, memmove */

/* ── Initialization ─────────────────────────────────────────────── */

void snake_init(Snake *snake, int start_x, int start_y)
{
    snake->length    = SNAKE_INITIAL_LENGTH;
    snake->direction = DIR_RIGHT;

    /*
     * Place the head at (start_x, start_y) and the body segments
     * trailing to the LEFT so the snake faces right.
     */
    for (int i = 0; i < snake->length; i++) {
        snake->body[i].x = start_x - i;
        snake->body[i].y = start_y;
    }
}

/* ── Movement ───────────────────────────────────────────────────── */

void snake_move(Snake *snake, int grow)
{
    /* If growing, keep the tail; otherwise discard it by ignoring
       the last element before shifting. */
    if (grow) {
        if (snake->length < SNAKE_MAX_LENGTH) {
            snake->length++;
        }
    }

    /* Shift every segment forward: body[i] = body[i-1] */
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    /* Move the head in the current direction */
    switch (snake->direction) {
        case DIR_UP:    snake->body[0].y--; break;
        case DIR_DOWN:  snake->body[0].y++; break;
        case DIR_LEFT:  snake->body[0].x--; break;
        case DIR_RIGHT: snake->body[0].x++; break;
    }
}

/* ── Direction Change ───────────────────────────────────────────── */

void snake_set_direction(Snake *snake, Direction new_dir)
{
    /* Prevent 180-degree reversal */
    if ((snake->direction == DIR_UP    && new_dir == DIR_DOWN)  ||
        (snake->direction == DIR_DOWN  && new_dir == DIR_UP)    ||
        (snake->direction == DIR_LEFT  && new_dir == DIR_RIGHT) ||
        (snake->direction == DIR_RIGHT && new_dir == DIR_LEFT)) {
        return;   /* ignore */
    }
    snake->direction = new_dir;
}

/* ── Collision Helpers ──────────────────────────────────────────── */

int snake_head_at(const Snake *snake, int x, int y)
{
    return (snake->body[0].x == x && snake->body[0].y == y);
}

int snake_occupies(const Snake *snake, int x, int y)
{
    for (int i = 0; i < snake->length; i++) {
        if (snake->body[i].x == x && snake->body[i].y == y) {
            return 1;
        }
    }
    return 0;
}

int snake_self_collision(const Snake *snake)
{
    /* Check if head overlaps any body segment (index 1+) */
    for (int i = 1; i < snake->length; i++) {
        if (snake->body[0].x == snake->body[i].x &&
            snake->body[0].y == snake->body[i].y) {
            return 1;
        }
    }
    return 0;
}

