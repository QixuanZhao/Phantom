#pragma once

class AxesController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(bool show READ isVisible WRITE setVisible)
	QML_ELEMENT
public:
	inline bool isVisible() const { return axes.isVisible(); }
	inline void setVisible(bool visible) { axes.setVisibility(visible); }

protected:
	CoordinateSystem& axes = Display::display.axes;
};
