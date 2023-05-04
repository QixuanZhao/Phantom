#pragma once

class Control {
private:
	static Control * control;

protected:
	Control() : cursor(Qt::BlankCursor) { }

public:
	static inline Control& instance() {
		if (!control) control = new Control();
		return *control;
	}

	QSet<Qt::Key> pressedKeys;
	QSet<Qt::MouseButton> pressedButtons;

	QCursor cursor;
	QPoint center;
	bool catchMouse = false;
};