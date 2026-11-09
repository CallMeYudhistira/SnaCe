/**
 * input.c - Platform-independent keyboard input implementation
 *
 * Windows  : Uses _kbhit() and _getch() from <conio.h>
 * Linux/Mac: Uses termios non-canonical mode
 */

#include "input.h"
#include <ctype.h>    /* tolower */

/* ═══════════════════════════════════════════════════════════════════
 *  Windows implementation
 * ═══════════════════════════════════════════════════════════════════ */
#ifdef _WIN32

#include <conio.h>
#include <windows.h>

void input_init(void)
{
    /* No special setup needed on Windows for _kbhit/_getch */
}

void input_cleanup(void)
{
    /* Nothing to restore on Windows */
}

int input_get_key(void)
{
    if (_kbhit()) {
        int ch = _getch();

        /* Handle arrow keys (preceded by 0 or 0xE0) */
        if (ch == 0 || ch == 0xE0) {
            ch = _getch();
            switch (ch) {
                case 72: return KEY_UP;     /* Arrow Up    */
                case 80: return KEY_DOWN;   /* Arrow Down  */
                case 75: return KEY_LEFT;   /* Arrow Left  */
                case 77: return KEY_RIGHT;  /* Arrow Right */
                default: return KEY_NONE;
            }
        }

        /* ESC key */
        if (ch == 27) return KEY_ESC;

        /* Enter key */
        if (ch == '\r' || ch == '\n') return KEY_ENTER;

        /* Return lowercase letter */
        return tolower(ch);
    }
    return KEY_NONE;
}

int input_wait_key(void)
{
    int ch = _getch();

    /* Handle arrow keys */
    if (ch == 0 || ch == 0xE0) {
        ch = _getch();
        switch (ch) {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
            default: return KEY_NONE;
        }
    }

    if (ch == 27) return KEY_ESC;
    if (ch == '\r' || ch == '\n') return KEY_ENTER;

    return tolower(ch);
}

void input_sleep_ms(int ms)
{
    Sleep((DWORD)ms);
}

/* ═══════════════════════════════════════════════════════════════════
 *  Linux / macOS implementation
 * ═══════════════════════════════════════════════════════════════════ */
#else

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

static struct termios original_termios;

void input_init(void)
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &original_termios);
    raw = original_termios;

    /* Turn off canonical mode and echo */
    raw.c_lflag &= ~(ICANON | ECHO);

    /* Minimum chars = 0, timeout = 0  →  non-blocking reads */
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void input_cleanup(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

int input_get_key(void)
{
    unsigned char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);
    if (n <= 0) return KEY_NONE;

    /* ESC sequence → check for arrow keys */
    if (ch == 27) {
        unsigned char seq[2];
        /* Non-blocking read of the next two bytes */
        if (read(STDIN_FILENO, &seq[0], 1) <= 0) return KEY_ESC;
        if (read(STDIN_FILENO, &seq[1], 1) <= 0) return KEY_ESC;

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return KEY_UP;
                case 'B': return KEY_DOWN;
                case 'D': return KEY_LEFT;
                case 'C': return KEY_RIGHT;
            }
        }
        return KEY_ESC;
    }

    if (ch == '\r' || ch == '\n') return KEY_ENTER;

    return tolower(ch);
}

int input_wait_key(void)
{
    /* Temporarily switch to blocking mode */
    struct termios blocking;
    tcgetattr(STDIN_FILENO, &blocking);
    blocking.c_cc[VMIN]  = 1;
    blocking.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &blocking);

    int key = KEY_NONE;
    unsigned char ch;
    if (read(STDIN_FILENO, &ch, 1) > 0) {
        if (ch == 27) {
            unsigned char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) > 0 &&
                read(STDIN_FILENO, &seq[1], 1) > 0 &&
                seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': key = KEY_UP;    break;
                    case 'B': key = KEY_DOWN;  break;
                    case 'D': key = KEY_LEFT;  break;
                    case 'C': key = KEY_RIGHT; break;
                    default:  key = KEY_ESC;   break;
                }
            } else {
                key = KEY_ESC;
            }
        } else if (ch == '\r' || ch == '\n') {
            key = KEY_ENTER;
        } else {
            key = tolower(ch);
        }
    }

    /* Restore non-blocking mode */
    struct termios nb;
    tcgetattr(STDIN_FILENO, &nb);
    nb.c_cc[VMIN]  = 0;
    nb.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &nb);

    return key;
}

void input_sleep_ms(int ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

#endif /* _WIN32 */

