#pragma once

class LightingController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(AmbientLightController* ambientLight READ getAmbientLightController)
	Q_PROPERTY(DirectionalLightController* parallelLight READ getDirectionalLightController)
	Q_PROPERTY(PointLightController* pointLight READ getPointLightController)
	Q_PROPERTY(SpotlightController* spotlight READ getSpotlightController)
	Q_PROPERTY(bool blinnPhong READ blinnPhongEnabled WRITE setBlinnPhongEnabled)
	Q_PROPERTY(bool bilateralReflective READ bilateralReflective WRITE setBilateralReflective)
	Q_PROPERTY(bool bloom READ bloomEnabled WRITE setBloomEnabled)
	Q_PROPERTY(qint8 pickedPointLightIndex READ pickedPointLightIndex WRITE setPickedPointLightIndex 
		NOTIFY pointLightPicked)
	Q_PROPERTY(qint8 pickedSpotlightIndex READ pickedSpotlightIndex WRITE setPickedSpotlightIndex
		NOTIFY spotlightPicked)
	QML_ELEMENT

signals:
	void pointLightPicked(qint8 index);
	void spotlightPicked(qint8 index);

public:
	inline LightingController() {
		pointLightController.setLight(nullptr);
		directionalLightController.setLight(nullptr);
		ambientLightController.setLight(&Lighting::lighting.ambientLight);
		spotlightController.setLight(&Display::display.camera.getFlash());
	}

	inline AmbientLightController* getAmbientLightController() { return &ambientLightController; }
	inline DirectionalLightController* getDirectionalLightController() { return &directionalLightController; }
	inline PointLightController* getPointLightController() { return &pointLightController; }
	inline SpotlightController* getSpotlightController() { return &spotlightController; }

	inline bool blinnPhongEnabled() const { return Lighting::lighting.blinnPhong; }
	inline void setBlinnPhongEnabled(const bool& enabled) { Lighting::lighting.blinnPhong = enabled; }
	inline bool bilateralReflective() const { return Lighting::lighting.bilateralReflective; }
	inline void setBilateralReflective(const bool& reflective) { Lighting::lighting.bilateralReflective = reflective; }
	inline bool bloomEnabled() const { return Lighting::lighting.bloom; }
	inline void setBloomEnabled(const bool& enabled) { Lighting::lighting.bloom = enabled; }

	inline qint8 pickedPointLightIndex() const { return Lighting::lighting.pickedPointLightIndex; }
	inline qint8 pickedSpotlightIndex() const { return Lighting::lighting.pickedSpotlightIndex; }

	inline void setPickedPointLightIndex(const qint8& index) { 
		Lighting::lighting.pickedPointLightIndex = index; 
		emit pointLightPicked(index);
	}

	inline void setPickedSpotlightIndex(const qint8& index) { 
		Lighting::lighting.pickedSpotlightIndex = index; 
		emit spotlightPicked(index);
	}

	Q_INVOKABLE void setDirectionalLight(const qint8& index);
	Q_INVOKABLE void setPointLight(const qint8& index);
	Q_INVOKABLE void setSpotlight(const qint8& index);

protected:
	AmbientLightController ambientLightController;
	DirectionalLightController directionalLightController;
	PointLightController pointLightController;
	SpotlightController spotlightController;
};
