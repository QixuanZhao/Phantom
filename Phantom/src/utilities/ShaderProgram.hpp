#include "stdafx.h"
#pragma once

/**
 * @file    ShaderProgram.hpp
 * @brief   This file defines the ShaderProgram class. <br/>
 *          此文件定义 ShaderProgram 类。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

/**
 * @class ShaderProgram
 * @brief Shader program class. <br/> 着色器程序类。
 * @details This class represents a shader program in OpenGL. <br/>
 *          这个类表示 OpenGL 中的一个着色器程序。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
class ShaderProgram : protected GLF
{
protected:
    /// program id <br/> 程序 id
    GLuint programId;
public:
    /**
     * @brief constructor <br/> 构造函数
     * @param vertexShader vertex shader source code <br/> 顶点着色器源代码
     * @param fragmentShader fragment shader source code <br/> 片段着色器源代码
     */
    inline ShaderProgram(const char* vertexShader, const char* fragmentShader);

    inline ShaderProgram(const QString& vertexShader, const QString& fragmentShader);
    /**
     * @brief constructor <br/> 构造函数
     * @param vertexShader vertex shader source code <br/> 顶点着色器源代码
     * @param fragmentShader fragment shader source code <br/> 片段着色器源代码
     */
    inline ShaderProgram(const char* vertexShader, const char* geometryShader, const char* fragmentShader);

    inline ShaderProgram(const QString& vertexShader, const QString& geometryShader, const QString& fragmentShader);

    /// destructor <br/> 析构函数
    virtual ~ShaderProgram() {
        glDeleteProgram(programId);

        Debug::putLog(
            QString("Shader program %1 %2")
            .arg(QString(Debug::numeric).arg(programId))
            .arg(QString(Debug::fail).arg("deleted"))
        );
    }

    /// use this shader program <br/> 使用此着色器程序
    inline void use();

    /// unuse this shader program <br/> 取消使用此着色器程序
    inline void unuse();

    inline void set(const char* name, bool value);
    inline void set(const char* name, int value);
    inline void set(const char* name, unsigned int value);
    inline void set(const char* name, float value);
    inline void set(const char* name, double value);
    inline void set(const char* name, size_t length, bool value[]);
    inline void set(const char* name, size_t length, int value[]);
    inline void set(const char* name, size_t length, float value[]);
    inline void set(const char* name, size_t length, double value[]);
    inline void set(const char* name, std::initializer_list<bool> values);
    inline void set(const char* name, std::initializer_list<int> values);
    inline void set(const char* name, std::initializer_list<float> values);
    inline void set(const char* name, std::initializer_list<double> values);
    inline void set(const char* name, const mat4& value);
    inline void set(const char* name, const mat3& value);
    inline void set(const char* name, const mat2& value);
    inline void set(const char* name, const mat4x3& value);
    inline void set(const char* name, const mat3x2& value);
    inline void set(const char* name, const mat4x2& value);
    inline void set(const char* name, const mat3x4& value);
    inline void set(const char* name, const mat2x3& value);
    inline void set(const char* name, const mat2x4& value);
    inline void set(const char* name, const vec4& value);
    inline void set(const char* name, const vec3& value);
    inline void set(const char* name, const vec2& value);
    inline void set(const char* name, const ivec4& value);
    inline void set(const char* name, const ivec3& value);
    inline void set(const char* name, const ivec2& value);

    inline void setInteger(const QString& name, const int& value) {
        set(name, value);
    }

    template<typename T>
    inline void set(const QString& name, const T& value);

    template<typename T>
    inline void set(const QString& name, size_t length, T value[]);

    template<typename T>
    inline void set(const string& name, const T& value);

    template<typename T>
    inline void set(const string& name, size_t length, T value[]);

    inline GLuint getId() const { return this->programId; }
};

