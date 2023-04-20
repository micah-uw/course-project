#include "Link2d.h"
#include <cmath>
#include "helpers.h"
#include <iostream>
#include "SDLFramework.h"

Link2d::Link2d()
	:chain{}, xEnd { 0 }, yEnd{ 0 }, m_iterations{ 0 }
{}

void Link2d::AddLink(float len) {
	LinkSegment newLink{0.f,0.f,0.f,0.f,0.f};
	newLink.len = len;
	chain.push_back(newLink);
}

void Link2d::RenderChain(Link2d* link2d, SDLFramework& fw)
{
	std::array<Uint8, 4> blue{ 0,0,255,1 };
	std::array<Uint8, 4> red{ 255,0,0,1 };
	std::array<Uint8, 4> green{ 60,179,113,1 };
	std::array<Uint8, 4> yellow{ 255,165,0 ,1 };
	std::array<Uint8, 4> pink{ 238,130,238,1 };
	std::array<Uint8, 4> purple{ 106,90,205,1 };
	std::array<Uint8, 4> orange{ 245,97,0,1 };
	std::array<std::array<Uint8, 4>, 7> colors{ {{blue},{red},{green},{yellow},{pink},{purple},{orange}} };
	fw.draw_lineF(blue,  link2d->chain[0].xpos, link2d->chain[0].ypos, link2d->chain[1].xpos, link2d->chain[1].ypos);
	auto chainSize = link2d->chain.size();
	for (size_t i = 1; i < chainSize - 1; i++)
	{
		auto colorNumber = (int)i % (int)colors.size();
		fw.draw_lineF(colors[colorNumber], link2d->chain[i].xpos, link2d->chain[i].ypos, link2d->chain[i + 1].xpos, link2d->chain[i + 1].ypos);
	}
	fw.draw_lineF(purple, link2d->chain[chainSize -1].xpos, link2d->chain[chainSize-1].ypos, link2d->xEnd, link2d->yEnd);
}

void Link2d::ForwardKinematics(int startJoint) {
	chain[startJoint].globalAngle = (startJoint > 0 ? chain[startJoint - 1].globalAngle : 0) + chain[startJoint].localAngle;

	for (int i = startJoint + 1; i < chain.size(); i++) 
	{
		chain[i].globalAngle = chain[i - 1].globalAngle + chain[i].localAngle;
		chain[i].xpos = findX(chain[i - 1].xpos, chain[i - 1].globalAngle, chain[i - 1].len);
		//chain[i].xpos = chain[i - 1].xpos + sin(chain[i - 1].globalAngle) * chain[i - 1].len;
		chain[i].ypos = chain[i - 1].ypos + cos(chain[i - 1].globalAngle) * chain[i - 1].len;
	}
	xEnd = chain[chain.size() - 1].xpos + sin(chain[chain.size() - 1].globalAngle) * chain[chain.size() - 1].len;
	yEnd = chain[chain.size() - 1].ypos + cos(chain[chain.size() - 1].globalAngle) * chain[chain.size() - 1].len;
}

bool Link2d::InverseKinematics(float xTarget, float yTarget, int iterations) 
{	
	m_iterations = iterations;
	bool found = false;
	while(!found && iterations--)
	{
		if (compare_float(xEnd, xTarget,.001f) && compare_float(yEnd, yTarget, .001f))
		{
			m_iterations -= iterations;
			found = true;
		}
		chain[chain.size() - 1].localAngle = atan2(xTarget - chain[chain.size() - 1].xpos, yTarget - chain[chain.size() - 1].ypos) - chain[chain.size() - 2].globalAngle;

		this->ForwardKinematics((int)chain.size() - 2);

		for (int j = (int)chain.size() - 2; j > 0; j--)
		{
			this->ForwardKinematics(j - 1);

			float currentAngle = atan2(xEnd - chain[j].xpos, yEnd - chain[j].ypos );
			float targetAngle = atan2( xTarget - chain[j].xpos, yTarget - chain[j].ypos );
			chain[j].globalAngle -= currentAngle - targetAngle;
			chain[j].localAngle = chain[j].globalAngle - chain[j - 1].globalAngle;
		}

		this->ForwardKinematics(0);

		float currentAngle = atan2(xEnd, yEnd);
		float targetAngle = atan2(xTarget, yTarget);

		chain[0].localAngle -= currentAngle - targetAngle;
	}
	this->ForwardKinematics(0);
	chain[chain.size() - 1].localAngle = atan2(xTarget - chain[chain.size() - 1].xpos, yTarget - chain[chain.size() - 1].ypos) - chain[chain.size() - 2].globalAngle;
	return found;
}

float Link2d::findX(float parentX, float parentGlobalAngle, float parentLength)
{
	return parentX + sin(parentGlobalAngle) * parentLength;
}

bool Link2d::compare_float(float x, float y, float epsilon)
{
	if (fabs(x - y) < epsilon)
		return true;
	return false;
}

void Link2d::resultLog(Link2d* link2d, bool foundSolution)
{
	if (foundSolution)
	{
		std::cout << "Found solution!\nAt iteration: " << link2d->m_iterations << std::endl;
	}
	else
	{
		std::cout << "No solution found :(" << std::endl;
	}
}