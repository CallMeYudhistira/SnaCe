/**
 * ui.h - Terminal-based user interface rendering
 *
 * All drawing / printing to the console happens through this module.
 * Keeps rendering logic separate from game logic.
 */

#ifndef UI_H
#define UI_H

/* Forward-declare GameState (defined in game.h with the same tag) */
struct GameState;
typedef struct GameState GameState;

/* ── Display Characters ─────────────────────────────────────────── */
#define CHAR_WALL_H   '-'
#define CHAR_WALL_V   '|'
#define CHAR_CORNER   '+'
#define CHAR_SNAKE_HEAD 'O'
#define CHAR_SNAKE_BODY 'o'
#define CHAR_FOOD       '*'
#define CHAR_EMPTY      ' '

/**
 * Clear the terminal screen (platform-independent).
 */
void ui_clear_screen(void);

/**
 * Render the main menu and return the selected option (1-4).
 */
int  ui_show_main_menu(void);

/**
 * Render the "How to Play" screen.
 */
void ui_show_help(void);

/**
 * Render the high-score screen.
 */
void ui_show_high_score(int high_score);

/**
 * Render the difficulty selection menu.
 * Returns 0 = Easy, 1 = Normal, 2 = Hard.
 */
int  ui_show_difficulty_menu(void);

/**
 * Render the game arena, snake, food, and score.
 * This is called every tick.
 */
void ui_render_game(const GameState *state);

/**
 * Render the pause overlay.
 */
void ui_show_pause(void);

/**
 * Render the Game Over screen and return the player's choice (1-3).
 */
int  ui_show_game_over(int score, int high_score);

#endif /* UI_H */
