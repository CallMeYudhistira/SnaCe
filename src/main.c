/**
 * main.c - Entry point for SnaCe (CLI Snake Game)
 *
 * Orchestrates the main menu, game loop, and program flow.
 */

#include "game.h"
#include "input.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

/* ── Forward declarations for menu handlers ─────────────────────── */
static void run_game(Difficulty difficulty);
static void handle_pause(GameState *state);

/* ════════════════════════════════════════════════════════════════════
 *  Main
 * ════════════════════════════════════════════════════════════════════ */

int main(void)
{
    input_init();

    int running = 1;

    while (running) {
        int choice = ui_show_main_menu();

        switch (choice) {
            case 1: {
                /* Select difficulty, then start game */
                int diff = ui_show_difficulty_menu();
                run_game((Difficulty)diff);
                break;
            }
            case 2:
                ui_show_help();
                break;
            case 3: {
                int hs = game_load_high_score();
                ui_show_high_score(hs);
                break;
            }
            case 4:
                running = 0;
                break;
        }
    }

    input_cleanup();
    ui_clear_screen();
    printf("Thanks for playing SnaCe! Goodbye.\n");
    return 0;
}

/* ── Game Session ───────────────────────────────────────────────── */

static void run_game(Difficulty difficulty)
{
    GameState state;
    int play_again = 1;

    while (play_again) {
        game_init(&state, difficulty);

        /* ── Game Loop ───────────────────────────────────────────── */
        while (state.status == STATUS_PLAYING) {
            ui_render_game(&state);
            input_sleep_ms(state.tick_delay_ms);
            game_update(&state);

            /* Handle pause */
            if (state.status == STATUS_PAUSED) {
                handle_pause(&state);
            }
        }

        /* ── Game Over ───────────────────────────────────────────── */
        if (state.status == STATUS_GAME_OVER) {
            /* Save high score */
            game_save_high_score(state.high_score);

            int go_choice = ui_show_game_over(state.score, state.high_score);
            switch (go_choice) {
                case 1:  /* Play Again */
                    play_again = 1;
                    break;
                case 2:  /* Back to Menu */
                    play_again = 0;
                    break;
                case 3:  /* Exit */
                    input_cleanup();
                    exit(0);
                    break;
            }
        } else {
            /* Player quit from pause menu */
            play_again = 0;
        }
    }
}

/* ── Pause Handler ──────────────────────────────────────────────── */

static void handle_pause(GameState *state)
{
    ui_show_pause();

    while (1) {
        int key = input_wait_key();

        if (key == KEY_ESC) {
            /* Resume game */
            state->status = STATUS_PLAYING;
            return;
        }
        if (key == 'q') {
            /* Quit to menu */
            state->status = STATUS_QUIT;
            return;
        }
    }
}

