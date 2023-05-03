#include "stdafx.h"

QQuickFramebufferObject::Renderer* RenderingFramebufferObject::createRenderer() const
{
	MainRenderer* renderer = new MainRenderer();
	connect(renderer, SIGNAL(fpsCalculated(qint32)),
		this, SLOT(setFps(qint32)));
	
	connect(this, SIGNAL(viewportResized(QSizeF)), renderer,
		SLOT(setViewportSize(QSizeF)));

	renderer->setViewportSize(QSizeF(width(), height()));
	renderer->setWindow(window());

	return renderer;
}

void RenderingFramebufferObject::setFps(qint32 fps)
{
	if (this->fps == fps) return;

	this->fps = fps;
	emit fpsChanged();
}

void RenderingFramebufferObject::setViewportSize(const QSizeF& size)
{
	emit viewportResized(size);
}
