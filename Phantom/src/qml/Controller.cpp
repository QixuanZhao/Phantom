#include "stdafx.h"

void Controller::controlCursor(const QPoint& pos)
{
	getFPSCameraController()->setMouse(pos);
	QCursor::setPos(pos);
}

void Controller::grabCursor()
{
	Control::instance().catchMouse = true;
	QApplication::setOverrideCursor(Control::instance().cursor);
}

void Controller::releaseCursor()
{
	Control::instance().catchMouse = false;
	QApplication::restoreOverrideCursor();
}

void Controller::pressKey(Qt::Key key)
{
	Control::instance().pressedKeys.insert(key);
}

void Controller::releaseKey(Qt::Key key)
{
	Control::instance().pressedKeys.remove(key);
}

void Controller::pressButton(Qt::MouseButton button)
{
	Control::instance().pressedButtons.insert(button);
}

void Controller::releaseButton(Qt::MouseButton button)
{
	Control::instance().pressedButtons.remove(button);
}

void Controller::setMouseCenter(const QPoint& center)
{
	Control::instance().center = center;
}
