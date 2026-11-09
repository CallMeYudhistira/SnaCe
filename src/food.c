/**
 * food.c - Food spawning logic
 */

#include "food.h"
#include <stdlib.h>   /* rand, srand */
#include <time.h>     /* time */

/* Track whether srand has been called */
static int seeded = 0;

/* ── Initialization ─────────────────────────────────────────────── */

void food_init(Food *food)
{
    food->pos.x  = 0;
    food->pos.y  = 0;
    food->active = 0;
}

/* ── Spawning ───────────────────────────────────────────────────── */

void food_spawn(Food *food, int arena_w, int arena_h, const Snake *snake)
{
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }

    /*
     * Keep trying random positions until we find one that
     * does NOT overlap any snake segment.
     *
     * Valid coordinates: x in [0, arena_w-1], y in [0, arena_h-1]
     * These are relative to the playable area inside the walls.
     */
    int max_attempts = arena_w * arena_h;  /* safety limit */
    int attempts     = 0;

    do {
        food->pos.x = rand() % arena_w;
        food->pos.y = rand() % arena_h;
        attempts++;

        /* If the arena is almost full, bail out */
        if (attempts >= max_attempts) {
            break;
        }
    } while (snake_occupies(snake, food->pos.x, food->pos.y));

    food->active = 1;
}

