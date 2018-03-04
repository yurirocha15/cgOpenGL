#include <vector>
#include <iostream>
#include <cmath>
#include "support.h"
#include "cgmath.h"		// slee's simple math library

#define CIRCLE_DEFINITION 100

//*******************************************************************
// index variables for OpenGL objects
GLuint	program = 0;					// ID holder for GPU program
GLuint	vertexPositionBuffer = 0;		// ID holder for vertex position buffer
GLuint	vertexColorBuffer = 0;		// ID holder for vertex color buffer

//*******************************************************************
// global variables
int		frame = 0;	// index of rendering frames
int		windowWidth		= 720;
int		windowHeight	= 480;
float 	scaleFactor = (float)windowWidth/(float)windowHeight;
bool	bMouseLButtonDown=false;
bool	bUseAnotherColor=false;
vec4	yellowColor = vec4(1.0f,1.0f,0.0f,1.0f); 
vec4	redColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
float x_0 = 0.0, y_0 = 0.0;
float radius = 0.75f;
int flagInit = 0;

//*******************************************************************
void update()
{
	// update uniform variables
	GLint uloc = glGetUniformLocation( program, "bUseAnotherColor" );
	if(uloc>-1) glUniform1i( uloc, bUseAnotherColor );

	uloc = glGetUniformLocation( program, "anotherColor" );
	if(uloc>-1) glUniform4f( uloc, redColor.x, redColor.y, redColor.z, redColor.w );

	uloc = glGetUniformLocation( program, "frame" );
	if(uloc>-1) glUniform1i( uloc, frame );

	uloc = glGetUniformLocation( program, "color" );
	if(uloc>-1) glUniform4f( uloc, yellowColor.x, yellowColor.y, yellowColor.z, yellowColor.w );

	uloc = glGetUniformLocation( program, "aspectRatio" );
	if(uloc>-1) glUniform1f( uloc, scaleFactor );
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// notify to GL that we like to use our program now
	glUseProgram( program );

	// bind vertex position buffer
	GLuint vertexPositionLoc = glGetAttribLocation( program, "position" );
	glEnableVertexAttribArray( vertexPositionLoc );
	glBindBuffer( GL_ARRAY_BUFFER, vertexPositionBuffer );
	glVertexAttribPointer( vertexPositionLoc, 4, GL_FLOAT, GL_FALSE, 0, 0 );

	// bind vertex color buffer
	GLuint vertexColorLoc = glGetAttribLocation( program, "color" );
	glEnableVertexAttribArray( vertexColorLoc );
	glBindBuffer( GL_ARRAY_BUFFER, vertexColorBuffer );
	glVertexAttribPointer( vertexColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0 );

	// render vertices: trigger shader programs to process vertex data
	glDrawArrays( GL_TRIANGLE_FAN, 0, CIRCLE_DEFINITION + 2 * sizeof(vec4));

	// now swap backbuffer with front buffer, and display it
	glutSwapBuffers();

	frame++;
}

void display()
{
	render();
	if(!flagInit)
	{
		update();
		flagInit = 1;
	}
}

void reshape( int width, int height )
{
	// update the scale factor
	scaleFactor = (float)width/(float)height;
	update();

	// set current viewport in pixels
	// viewport: the window area that are affected by rendering
	// (win_x, win_y, win_width, win_height)
	glViewport( 0, 0, width, height );

	// post signal to call display
	// this causes GL to call display() soon
	// but, we do not know exactly when dipslay() is called
	glutPostRedisplay();				
}

void mouse( int button, int state, int x, int y )
{
	if(button==GLUT_LEFT_BUTTON)
	{
		bMouseLButtonDown = (state==GLUT_DOWN);
		if(bMouseLButtonDown) printf( "Left mouse button pressed at (%d, %d)\n", x, y );
	}
}

void motion( int x, int y )
{
}

void idle()
{
	glutPostRedisplay();	// signal to call display soon
}

void keyboard( unsigned char key, int x, int y )
{
	if(key==27||key=='q'||key=='Q')		// ESCAPE
	{
		exit(0);
	}
	else if(key==' ')
	{
		bUseAnotherColor = !bUseAnotherColor;
		update();
		printf( "bUseAnotherColor = %d\n", bUseAnotherColor );
	}
}

