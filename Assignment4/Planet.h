#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <iostream>
#include "cgmath.h"
#include "GL/glew.h"

class Planet
{
public:
	Planet(float distance, float translationT, float rotationT, float radius, std::string imagepath);
	~Planet();
	void SimulateStep(float time);
	vec3 GetPosition();
	float GetRotation();
	float GetRadius();
	bool IsSun();
	GLuint GetTexture();

private:
	float distance;
	float translationT;
	float rotationT;
	float radius;
	vec3 position;
	float rotation;
	GLuint textureObject;
};

#endif

