#pragma once

class GraphicsController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(bool deferredRendering READ deferredRendering WRITE setDeferredRenderingEnabled NOTIFY deferredRenderingToggled)
	Q_PROPERTY(bool pbrEnabled READ pbrEnabled WRITE setPBREnabled NOTIFY pbrToggled)
	QML_ELEMENT
signals:
	void deferredRenderingToggled(bool enabled);
	void pbrToggled(bool enabled);
public:
	inline bool deferredRendering() const { return GLConfiguration::configuration.deferredRendering; }
	inline void setDeferredRenderingEnabled(const bool& enabled) { 
		GLConfiguration::configuration.deferredRendering = enabled; 
		emit deferredRenderingToggled(enabled);
	}

	inline bool pbrEnabled() const { return GLConfiguration::configuration.pbrEnabled; }
	inline void setPBREnabled(const bool& enabled) {
		GLConfiguration::configuration.pbrEnabled = enabled;
		emit pbrToggled(enabled);
	}
};
