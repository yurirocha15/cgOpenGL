#version 120	// GLSL > 1.2 necessary for mat3 casting

// vertex attributes
attribute vec3 position;
attribute vec3 normal;
attribute vec3 texcoord;

// outputs of vertex shader
varying vec4 ecPos;	// eye-coordinate position
varying vec3 norm;	// per-vertex normal before interpolation
varying vec2 tc;

// uniform matrices
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	mat4 modelViewMatrix = viewMatrix*modelMatrix;
	ecPos = modelViewMatrix*vec4(position,1);		// eye-coordinate position
	gl_Position = projectionMatrix*ecPos;

	norm = normalize(mat3(modelViewMatrix)*normal);	// per-vertex normal passed to the rasterizer

	tc = vec2(texcoord);
}