/* 
 * File:   conio.c
 * Author: nowres RAFID <nowres.rafed@gmail.com>
 *
 * Created on September 18, 2012, 10:39 PM
 */

#include "conio.h"
#include <string.h>
#include <stdarg.h>

#define max(x,y) ( (x)<(y)?(y):(x) )
#define min(x,y) ( (x)<(y)?(x):(y) )

#define _cu_get_color_pair(f,b) ((f)|((b)<<3))

int screen_initialized;
WINDOW *_working_window;

int _cu_fore_color;
int _cu_bkgnd_color;

void init_screen(void) {
    if (!screen_initialized) {
        stdscr = initscr();
        //cbreak();
        raw();
        noecho();
        keypad(stdscr, TRUE);
        _cu_bkgnd_color = BLACK;
        _cu_fore_color = WHITE;
        if (has_colors()) {
            start_color();
            init_pair(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color), _cu_fore_color, _cu_bkgnd_color);
            attron(COLOR_PAIR(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color)));
            init_pair(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color), _cu_fore_color, _cu_bkgnd_color);
            attron(COLOR_PAIR(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color)));
        }

        clear();
        fflush(stdin);
        refresh();
        _working_window = stdscr;
        screen_initialized = 1;
    }
}

void refresh_screen(void) {
    wrefresh(_working_window);
}

void gotoxy(int x, int y) {
    init_screen();
    wmove(_working_window, y, x);
    refresh_screen();
}

void clrscr(void) {
    init_screen();
    wclear(_working_window);
    wbkgd(_working_window, COLOR_PAIR(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color)));
    wrefresh(_working_window);
}

void clreol(void) {
    init_screen();
    wclrtoeol(_working_window);
    wrefresh(_working_window);
}

int kbhit(void) {
    int temp;
    if (!screen_initialized) init_screen();

    wtimeout(stdscr, 0);
    if ((temp = wgetch(stdscr)) != EOF) {
        ungetch(temp);
        nodelay(stdscr, FALSE);
        return TRUE;
    } else {
        nodelay(stdscr, FALSE);
        return FALSE;
    }
}

void textbackground(int color) {
    init_screen();
    _cu_bkgnd_color = color;
    init_pair(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color), _cu_fore_color, _cu_bkgnd_color);
    attron(COLOR_PAIR(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color)));
}

void textcolor(int color) {
    init_screen();
    _cu_fore_color = color;
    init_pair(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color), _cu_fore_color, _cu_bkgnd_color);
    attron(COLOR_PAIR(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color)));
}

void delline(void) {
    wmove(_working_window, getcury(_working_window), 0);
    wclrtoeol(_working_window);
}

void window(int left, int top, int right, int bottom) {
    init_screen();
    int height, width;
    if (_working_window != stdscr)
        delwin(_working_window);
    if ((height = bottom - top) > 0 && (width = right - left) > 0 && left >= 0 && top >= 0)
        _working_window = newwin(height, width, top, left);
    else
        _working_window = stdscr;
}

int getch(void) {
    init_screen();
    return wgetch(_working_window);
}

int getche(void) {
    int temp;
    init_screen();
    waddch(_working_window, temp = getch());
    wrefresh(_working_window);
    return temp;
}

int wherex(void) {
    init_screen();
    return getcurx(_working_window);
}

int wherey(void) {
    init_screen();
    return getcury(_working_window);
}

int cputs(char* buf) {
    int ret;
    init_screen();
    ret = wprintw(_working_window, "%s\n", buf);
    wrefresh(_working_window);
    return ret == ERR;
}

char* cgets(char* buf) {
    init_screen();
    echo();
    wgetnstr(_working_window, buf + 2, buf[0]);
    wrefresh(_working_window);
    noecho();
    buf[1] = strlen(buf + 2);
    return buf + 2;
}

int cprintf(const char* fmt, ...) {
    va_list v;
    
    init_screen();
    va_start(v, fmt);
    vwprintw(_working_window, fmt, v);
    va_end(v);
    wrefresh(_working_window);
    
    return strlen(fmt);
}

int cscanf(const char* fmt, ...) {
    va_list v;
    int ret;
    
    init_screen();
    echo();
    va_start(v, fmt);
    ret = vwscanw(_working_window, fmt, v);
    va_end(v);
    wrefresh(_working_window);
    noecho();
    
    return ret == ERR ? 0 : ret;
}
