#pragma once

class AmbientLightController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QColor colour READ colour WRITE setColour)
	Q_PROPERTY(bool ssao READ ssao WRITE setSSAOEnabled)
	Q_PROPERTY(qint8 ssaoFilterSize READ ssaoFilterSize WRITE setSSAOFilterSize)
	Q_PROPERTY(bool ssaoRadiusFollowingCamera READ isSSAORadiusFollowingCamera WRITE setSSAORadiusFollowingCamera)
	Q_PROPERTY(qreal ssaoRadius READ ssaoRadius WRITE setSSAORadius)
	Q_PROPERTY(qint8 ssaoBlurPassCount READ ssaoBlurPassCount WRITE setSSAOBlurPassCount)
	Q_PROPERTY(qint8 ssaoBlurAlgorithm READ ssaoBlurAlgorithm WRITE setSSAOBlurAlgorithm)
	QML_ELEMENT
public:
	inline virtual void setLight(AmbientLight* light) {
		this->light = light;
	}

	inline QColor colour() const { 
		if (!light) return QColor();
		vec3 colour = light->getAmbient() * 255.0f; 
		return QColor(colour.r, colour.g, colour.b);
	}

	inline void setColour(const QColor color) {
		if (!light) return;
		light->setAmbient(vec3(color.redF(), color.greenF(), color.blueF()));
	}

	inline bool ssao() const { return Lighting::lighting.ssaoEnabled; }
	inline void setSSAOEnabled(const bool& enabled) { Lighting::lighting.ssaoEnabled = enabled; }

	inline qint8 ssaoFilterSize() const { return Lighting::lighting.ssaoFilterSize; }
	inline void setSSAOFilterSize(const qint8& size) { Lighting::lighting.ssaoFilterSize = size; }

	inline bool isSSAORadiusFollowingCamera() const { return Lighting::lighting.ssaoRadiusFollowingCamera; }
	inline void setSSAORadiusFollowingCamera(const bool& following) { Lighting::lighting.ssaoRadiusFollowingCamera = following; }

	inline qreal ssaoRadius() const { return Lighting::lighting.ssaoRadius; }
	inline void setSSAORadius(const qreal& radius) { Lighting::lighting.ssaoRadius = radius; }

	inline qint8 ssaoBlurPassCount() const { return Lighting::lighting.ssaoBlurPassCount; }
	inline void setSSAOBlurPassCount(const qint8& value) const { Lighting::lighting.ssaoBlurPassCount = value; }

	inline qint8 ssaoBlurAlgorithm() const { return (int)Lighting::lighting.ssaoBlurAlgorithm; }
	inline void setSSAOBlurAlgorithm(const qint8& value) const { Lighting::lighting.ssaoBlurAlgorithm = (GLConfiguration::BlurAlgorithm)value; }
protected:
	AmbientLight* light = nullptr;
};
