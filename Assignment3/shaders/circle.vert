
varying vec4 vertexColor;	// second output of vertex shader = input to fragment shader

attribute vec4 position;

uniform vec4 color;
uniform float aspectRatio;
uniform mat4 modelMatrix;

void main()
{
	// gl_Position: builtin output variable of vertex shader
	// you need to write the screen position of vertex

	gl_Position = vec4(position);

	gl_Position = modelMatrix * gl_Position;
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