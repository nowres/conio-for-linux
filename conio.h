/* 
 * File:   conio.h
 * Author: nowres RAFED <nowres.rafed@gmail.com>
 *
 * Created on September 18, 2012, 10:30 PM
 */

#ifndef CONIO_H
#define	CONIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <ncurses.h>

#define BLUE COLOR_BLUE
#define RED COLOR_RED
#define WHITE COLOR_WHITE
#define BLACK COLOR_BLACK
#define MAGENTA COLOR_MAGENTA
#define CYAN COLOR_CYAN
#define GREEN COLOR_GREEN
#define YELLOW COLOR_YELLOW
#define LIGHTBLUE COLOR_BLUE
#define LIGHTRED COLOR_RED
#define LIGHTGREEN COLOR_GREEN


int screen_initialized;
WINDOW *working_window;

void init_screen(void);

void gotoxy(int x,int y);
void clrscr(void);
void clreol(void);
int kbhit(void);
void textbackground(int color);
void textcolor(int color);
void delline(void);
void window(int left, int top, int right, int bottom);

#define printf(format, args...) (init_screen(),wprintw(working_window,format , ##args),wrefresh(working_window))
#define cprintf printf

#define scanf(format, args...) (init_screen(),echo(),wscanw(working_window,format , ##args),wrefresh(working_window),noecho())
#define cscanf scanf

#define cputs(str) (init_screen(),wprintw(working_window,"%s\n" , str),wrefresh(working_window))

#define cgets(str) (init_screen(),echo(),wgetstr(working_window,str),wrefresh(working_window),noecho())

#undef getch
#define getch() (init_screen(),wgetch(working_window))
unsigned int _temp_char;
#define getche() (init_screen(),waddch(working_window,_temp_char=getch()),wrefresh(working_window),_temp_char)

#define wherex() (init_screen(),getcurx(working_window))
#define wherey() (init_screen(),getcury(working_window))

#define _cu_get_color_pair(f,b) ((f)|((b)<<3))

int _cu_fore_color;
int _cu_bkgnd_color;


#ifdef	__cplusplus
}
#endif

#endif	/* CONIO_H */

