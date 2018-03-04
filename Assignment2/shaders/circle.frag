#version 120

// the second input from vertex shader
varying vec4 vertexColor;

// shader's global variables, called the uniform variables
uniform bool bUseAnotherColor;
uniform vec4 anotherColor;

void main()
{
	// gl_FragColor: builtin output variable
	// to produce color, you must specify it
	gl_FragColor = bUseAnotherColor ? anotherColor : vertexColor;
}
