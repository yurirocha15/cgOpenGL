#include <iostream>
#include "support.h"
#include "cgmath.h"		// slee's simple math library
#include "trackball.h"
#include "mesh.h"
#include "light.h"
#include "Planet.h"
#include <vector>
#include <cmath>

//*******************************************************************
// index variables for OpenGL objects
GLuint	program=0;					// ID holder for GPU program
GLuint	vertexPositionBuffer=0;		// ID holder for vertex position buffer
GLuint	indexPositionBuffer=0;
GLuint 	textureObject = 0;

//*******************************************************************
// global variables
int			frame=0;	// index of rendering frames
bool		bMouseLButtonDown=false;
Camera		camera;
Trackball	trackball(camera.viewMatrix, 1.0f);
Light		light;
Material	material;
int 		indexCount;
int windowWidth		= 1280;
int windowHeight	= 720;
int type = -1;

std::vector<vertex> vertices;
std::vector<GLushort> indices;

std::vector<Planet> planets;


//*******************************************************************

#define DISTANCE_SCALE 0.05f
#define RADIUS_SCALE 0.05f
void update()
{
	// configure projection matrix
	camera.fovy = PI/6.0f;
	//camera.aspectRatio; // udpate in reshape()
	camera.dNear = 0.01f;
	camera.dFar = 10000.0f;
	camera.projectionMatrix = mat4::perspective( camera.fovy, camera.aspectRatio, camera.dNear, camera.dFar );

	glUniformMatrix4fv( glGetUniformLocation( program, "viewMatrix" ), 1, GL_TRUE, camera.viewMatrix );
	glUniformMatrix4fv( glGetUniformLocation( program, "projectionMatrix" ), 1, GL_TRUE, camera.projectionMatrix );

	// setup light properties
	glUniform4fv( glGetUniformLocation( program, "lightPosition" ), 1, light.position );
	glUniform4fv( glGetUniformLocation( program, "Ia" ), 1, light.ambient );
	glUniform4fv( glGetUniformLocation( program, "Id" ), 1, light.diffuse );
	glUniform4fv( glGetUniformLocation( program, "Is" ), 1, light.specular );

    // setup material properties
	glUniform4fv( glGetUniformLocation( program, "Ka" ), 1, material.ambient );
	glUniform4fv( glGetUniformLocation( program, "Kd" ), 1, material.diffuse );
	glUniform4fv( glGetUniformLocation( program, "Ks" ), 1, material.specular );
	glUniform1f( glGetUniformLocation( program, "shininess" ), material.shininess );
}

void renderPlanets()
{
	int isSun = 1;
	for(int i = 0; i < 9; i++, isSun++)
	{
		glUniform1i( glGetUniformLocation( program, "isSun" ), isSun );
		planets[i].SimulateStep(frame * 0.2);
		// configure parameters for model matrix
		float theta = planets[i].GetRotation();
		float scale = planets[i].GetRadius() * RADIUS_SCALE;

		if(planets[i].IsSun() && scale > 8.0f)
		{
			scale = 8.0f;
		}

		if(scale < 0.7f)
		{
			if(scale < 0.5f)
				scale *= 8;
			else
				scale *= 5;

		}

		mat4 modelMatrix;
		vec3 tmp = -camera.at;
		modelMatrix = mat4::translate( tmp ) * modelMatrix;
		tmp = vec3(0,1.0,0);
		modelMatrix = mat4::rotate( tmp, theta ) * modelMatrix;
		modelMatrix = mat4::scale( scale, scale, scale ) * modelMatrix;
		tmp = camera.at + planets[i].GetPosition() * DISTANCE_SCALE;
		modelMatrix = mat4::translate( tmp ) * modelMatrix;

		// setup uniform matrix
		glUniformMatrix4fv( glGetUniformLocation( program, "modelMatrix" ), 1, GL_TRUE, modelMatrix );
		
		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, planets[i].GetTexture() );
		glUniform1i( glGetUniformLocation( program, "TEX" ), 0 );	 // GL_TEXTURE0

				// bind vertex position buffer
		GLuint vertexPositionLoc = glGetAttribLocation( program, "position" );
		glEnableVertexAttribArray( vertexPositionLoc );
		glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0 );

		// bind vertex normal buffer
		GLuint vertexNormalLoc = glGetAttribLocation( program, "normal" );
		glEnableVertexAttribArray( vertexNormalLoc );
		glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)) );

		// bind vertex texture buffer
		GLuint vertexTexlLoc = glGetAttribLocation( program, "texcoord" );
		glEnableVertexAttribArray( vertexTexlLoc );
		glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)+sizeof(vertex().norm)) );

		// render vertices: trigger shader programs to process vertex data
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexPositionBuffer );
		glDrawElements( GL_QUADS, indices.size() * 4, GL_UNSIGNED_SHORT, NULL );

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
	



