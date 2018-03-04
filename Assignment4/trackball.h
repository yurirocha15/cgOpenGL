#pragma once

#include "cgmath.h"

// camera
struct Camera
{
	mat4	viewMatrix;
	vec3	eye;
	vec3	at;
	vec3	up;
		
	mat4	projectionMatrix;
	float	fovy;			// in radian
	float	aspectRatio;
	float	dNear;
	float	dFar;
};

struct Trackball
{
	bool	bTracking;
	float	rotationScale;		// controls how much rotation is applied
	vec2	m0;			// the last mouse position
	mat4	viewMatrix0;		// initial view matrix, current view matrix
	mat4&	trackballMatrix;	// trackball matrix == viewMatrix


	Trackball( mat4& viewMatrix, float rotationScale=0.7f ):bTracking(false),rotationScale(rotationScale),trackballMatrix(viewMatrix){}

	void begin( float x, float y, mat4& viewMatrix )
	{
		m0 = vec2(x,y) * 2.0f - 1.0f;   // Convert x, y in [0, 1] to x, y in [-1, 1]
		viewMatrix0 = trackballMatrix;  // Svae current view matrix
		bTracking=true;                 // Enable flag
	}
	void end(){ bTracking=false; }
	//type = 0 -> rotate; type = 1 -> zooming; type = 2 -> panning
	void update( float x, float y, int type )
	{
		vec2 m = vec2(x, y)*2.0f - 1.0f;      // Convert x, y in [0, 1] to x, y in [-1, 1]

		// if not tracking or little movements then, skip
		if(!bTracking||fabs(m0.x-m.x)<0.001f&&fabs(m0.y-m.y)<0.001f) return;

		// project to a unit sphere
		vec3 p0 = vec3(0, 0, 1.0f);									// original position of the camera
		vec3 p1 = vec3(m.x-m0.x, m0.y-m.y, 0) * rotationScale;		// calculate displacement with y-swapping
		p1.z = sqrtf(max(0, 1.0f - vec2(p1.x, p1.y).length2()));	// adjust p1.z to make unit sphere: z^2 = 1 - (x^2 + y^2)
		p1 = p1.normalize();						// normalize p1 vector in case of p1.z = 0.

		if(type == 0)
		{
			// rotate the two points on a sphere in camera space to the world coordinate
			// i.e., apply rotation of inverse view transformation
			mat4 iv = viewMatrix0.inverse();
			float tmpFl1,tmpFl2, tmpFl3;
			vec3 tmp1;
			tmp1 = vec3(iv.a+0);
			tmpFl1 = dot(tmp1,p0);
			tmp1 = vec3(iv.a+4);
			tmpFl2 = dot(tmp1,p0);
			tmp1 = vec3(iv.a+8);
			tmpFl3 = dot(tmp1,p0);
			p0 = vec3( tmpFl1, tmpFl2, tmpFl3 );
			tmp1 = vec3(iv.a+0);
			tmpFl1 = dot(tmp1,p1);
			tmp1 = vec3(iv.a+4);
			tmpFl2 = dot(tmp1,p1);
			tmp1 = vec3(iv.a+8);
			tmpFl3 = dot(tmp1,p1);
			p1 = vec3( tmpFl1, tmpFl2, tmpFl3 );
			
			vec3 n = p0.cross(p1);
			float angle = asin( min(n.length(),0.999f) );
			n=n.normalize();

			// update rotation matrix
			float c=cos(angle), s=sin(angle);
			mat4 tmpM = mat4(
				n.x*n.x*(1-c)+c,		n.x*n.y*(1-c)-n.z*s,	n.x*n.z*(1-c)+n.y*s,	0.0f,
				n.x*n.y*(1-c)+n.z*s,	n.y*n.y*(1-c)+c,		n.y*n.z*(1-c)-n.x*s,	0.0f,
				n.x*n.z*(1-c)-n.y*s,	n.y*n.z*(1-c)+n.x*s,	n.z*n.z*(1-c)+c,		0.0f,
				0,						0,						0,						1.0f
				);
			trackballMatrix = viewMatrix0 * tmpM;
		}
		else if(type == 1)
		{
			
		}
		else if(type == 2)
		{

		}
	}
	vec3 zoom( float x, float y, vec3 eye )
	{
		vec2 m = vec2(x, y)*2.0f - 1.0f;      // Convert x, y in [0, 1] to x, y in [-1, 1]

		// if not tracking or little movements then, skip
		if(!bTracking||fabs(m0.x-m.x)<0.001f&&fabs(m0.y-m.y)<0.001f) return eye;

		vec3 tmp = vec3(eye.x - 50 * (m.y - m0.y), eye.y - 50 * (m.y - m0.y), eye.z - 50 * (m.y - m0.y));
		m0 = m;
		return tmp;

	}
	vec3 pan( float x, float y, vec3 eye )
	{
		vec2 m = vec2(x, y)*2.0f - 1.0f;      // Convert x, y in [0, 1] to x, y in [-1, 1]

		// if not tracking or little movements then, skip
		if(!bTracking||fabs(m0.x-m.x)<0.001f&&fabs(m0.y-m.y)<0.001f) return eye;

		vec3 tmp = vec3(eye.x + 50 * (m.x - m0.x), eye.y, eye.z + 50 * (m.y - m0.y));
		m0 = m;
		return tmp;

	}
};
