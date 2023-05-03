#pragma once

class ShadowController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(bool shadowEnabled READ shadowEnabled WRITE setShadowEnabled)
	Q_PROPERTY(bool linearFilterEnabled READ linearFilterEnabled WRITE setLinearFilterEnabled)
	Q_PROPERTY(bool samplingEnabled READ samplingEnabled WRITE setSamplingEnabled)
	Q_PROPERTY(int samplingMethod READ samplingMethod WRITE setSamplingMethod)
	Q_PROPERTY(bool samplingRounded READ samplingRounded WRITE setSamplingRounded)
	Q_PROPERTY(quint8 filterSize READ filterSize WRITE setFilterSize)
	Q_PROPERTY(bool samplingStratified READ samplingStratified WRITE setSamplingStratified)
	Q_PROPERTY(qreal samplingSpread READ samplingSpread WRITE setSamplingSpread)
	Q_PROPERTY(qint8 blurPassCount READ blurPassCount WRITE setBlurPassCount)
	Q_PROPERTY(qint8 blurAlgorithm READ blurAlgorithm WRITE setBlurAlgorithm)
	QML_ELEMENT

public:
	inline bool shadowEnabled() const { return Lighting::lighting.shadowSettings.enabled; }
	inline bool linearFilterEnabled() const { return Lighting::lighting.shadowSettings.linearFilterEnabled; }
	inline bool samplingEnabled() const { return Lighting::lighting.shadowSettings.samplingEnabled; }
	inline int samplingMethod() const { return int(Lighting::lighting.shadowSettings.samplingMethod); }
	inline bool samplingRounded() const { return Lighting::lighting.shadowSettings.samplingRounded; }
	inline quint8 filterSize() const { return Lighting::lighting.shadowSettings.filterSize; }
	inline bool samplingStratified() const { return Lighting::lighting.shadowSettings.samplingStratified; }
	inline qreal samplingSpread() const { return Lighting::lighting.shadowSettings.samplingSpread; }
	inline qint8 blurPassCount() const { return Lighting::lighting.shadowSettings.blurPassCount; }
	inline qint8 blurAlgorithm() const { return (int) Lighting::lighting.shadowSettings.blurAlgorithm; }

	inline void setShadowEnabled(const bool& value) { Lighting::lighting.shadowSettings.enabled = value; }
	inline void setLinearFilterEnabled(const bool& value) { 
		Lighting::lighting.shadowSettings.linearFilterEnabled = value;
		for (ParallelLight& pl : Lighting::lighting.parallelLights)
			pl.getShadow().setLinearFilterEnabled(value);
		for (ShadowCastingPointLight& sl : Lighting::lighting.pointLights) 
			sl.getShadow().setLinearFilterEnabled(value);
	}
	inline void setSamplingEnabled(const bool& value) { Lighting::lighting.shadowSettings.samplingEnabled = value; }
	inline void setSamplingMethod(const int& value) { Lighting::lighting.shadowSettings.samplingMethod = Shadow::SamplingMethod(value); }
	inline void setSamplingRounded(const bool& value) { Lighting::lighting.shadowSettings.samplingRounded = value; }
	inline void setFilterSize(const quint8& value) { Lighting::lighting.shadowSettings.filterSize = value; }
	inline void setSamplingStratified(const bool& value) { Lighting::lighting.shadowSettings.samplingStratified = value; }
	inline void setSamplingSpread(const qreal& value) { Lighting::lighting.shadowSettings.samplingSpread = value; }
	inline void setBlurPassCount(const qint8& value) const { Lighting::lighting.shadowSettings.blurPassCount = value; }
	inline void setBlurAlgorithm(const qint8& value) const { Lighting::lighting.shadowSettings.blurAlgorithm = (GLConfiguration::BlurAlgorithm) value; }
};
