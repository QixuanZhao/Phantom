#pragma once

class DisplayController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QColor backgroundColour READ backgroundColour WRITE setBackgroundColour)
	Q_PROPERTY(qreal gamma READ gamma WRITE setGamma)
	Q_PROPERTY(bool toneMappingEnabled READ isToneMappingEnabled WRITE setToneMappingEnabled)
	Q_PROPERTY(qint8 blurPassCount READ blurPassCount WRITE setBlurPassCount)
	Q_PROPERTY(qint8 blurAlgorithm READ blurAlgorithm WRITE setBlurAlgorithm)
	Q_PROPERTY(ToneMappingMethod toneMappingMethod READ toneMappingMethod WRITE setToneMappingMethod)
	Q_PROPERTY(qreal exposure READ exposure WRITE setExposure)
	QML_ELEMENT
public:
	enum class ToneMappingMethod {
		Reinhard,
		Exposure,
		Unknown
	};

	Q_ENUM(ToneMappingMethod)

	inline QColor backgroundColour() const {
		vec3 colour = Display::display.backgroundColour * 255.0f;
		return QColor(colour.r, colour.g, colour.b);
	}
	
	inline void setBackgroundColour(const QColor& colour) {
		Display::display.backgroundColour = vec3(colour.redF(), colour.greenF(), colour.blueF());
	}

	inline qreal gamma() const { return Display::display.gamma; }
	inline void setGamma(qreal gamma) { Display::display.gamma = gamma; }

	inline bool isToneMappingEnabled() const { return Display::display.toneMappingEnabled; }
	inline void setToneMappingEnabled(bool enabled) { Display::display.toneMappingEnabled = enabled; }

	inline qint8 blurPassCount() const { return Display::display.blurPassCount; }
	inline void setBlurPassCount(qint8 count) { Display::display.blurPassCount = count; }

	inline qint8 blurAlgorithm() const { return (qint8) Display::display.blurAlgorithm; }
	inline void setBlurAlgorithm(qint8 algorithm) { Display::display.blurAlgorithm = (GLConfiguration::BlurAlgorithm) algorithm; }

	inline ToneMappingMethod toneMappingMethod() const {
		switch (Display::display.toneMappingMethod) {
		case Display::ToneMappingMethod::Reinhard:
			return ToneMappingMethod::Reinhard;
		case Display::ToneMappingMethod::Exposure:
			return ToneMappingMethod::Exposure;
		default: 
			return ToneMappingMethod::Unknown;
		}
	}

	inline void setToneMappingMethod(ToneMappingMethod method) {
		switch (method) {
		case ToneMappingMethod::Reinhard:
			Display::display.toneMappingMethod = Display::ToneMappingMethod::Reinhard;
			break;
		case ToneMappingMethod::Exposure:
			Display::display.toneMappingMethod = Display::ToneMappingMethod::Exposure;
			break;
		}
	}

	inline qreal exposure() const { return Display::display.exposure; }
	inline void setExposure(qreal exposure) { Display::display.exposure = exposure; }
};
