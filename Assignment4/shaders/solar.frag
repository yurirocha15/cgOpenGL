#version 120

// input from the rasterizer
varying vec4 ecPos;
varying vec3 norm;
varying vec2 tc;

// light properties
uniform vec4 lightPosition, Ia, Id, Is;
uniform int isSun;

// material properties
uniform vec4	Ka, Kd, Ks;
uniform float	shininess;

uniform mat4 viewMatrix;

uniform sampler2D TEX;	//texture object from application

void main()
{
	vec4 colorDiffuse;
	colorDiffuse = texture2D(TEX, tc);

	vec4 lPos = viewMatrix*lightPosition;	// light position in the eye-space coordinate

	vec3 n = normalize(norm);	// norm interpolated via rasterizer should be normalized again here
	vec3 p = ecPos.xyz;			// 3D position of this fragment
	vec3 l = normalize(lPos.xyz-(lPos.a==0.0?vec3(0):p));	// lPos.a==0 means directional light
	vec3 v = normalize(-p);									// eye-ecPos = vec3(0)-ecPos
	vec3 h = normalize(l+v);	// the halfway vector

	vec4 Ira = Ka*Ia;									// ambient reflection
	vec4 Ird = max(Kd*dot(n,l)*Id,0.0);	// diffuse reflection
	vec4 Irs = max(Ks*pow(dot(h,n),shininess)*Is,0.0);	// specular reflection

	gl_FragColor = Ira * colorDiffuse + Ird * colorDiffuse + Irs;

	if(isSun == 1)
	{
		gl_FragColor = colorDiffuse; 
	}
}