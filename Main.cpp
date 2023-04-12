#include "TestHarness.h"
#include <iostream>
#include "SDL.h"
#include <iomanip>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "SDLFrameWork.h"
#include "Link2d.h"
#include "helpers.h"
#undef main

void resultLog(Link2d* chain, bool foundSolution)
{
	if (foundSolution)
	{
		std::cout << "Found solution!\nAt iteration: " << chain->m_iterations << std::endl;
	}
	else
	{
		std::cout << "No solution found :(" << std::endl;
	}
}
int main(int argc, char* argv[])
{
	argc;
	(void*)argv;
	static int width = 1280;
	static int height = 720;
	static unsigned int centerX = width / 2;
	static unsigned int centerY = height / 2;
	SDLFramework fw(width, height);
	std::array<Uint8, 4> blue{ 0,0,255,1 };
	std::array<Uint8, 4> red{ 255,0,0,1 };
	std::array<Uint8, 4> green{ 60,179,113,1 };
	std::array<Uint8, 4> yellow{ 255,165,0 ,1 };
	std::array<Uint8, 4> pink{ 238,130,238,1 };
	std::array<Uint8, 4> purple{ 106,90,205,1 };

	//draw graph
	fw.draw_lineF(purple, (float) - width, .0f, (float)width, .0f);
	fw.draw_lineF(purple, .0f,(float) - height, .0f, (float)height);

	glm::vec2 target{ 100.0f, 75.0f };
	fw.draw_circle(red, (int)target.x, (int)target.y, 10);

	//instantiate chain
	Link2d* chain = new Link2d();
	chain->AddLink(100.f);
	chain->AddLink(100.f);
	chain->AddLink(75.f);
	chain->AddLink(25.f);
	//set chain
	chain->ForwardKinematics(0);
	//render pre-transform
	chain->RenderChain(chain, fw);
	//try to solve for <.001f epsilon under 10000
	bool foundSolution = chain->InverseKinematics(target.x, target.y, 10000);
	resultLog(chain, foundSolution);
	//render translated chain
	chain->RenderChain(chain, fw);

	//try to hit another target with another chain
	glm::vec2 target2{ 200.0f, -205.0f };
	fw.draw_circle(red, (int)target2.x, (int)target2.y, 10);
	Link2d* chain2 = new Link2d();
	chain2->AddLink(100.f);
	chain2->AddLink(100.f);
	chain2->AddLink(75.f);
	chain2->AddLink(50.f);
	chain2->AddLink(50.f);
	chain2->AddLink(50.f);
	//add more links to chain

	//chain base position
	chain2->chain[0].xpos = { 100.f };
	chain2->chain[0].ypos = { 100.f };
	chain2->ForwardKinematics(0);
	bool foundSolution2 = chain2->InverseKinematics(target2.x, target2.y, 10000);
	resultLog(chain2, foundSolution2);
	//chain2->ForwardKinematics(0);
	chain2->RenderChain(chain2, fw);

	SDL_Event event{};    // Event variable
	// Below while loop checks if the window has terminated using close in the
	//  corner.
	while (!(event.type == SDL_QUIT)) {
		SDL_Delay(10);  // setting some Delay
		SDL_PollEvent(&event);  // Catching the poll event.
	}
}