#include "stdafx.h"

void ParallelLight::setDirection(const vec3& direction)
{
    DirectionalLight::setDirection(direction);

    vec3 position = Data::getInstance().getZoomTarget()
        - direction * Data::getInstance().getZoomDistance() / 1.7f;

    shadow.setDirectionAndPosition(direction, position);
}