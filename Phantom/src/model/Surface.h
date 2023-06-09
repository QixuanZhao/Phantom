﻿#include "stdafx.h"
#pragma once

#ifndef SURFACE_H
#define SURFACE_H

class Display;
class ObserverCamera;
class Object3D;

class Surface : protected GLF
{
public:
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texture;
    };

    struct Texture {
        GLuint id;
        string type;
        string path;
    };

    struct Face {
        vector<int> vertexIndices;
        vector<int> vertexNormalIndices;
        vector<int> vertexTextureIndices;
    };

    Surface(const QString& path);
    Surface(const vector<vector<vec3>>& faces);
    virtual ~Surface();

    void setupGL();

    void configureShaderProgram(ShaderProgram& sp);

    void draw(ShaderProgram& sp);
    void draw(ShaderProgram& sp, int id);

    inline float getMetallic() const { return metallic; }
    inline void setMetallic(float metallic) { this->metallic = metallic; }

    inline float getRoughness() const { return roughness; }
    inline void setRoughness(float roughness) { this->roughness = roughness; }

    inline vec3 getK() const { return K; }
    inline void setK(const vec3& K) { this->K = K; }

    inline float getShininess() const { return shininess; }
    inline void setShininess(float shininess) { this->shininess = shininess; }

    inline const QString& getDirectory() const { return directory; }
    inline const QString& getName() const { return name; }
    inline void setName(const QString& name) { this->name = name; }

    inline const vector<vec3> & getVertices() const { return vertices; }
    inline const vector<Face*> & getFaces() const { return faces; }

    inline const AABB& getAABB() const { return aabb; }
    inline vec3 getZoomTarget() const { return translation + (aabb.max + aabb.min) / 2.0f; }
    inline float getZoomDistance() const { return glm::length(aabb.max - aabb.min); }

    inline void setPolygonMode(GLenum polygonMode) { this->polygonMode = polygonMode; }
    inline GLenum getPolygonMode() const { return polygonMode; }

    inline bool isVisible() const { return visible; }
    inline void setVisible(bool visible) { this->visible = visible; }

    inline const vec3& getColour() const { return colour; }
    inline void setColour(const vec3& colour) { this->colour = colour; }

    inline const vec3& getTranslation() const { return translation; }
    inline void setTranslation(const vec3& translate) { this->translation = translate; }

    inline const vec3& getScale() const { return scale; }
    inline void setScale(const vec3& scale) { this->scale = scale; }

    inline const quat& getRotation() const { return rotation; }
    inline void setRotation(const quat& rotation) { this->rotation = rotation; }

    inline const vec3& getCenter() const { return center; }

    void zoom(ObserverCamera& camera) const;
    void zoom() const;

    inline bool isAvailable() const { return available; }
    inline void setAvailable(bool available) { this->available = available; }

    inline void setObject(const Object3D* object) {
        this->object = object;
    }
    inline const Object3D* getObject() const { return object; }

protected:
    GLuint vertexArray;
    GLuint vertexBuffer;

    vector<vec3> vertices;
    vector<vec3> vertexNormals;
    vector<vec2> vertexTextures;

    vector<Face*> faces;
    vector<Vertex> vertexData; 

    AABB aabb;

    GLenum polygonMode = GL_FILL; 

    bool visible = true;

    QString directory;
    QString name;

    vector<Texture> textures;

    float metallic = 0.5f;
    float roughness = 0.5f;
    vec3 K = vec3(0.0f);

    float shininess = 64.0f; 

    vec3 translation = vec3(0.0f);
    vec3 scale = vec3(1.0f);
    quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
    vec3 center = vec3(0.0f);

    vec3 colour = vec3(1.0f);
    bool showBorder = false;

    const Object3D* object = nullptr;

    void loadModel(const QString& path);
    void loadModel(const vector<vector<vec3>>& faces);
    void parseData();

private:
    bool available = false;
};

#endif