inline ShaderProgram::ShaderProgram(const char* vertexShader, const char* fragmentShader)
{
    initializeOpenGLFunctions();

    GLuint ver, fra;
    GLint success;
    GLchar infoLog[512];
    ver = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ver, 1, &vertexShader, nullptr);
    glCompileShader(ver);
    glGetShaderiv(ver, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ver, 512, nullptr, infoLog);
        Debug::putLog(Debug::Format::FAIL,
            QString("Fail to compile vertex shader for program %1")
            .arg(QString(Debug::numeric).arg(programId))
        );
        const QStringList& infoList = QString(infoLog).split("\n");
        for (const QString& info : infoList) Debug::putLog(Debug::Format::FAIL, info);
    }

    fra = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fra, 1, &fragmentShader, nullptr);
    glCompileShader(fra);
    glGetShaderiv(fra, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fra, 512, nullptr, infoLog);
        Debug::putLog(Debug::Format::FAIL,
            QString("Fail to compile fragment shader for program %1")
            .arg(QString(Debug::numeric).arg(programId))
        );
        const QStringList& infoList = QString(infoLog).split("\n");
        for (const QString& info : infoList) Debug::putLog(Debug::Format::FAIL, info);
    }

    this->programId = glCreateProgram();
    glAttachShader(programId, ver);
    glAttachShader(programId, fra);
    glLinkProgram(programId);
    glDetachShader(programId, ver);
    glDetachShader(programId, fra);
    glDeleteShader(ver);
    glDeleteShader(fra);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        Debug::putLog(Debug::Format::FAIL,
            QString("Fail to link shader program %1")
            .arg(QString(Debug::numeric).arg(programId))
        );
        const QStringList& infoList = QString(infoLog).split("\n");
        for (const QString& info : infoList) Debug::putLog(Debug::Format::FAIL, info);

        return;
    }

    Debug::putLog(
        QString("Shader program %1 %2")
        .arg(QString(Debug::numeric).arg(programId))
        .arg(QString(Debug::success).arg("linked"))
    );
}

inline ShaderProgram::ShaderProgram(
    const QString& vertexShader, 
    const QString& fragmentShader
) : ShaderProgram(
        vertexShader.toLocal8Bit().constData(), 
        fragmentShader.toLocal8Bit().constData()
    )
{
}

inline ShaderProgram::ShaderProgram(
    const char* vertexShader, 
    const char* geometryShader, 
    const char* fragmentShader
) {
    initializeOpenGLFunctions();

    GLuint ver, geo, fra;
    GLint success;
    GLchar infoLog[512];
    ver = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ver, 1, &vertexShader, nullptr);
    glCompileShader(ver);
    glGetShaderiv(ver, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(ver, 512, nullptr, infoLog);
        Debug::putLog(Debug::Format::FAIL, 
            QString("Fail to compile vertex shader for program %1")
            .arg(QString(Debug::numeric).arg(programId))
        );
        const QStringList& infoList = QString(infoLog).split("\n");
        for (const QString& info : infoList) Debug::putLog(Debug::Format::FAIL, info);
    }

    geo = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geo, 1, &geometryShader, nullptr);
    glCompileShader(geo);
    glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geo, 512, nullptr, infoLog);
        Debug::putLog(Debug::Format::FAIL,
            QString("Fail to compile geometry shader for program %1")
            .arg(QString(Debug::numeric).arg(programId))
        );
        const QStringList& infoList = QString(infoLog).split("\n");
        for (const QString& info : infoList) Debug::putLog(Debug::Format::FAIL, info);
    }

    fra = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fra, 1, &fragmentShader, nullptr);
    glCompileShader(fra);
    glGetShaderiv(fra, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fra, 512, nullptr, infoLog);
        Debug::putLog(Debug::Format::FAIL,
            QString("Fail to compile fragment shader for program %1")
            .arg(QString(Debug::numeric).arg(programId))
        );
        const QStringList& infoList = QString(infoLog).split("\n");
        for (const QString& info : infoList) Debug::putLog(Debug::Format::FAIL, info);
    }

    this->programId = glCreateProgram();
    glAttachShader(programId, ver);
    glAttachShader(programId, geo);
    glAttachShader(programId, fra);
    glLinkProgram(programId);
    glDetachShader(programId, ver);
    glDetachShader(programId, geo);
    glDetachShader(programId, fra);
    glDeleteShader(ver);
    glDeleteShader(geo);
    glDeleteShader(fra);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        Debug::putLog(Debug::Format::FAIL, 
            QString("Fail to link shader program %1")
            .arg(QString(Debug::numeric).arg(programId))
        );
        const QStringList& infoList = QString(infoLog).split("\n");
        for (const QString& info : infoList) Debug::putLog(Debug::Format::FAIL, info);

        return;
    }

    Debug::putLog(
        QString("Shader program %1 %2")
        .arg(QString(Debug::numeric).arg(programId))
        .arg(QString(Debug::success).arg("linked"))
    );
}

