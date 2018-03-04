#include "Planet.h"
#include <cmath>

// forward declaration of image loader function available at stb_image.c
extern "C" unsigned char*	stbi_load( const char* filename, int* x, int* y, int* comp, int req_comp );
extern "C" void				stbi_image_free( void* retval_from_stbi_load );

Planet::Planet(float distance, float translationT, float rotationT, float radius, std::string imagepath)
{
	this->distance = distance;
	this->translationT = translationT;
	this->rotationT = rotationT;
	this->radius = radius;

	position.x = distance;
	position.y = 0;
	position.z = distance;

	rotation = 0;

	// load an image
	int comp, height, width;;

	unsigned char* image0 = stbi_load( imagepath.c_str(), &width, &height, &comp, 4 );
        
	glGenTextures( 1, &textureObject );
	glBindTexture( GL_TEXTURE_2D, textureObject );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image0 );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free( image0 ); // release the original image

}

Planet::~Planet()
{
}

void Planet::SimulateStep(float time)
{
	if(distance > 0.001f)
	{
		// find the angle of orientation of the orbit around the sun
		float angle = time * 3.1419f / translationT;
		
		// use trig to find the position in space
		position.x = sin(angle) * distance;
		position.y = 0;
		position.z = cos(angle) * distance;;

		if(distance > 300000.0)
		{
			position.x *= 0.4;
			position.y *= 0.4;
		}
	}

	// find the rotation of the planet around its axis
	rotation = time * 360 / rotationT;
}

vec3 Planet::GetPosition()
{
	return this->position;
}

float Planet::GetRotation()
{
	return this->rotation;
}

float Planet::GetRadius()
{
	return this->radius;
}

bool Planet::IsSun()
{
	return (distance < 0.01f);
}

GLuint Planet::GetTexture()
{
	return textureObject;
}
