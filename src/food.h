/**
 * food.h - Food spawning and management
 *
 * Handles random placement of food within the arena,
 * ensuring it does not overlap with the snake body.
 */

#ifndef FOOD_H
#define FOOD_H

#include "snake.h"

/* ── Food ───────────────────────────────────────────────────────── */
typedef struct {
    Position pos;
    int      active;   /* 1 = food is on the board */
} Food;

/**
 * Initialize food (inactive).
 */
void food_init(Food *food);

/**
 * Spawn food at a random valid position inside the arena.
 * `arena_w` and `arena_h` are the inner playable dimensions.
 * The position will not overlap any segment of `snake`.
 */
void food_spawn(Food *food, int arena_w, int arena_h, const Snake *snake);

#endif /* FOOD_H */