inline ShaderProgram::ShaderProgram(
    const QString& vertexShader, 
    const QString& geometryShader, 
    const QString& fragmentShader
) : ShaderProgram(
        vertexShader.toLocal8Bit().constData(),
        geometryShader.toLocal8Bit().constData(),
        fragmentShader.toLocal8Bit().constData()
    )
{ }

inline void ShaderProgram::use() {
    glUseProgram(programId);
}

inline void ShaderProgram::unuse() {
    glUseProgram(0);
}

inline void ShaderProgram::set(const char* name, bool value)
{
    glProgramUniform1i(programId, glGetUniformLocation(programId, name), (int)value);
}

inline void ShaderProgram::set(const char* name, int value)
{
    glProgramUniform1i(programId, glGetUniformLocation(programId, name), value);
}

inline void ShaderProgram::set(const char* name, unsigned int value)
{
    glProgramUniform1ui(programId, glGetUniformLocation(programId, name), value);
}

inline void ShaderProgram::set(const char* name, float value)
{
    glProgramUniform1f(programId, glGetUniformLocation(programId, name), value);
}

inline void ShaderProgram::set(const char* name, double value)
{
    glProgramUniform1d(programId, glGetUniformLocation(programId, name), value);
}

inline void ShaderProgram::set(const char* name, size_t length, bool value[])
{
    switch (length) {
    case 2:
        glProgramUniform2i(programId, glGetUniformLocation(programId, name), (int)value[0], (int)value[1]);
        break;
    case 3:
        glProgramUniform3i(programId, glGetUniformLocation(programId, name), (int)value[0], (int)value[1], (int)value[2]);
        break;
    case 4:
        glProgramUniform4i(programId, glGetUniformLocation(programId, name), (int)value[0], (int)value[1], (int)value[2], (int)value[3]);
        break;
    default:
        break;
    }
}

inline void ShaderProgram::set(const char* name, size_t length, int value[])
{
    switch (length) {
    case 2:
        glProgramUniform2i(programId, glGetUniformLocation(programId, name), value[0], value[1]);
        break;
    case 3:
        glProgramUniform3i(programId, glGetUniformLocation(programId, name), value[0], value[1], value[2]);
        break;
    case 4:
        glProgramUniform4i(programId, glGetUniformLocation(programId, name), value[0], value[1], value[2], value[3]);
        break;
    default:
        break;
    }
}

inline void ShaderProgram::set(const char* name, size_t length, float value[])
{
    switch (length) {
    case 2:
        glProgramUniform2f(programId, glGetUniformLocation(programId, name), value[0], value[1]);
        break;
    case 3:
        glProgramUniform3f(programId, glGetUniformLocation(programId, name), value[0], value[1], value[2]);
        break;
    case 4:
        glProgramUniform4f(programId, glGetUniformLocation(programId, name), value[0], value[1], value[2], value[3]);
        break;
    default:
        break;
    }
}

inline void ShaderProgram::set(const char* name, size_t length, double value[])
{
    switch (length) {
    case 2:
        glProgramUniform2d(programId, glGetUniformLocation(programId, name), value[0], value[1]);
        break;
    case 3:
        glProgramUniform3d(programId, glGetUniformLocation(programId, name), value[0], value[1], value[2]);
        break;
    case 4:
        glProgramUniform4d(programId, glGetUniformLocation(programId, name), value[0], value[1], value[2], value[3]);
        break;
    default:
        break;
    }
}

inline void ShaderProgram::set(const char* name, std::initializer_list<bool> values)
{
    const bool* bv = values.begin();
    switch (values.size()) {
    case 1:
        glProgramUniform1i(programId, glGetUniformLocation(programId, name), (int)bv[0]);
        break;
    case 2:
        glProgramUniform2i(programId, glGetUniformLocation(programId, name), (int)bv[0], (int)bv[1]);
        break;
    case 3:
        glProgramUniform3i(programId, glGetUniformLocation(programId, name), (int)bv[0], (int)bv[1], (int)bv[2]);
        break;
    case 4:
        glProgramUniform4i(programId, glGetUniformLocation(programId, name), (int)bv[0], (int)bv[1], (int)bv[2], (int)bv[3]);
        break;
    default:
        break;
    }
}

