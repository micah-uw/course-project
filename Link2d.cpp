#include "Link2d.h"
#include <numeric>
#include "helpers.h"
#include <iostream>
#include "SDLFramework.h"

void Link2d::AddLink(float len) {
	LinkSegment newLink = { 0 };
	newLink.len = len;
	chain.push_back(newLink);
}

void Link2d::RenderChain(Link2d* chain, SDLFramework& fw)
{
	std::array<Uint8, 4> blue{ 0,0,255,1 };
	std::array<Uint8, 4> red{ 255,0,0,1 };
	std::array<Uint8, 4> green{ 60,179,113,1 };
	std::array<Uint8, 4> yellow{ 255,165,0 ,1 };
	std::array<Uint8, 4> pink{ 238,130,238,1 };
	std::array<Uint8, 4> purple{ 106,90,205,1 };
	std::array<Uint8, 4> orange{ 245,97,0,1 };
	std::array<std::array<Uint8, 4>, 7> colors{ {{blue},{red},{green},{yellow},{pink},{purple},{orange}} };
	fw.draw_lineF(blue,  chain->chain[0].xpos, chain->chain[0].ypos, chain->chain[1].xpos, chain->chain[1].ypos);
	auto chainSize = chain->chain.size();
	for (size_t i = 1; i < chainSize - 1; i++)
	{
		auto colorNumber = (int)i % (int)colors.size();
		fw.draw_lineF(colors[colorNumber], chain->chain[i].xpos, chain->chain[i].ypos, chain->chain[i + 1].xpos, chain->chain[i + 1].ypos);
	}
	fw.draw_lineF(purple, chain->chain[chainSize -1].xpos, chain->chain[chainSize-1].ypos, chain->xEnd, chain->yEnd);
}

void Link2d::ForwardKinematics(int startJoint) {
	chain[startJoint].globalAngle = (startJoint > 0 ? chain[startJoint - 1].globalAngle : 0) + chain[startJoint].localAngle;

	for (int i = startJoint + 1; i < chain.size(); i++) 
	{

		//global angle of link = global angle of parent link + link local angle
		chain[i].globalAngle = chain[i - 1].globalAngle + chain[i].localAngle;
		//link xpos = parent link xpos + sin(parent.globalangle) * parent.length
		chain[i].xpos = chain[i - 1].xpos + sin(chain[i - 1].globalAngle) * chain[i - 1].len;
		//link ypos = parent link ypos + cos(parent.globalangle) * parent.length
		chain[i].ypos = chain[i - 1].ypos + cos(chain[i - 1].globalAngle) * chain[i - 1].len;
	}
	//final link xpos + sin(final link globalangle) * finallink lenght
	xEnd = chain[chain.size() - 1].xpos + sin(chain[chain.size() - 1].globalAngle) * chain[chain.size() - 1].len;
	//final link ypos + cos(final link globalangle) * finallink lenght
	yEnd = chain[chain.size() - 1].ypos + cos(chain[chain.size() - 1].globalAngle) * chain[chain.size() - 1].len;
}

bool Link2d::InverseKinematics(float xTarget, float yTarget, int iterations) 
{	
	m_iterations = iterations;
	bool found = false;
	while(!found && iterations--)
	{
		if (compare_float(xEnd, xTarget) && compare_float(yEnd, yTarget))
		{
			m_iterations -= iterations;
			found = true;
		}
		//last link localangle = arctan(x,y) atan2( xTarget - finallink x, yTarget - final link y) - second to last link global angle
		LinkSegment lastLink = chain[chain.size() - 1];
		LinkSegment secondToLast = chain[chain.size() - 2];

		lastLink.localAngle = atan2(xTarget - lastLink.xpos, yTarget - lastLink.ypos) - secondToLast.globalAngle;
		//auto degrees = lastLink.localAngle * (180.0 / PI);
		this->ForwardKinematics((int)chain.size() - 2);

		for (int j = (int)chain.size() - 2; j > 0; j--)
		{
			this->ForwardKinematics(j - 1);
			float currentAngle = atan2(xEnd - chain[j].xpos, yEnd - chain[j].ypos);
			float targetAngle = atan2(xTarget - chain[j].xpos, yTarget - chain[j].ypos);
			chain[j].globalAngle -= currentAngle - targetAngle;
			chain[j].localAngle = chain[j].globalAngle - chain[j - 1].globalAngle;
		}
		this->ForwardKinematics(0);
		float currentAngle = atan2(xEnd, yEnd);
		float targetAngle = atan2(xTarget, yTarget);
		//auto degrees2 = targetAngle * (180.0 / PI);
		chain[0].localAngle -= currentAngle - targetAngle;
	}
	this->ForwardKinematics(0);
	chain[chain.size() - 1].localAngle = atan2(xTarget - chain[chain.size() - 1].xpos, yTarget - chain[chain.size() - 1].ypos) - chain[chain.size() - 2].globalAngle;
	return found;
}