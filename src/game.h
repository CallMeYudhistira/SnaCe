/**
 * game.h - Game state management and core game loop
 *
 * Contains the main game state structure and functions for
 * initializing, running, and cleaning up the game.
 */

#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "food.h"

/* ── Arena Dimensions ───────────────────────────────────────────── */
#define ARENA_WIDTH  30
#define ARENA_HEIGHT 15

/* ── Scoring ────────────────────────────────────────────────────── */
#define SCORE_PER_FOOD 10

/* ── Speed (milliseconds per tick) ──────────────────────────────── */
#define SPEED_EASY   200
#define SPEED_NORMAL 130
#define SPEED_HARD   80

/* Speed increase per food eaten (ms subtracted from delay) */
#define SPEED_INCREMENT     3
#define SPEED_MIN_DELAY    40   /* fastest possible tick */

/* ── High-score file path ───────────────────────────────────────── */
#define HIGHSCORE_FILE "data/highscore.dat"

/* ── Game Status ────────────────────────────────────────────────── */
typedef enum {
    STATUS_MENU,
    STATUS_PLAYING,
    STATUS_PAUSED,
    STATUS_GAME_OVER,
    STATUS_QUIT
} GameStatus;

/* ── Difficulty Level ───────────────────────────────────────────── */
typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_NORMAL,
    DIFFICULTY_HARD
} Difficulty;

/* ── Game State ─────────────────────────────────────────────────── */
typedef struct GameState {
    Snake      snake;
    Food       food;
    int        score;
    int        high_score;
    int        tick_delay_ms;   /* current speed in ms */
    Difficulty difficulty;
    GameStatus status;
} GameState;

/**
 * Initialize a new game state.
 * Resets snake, places food, loads high score, and sets speed
 * according to the chosen difficulty.
 */
void game_init(GameState *state, Difficulty difficulty);

/**
 * Run one tick of the game loop:
 *  1. Process input (direction change).
 *  2. Move the snake.
 *  3. Check collisions (wall, self).
 *  4. Check food consumption.
 *  5. Update score / speed.
 */
void game_update(GameState *state);

/**
 * Load the persisted high score from HIGHSCORE_FILE.
 * Returns 0 if no file exists yet.
 */
int  game_load_high_score(void);

/**
 * Save the high score to HIGHSCORE_FILE.
 * Creates the data/ directory if needed.
 */
void game_save_high_score(int score);

#endif /* GAME_H */
