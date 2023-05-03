#pragma once

class Control {
public:
	static Control control;

	QSet<Qt::Key> pressedKeys;
};