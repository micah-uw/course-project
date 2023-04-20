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

    Link2d();
    Link2d(const Link2d&) = delete;
    Link2d(Link2d&&) = delete;
    Link2d& operator=(const Link2d&) = delete;
    Link2d& operator=(Link2d&&) = delete;
    ~Link2d() = default;
    void AddLink(float len);
    void ForwardKinematics(int startLink);
    bool InverseKinematics(float xTarget, float yTarget, int steps);
    float findX(float parentX, float parentGlobalAngle, float parentLength);
    void RenderChain(Link2d*, SDLFramework& fw);
    void resultLog(Link2d*, bool );
    bool compare_float(float , float , float);
};

