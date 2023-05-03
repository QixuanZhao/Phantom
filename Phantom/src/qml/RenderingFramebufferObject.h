#pragma once

class RenderingFramebufferObject : public QQuickFramebufferObject
{
	Q_OBJECT
	Q_PROPERTY(qint32 fps READ getFps NOTIFY fpsChanged)
	Q_PROPERTY(QSizeF viewportSize READ getViewportSize WRITE setViewportSize)
	QML_ELEMENT
public:
	Renderer* createRenderer() const override;

	inline qint32 getFps() const { return fps; }
	inline QSizeF getViewportSize() const { return QSizeF(width(), height()); }

public slots:
	void setFps(qint32 fps);
	Q_INVOKABLE void setViewportSize(const QSizeF& size);

signals:
	void fpsChanged();
	void viewportResized(const QSizeF&);

protected:
	qint32 fps;

	friend class MainRenderer;
};
