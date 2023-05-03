#include "stdafx.h"

CoordinateSystem::CoordinateSystem () : 
	CoordinateSystem(
		vec3(0.0f), 
		vec3(1.0f, 0.0f, 0.0f), 
		vec3(0.0f, 1.0f, 0.0f), 
		vec3(0.0f, 0.0f, 1.0f)
	) { }

CoordinateSystem::CoordinateSystem(
	const vec3& origin, 
	const vec3& xAxis, 
	const vec3& yAxis, 
	const vec3& zAxis
) : origin(origin), 
	xAxis(xAxis), 
	yAxis(yAxis), 
	zAxis(zAxis) 
{
	vao = vbo = 0;
	length = 10.0f;
}

void CoordinateSystem::setupGL() {
	initializeOpenGLFunctions();

	static GLfloat axes[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		xAxis.x, xAxis.y, xAxis.z, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		yAxis.x, yAxis.y, yAxis.z, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		zAxis.x, zAxis.y, zAxis.z, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	
	// axis position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0);
	glEnableVertexAttribArray(0);

	// axis colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

