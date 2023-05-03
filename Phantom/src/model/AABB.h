#pragma once

#ifndef AABB_H
#define AABB_H

class AABB
{
public:
    /// minimum coordinate <br/> 最小坐标
    vec3 min = vec3(0.0f);
    /// maximum coordinate <br/> 最大坐标
    vec3 max = vec3(0.0f);
};

#endif