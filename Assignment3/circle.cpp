#include "circle.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

Circle::Circle (int w, int h) : numberOfRepositions(0)
{
	//std::srand(std::time(0));

	radius = (float)(std::rand() % MAX_RADIUS);
	if (radius < (float)MIN_RADIUS)
		radius = (float)MIN_RADIUS; 

	x = (float)(std::rand() % (w - 2 * (int)radius) + (int)radius);

	y = (float)(std::rand() % (h - 2 * (int)radius) + (int)radius);


	x /= 0.5 * w;
	y /= 0.5 * h;
	x -= 1.0;
	y -= 1.0;

	float tmp1 = (float)(std::rand() % MAX_SPEED);
	float tmp2 = (float)(std::rand() % MAX_SPEED);

	tmp1 /= w;
	tmp2 /= h;
	speed = vec2(tmp1, tmp2);

	float f1 = (float)std::rand() / (float)RAND_MAX;
	float f2 = (float)std::rand() / (float)RAND_MAX;
	float f3 = (float)std::rand() / (float)RAND_MAX;
	color = vec4(f1, f2, f3, 1.0f);

	//int area = pow(radius,2) * M_PI;
	radius /= 240.0;
	mass = radius;//MASS_MULTIPLIER * area * MAX_MASS / MAX_AREA;

	
	origRadius = radius;
}

Circle::~Circle()
{

}

void Circle::SetX(float)
{
	this->x = x;
}

void Circle::SetY(float)
{
	this->y = y;
}

void Circle::SetSpeedX(float speedX)
{
	this->speed.x = speedX;
}

void Circle::SetSpeedY(float speedY)
{
	this->speed.y = speedY;
}

void Circle::SetRadius(float radius)
{
	this->radius = radius;
}

float Circle::GetX()
{
	return x;
}

float Circle::GetY()
{
	return y;
}

float Circle::GetRadius()
{
	return radius;
}

float Circle::GetOrigRadius()
{
	return origRadius;
}

float Circle::GetSpeedX()
{
	return speed.x;
}

float Circle::GetSpeedY()
{
	return speed.y;
}

float Circle::GetMass()
{
	return mass;
}

vec4 Circle::GetColor()
{
	return color;
}

void Circle::Reposition(int w, int h)
{
	numberOfRepositions++;
	radius = (float)(std::rand() % MAX_RADIUS);
	if (radius < (float)MIN_RADIUS || numberOfRepositions > 10)
		radius = (float)MIN_RADIUS;

	x = (float)(std::rand() % (w - 2 * (int)radius)) + (int)radius;

	y = (float)(std::rand() % (h - 2 * (int)radius)) + (int)radius;

	x /= 0.5 * w;
	y /= 0.5 * h;
	x -= 1.0;
	y -= 1.0;

	radius /= 100.0;
	mass = radius;
}

void Circle::Update(float w, float h)
{
	/*if((int)(FPS / speed.x) % frame == 0)
		x += speed.x/abs(speed.x);
	if((int)(FPS / speed.y) % frame == 0)
		y += speed.y/abs(speed.y);*/
		x += speed.x;
		y += speed.y;
	if (x >= w - radius && speed.x > 0) speed.x = -speed.x;
	else if (x <= radius - w && speed.x < 0) speed.x = -speed.x;
	else if (y >= h - radius && speed.y > 0) speed.y = -speed.y;
	else if (y <= radius - h  && speed.y < 0) speed.y = -speed.y;
}

bool Circle::CheckCollision (Circle other)
{
	if (this->x + this->radius + other.GetRadius() > other.GetX() 
	&& this->x < other.GetX() + this->radius + other.GetRadius()
	&& this->y + this->radius + other.GetRadius() > other.GetY() 
	&& this->y < other.GetY() + this->radius + other.GetRadius())	//Balls close to each other
	{
	    int distanceBetweenCenters = (this->x - other.GetX()) * (this->x - other.GetX()) + (this->y - other.GetY()) * (this->y - other.GetY());
	    //std::cout << "D: " << distanceBetweenCenters << std::endl;
	    if(distanceBetweenCenters < pow(this->radius + other.GetRadius(),2))
	    {
	    	return true;
	    }
	}
	return false;
}

