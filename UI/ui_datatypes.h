/*
 * ui_datatypes.h
 *
 *  Created on: Apr 9, 2019
 *      Author: tim
 */

#ifndef UI_UI_DATATYPES_H_
#define UI_UI_DATATYPES_H_

class Point
{
public:
	int x;
	int y;
};

class Rectangle
{
public:
	int x;
	int y;
	int width;
	int height;

	static Rectangle fromPoints(Point a, Point b)
	{
		Rectangle rect;

		rect.x = a.x;
		rect.y = a.y;

		rect.width = b.x - a.x;
		rect.height = b.y - a.x;
		return rect;
	}

	void shrink(int amount)
	{
		x += amount;
		y += amount;
		width -= amount;
		height -= amount;
	}
};

#endif /* UI_UI_DATATYPES_H_ */
