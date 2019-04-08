/*
 * ProgressBar.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef UI_PROGRESSBAR_H_
#define UI_PROGRESSBAR_H_

#include <ncurses.h>
#include "ui_datatypes.h"

class ProgressBar
{
protected:
	Rectangle area;
	WINDOW* parent;
	int value;
	int maximum;
public:
	ProgressBar();

	void setMaximum(int max);

	void setLocation(int x, int y);

	void setSize(int width, int height);

	void setArea(Rectangle& rect);
	void setParent(WINDOW* parent);

	void clearDrawing();

	void setValue(int newValue);

	void incrementValue(int amount);

	void draw();

};

#endif /* UI_PROGRESSBAR_H_ */
