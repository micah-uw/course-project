#pragma warning(disable: 4201 )
#pragma warning(disable: 4505 )
//#pragma warning(default: 4201)
#define SDL_MAIN_HANDLED
#include <stdlib.h>
#include <stdio.h>
#include <SDL_config.h>
#include <SDL_stdinc.h>
#include <SDL.h>
#include <glad.h>
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "TestHarness.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <list>
#include <array>
#include "Link2d.h"

//globals
//screen dims
int g_ScreenHeight = 480;
int g_ScreenWidth = 640;
SDL_Window* g_GraphicsWindow = nullptr;
SDL_GLContext g_OpenGLContext = nullptr;
//main loop flag
bool gQuit = false;
//VAO vertex array object
GLuint gVertexArrayObj = 0;
GLuint gVertexArrayObj2 = 0;
//VBO
GLuint gVertexBufferObject = 0;
GLuint gVertexBufferObject2 = 0;
//Program object for shaders - graphics pipeline
GLuint gGraphicsPipelineShaderProgram = 0;

const std::string gVertexShaderSource = 
"#version 410 core\n" 
"in vec4 position;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
"}\n";

const std::string gFragmentShaderSource = 
"#version 410 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f,0.5f,0.0f,1.0f);\n"
"}\n";

GLuint CompileShader(GLuint type, const std::string& source)
{
	GLuint shaderObject = 0;
	if (type == GL_VERTEX_SHADER)
	{
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}
	const char* src = source.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	return shaderObject;
}
GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	GLuint programObject = glCreateProgram();
	GLuint m_VertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(programObject, m_VertexShader);
	glAttachShader(programObject, m_FragmentShader);
	glLinkProgram(programObject);

	//validate out program
	glValidateProgram(programObject);
	return programObject;
}
void CreateGraphicsPipeline()
{
	gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}
void GetOpenGLInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
void VertexSpecification()
{
	Link2d* chain = new Link2d();
	chain->AddLink(.5f);
	chain->AddLink(.5f);
	chain->AddLink(.5f);
	chain->AddLink(.5f);
	chain->InverseKinematics(3.f, 3.f, 1000);
	
	//GLfloat link1{ chain->joints[0].xpos, chain->joints[0].ypos, 0.0 };
	//lives on cpu
	//geometry data
	//store x,y,z position attributes within vertexPositions
	//starts stored in cpu
	//in a call to glBufferData stores info in a vertex buffer object on gpu
	//
	const std::vector<GLfloat> vertexPositions{ chain->chain[0].xpos, chain->chain[0].ypos, 0.0f, //vertex1
												chain->chain[1].xpos, chain->chain[1].ypos, 0.0f, //vertex2
	};

	const std::vector<GLfloat> vertexPositions2{ chain->chain[1].xpos, chain->chain[1].ypos, 0.0f, //vertex1
												chain->chain[2].xpos, chain->chain[2].ypos, 0.0f, //vertex2
	};
	//const std::vector<GLfloat> vertexPositions{ 1.f,0.0f,0.0f, //vertex1
	//										0.0f,-1.0f,0.0f //vertex2
	//										//0.0f,0.8f,0.0f //vertex3
	//};
	
	//set things up on GPU
	//Vertex Arrays Object (VAO)
	//VAO can be thought of as a wrapper around all of vertex buffer objects VBO's
	//glBindVertexArray select the VAO we want to use
	glGenVertexArrays(1, &gVertexArrayObj);
	glBindVertexArray(gVertexArrayObj);

	glGenVertexArrays(1, &gVertexArrayObj2);
	glBindVertexArray(gVertexArrayObj2);
	//Start generating vertex buffer object
	//Create new VBO
	glGenBuffers(1, &gVertexBufferObject);
	glGenBuffers(1, &gVertexBufferObject2);
	//equivilent to 'selecting the active buffer object'
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject2);
	//in the currently binded buffer, we populate the data from our 'vertexPositions' (which is on the cpu)
	//onto the buffer that will live on the GPU
	glBufferData(GL_ARRAY_BUFFER,
		vertexPositions.size() * sizeof(GL_FLOAT),
		vertexPositions.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER,
		vertexPositions2.size() * sizeof(GL_FLOAT),
		vertexPositions2.data(), GL_STATIC_DRAW);
	//for given vertex array object, we need to tell opengl 'how' the information
	//in our buffer will be used
	glEnableVertexAttribArray(0);
	//for the specific attribute in our vertex specification, we user 
	//'glVertexAttribPointer' to figure out how we are going to move through the data
	glVertexAttribPointer(0, //attribute 0 corresponds to the neabled glEnableVertexAttribArray
		3,	//the number of components (e.g. x,y,z = 3 components)
		GL_FLOAT, 
		GL_FALSE, 
		0, 
		(void*)0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}


void InitializeProgram()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL2 could not initialize video subsystem."<< std::endl;
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


	g_GraphicsWindow = SDL_CreateWindow("OpenGL IK window",
		200, 200, g_ScreenWidth, g_ScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (g_GraphicsWindow == nullptr)
	{
		std::cout << "SDL_Window was not able to be created." << std::endl;
		exit(1);
	}

	//Create an OpenGL Graphics Context
	g_OpenGLContext = SDL_GL_CreateContext(g_GraphicsWindow);
	if (g_OpenGLContext == nullptr)
	{
		std::cout << "OpenGL context not available." << std::endl;
		exit(1);
	}

	//initialize glad library
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cout << "glad was not initialized." << std::endl;
		exit(1);
	}

	GetOpenGLInfo();
}

void Input()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			std::cout << "Goodbye!" << std::endl;
			gQuit = true;
		}
	}
}

void PreDraw()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, g_ScreenWidth, g_ScreenHeight);
	glClearColor(1.f, 1.f, 0.1f, 1.f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw()
{
	glBindVertexArray(gVertexArrayObj);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glDrawArrays(GL_LINE_STRIP, 0, 4);

	glBindVertexArray(gVertexArrayObj2);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject2);
	glDrawArrays(GL_LINE_STRIP, 0, 4);
	
}

void MainLoop()
{
	while (!gQuit)
	{
		Input();

		PreDraw();

		Draw();

		//update the screen
		SDL_GL_SwapWindow(g_GraphicsWindow);
	}
}

void CleanUp() 
{
	SDL_DestroyWindow(g_GraphicsWindow);
	SDL_Quit();
}

int main() 
{
	InitializeProgram();

	VertexSpecification();

	CreateGraphicsPipeline();

	MainLoop();

	CleanUp();

	return 0;

}
