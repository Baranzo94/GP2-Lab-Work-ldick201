//Header files
#include <iostream>

//header for SDL2 functionality
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>


//Global variables go here

GLuint triangleVBO;

//Pointer to our SDL Windows
SDL_Window*window;

//SDL GL Context
SDL_GLContext glcontext = NULL;

float triangleData[] = { 0.0f, 1.0f, 0.0f,   //Top
						-1.0f, -1.0f, 0.0f,  //Bottom Left
						1.0f, -1.0f, 0.0f }; // Bottom Righ


//Constants to control window creation
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
bool running = true;

// Global functions
void InitWindow(int width, int height, bool fullscreen)
{
	//Create a window
	window = SDL_CreateWindow
		(
		"Lab 1", //Window Title
		SDL_WINDOWPOS_CENTERED, //x position, centered
		SDL_WINDOWPOS_CENTERED, //y position, centered
		width, //width in pixels
		height, //height in pixels
		SDL_WINDOW_OPENGL //flags
		);
}

void initGeometry()
{

	//Create buffer
	glGenBuffers(1, &triangleVBO);

	//Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	//Copy Vertex Data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData),
		triangleData, GL_STATIC_DRAW);
}
//Used to cleanup once we exit
void CleanUp()
{
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	glDeleteBuffers(1, &triangleVBO);

}

//Function to intialise OpenGL
void initOpenGL()
{
	//Create OpenGL Context
	glcontext = SDL_GL_CreateContext(window);
	//something went wrong in creating the context, if it is still NULL
	if (!glcontext)
	{
		std::cout << "Error Creating OpenGL Context" << SDL_GetError() <<
		std::endl;
	}

	//Smooth shading 
	glShadeModel(GL_SMOOTH);

	//Clear the background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Clear the depth buffer to 1.0
	glClearDepth(1.0f);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//The depth test to use
	glDepthFunc(GL_LEQUAL);

	//Turn on best perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/*Problem : glewInit failed, something is seriously wrong.*/
		std::cout << "Error:" << glewGetErrorString(err) << std::endl;
	}

}

//Function to set/reset viewport
void setViewport(int width, int height)
{
//screen ration
	GLfloat ratio;

	//make sure height is always above 0
	if (height == 0)
	{
		height = 1;
	}

	//calculate screen ration
	ratio = (GLfloat)width / (GLfloat)height;

	// Setup viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//Change to project matrix mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Calculate perspective matrix, using glu library functions
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);

	//Reset using the Identity Matrix
	glLoadIdentity();
}

//Function to draw
void render()
{
	//Set the clear colour(background)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Make the new VBO active. Repeat here as a sanity check (may have changed since intialisation)
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	
	//Establish its 3 coordinates per vertex with zero stride(space between elements
	//in array and contain floating point numbers
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//Establish array contains vertices (not normals, colours, texture coords etc)
	glEnableClientState(GL_VERTEX_ARRAY);

	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);

	//Reset using the Identity Matrix
	glLoadIdentity();

	//Translate
	glTranslatef(0.0f, 0.0f,-6.0f);


	//Actually draw the triangle, giving the number of vertices provided
	glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / (3 * sizeof(float)));

	


	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);

	//Reset using the Identity Matrix
	glLoadIdentity();

	//Translate to -5.0f on z-axis
	glTranslatef(0.0f, 0.0f, -5.0f);

	glTranslatef(2.0f, 0.0f, -6.0f);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / (3 * sizeof(float)));
	glLoadIdentity();

	//Begin drawing triangles
	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0f, 0.0f, 0.0f); //Colour of the vertices
	//glVertex3f(0.0f, 1.0f, 0.0f); //Top
	//glVertex3f(-1.0f, -1.0f, 0.0f); //Bottom Left
	//glVertex3f(1.0f, -1.0f, 0.0f); //Bottom Right
	//glEnd();

	//require to swap the back and front buffer
	SDL_GL_SwapWindow(window);

}

//Function to update game state
void update()
{

}
//Main Method - Entry Point
int main(int argc, char*arg[])
{
	//init everything - SDL, if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;
		return -1;
		
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	//Call our InitOpenGL Function
	initOpenGL();
	//Calling InitGeometry Function
	initGeometry();

	//Set our viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

	SDL_Event event;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
			{
				//set our boolean which controls the game loop to false
				running = false;
			}
		}
		update();
		render();
	}

	CleanUp();

	
	return 0;
}