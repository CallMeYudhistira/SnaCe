/**
 * input.h - Platform-independent keyboard input
 *
 * Provides non-blocking, real-time keyboard reading.
 * On Windows it uses _kbhit() / _getch() from <conio.h>.
 * On Linux/macOS it uses termios non-canonical mode.
 */

#ifndef INPUT_H
#define INPUT_H

/* Special key codes returned by input_get_key() */
#define KEY_NONE    0
#define KEY_UP    200
#define KEY_DOWN  201
#define KEY_LEFT  202
#define KEY_RIGHT 203
#define KEY_ESC    27
#define KEY_ENTER  13

/**
 * Set up the terminal for non-blocking input.
 * Must be called once before the game loop.
 */
void input_init(void);

/**
 * Restore the terminal to its original settings.
 * Must be called before the program exits.
 */
void input_cleanup(void);

/**
 * Return the key currently pressed, or KEY_NONE if nothing is pressed.
 * Non-blocking: returns immediately.
 * Returned values are lowercase letters 'w','a','s','d', KEY_ESC, etc.
 */
int  input_get_key(void);

/**
 * Block until a key is pressed and return it.
 */
int  input_wait_key(void);

/**
 * Platform-independent sleep for `ms` milliseconds.
 */
void input_sleep_ms(int ms);

#endif /* INPUT_H */
