#pragma once

#ifndef ORTHOGRAPHIC_SHADOW_H
#define ORTHOGRAPHIC_SHADOW_H

class OrthographicShadow : public UnidirectionalShadow {
public:
	inline void setWidth(const float& width) {
		this->width = width;
		computeProjection();
	}

	inline void setHeight(const float& height) {
		this->height = height;
		computeProjection();
	}

	inline void setWidthAndHeight(const float& width, const float& height) {
		this->width = width;
		this->height = height;
		computeProjection();
	}

	inline void setHeightAndWidth(const float& height, const float& width) {
		setWidthAndHeight(width, height);
	}

	inline void setNear(const float& zNear) {
		this->zNear = zNear;
		computeProjection();
	}

	inline void setFar(const float& zFar) {
		this->zFar = zFar;
		computeProjection();
	}

	inline void setNearAndFar(const float& zNear, const float& zFar) {
		this->zNear = zNear;
		this->zFar = zFar;
		computeProjection();
	}

	inline const float& getWidth() const { return width; }
	inline const float& getHeight() const { return height; }
	inline const float& getNear() const { return zNear; }
	inline const float& getFar() const { return zFar; }

protected:
	inline virtual void computeProjection() override {
		projection = glm::ortho(
			-width / 2.0f, 
			width / 2.0f, 
			-height / 2.0f, 
			height / 2.0f,
			zNear,
			zFar
		);

		UnidirectionalShadow::computeProjection();
	}

	float width = 1000.0f;
	float height = width;
	float zNear = 1e-2f;
	float zFar = 1e3f;
};

#endif