void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// notify to GL that we like to use our program now
	glUseProgram( program );

	// bind vertex position buffer
	glBindBuffer( GL_ARRAY_BUFFER, vertexPositionBuffer );

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	renderPlanets();

	// now swap backbuffer with front buffer, and display it
	glutSwapBuffers();

	// increment FRAME index
	frame++;
}

void display()
{
	update();
	render();
}

void reshape( int width, int height )
{
	// set current viewport in pixels
	// viewport: the window area that are affected by rendering
	// (win_x, win_y, win_width, win_height)
	glViewport( 0, 0, width, height );

	camera.aspectRatio = float(width)/float(height);

	// post signal to call display
	// this causes GL to call display() soon
	// but, we do not know exactly when dipslay() is called
	glutPostRedisplay();				
}

void mouse( int button, int state, int x, int y )
{
	int modifier = glutGetModifiers();
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state==GLUT_DOWN)	
		{
			type = 0;
			if(modifier == GLUT_ACTIVE_SHIFT)
				type = 1;
			else if(modifier == GLUT_ACTIVE_CTRL)
				type = 2;
			trackball.begin( x/float(windowWidth-1), y/float(windowHeight-1), camera.viewMatrix );
		}
		else if(state==GLUT_UP) 
			trackball.end();
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		if(state==GLUT_DOWN)	
		{
			type = 1;
			trackball.begin( x/float(windowWidth-1), y/float(windowHeight-1), camera.viewMatrix );
		}
		else if(state==GLUT_UP) 
			trackball.end();
	}
	else if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state==GLUT_DOWN)	
		{
			type = 2;
			trackball.begin( x/float(windowWidth-1), y/float(windowHeight-1), camera.viewMatrix );
		}
		else if(state==GLUT_UP) 
			trackball.end();
	}
}

