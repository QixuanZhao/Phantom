#pragma once

#ifndef PERSPECTIVE_SHADOW_H
#define PERSPECTIVE_SHADOW_H

class PerspectiveShadow : public UnidirectionalShadow {
public:
	inline void setOuterCutoff(const float& cutoff) {
		outerCutoff = cutoff;
		computeProjection();
	}

	inline const float& getOuterCutoff() const { return outerCutoff; }

protected:
	inline virtual void computeProjection() override {
		float tangentTheta = glm::tan(outerCutoff);
		float denominator = glm::sqrt(1 + 2 * tangentTheta * tangentTheta);
		float fov = 2.0f * glm::acos(glm::cos(outerCutoff) / denominator);
		projection = glm::perspective(
			fov, 1.0f, NEAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE
		);

		UnidirectionalShadow::computeProjection();
	}

	float outerCutoff = 0.5f;

	const float NEAR_PLANE_DISTANCE = 1e-3f;
	const float FAR_PLANE_DISTANCE = 4e3f;
};

#endif