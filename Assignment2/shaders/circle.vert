#version 120

attribute vec4 position;	// vertex input attribute

varying vec4 vertexColor;	// second output of vertex shader = input to fragment shader

uniform int frame;
uniform vec4 color;
uniform float aspectRatio;

void main()
{
	// gl_Position: builtin output variable of vertex shader
	// you need to write the screen position of vertex
	gl_Position = vec4(position);

	// scale the vertice
	gl_Position.x = position.x;
	gl_Position.y = position.y;

	if(aspectRatio >= 1.0)
	{
		gl_Position.x /= aspectRatio;
	}
	else
	{
		gl_Position.y *= aspectRatio;
	}

	// another output passed via varying variable
	vertexColor = color;
}
