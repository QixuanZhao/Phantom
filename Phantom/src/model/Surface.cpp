#include "stdafx.h"

using std::istringstream;
using glm::max;
using glm::min;
using glm::cross;

void Surface::setupGL()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glNamedBufferData(vertexBuffer, vertexData.size() * sizeof(Vertex), &vertexData[0], GL_STATIC_DRAW);

    // world space coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // textures
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    available = true;
}

Surface::Surface(const QString& path)
{
    loadModel(path);
    parseData();
}

Surface::Surface(const vector<vector<vec3>>& faces)
{
    loadModel(faces);
    parseData();
}

Surface::~Surface() {
    for (Face* face : faces)
        delete face;

    faces.clear();

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
}

void Surface::zoom() const
{
    Display::display.camera.setDistance(getZoomDistance());
    Display::display.camera.setTarget(getZoomTarget());
}

void Surface::loadModel(const QString& path) {
    name = path.sliced(path.lastIndexOf('/') + 1);
    directory = path.first(path.lastIndexOf('/'));

    TextFile objFile(path);
    QString objContent = objFile.getContent();
    QTextStream obj(&objContent, QIODevice::ReadOnly);

    QString linetype;
    bool hasVt(false), hasVn(false);
    bool firstVertex(true);

    while (!obj.atEnd()) {
        obj >> linetype;
        if (linetype == "v") {
            double x, y, z;

            obj >> x >> z >> y;
            vertices.push_back(vec3(x, -y, z));
            if (firstVertex) {
                firstVertex = false;
                aabb.max = aabb.min = vertices.back();
            }
            else {
                aabb.max = MathUtility::emax(aabb.max, vertices.back());
                aabb.min = MathUtility::emin(aabb.min, vertices.back());
            }
        }
        else if (linetype == "vt") {
            hasVt = true;
            double x, y;

            obj >> x >> y;
            vertexTextures.push_back(glm::vec2(x, y));
        }
        else if (linetype == "vn") {
            hasVn = true;
            double x, y, z;

            obj >> x >> z >> y;
            vertexNormals.push_back(vec3(x, -y, z));
        }
        else if (linetype == "f") {
            int index;
            QChar character;

            Face * face = new Face();

            QString line = obj.readLine();
            QTextStream lineStream(&line);

            lineStream >> character;
            while (!lineStream.atEnd()) {
                lineStream >> index;
                lineStream >> character;

                face->vertexIndices.push_back(index - 1);

                if (hasVt) {
                    lineStream >> index;
                    lineStream >> character;
                    face->vertexTextureIndices.push_back(index - 1);
                }

                if (hasVn) {
                    lineStream >> index;
                    lineStream >> character;
                    face->vertexNormalIndices.push_back(index - 1);
                }
            }

            if (face->vertexIndices.size() == 3) faces.push_back(face);
            else delete face;
        }
    }
}

void Surface::loadModel(const vector<vector<vec3>>& faces)
{
    name = "表面";
    directory = "";

    bool firstVertex = true;

    for (const vector<vec3>& face : faces) {
        for (const vec3& vertex : face) {
            vertices.push_back(vertex);

            if (firstVertex) {
                firstVertex = false;
                aabb.max = aabb.min = vertices.back();
            }
            else {
                aabb.max = MathUtility::emax(aabb.max, vertices.back());
                aabb.min = MathUtility::emin(aabb.min, vertices.back());
            }
        }
        
        if (face.size() == 3) {
            Face* facePtr = new Face();
            facePtr->vertexIndices = vector<int>({
                int(vertices.size()) - 3,
                int(vertices.size()) - 2,
                int(vertices.size()) - 1
            });

            this->faces.push_back(facePtr);
        }
    }
}

void Surface::parseData()
{
    bool hasNormal = false;
    bool hasTexture = false;
    if (!faces.empty()) {
        hasNormal = faces[0]->vertexNormalIndices.size() > 0;
        hasTexture = faces[0]->vertexTextureIndices.size() > 0;
    }

    for (Face* face : faces) {
        Vertex vertex1, vertex2, vertex3;
        vertex1.position = vertices[face->vertexIndices[0]];
        vertex2.position = vertices[face->vertexIndices[1]];
        vertex3.position = vertices[face->vertexIndices[2]];

        if (hasNormal && face->vertexNormalIndices[0] >= 0) {
            vertex1.normal = vertexNormals[face->vertexNormalIndices[0]];
            vertex2.normal = vertexNormals[face->vertexNormalIndices[1]];
            vertex3.normal = vertexNormals[face->vertexNormalIndices[2]];
        }
        else {
            vec3 normal = glm::cross(vertex2.position - vertex1.position, vertex3.position - vertex2.position);
            vertex1.normal = vertex2.normal = vertex3.normal = normal;
        }

        if (hasTexture && face->vertexTextureIndices[0] >= 0) {
            vertex1.texture = vertexTextures[face->vertexTextureIndices[0]];
            vertex2.texture = vertexTextures[face->vertexTextureIndices[1]];
            vertex3.texture = vertexTextures[face->vertexTextureIndices[2]];
        }
        else {
            vertex1.texture = vertex2.texture = vertex3.texture = glm::vec2(0.0f);
        }

        vertexData.push_back(vertex1);
        vertexData.push_back(vertex2);
        vertexData.push_back(vertex3);
    }
}

void Surface::draw(ShaderProgram& sp) {
    if (!available) setupGL();
    if (!visible) return;

    //glNamedBufferData(vertexBuffer, vertexData.size() * sizeof(Vertex), &vertexData[0], GL_DYNAMIC_DRAW);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    sp.use();
    mat4 model(1.0f);
    model = glm::translate(model, translation);
    model = glm::scale(model, scale);
    sp.set("model", model);
    sp.set("originalColour", colour);
    sp.set("shininess", shininess);
    sp.set("metallic", metallic);
    sp.set("roughness", roughness);
    sp.set("K", K);
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
    glBindVertexArray(0);
}

void Surface::draw(ShaderProgram& sp, int id)
{
    if (!available) setupGL();
    if (!visible) return;

    //glNamedBufferData(vertexBuffer, vertexData.size() * sizeof(Vertex), &vertexData[0], GL_DYNAMIC_DRAW);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    sp.use();
    mat4 model(1.0f);
    model = glm::translate(model, translation);
    model = glm::scale(model, scale);
    sp.set("model", model);
    sp.set("originalColour", colour);
    sp.set("shininess", shininess);
    sp.set("metallic", metallic);
    sp.set("roughness", roughness);
    sp.set("K", K);
    glBindVertexArray(vertexArray);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexData.size(), id);
    glBindVertexArray(0);
}
