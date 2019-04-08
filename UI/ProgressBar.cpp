/*
 * ProgressBar.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
	value = 0;
	maximum = 100;
	parent = nullptr;
}

void ProgressBar::setMaximum(int max)
{
	maximum = max;
}

void ProgressBar::setLocation(int x, int y)
{
	area.x = x;
	area.y = y;
}

void ProgressBar::setSize(int width, int height)
{
	area.width = width;
	area.height = height;
}

void ProgressBar::setArea(Rectangle& rect)
{
	area = rect;
}

void ProgressBar::setParent(WINDOW* parent)
{
	this->parent = parent;
}

void ProgressBar::clearDrawing()
{
	// clear area
	mvwhline(parent, area.y, area.x, ' ', area.width);
}

void ProgressBar::setValue(int newValue)
{
	value = newValue;
}

void ProgressBar::incrementValue(int amount)
{
	value += amount;
}

void ProgressBar::draw()
{
	clearDrawing();

	mvwaddch(parent, area.y, area.x, '[');
	mvwaddch(parent, area.y, area.x + 1 + area.width - 5, ']');

	// print the filling of the progress bar

	float percentage = (float) value / maximum;
	int mappedValue = (area.width - 4) * percentage;
	mvwhline(parent, area.y, area.x + 1, '=', mappedValue);

	// display percentage string
	mvwprintw(parent, area.y, area.x + 1 + area.width - 3, "%.0f%%", (100 * percentage));
	refresh();
}
