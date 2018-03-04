// the second input from vertex shader
varying vec4 vertexColor;

void main()
{
	// gl_FragColor: builtin output variable
	// to produce color, you must specify it
	gl_FragColor = vertexColor;
}