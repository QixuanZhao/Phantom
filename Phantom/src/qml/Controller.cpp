#include "stdafx.h"

void Controller::controlCursor(const QPoint& pos)
{
	Display::display.mouseCamera().setMouse(pos.x(), pos.y());
	cursor.setPos(pos);
}

void Controller::grabCursor()
{
	cursor.setShape(Qt::CursorShape::BlankCursor);
	QApplication::setOverrideCursor(cursor);
}

void Controller::releaseCursor()
{
	QApplication::restoreOverrideCursor();
}

void Controller::press(Qt::Key key)
{
	Control::control.pressedKeys.insert(key);
}

void Controller::release(Qt::Key key)
{
	Control::control.pressedKeys.remove(key);
}
