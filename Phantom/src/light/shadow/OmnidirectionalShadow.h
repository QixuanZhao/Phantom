#pragma once

#ifndef OMNIDIRECTIONAL_SHADOW_H
#define OMNIDIRECTIONAL_SHADOW_H
class OmnidirectionalShadow : public Shadow {
public:
    const float NEAR_PLANE_DISTANCE = 1e-3f;
    const float FAR_PLANE_DISTANCE = 4e3f;

    virtual void setupGL() override;
    virtual void updateDepth(ShaderProgram* sp) override;
    virtual void updateShadow(ShaderProgram* sp) override;

    inline const vec3& getPosition() const { return position; }
    inline void setPosition(const vec3& position) 
    {
        this->position = position;
        computeView();
    }

    inline const mat4& getPV(int index) const { return pv[index]; }

protected:

    inline void computeView() {
        const static vec3 directions[6] = {
            vec3(1, 0 ,0), vec3(-1, 0, 0),
            vec3(0, 1, 0), vec3(0, -1, 0),
            vec3(0, 0, 1), vec3(0, 0, -1)
        };

        const static vec3 ups[6] = {
            vec3(0, -1, 0), vec3(0, -1, 0),
            vec3(0, 0, 1), vec3(0, 0, -1),
            vec3(0, -1, 0), vec3(0, -1, 0)
        };

        for (int i = 0; i < 6; i++) {
            pv[i] = projection * glm::lookAt(
                position,
                position + directions[i],
                ups[i]
            );
        }

        requireUpdate();
    }

    vec3 position = vec3(0.0f);

	const mat4 projection = glm::perspective(
        glm::radians(90.0f), 
        1.0f, 
        NEAR_PLANE_DISTANCE, 
        FAR_PLANE_DISTANCE
    );

    mat4 pv[6];
};

#endif