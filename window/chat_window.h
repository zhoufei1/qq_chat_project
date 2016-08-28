
#pragma once

#include <iostream>
#include <string>
#include <ncurses.h>
#include <string.h>
using namespace std;

class chat_window
{
	public:
		chat_window():header(NULL),output(NULL),list(NULL),input(NULL)
		{
			init();
		}
		~chat_window()
		{
			delwin(header);
			delwin(output);
			delwin(list);
			delwin(input);
			endwin();
		}
	public:
		void init()
		{
			initscr();
		}
		void draw_header()
		{
			int height=LINES/5;
			int wide=COLS;
			int y=0;
			iny x=0;
			header=create_win(height,wide,y,x);
		}
		void draw_output()
		{
			int height=(LINES*3)/5;
			int wide=(COLS*3)/4;

			int y=LINES/5;
			int x=0;
			output=create_win(height,wide,y,x);
		}
		void draw_list()
		{
			int height=LINES*3/5;
			int wide=COLS/4;
			int y=LINES/5;
			int x=COLS*3/4;
			list=create_win(height,wide,y,x);
		}
		void draw_input()
		{
			int height=LINES/5;
			int wide=COLS;
			int y=LINES*4/5;
			int x=0;
			input=create_win(height,wide,y,x);
		}
	public:
		static void win_refresh(WINDOW *win)
		{
			box(win,0,0);
			wrefresh(win);
		}
		static void put_str_to_win(WINDOW *win,int x,int y,string &str)
		{
			mvwaddstr(win,y,x,str.c_str());
		}
		static void get_str_to_win(WINDOW *win,string &msg)
		{
			const int BUF_SIZE=1024;
			char buf[BUF_SIZE];
			memset(buf,'\0',BUF_SIZE);

			wgetnstr(win,buf,BUF_SIZE);
			msg=buf;
		}
		static void clear_win_line(WINDOW *win,int begin,int num)
		{
			while(num-- >0)
			{
				wmove(win,begin++,0);
				wclrtoeol(win);
			}
		}

	public:
		WINDOW* get_header()
		{
			return header;
		}
		WINDOW* get_output()
		{
			return output;
		}
		WINDOW* get_list()
		{
			return list;
		}
		WINDOW* get_input()
		{
			return input;
		}
	private:
		WINDOW* create_win(int height,int wide,int x,int y)
		{
			return newwin(height,wide,y,x);
		}
	private:
		WINDOW *header;
		WINDOW *output;
		WINDOW *list;
		WINDOW *input;
};