bool initShaders( const char* vertShaderPath, const char* fragShaderPath )
{
	// create a program before linking shaders
	program = glCreateProgram();
	glUseProgram( program );

	// compile shader sources
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	const char* vertexShaderSource = readShader( vertShaderPath ); if(vertexShaderSource==NULL) return false;
	GLint vertexShaderLength = strlen(vertexShaderSource);
	glShaderSource( vertexShader, 1, &vertexShaderSource, &vertexShaderLength );
	glCompileShader( vertexShader );
	if(!checkShader( vertexShader, "vertexShader" )){ printf( "Unable to compile vertex shader\n" ); return false; }
	
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	const char* fragmentShaderSource = readShader( fragShaderPath ); if(fragmentShaderSource==NULL) return false;
	GLint fragmentShaderLength = strlen(fragmentShaderSource);
	glShaderSource( fragmentShader, 1, &fragmentShaderSource, &fragmentShaderLength );
	glCompileShader( fragmentShader );
	if(!checkShader( fragmentShader, "fragmentShader" )){ printf( "Unable to compile fragment shader\n" ); return false; }
	
	// attach vertex/fragments shaders and link program
	glAttachShader( program, vertexShader );
	glAttachShader( program, fragmentShader );
	glLinkProgram( program );
	if(!checkProgram( program, "program" )){ printf( "Unable to link program\n" ); return false; }

	// deallocate string
	free((void*)vertexShaderSource);
	free((void*)fragmentShaderSource);

	return true;
}

bool userInit()
{
	// init GL states
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );		// clear color for glClear()
	glEnable( GL_CULL_FACE );					// turn on backface culling
	glEnable( GL_DEPTH_TEST );					// turn on depth tests

	// create a vertex array for a triangle position
	// default viewing volume: [-1~1, -1~1, -1~1]
	std::vector<vec4> triVertices;
	float angle = 2 * M_PI/(float)CIRCLE_DEFINITION;
	vec4 position = vec4(x_0, y_0, 0.0, 1.0);
	triVertices.push_back(position);

	for(int i = 0; i < CIRCLE_DEFINITION; i++)
	{
		position = vec4(x_0 + radius * cos((float)(i) * angle), y_0 + radius * sin((float)(i) * angle), 0.0, 1.0);
		triVertices.push_back(position);
	}
	position = vec4(x_0 + radius * cos(0), y_0 + radius * sin(0), 0.0, 1.0);
	triVertices.push_back(triVertices[1]);
	
	glGenBuffers( 1, &vertexPositionBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexPositionBuffer );
	glBufferData( GL_ARRAY_BUFFER, (CIRCLE_DEFINITION + 2) * sizeof(vec4), &(triVertices[0]), GL_STATIC_DRAW );

	update();
	// usage
	printf( "press spacebar to toggle the color\n" );

	return true;
}

int main( int argc, char* argv[] )
{
	// GLUT initialization
	glutInit( &argc, argv );							// default initialization for GLUT
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );		// double buffering with RGBA frame buffer

	// initialize GLUT window
	glutInitWindowSize( windowWidth, windowHeight );
	glutInitWindowPosition( (glutGet(GLUT_SCREEN_WIDTH)-windowWidth)/2, (glutGet(GLUT_SCREEN_HEIGHT)-windowHeight)/2 );
	glutCreateWindow( "Hello Circle" );

	// Register callbacks
	glutDisplayFunc( display );		// callback when window is drawing
	glutReshapeFunc( reshape );		// callback when window is resized
	glutKeyboardFunc( keyboard );	// callback for keyboard input
	glutMouseFunc( mouse );			// callback for mouse click input
	glutMotionFunc( motion );		// callback for mouse movement input
	glutIdleFunc( idle );			// callback for idle time

	// load and init GL extensions through glew
	if(!initExtensions()){ printf( "Failed to init extensions.\n" ); return 0;}
	
	// create and compile shaders/program
	if(!initShaders("shaders/circle.vert","shaders/circle.frag")){ printf( "Failed to init program and shaders\n" ); return 0; }

	// user initialization
	if(!userInit()){ printf( "Failed to userInit()\n" ); return 0; }

	// Start rendering loop
	glutMainLoop();					// enters into rendering loop

	return 0;
}
