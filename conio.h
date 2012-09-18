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

WINDOW *_conio_working_window;

void init_screen(void);

void gotoxy(int x,int y);
void clrscr(void);
void clreol(void);
int kbhit(void);
void textbackground(int color);
void textcolor(int color);
void delline(void);
void window(int left, int top, int right, int bottom);

#define printf(format, args...) (init_screen(),wprintw(_conio_working_window,format , ##args),wrefresh(_conio_working_window))
#define cprintf printf

#define scanf(format, args...) (init_screen(),echo(),wscanw(_conio_working_window,format , ##args),wrefresh(_conio_working_window),noecho())
#define cscanf scanf

#define cputs(str) (init_screen(),wprintw(_conio_working_window,"%s\n" , str),wrefresh(_conio_working_window))

#define cgets(str) (init_screen(),echo(),wgetstr(_conio_working_window,str),wrefresh(_conio_working_window),noecho())

#undef getch
#define getch() (init_screen(),wgetch(_conio_working_window))
unsigned int _conio_temp_char;
#define getche() (init_screen(),waddch(_conio_working_window,_conio_temp_char=getch()),wrefresh(_conio_working_window),_conio_temp_char)

#define wherex() (init_screen(),getcurx(_conio_working_window))
#define wherey() (init_screen(),getcury(_conio_working_window))


#ifdef	__cplusplus
}
#endif

#endif	/* CONIO_H */

