#include "stdafx.h"

void ParallelLight::setDirection(const vec3& direction)
{
    DirectionalLight::setDirection(direction);

    vec3 position = Data::instance().getZoomTarget()
        - direction * Data::instance().getZoomDistance() / 1.7f;

    shadow.setDirectionAndPosition(direction, position);
}