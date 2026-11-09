/**
 * ui.c - Terminal UI rendering
 *
 * Responsible for drawing menus, the game arena, and all text output.
 * Uses ANSI escape codes for cursor positioning when possible,
 * with a fallback to system("cls")/system("clear").
 */

#include "ui.h"

/* We need the full GameState definition here */
#include "game.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>   /* system() */
#include <string.h>

/* ── Screen Clearing ────────────────────────────────────────────── */

void ui_clear_screen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ── Helper: print a horizontal border line ─────────────────────── */

static void print_horizontal_border(int width)
{
    putchar(CHAR_CORNER);
    for (int i = 0; i < width; i++) {
        putchar(CHAR_WALL_H);
    }
    putchar(CHAR_CORNER);
    putchar('\n');
}

/* ── Main Menu ──────────────────────────────────────────────────── */

int ui_show_main_menu(void)
{
    int choice = 0;

    while (choice < 1 || choice > 4) {
        ui_clear_screen();
        printf("\n");
        printf("  ================================\n");
        printf("         C CLI SNAKE GAME          \n");
        printf("  ================================\n");
        printf("\n");
        printf("    1. Start Game\n");
        printf("    2. How to Play\n");
        printf("    3. High Score\n");
        printf("    4. Exit\n");
        printf("\n");
        printf("  Select option: ");
        fflush(stdout);

        int key = input_wait_key();
        if (key >= '1' && key <= '4') {
            choice = key - '0';
        }
    }

    return choice;
}

/* ── How to Play ────────────────────────────────────────────────── */

void ui_show_help(void)
{
    ui_clear_screen();
    printf("\n");
    printf("  ================================\n");
    printf("          HOW TO PLAY              \n");
    printf("  ================================\n");
    printf("\n");
    printf("  Controls:\n");
    printf("    W / Up Arrow    = Move Up\n");
    printf("    A / Left Arrow  = Move Left\n");
    printf("    S / Down Arrow  = Move Down\n");
    printf("    D / Right Arrow = Move Right\n");
    printf("    ESC             = Pause / Menu\n");
    printf("\n");
    printf("  Rules:\n");
    printf("    - Eat food (*) to grow and score.\n");
    printf("    - Don't hit the walls or yourself!\n");
    printf("    - Speed increases as you score.\n");
    printf("\n");
    printf("  Press any key to go back...\n");
    fflush(stdout);
    input_wait_key();
}

/* ── High Score Display ─────────────────────────────────────────── */

void ui_show_high_score(int high_score)
{
    ui_clear_screen();
    printf("\n");
    printf("  ================================\n");
    printf("           HIGH SCORE              \n");
    printf("  ================================\n");
    printf("\n");
    printf("    Best Score: %d\n", high_score);
    printf("\n");
    printf("  Press any key to go back...\n");
    fflush(stdout);
    input_wait_key();
}

/* ── Difficulty Menu ────────────────────────────────────────────── */

int ui_show_difficulty_menu(void)
{
    int choice = -1;

    while (choice < 0 || choice > 2) {
        ui_clear_screen();
        printf("\n");
        printf("  ================================\n");
        printf("        SELECT DIFFICULTY          \n");
        printf("  ================================\n");
        printf("\n");
        printf("    1. Easy   (slow speed)\n");
        printf("    2. Normal (medium speed)\n");
        printf("    3. Hard   (fast speed)\n");
        printf("\n");
        printf("  Select option: ");
        fflush(stdout);

        int key = input_wait_key();
        if (key >= '1' && key <= '3') {
            choice = key - '1';   /* 0, 1, or 2 */
        }
    }

    return choice;
}

/* ── Game Arena Rendering ───────────────────────────────────────── */

void ui_render_game(const GameState *state)
{
    ui_clear_screen();

    const Snake *snake = &state->snake;
    const Food  *food  = &state->food;

    /*
     * The arena interior is ARENA_WIDTH x ARENA_HEIGHT.
     * We add a 1-cell border on each side.
     */

    /* Top border */
    print_horizontal_border(ARENA_WIDTH + 2);

    /* Score line inside the border */
    printf("%c ", CHAR_WALL_V);

    /* Build the score string */
    char score_str[64];
    snprintf(score_str, sizeof(score_str), "Score: %d", state->score);
    int score_len = (int)strlen(score_str);

    printf("%s", score_str);
    /* Pad the rest of the line */
    for (int i = score_len; i < ARENA_WIDTH; i++) {
        putchar(CHAR_EMPTY);
    }
    printf(" %c\n", CHAR_WALL_V);

    /* Separator after score */
    printf("%c", CHAR_WALL_V);
    for (int i = 0; i < ARENA_WIDTH + 2; i++) {
        putchar(CHAR_WALL_H);
    }
    printf("%c\n", CHAR_WALL_V);

    /* Arena rows */
    for (int y = 0; y < ARENA_HEIGHT; y++) {
        printf("%c ", CHAR_WALL_V);

        for (int x = 0; x < ARENA_WIDTH; x++) {
            /* Check what's at this position */
            if (snake_head_at(snake, x, y)) {
                putchar(CHAR_SNAKE_HEAD);
            } else if (snake_occupies(snake, x, y)) {
                putchar(CHAR_SNAKE_BODY);
            } else if (food->active && food->pos.x == x && food->pos.y == y) {
                putchar(CHAR_FOOD);
            } else {
                putchar(CHAR_EMPTY);
            }
        }

        printf(" %c\n", CHAR_WALL_V);
    }

    /* Bottom border */
    print_horizontal_border(ARENA_WIDTH + 2);

    fflush(stdout);
}

/* ── Pause Screen ───────────────────────────────────────────────── */

void ui_show_pause(void)
{
    printf("\n");
    printf("  ================================\n");
    printf("            GAME PAUSED            \n");
    printf("  ================================\n");
    printf("\n");
    printf("  Press ESC to resume or Q to quit.\n");
    fflush(stdout);
}

/* ── Game Over Screen ───────────────────────────────────────────── */

int ui_show_game_over(int score, int high_score)
{
    int choice = 0;

    while (choice < 1 || choice > 3) {
        ui_clear_screen();
        printf("\n");
        printf("  ================================\n");
        printf("            GAME OVER              \n");
        printf("  ================================\n");
        printf("\n");
        printf("    Your Score : %d\n", score);
        printf("    High Score : %d\n", high_score);
        printf("\n");
        printf("    1. Play Again\n");
        printf("    2. Back to Menu\n");
        printf("    3. Exit\n");
        printf("\n");
        printf("  Select option: ");
        fflush(stdout);

        int key = input_wait_key();
        if (key >= '1' && key <= '3') {
            choice = key - '0';
        }
    }

    return choice;
}
