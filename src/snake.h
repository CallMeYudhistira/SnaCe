/**
 * snake.h - Snake data structure and movement logic
 *
 * The snake is represented as an array of (x, y) positions.
 * Index 0 is the head; the last occupied index is the tail.
 */

#ifndef SNAKE_H
#define SNAKE_H

/* Maximum possible length = entire arena area */
#define SNAKE_MAX_LENGTH 450   /* ARENA_WIDTH * ARENA_HEIGHT */

/* Starting length of the snake */
#define SNAKE_INITIAL_LENGTH 3

/* ── Position ───────────────────────────────────────────────────── */
typedef struct {
    int x;
    int y;
} Position;

/* ── Direction ──────────────────────────────────────────────────── */
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

/* ── Snake ──────────────────────────────────────────────────────── */
typedef struct {
    Position  body[SNAKE_MAX_LENGTH];
    int       length;
    Direction direction;
} Snake;

/**
 * Initialize the snake at a given starting position with
 * SNAKE_INITIAL_LENGTH segments heading right.
 */
void snake_init(Snake *snake, int start_x, int start_y);

/**
 * Move the snake one step in its current direction.
 * If `grow` is non-zero the tail is NOT removed (snake gets longer).
 */
void snake_move(Snake *snake, int grow);

/**
 * Try to change the snake's direction.
 * Ignores the request if it would reverse direction (e.g. UP → DOWN).
 */
void snake_set_direction(Snake *snake, Direction new_dir);

/**
 * Return non-zero if the snake's head occupies position (x, y).
 */
int  snake_head_at(const Snake *snake, int x, int y);

/**
 * Return non-zero if any body segment (including head) is at (x, y).
 */
int  snake_occupies(const Snake *snake, int x, int y);

/**
 * Return non-zero if the head collides with any other body segment.
 */
int  snake_self_collision(const Snake *snake);

#endif /* SNAKE_H */

