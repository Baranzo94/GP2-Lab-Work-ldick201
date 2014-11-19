//Header files
#include <iostream>

//header for SDL2 functionality
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>
#include "Vertex.h"
#include "Shader.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

//Maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifdef _DEBUG && WIN32
const std::string ASSET_PATH = "./assets";
#else
const std::string ASSET_PATH = "assets";
#endif

const std::string SHADER_PATH = "/shaders";

//Our headers
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"


//Global variables go here

GLuint triangleVBO;

GLuint triangleEBO;

GLuint VAO;

//Shader Program
GLuint shaderProgram = 0;

//matrices
mat4 viewMatrix;
mat4 projMatrix;
mat4 worldMatrix;

GLuint texture = 0;

//Pointer to our SDL Windows
SDL_Window*window;

//SDL GL Context
SDL_GLContext glcontext = NULL;

Vertex triangleData[] = {

	{ vec3(-0.5f, 0.5f, 0.5f), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Top Left
	{ vec3(-0.5f, -0.5f, 0.5f), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },// Bottom Left
	{ vec3(0.5f, -0.5f, 0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, //Bottom Right
	{ vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Top Right
	//back
	{ vec3(-0.5f, 0.5f, -0.5f), vec2(0.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },// Top Left
	{ vec3(-0.5f, -0.5f, -0.5f), vec2(0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Bottom Left
	{ vec3(0.5f, -0.5f, -0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f) }, //Bottom Right
	{ vec3(0.5f, 0.5f, -0.5f), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) }// Top Right
	//{ vec3(-0.5f, 0.5f, 0.0f), vec4(1.0f,0.0f,0.0f,1.0f) },
/*	//Front
	{ -0.5f, 0.5f, 0.5f,
	1.0f, 0.0f, 1.0f, 1.0f }, //Top Left

	{ -0.5f, -0.5f, 0.5f,
	1.0f, 1.0f, 0.0f, 1.0f }, //Bottom Left

	{ 0.5f, -0.5f, 0.5f,
	0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right

	{ 0.5f, 0.5f, 0.5f,
	1.0f, 0.0f, 1.0f, 1.0f }, //Top Right

	//Back
	{ -0.5f, 0.5f, -0.5f,
	1.0f, 0.0f, 1.0f, 1.0f }, //Top Left

	{-0.5f,-0.5f,-0.5f,
	1.0f,1.0f,0.0f,1.0f}, //Bottom Left

	{0.5f,-0.5f,-0.5f,
	0.0f,1.0f,1.0f,1.0f}, //Bottom Right

	{0.5f,0.5f,-0.5f,
	1.0f,0.0f,1.0f,1.0f}, //Top Right
*/
};

GLuint indices[] = {

	//Front
	0,1,2,
	0,3,2,

	//Left
	4,5,1,
	4,1,0,

	//Right
	3,7,2,
	7,6,2,

	//Bottom
	1,5,2,
	6,2,1,

	//Top
	5,0,7,
	5,7,3,

	//Back
	4,5,6,
	4,7,6,

};

//Constants to control window creation
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
bool running = true;

std::vector<GameObject*> displayList;
GameObject * mainCamera;

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

	//Create buffer
	glGenBuffers(1, &triangleEBO);

	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,triangleEBO);

	//Copy Index data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}
//Used to cleanup once we exit
void CleanUp()
{
	auto iter = displayList.begin();
	while (iter != displayList.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = displayList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	displayList.clear();
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteProgram(shaderProgram);
	//glDeleteBuffers(1, &triangleEBO);
	//glDeleteBuffers(1, &triangleVBO);
	//glDeleteTextures(1,&texture);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

//Function to intialise OpenGL
void initOpenGL()
{
	//Ask for version 3.2 of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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

	glewExperimental = GL_TRUE;

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

}

void Initialise()
{
	mainCamera = new GameObject();
	mainCamera->setName("MainCamera");
	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 10.0f);
	mainCamera->setTransform(t);
	Camera * c = new Camera();
	c->setAspectRatio((float)(WINDOW_WIDTH / WINDOW_HEIGHT));
	c->setFOV(45.0f);
	c->setNearClip(0.1f);
	c->setFarClip(1000.0f);
	mainCamera->setCamera(c);
	displayList.push_back(mainCamera);
	GameObject * cube = new GameObject();
	cube->setName("Cube");
	Transform *transform = new Transform();
	transform->setPosition(0.0f, 0.0f, 0.0f);
	cube->setTransform(transform);
	Material * material = new Material();
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	material->loadShader(vsPath, fsPath);
	cube->setMaterial(material);
	Mesh * mesh = new Mesh();
	cube->setMesh(mesh);
	displayList.push_back(cube);
	//alternative sytanx
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->init();
	}
	mesh->copyVertexData(8, sizeof(Vertex), (void**)triangleData);
	mesh->copyIndexData(36, sizeof(int), (void**)indices);
}


//Function to draw
void render()
{
	//Set the clear colour(background)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	//Clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Make the new VBO active. Repeat here as a sanity check (may have changed since intialisation)
	//glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//Same with the EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);

	//glBindVertexArray(VAO);

	//glUseProgram(shaderProgram);
	//GLint texture0Location = glGetUniformLocation(shaderProgram,"texture0");
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(texture0Location,0);

	//GLint MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	//mat4 MVP = projMatrix*viewMatrix*worldMatrix;
	//glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
	//Tell the shader that 0 is the position element
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),NULL);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)sizeof(vec3));
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)+sizeof(vec2)));

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->render();
		Mesh * currentMesh = (*iter)->getMesh();
		Transform * currentTransform = (*iter)->getTransform();
		Material * currentMaterial = (*iter)->getMaterial();
		if (currentMesh && currentMaterial && currentTransform)
		{
			currentMaterial->bind();
			currentMesh->bind();
			GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
			Camera * cam = mainCamera->getCamera();
			mat4 MVP = cam->getProjection()*cam->getView()*currentTransform->getModel();
			glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
			glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);
		}
	}

	//glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	//require to swap the back and front buffer
	SDL_GL_SwapWindow(window);

}

void createShader()
{
	GLuint vertexShaderProgram = 0;
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/textureVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/textureFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);
	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderProgram);
	glAttachShader(shaderProgram, fragmentShaderProgram);
	glLinkProgram(shaderProgram);
	checkForLinkErrors(shaderProgram);
	glBindAttribLocation(shaderProgram, 0, "vertexPosition");
	glBindAttribLocation(shaderProgram, 1, "vertexTexCoords");
	glBindAttribLocation(shaderProgram, 2, "vertexColour");

	//now we can delte the VS & FS Programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);

}

//Function to update game state
void update()
{
	//projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	//viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//worldMatrix = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));

	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->update();
	}
}

void createTexture()
{
	std::string texturePath = ASSET_PATH + "/texture.png";
	texture = loadTextureFromFile(texturePath);
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

	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags) {
		std::cout << "ERROR SDL_Image Init " << IMG_GetError() << std::endl;
		// handle error
	}
	if (TTF_Init() == -1) {
		std::cout << "TTF_Init: " << TTF_GetError();
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	//Call our InitOpenGL Function
	initOpenGL();
	//Calling InitGeometry Function
	//initGeometry();

	//Set our viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

	Initialise();

	SDL_Event event;

	createShader();

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