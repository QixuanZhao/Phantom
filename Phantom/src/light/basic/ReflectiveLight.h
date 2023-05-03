#pragma once

class ReflectiveLight {
protected:
	bool radiometry; 

	CompositeLight cl;
	RadiometricLight rl;

	QString name;

public:
	ReflectiveLight(
		const QString& name,
		const vec3& diffuse,
		const vec3& specular
	) : cl(diffuse, specular),
		rl(diffuse * 1.0f),
		radiometry(false)
	{ }

	ReflectiveLight(
		const QString& name = QStringLiteral("Light"),
		const vec3& flux = vec3(10.0f)
	) : cl(flux * 1e-1f, flux),
		rl(flux),
		radiometry(true)
	{ }

	inline const QString& getName() const { return name; }
	inline void setName(const QString& name) { this->name = name; }

	inline bool isRadiometric() const { return radiometry; }
	inline bool isComposite() const { return !radiometry; }

	inline void switchRadiometric() { radiometry = true; }
	inline void switchComposite() { radiometry = false; }

	inline void setRadiometric(bool enabled = true) { radiometry = enabled; }
	inline void setComposite(bool enabled = true) { radiometry = !enabled; }

	inline vec3 diffuse() const { return cl.getDiffuse(); }
	inline vec3 specular() const { return cl.getSpecular(); }
	inline vec3 flux() const { return rl.getRadiantFlux(); }
	inline vec3 colour() const { return rl.getColour(); }
	inline float power() const { return rl.getPower(); }

	inline void setDiffuse(const vec3& diffuse) { cl.setDiffuse(diffuse); }
	inline void setSpecular(const vec3& specular) { cl.setSpecular(specular); }

	inline void setColour(const vec3& colour) { rl.setColour(colour); }
	inline void setPower(const float& power) { rl.setPower(power); }

	inline void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
		if (radiometry) rl.configureShaderProgram(sp, varName);
		else cl.configureShaderProgram(sp, varName);
		sp->set(QStringLiteral("%1.radiometry").arg(varName), radiometry);
	}

	inline virtual bool operator==(const ReflectiveLight& B) const {
		return name == B.name;
	}
};