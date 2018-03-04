#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include <iostream>
#include "cgmath.h"
#include <cmath>
#include <vector>

#define CIRCLE_DEFINITION 80
#define MAX_RADIUS 22
#define MIN_RADIUS 10
#define MAX_SPEED 3
#define MAX_MASS 200
//#define MASS_MULTIPLIER 0.1
#define MAX_AREA (MAX_RADIUS * MAX_RADIUS * M_PI)

class Circle
{
public:
	Circle(int, int);
	~Circle();
	
	void SetX(float);
	void SetY(float);
	void SetSpeedX(float);
	void SetSpeedY(float);
	void SetRadius(float);
	float GetX();
	float GetY();
	float GetRadius();
	float GetOrigRadius();
	float GetSpeedX();
	float GetSpeedY();
	float GetMass();
	vec4 GetColor();

	void Reposition(int, int);
	void Update(float, float);
	bool CheckCollision(Circle);
	void HandleCollision(Circle*, float, float);


private:
	float x;
	float y;
	float radius;
	float origRadius;
	vec2 speed;
	vec4 color;
	float mass;
	int numberOfRepositions;
};

#endif //_CIRCLE_H_