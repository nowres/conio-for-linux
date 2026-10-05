/* Unit tests for conio. Each test runs in a forked child attached to a pty. */
#include "conio.h"
#include <pty.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

extern int screen_initialized;
extern WINDOW *_working_window;

static int g_msgfd;

#define CHECK(c) do { if (!(c)) { dprintf(g_msgfd, "%s:%d: %s\n", __FILE__, __LINE__, #c); return 1; } } while (0)

static int ch_at(int y, int x) { return mvinch(y, x) & A_CHARTEXT; }

static int t_init_idempotent(void) {
    init_screen();
    init_screen();
    CHECK(screen_initialized == 1);
    CHECK(_working_window == stdscr);
    return 0;
}

static int t_gotoxy_wherexy(void) {
    gotoxy(5, 3);
    CHECK(wherex() == 5);
    CHECK(wherey() == 3);
    return 0;
}

static int t_cprintf_advances(void) {
    gotoxy(0, 0);
    cprintf("abc%d", 7);
    CHECK(wherex() == 4);
    CHECK(ch_at(0, 0) == 'a' && ch_at(0, 3) == '7');
    return 0;
}

static int t_cputs_newline(void) {
    gotoxy(0, 0);
    CHECK(cputs("hi") == 0);
    CHECK(wherey() == 1);
    CHECK(wherex() == 0);
    return 0;
}

static int t_clrscr(void) {
    gotoxy(0, 0);
    cprintf("xyz");
    gotoxy(4, 4);
    clrscr();
    CHECK(wherex() == 0 && wherey() == 0);
    CHECK(ch_at(0, 0) == ' ');
    return 0;
}

static int t_clreol(void) {
    gotoxy(0, 0);
    cprintf("abcdef");
    gotoxy(3, 0);
    clreol();
    CHECK(ch_at(0, 2) == 'c');
    CHECK(ch_at(0, 3) == ' ');
    return 0;
}

static int t_delline(void) {
    gotoxy(0, 0);
    cprintf("abcdef");
    gotoxy(2, 0);
    delline();
    CHECK(ch_at(0, 0) == ' ');
    CHECK(wherex() == 0);
    return 0;
}

static int t_colors(void) {
    short f, b;
    textcolor(RED);
    textbackground(YELLOW);
    gotoxy(0, 0);
    cprintf("x");
    chtype c = mvinch(0, 0);
    pair_content(PAIR_NUMBER(c & A_COLOR), &f, &b);
    CHECK(f == RED && b == YELLOW);
    return 0;
}

static int t_window(void) {
    init_screen();
    window(10, 5, 30, 15);
    CHECK(_working_window != stdscr);
    CHECK(getbegx(_working_window) == 10 && getbegy(_working_window) == 5);
    CHECK(getmaxx(_working_window) == 20 && getmaxy(_working_window) == 10);
    cprintf("hi");
    CHECK(wherex() == 2);
    window(0, 0, 0, 0);
    CHECK(_working_window == stdscr);
    return 0;
}

static int t_getch(void) {
    CHECK(getch() == 'a');
    return 0;
}

static int t_getche(void) {
    gotoxy(0, 0);
    CHECK(getche() == 'b');
    CHECK(wherex() == 1);
    CHECK(ch_at(0, 0) == 'b');
    return 0;
}

static int t_kbhit_false(void) {
    CHECK(kbhit() == FALSE);
    return 0;
}

static int t_kbhit_true(void) {
    init_screen();
    usleep(500000);
    CHECK(kbhit() == TRUE);
    CHECK(getch() == 'z');
    return 0;
}

static int t_cgets(void) {
    char buf[16] = {0};
    buf[0] = 10;
    char *r = cgets(buf);
    CHECK(r == buf + 2);
    CHECK(strcmp(r, "hello") == 0);
    CHECK(buf[1] == 5);
    return 0;
}

static int t_cscanf(void) {
    int n = 0;
    char s[16] = {0};
    CHECK(cscanf("%d %15s", &n, s) == 2);
    CHECK(n == 12);
    CHECK(strcmp(s, "abc") == 0);
    return 0;
}

struct test {
    const char *name;
    int (*fn)(void);
    const char *input;
    int input_delay_ms;
};

static const struct test tests[] = {
    {"init_screen idempotent", t_init_idempotent, NULL, 0},
    {"gotoxy/wherex/wherey", t_gotoxy_wherexy, NULL, 0},
    {"cprintf advances cursor", t_cprintf_advances, NULL, 0},
    {"cputs newline", t_cputs_newline, NULL, 0},
    {"clrscr", t_clrscr, NULL, 0},
    {"clreol", t_clreol, NULL, 0},
    {"delline", t_delline, NULL, 0},
    {"textcolor/textbackground", t_colors, NULL, 0},
    {"window", t_window, NULL, 0},
    {"getch", t_getch, "a", 400},
    {"getche echoes", t_getche, "b", 400},
    {"kbhit false when idle", t_kbhit_false, NULL, 0},
    {"kbhit true with pending key", t_kbhit_true, "z", 200},
    {"cgets", t_cgets, "hello\n", 400},
    {"cscanf", t_cscanf, "12 abc\n", 400},
};

static int run(const struct test *t, char *msg, size_t msgsz) {
    int p[2], master, status = 0;
    struct winsize ws = {24, 80, 0, 0};
    msg[0] = 0;
    if (pipe(p) < 0) return -1;
    pid_t pid = forkpty(&master, NULL, NULL, &ws);
    if (pid < 0) return -1;
    if (pid == 0) {
        setenv("TERM", "xterm", 1);
        close(p[0]);
        g_msgfd = p[1];
        int rc = t->fn();
        if (screen_initialized) endwin();
        _exit(rc ? 1 : 0);
    }
    close(p[1]);
    if (t->input) {
        usleep(t->input_delay_ms * 1000);
        if (write(master, t->input, strlen(t->input)) < 0) {}
    }
    int timed_out = 0;
    for (;;) {
        struct pollfd pf = {master, POLLIN, 0};
        int r = poll(&pf, 1, 5000);
        if (r == 0) { timed_out = 1; kill(pid, SIGKILL); break; }
        char junk[4096];
        if (r < 0 || read(master, junk, sizeof junk) <= 0) break;
    }
    ssize_t n = read(p[0], msg, msgsz - 1);
    msg[n > 0 ? n : 0] = 0;
    close(p[0]);
    close(master);
    waitpid(pid, &status, 0);
    if (timed_out) { snprintf(msg, msgsz, "timed out\n"); return 1; }
    return !(WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

int main(void) {
    int failed = 0, total = (int)(sizeof tests / sizeof tests[0]);
    char msg[512];
    for (int i = 0; i < total; i++) {
        int bad = run(&tests[i], msg, sizeof msg);
        printf("%s %s\n", bad ? "FAIL" : "ok  ", tests[i].name);
        if (bad) { failed++; if (msg[0]) printf("     %s", msg); }
    }
    printf("%d/%d passed\n", total - failed, total);
    return failed ? 1 : 0;
}
