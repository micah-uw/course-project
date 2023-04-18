#pragma once
#include "glm.hpp"
#include "SDLFramework.h"
#include <vector>

struct LinkSegment
{
    float xpos, ypos;
    float len;
    float localAngle;
    float globalAngle;
};

class Link2d
{
public:
    std::vector<LinkSegment> chain;
    float xEnd, yEnd;
    int m_iterations;

    void AddLink(float len);
    void ForwardKinematics(int startLink);
    bool InverseKinematics(float xTarget, float yTarget, int steps);
    void RenderChain(Link2d*, SDLFramework& fw);
    void resultLog(Link2d*, bool );
    bool compare_float(float , float , float);
};