void motion( int x, int y )
{
	// while moving mouse, update camera matrix
	trackball.update( x/float(windowWidth-1), y/float(windowHeight-1), type );
	if(type == 1)
	{
		camera.eye = trackball.zoom(x/float(windowWidth-1), y/float(windowHeight-1), camera.eye);
		camera.at = vec3( 0, 0, 0 );
		camera.up = vec3( 0, 1, 0 );
		camera.viewMatrix = mat4::lookAt( camera.eye, camera.at, camera.up );
	}
	if(type == 2)
	{
		camera.eye = trackball.pan(x/float(windowWidth-1), y/float(windowHeight-1), camera.eye);
		camera.at = vec3( 0, 0, 0 );
		camera.up = vec3( 0, 1, 0 );
		camera.viewMatrix = mat4::lookAt( camera.eye, camera.at, camera.up );
	}
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
	else if (key == 'r' || key=='R')	// reset camera
	{
		camera.eye = vec3( 150, 150, 150 );
		camera.at = vec3( 0, 0, 0 );
		camera.up = vec3( 0, 1, 0 );
		camera.viewMatrix = mat4::lookAt( camera.eye, camera.at, camera.up );
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
	if(!checkShader( vertexShader, "../shaders/vertexShader" )){ printf( "Unable to compile vertex shader\n" ); return false; }

	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	const char* fragmentShaderSource = readShader( fragShaderPath ); if(fragmentShaderSource==NULL) return false;
	GLint fragmentShaderLength = strlen(fragmentShaderSource);
	glShaderSource( fragmentShader, 1, &fragmentShaderSource, &fragmentShaderLength );
	glCompileShader( fragmentShader );
	if(!checkShader( fragmentShader, "../shaders/fragmentShader" )){ printf( "Unable to compile fragment shader\n" ); return false; }

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

void createSphere(unsigned int rings, unsigned int sectors)
{
	float const R = 1.0/(float)(rings-1);
    float const S = 1.0/(float)(sectors-1);
    int r, s;

    for(r = 0; r < rings; r++) 
	{
    	for(s = 0; s < sectors; s++) 
    	{
	        float const y = sin( -M_PI_2 + M_PI * (float)r * (float)R );
	        float tmpSin = sin( M_PI * (float)r * (float)R );
	        float tmpAngle = 2*M_PI * (float)s * (float)S;
	        float const x = cos(tmpAngle) * tmpSin;
	        float const z = sin(tmpAngle) * tmpSin;

	        //std::cout << x << " " << y << " " << z << std::endl;
	        vertex tmp;

	        tmp.tex = vec2((float)s*(float)S,(100.0-(float)r)*(float)R);

	        tmp.pos = vec3(x,y,z);

	        tmp.norm = vec3(-x,-y,-z); 

	        vertices.push_back(tmp);      
    	}
    }

    for(r = 0; r < rings; r++)
    {
        for(s = 0; s < sectors; s++) 
        {         
            indices.push_back((GLushort)(r * sectors + s));
            indices.push_back((GLushort)(r * sectors + (s+1)));
            indices.push_back((GLushort)((r+1) * sectors + (s+1)));
            indices.push_back((GLushort)((r+1) * sectors + s));
    	}
    }
}

void CreatePlanets()
{
	float fakeDistance = 150.0;
	float distance = 0.0;
	Planet *tmp;
	tmp = new Planet(distance, 0.0, 25.38, 696.0, "../textures/sun.jpg");	//Sun
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 300.0;
	tmp = new Planet(/*57909.18*/distance, 87.97, 58.65, 2.44, "../textures/mercury.jpg");	//Mercury
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 2.44;
	tmp = new Planet(/*108208.93*/distance, 224.696, -243.0187, 6.052, "../textures/venus.jpg");	//Venus
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 6.052;
	tmp = new Planet(/*149597.89*/distance, 365.249, 0.9973, 6.378, "../textures/earth.jpg");	//Earth
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 6.378;
	tmp = new Planet(/*227936.64*/distance, 686.965, 1.02596, 3.397, "../textures/mars.jpg");	//Mars
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 3.397;
	tmp = new Planet(/*778412.01*/distance, 4332.7276, 0.41354, 71.49268, "../textures/jupter.jpg");	//Jupter
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 71.49268;
	tmp = new Planet(/*1426725.4*/distance, 10755.4689, 0.44401, 60.26714, "../textures/saturn.jpg");	//Saturn
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 60.26714;
	tmp = new Planet(/*2870972.2*/distance, 30686.4976, -0.71833, 25.55725, "../textures/uranus.jpg");	//Uranus
	planets.push_back(*tmp);
	delete tmp;
	distance += fakeDistance + 25.55725;
	tmp = new Planet(/*4498252.9*/distance, 60188.7441, 0.67125, 24.76636, "../textures/neptune.jpg");	//Neptune
	planets.push_back(*tmp);
	delete tmp;
	//delete(tmp);
}

bool userInit()
{
	//create sphere
	createSphere(100, 100);

	// init GL states
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );		// clear color for glClear()
	glEnable( GL_CULL_FACE );					// turn on backface culling
	glEnable( GL_DEPTH_TEST );					// turn on depth tests
	glShadeModel( GL_SMOOTH );

	// create a vertex buffer
	glGenBuffers( 1, &vertexPositionBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexPositionBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), &(vertices[0].pos.x), GL_STATIC_DRAW );

	// create a index buffer
	glGenBuffers( 1, &indexPositionBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexPositionBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW );

	// create textures
	glGenTextures( 1, &textureObject );
	glBindTexture( GL_TEXTURE_2D, textureObject );

	//create planets
	CreatePlanets();
	
	// init camera
	camera.eye = vec3( 150, 150, 150 );
	camera.at = vec3( 0, 0, 0 );
	camera.up = vec3( 0, 1, 0 );
	camera.viewMatrix = mat4::lookAt( camera.eye, camera.at, camera.up );

	// init light properties
	light.position = vec4( 0.0f, 0.0f, 0.0f, 1.0f );   // directional light
    light.ambient  = vec4( 0.5f, 0.5f, 0.5f, 1.0f );
    light.diffuse  = vec4( 0.8f, 0.8f, 0.8f, 1.0f );
    light.specular = vec4( 1.0f, 1.0f, 1.0f, 1.0f );

	// init material properties
	material.ambient  = vec4( 0.2f, 0.2f,  0.2f, 1.0f );
    material.diffuse  = vec4( 0.8f, 0.8f, 0.8f, 1.0f );
    material.specular = vec4( 0.1f, 0.1f, 0.1f, 1.0f );
	material.shininess = 100.0f;



	return true;
}

int main( int argc, char* argv[] )
{
	// GLUT initialization
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	int screenWidth		= glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight	= glutGet(GLUT_SCREEN_HEIGHT);
	
	glutInitWindowSize( windowWidth, windowHeight );
	glutInitWindowPosition( (screenWidth-windowWidth)/2, (screenHeight-windowHeight)/2 );
	glutCreateWindow( "Solar System" );

	// Register callbacks
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );		// callback when the window is resized
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );		// callback when the mouse is moving
	glutIdleFunc( idle );			// idle-time callback

	// init and check GLEW, version, extensions
	if(!initExtensions()){ printf( "Failed to init extensions.\n" ); return 0; }
	
	// create and compile shaders/program
	if(!initShaders("shaders/solar.vert","shaders/solar.frag")){ printf( "Failed to init program and shaders\n" ); return 0; }

	// user initialization
	if(!userInit()){ printf( "Failed to userInit()\n" ); return 0; }

	// Start rendering loop
	glutMainLoop();
	return 0;
}
