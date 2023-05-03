#pragma once

#ifndef UNIDIRECTIONAL_SHADOW_H
#define UNIDIRECTIONAL_SHADOW_H

class UnidirectionalShadow : public Shadow {
public:
    inline const vec3& getDirection() const { return direction; }
    inline const vec3& getPosition() const { return position; }

    inline void setDirection(const vec3& direction) {
        this->direction = direction;
        computeView();
    }

    inline void setPosition(const vec3& position) {
        this->position = position;
        computeView();
    }

    inline void setDirectionAndPosition(const vec3& direction, const vec3& position) {
        this->direction = direction;
        this->position = position;
        computeView();
    }

    inline void setPositionAndDirection(const vec3& position, const vec3& direction) {
        setDirectionAndPosition(direction, position);
    }

    inline const mat4& getView() const { return view; }
    inline const mat4& getProjection() const { return projection; }
    inline const mat4& getPV() const { return pv; }

	virtual void setupGL() override;
    virtual void updateDepth(ShaderProgram* sp) override;
    virtual void updateShadow(ShaderProgram* sp) override;
protected:
    inline void computeView()
    {   
        view = glm::lookAt(position, position + direction, vec3(0.0f, 0.0f, 1.0f));
        pv = projection * view;

        requireUpdate();
    }

    inline virtual void computeProjection() = 0 {
        pv = projection * view;
        requireUpdate();
    }
    
    vec3 direction = vec3(1.0f, 0.0f, 0.0f);
    vec3 position = vec3(0.0f);

    mat4 view = mat4(1.0f);
    mat4 projection = mat4(1.0f);

    mat4 pv = mat4(1.0f);
};

#endif