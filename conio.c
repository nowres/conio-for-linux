/* 
 * File:   conio.c
 * Author: nowres RAFID <nowres.rafed@gmail.com>
 *
 * Created on September 18, 2012, 10:39 PM
 */

#include "conio.h"

#define max(x,y) ( (x)<(y)?(y):(x) )
#define min(x,y) ( (x)<(y)?(x):(y) )

#define _cu_get_color_pair(f,b) ((f)|((b)<<3))

int screen_initialized;

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
        _conio_working_window = stdscr;
        screen_initialized = 1;
    }
}

void refresh_screen(void) {
    wrefresh(_conio_working_window);
}

void gotoxy(int x, int y) {
    init_screen();
    wmove(_conio_working_window, y, x);
    refresh_screen();
}

void clrscr(void) {
    init_screen();
    wclear(_conio_working_window);
    wbkgd(_conio_working_window, COLOR_PAIR(_cu_get_color_pair(_cu_fore_color, _cu_bkgnd_color)));
    wrefresh(_conio_working_window);
}

void clreol(void) {
    init_screen();
    wclrtoeol(_conio_working_window);
    wrefresh(_conio_working_window);
}

int kbhit(void) {
    if (!screen_initialized) init_screen();

    wtimeout(stdscr, 0);
    if ((_conio_temp_char = wgetch(stdscr)) != EOF) {
        ungetch(_conio_temp_char);
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
    wmove(_conio_working_window, getcury(_conio_working_window), 0);
    wclrtoeol(_conio_working_window);
}

void window(int left, int top, int right, int bottom) {
    init_screen();
    int height, width;
    if (_conio_working_window != stdscr)
        delwin(_conio_working_window);
    if ((height = bottom - top) > 0 && (width = right - left) > 0 && left >= 0 && top >= 0)
        _conio_working_window = newwin(height, width, top, left);
    else
        _conio_working_window = stdscr;
}