void Circle::HandleCollision(Circle *other, float w, float h)
{
	double collisionAngle = atan2(this->y - other->GetY(), this->x - other->GetX());
	double cos_ColAngle = cos(collisionAngle);
	double sin_ColAngle = sin(collisionAngle);
	double theta1 = atan2(this->speed.y, this->speed.x);
	double theta2 = atan2(other->GetSpeedY(), other->GetSpeedX());
	double v1 = sqrt(pow(this->speed.x, 2) + pow(this->speed.y, 2));
	double v2 = sqrt(pow(other->GetSpeedX(), 2) + pow(other->GetSpeedY(), 2));
	vec2 tmpS1, tmpS2;

	/*float baseCalc = ((this->speed.x * cos_ColAngle + this->speed.y * sin_ColAngle) * (this->mass - other->GetMass()) 
				+ 2 * other->GetMass() * (other->GetSpeedX() * cos_ColAngle + other->GetSpeedY() * sin_ColAngle)) / (this->mass + other->GetMass());

	tmpS1.x = baseCalc * cos_ColAngle + (this->speed.y * cos_ColAngle - this->speed.x * sin_ColAngle) * cos(collisionAngle + M_PI_2);
	tmpS1.y = baseCalc * sin_ColAngle + (this->speed.y * cos_ColAngle - this->speed.x * sin_ColAngle) * sin(collisionAngle + M_PI_2);

	baseCalc = ((other->GetX() * cos_ColAngle + other->GetY() * sin_ColAngle) * (other->GetMass() - this->mass) 
				+ 2 * this->mass * (this->speed.x * cos_ColAngle + this->speed.y * sin_ColAngle)) / (other->GetMass() + this->mass);

	tmpS2.x = baseCalc * cos_ColAngle + (other->GetY() * cos_ColAngle - other->GetX() * sin_ColAngle) * cos(collisionAngle + M_PI_2);
	tmpS2.y = baseCalc * sin_ColAngle + (other->GetY() * cos_ColAngle - other->GetX() * sin_ColAngle) * sin(collisionAngle + M_PI_2);*/

	double baseCalc = (v1 * cos(theta1 - collisionAngle) * (this->mass - other->GetMass()) + 2 * other->GetMass() * v2 * cos(theta2 - collisionAngle))/(this->mass + other->GetMass());
	tmpS1.x = baseCalc * cos_ColAngle + v1 * sin(theta1 - collisionAngle) * cos(collisionAngle + M_PI_2);
	tmpS1.y = baseCalc * sin_ColAngle + v1 * sin(theta1 - collisionAngle) * sin(collisionAngle + M_PI_2);

	baseCalc = (v2 * cos(theta2 - collisionAngle) * (other->GetMass() - this->mass) + 2 * this->mass * v1 * cos(theta1 - collisionAngle))/(this->mass + other->GetMass());
	tmpS2.x = baseCalc * cos_ColAngle + v2 * sin(theta2 - collisionAngle) * cos(collisionAngle + M_PI_2);
	tmpS2.y = baseCalc * sin_ColAngle + v2 * sin(theta2 - collisionAngle) * sin(collisionAngle + M_PI_2);

	this->speed.x = (float)tmpS1.x;
	this->speed.y = (float)tmpS1.y;
	other->SetSpeedX((float)tmpS2.x);
	other->SetSpeedY((float)tmpS2.y);
	Update(w, h);
	other->Update(w,h);

	/*float newVelX1 = (this->speed.x * (this->mass - other->GetMass()) + (2 * other->GetMass() * other->GetSpeedX())) / (this->mass + other->GetMass());
	float newVelY1 = (this->speed.y * (this->mass - other->GetMass()) + (2 * other->GetMass() * other->GetSpeedY())) / (this->mass + other->GetMass());
	float newVelX2 = (other->GetSpeedX() * (other->GetMass() - this->mass) + (2 * this->mass * this->speed.x)) / (this->mass + other->GetMass());
	float newVelY2 = (other->GetSpeedY() * (other->GetMass() - this->mass) + (2 * this->mass * this->speed.y)) / (this->mass + other->GetMass());

	speed.x = newVelX1;
	speed.y = newVelY1;
	other->SetSpeedX(newVelX2);
	other->SetSpeedY(newVelY2);*/

}

