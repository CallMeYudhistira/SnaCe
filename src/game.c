/**
 * game.c - Core game logic
 *
 * Manages game initialization, the update loop (one tick),
 * and high-score persistence.
 */

#include "game.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#ifdef _WIN32
#include <direct.h>   /* _mkdir */
#endif

/* ── High Score I/O ─────────────────────────────────────────────── */

int game_load_high_score(void)
{
    FILE *fp = fopen(HIGHSCORE_FILE, "rb");
    if (!fp) {
        return 0;
    }

    int score = 0;
    if (fread(&score, sizeof(int), 1, fp) != 1) {
        score = 0;
    }
    fclose(fp);
    return score;
}

void game_save_high_score(int score)
{
    /* Create the data/ directory if it doesn't exist */
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0755);
#endif

    FILE *fp = fopen(HIGHSCORE_FILE, "wb");
    if (!fp) {
        fprintf(stderr, "Warning: could not save high score.\n");
        return;
    }
    fwrite(&score, sizeof(int), 1, fp);
    fclose(fp);
}

/* ── Game Initialization ────────────────────────────────────────── */

void game_init(GameState *state, Difficulty difficulty)
{
    /* Set difficulty and speed */
    state->difficulty = difficulty;
    switch (difficulty) {
        case DIFFICULTY_EASY:   state->tick_delay_ms = SPEED_EASY;   break;
        case DIFFICULTY_NORMAL: state->tick_delay_ms = SPEED_NORMAL; break;
        case DIFFICULTY_HARD:   state->tick_delay_ms = SPEED_HARD;   break;
    }

    /* Reset score */
    state->score = 0;

    /* Load high score */
    state->high_score = game_load_high_score();

    /* Initialize snake at the center of the arena */
    int center_x = ARENA_WIDTH  / 2;
    int center_y = ARENA_HEIGHT / 2;
    snake_init(&state->snake, center_x, center_y);

    /* Spawn the first food */
    food_init(&state->food);
    food_spawn(&state->food, ARENA_WIDTH, ARENA_HEIGHT, &state->snake);

    /* Set game status */
    state->status = STATUS_PLAYING;
}

/* ── Game Update (one tick) ─────────────────────────────────────── */

void game_update(GameState *state)
{
    Snake *snake = &state->snake;
    Food  *food  = &state->food;

    /* 1. Process buffered input */
    int key = input_get_key();

    switch (key) {
        case KEY_UP:   case 'w': snake_set_direction(snake, DIR_UP);    break;
        case KEY_DOWN: case 's': snake_set_direction(snake, DIR_DOWN);  break;
        case KEY_LEFT: case 'a': snake_set_direction(snake, DIR_LEFT);  break;
        case KEY_RIGHT:case 'd': snake_set_direction(snake, DIR_RIGHT); break;
        case KEY_ESC:
            state->status = STATUS_PAUSED;
            return;
        default:
            break;
    }

    /* 2. Determine if the snake will eat food this tick */
    Position next_head = snake->body[0];
    switch (snake->direction) {
        case DIR_UP:    next_head.y--; break;
        case DIR_DOWN:  next_head.y++; break;
        case DIR_LEFT:  next_head.x--; break;
        case DIR_RIGHT: next_head.x++; break;
    }

    int eating = (food->active &&
                  next_head.x == food->pos.x &&
                  next_head.y == food->pos.y);

    /* 3. Move the snake (grow if eating) */
    snake_move(snake, eating);

    /* 4. Check wall collision */
    if (snake->body[0].x < 0 || snake->body[0].x >= ARENA_WIDTH ||
        snake->body[0].y < 0 || snake->body[0].y >= ARENA_HEIGHT) {
        state->status = STATUS_GAME_OVER;
        return;
    }

    /* 5. Check self collision */
    if (snake_self_collision(snake)) {
        state->status = STATUS_GAME_OVER;
        return;
    }

    /* 6. Handle food consumption */
    if (eating) {
        state->score += SCORE_PER_FOOD;

        /* Update high score if beaten */
        if (state->score > state->high_score) {
            state->high_score = state->score;
        }

        /* Increase speed (reduce tick delay) */
        state->tick_delay_ms -= SPEED_INCREMENT;
        if (state->tick_delay_ms < SPEED_MIN_DELAY) {
            state->tick_delay_ms = SPEED_MIN_DELAY;
        }

        /* Spawn new food */
        food_spawn(food, ARENA_WIDTH, ARENA_HEIGHT, snake);
    }
}
