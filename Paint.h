/*
 * Change msg color in CLI
 * 
 * by wareric@163.com
 * 2018-08-13
 */
#ifndef PAINT_H_
#define PAINT_H_

#include <string>
using std::string;

enum color {highlight = 1, black = 30, red = 31, yellow = 33, blue = 34, purple = 35, deepgreen = 36, white = 37};

class Paint{
	public:
		Paint(enum color txtclr): text(txtclr), start("\033["+std::to_string(txtclr)+"m"), end("\033[0m"){}
		Paint(const Paint &pt);
		Paint& operator=(const Paint &pt);

		string paint(const string &msg);
		
		bool operator==(const Paint &pt);
		bool operator!=(const Paint &pt);
	private:
		enum color text;
		string start;
		string end;
};
#endif