inline void ShaderProgram::set(const char* name, std::initializer_list<int> values)
{
    const int* bv = values.begin();
    switch (values.size()) {
    case 1:
        glProgramUniform1i(programId, glGetUniformLocation(programId, name), bv[0]);
        break;
    case 2:
        glProgramUniform2i(programId, glGetUniformLocation(programId, name), bv[0], bv[1]);
        break;
    case 3:
        glProgramUniform3i(programId, glGetUniformLocation(programId, name), bv[0], bv[1], bv[2]);
        break;
    case 4:
        glProgramUniform4i(programId, glGetUniformLocation(programId, name), bv[0], bv[1], bv[2], bv[3]);
        break;
    default:
        break;
    }
}

inline void ShaderProgram::set(const char* name, std::initializer_list<float> values)
{
    const float* bv = values.begin();
    switch (values.size()) {
    case 1:
        glProgramUniform1f(programId, glGetUniformLocation(programId, name), bv[0]);
        break;
    case 2:
        glProgramUniform2f(programId, glGetUniformLocation(programId, name), bv[0], bv[1]);
        break;
    case 3:
        glProgramUniform3f(programId, glGetUniformLocation(programId, name), bv[0], bv[1], bv[2]);
        break;
    case 4:
        glProgramUniform4f(programId, glGetUniformLocation(programId, name), bv[0], bv[1], bv[2], bv[3]);
        break;
    default:
        break;
    }
}


inline void ShaderProgram::set(const char* name, std::initializer_list<double> values)
{
    const double* bv = values.begin();
    switch (values.size()) {
    case 1:
        glProgramUniform1d(programId, glGetUniformLocation(programId, name), bv[0]);
        break;
    case 2:
        glProgramUniform2d(programId, glGetUniformLocation(programId, name), bv[0], bv[1]);
        break;
    case 3:
        glProgramUniform3d(programId, glGetUniformLocation(programId, name), bv[0], bv[1], bv[2]);
        break;
    case 4:
        glProgramUniform4d(programId, glGetUniformLocation(programId, name), bv[0], bv[1], bv[2], bv[3]);
        break;
    default:
        break;
    }
}

inline void ShaderProgram::set(const char* name, const mat4& value)
{
    glProgramUniformMatrix4fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat3& value)
{
    glProgramUniformMatrix3fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat2& value)
{
    glProgramUniformMatrix2fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat4x3& value)
{
    glProgramUniformMatrix4x3fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat3x2& value)
{
    glProgramUniformMatrix3x2fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat4x2& value)
{
    glProgramUniformMatrix4x2fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat3x4& value)
{
    glProgramUniformMatrix3x4fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat2x3& value)
{
    glProgramUniformMatrix2x3fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const mat2x4& value)
{
    glProgramUniformMatrix2x4fv(programId, glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const vec4& value)
{
    glProgramUniform4fv(programId, glGetUniformLocation(programId, name), 1, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const vec3& value)
{
    glProgramUniform3fv(programId, glGetUniformLocation(programId, name), 1, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const vec2& value)
{
    glProgramUniform2fv(programId, glGetUniformLocation(programId, name), 1, glm::value_ptr(value));
}

inline void ShaderProgram::set(const char* name, const ivec4& value)
{
    glProgramUniform4iv(programId, glGetUniformLocation(programId, name), 1, glm::value_ptr(value));

}

inline void ShaderProgram::set(const char* name, const ivec3& value)
{
    glProgramUniform3iv(programId, glGetUniformLocation(programId, name), 1, glm::value_ptr(value));

}

inline void ShaderProgram::set(const char* name, const ivec2& value)
{
    glProgramUniform2iv(programId, glGetUniformLocation(programId, name), 1, glm::value_ptr(value));

}

template<typename T>
inline void ShaderProgram::set(const QString& name, const T& value)
{
    set(name.toLocal8Bit().constData(), value);
}

template<typename T>
inline void ShaderProgram::set(const QString& name, size_t length, T value[])
{
    set(name.toLocal8Bit().constData(), length, value);
}

template<typename T>
inline void ShaderProgram::set(const string& name, const T& value)
{
    set(name.c_str(), value);
}

template<typename T>
inline void ShaderProgram::set(const string& name, size_t length, T value[])
{
    set(name.c_str(), length, value);
}

#endif
