#pragma once
#include "glm.hpp"
#include <iostream>

class Link
{
private:
	glm::vec4 m_link;
   
public:
    Link();
    Link(float x, float y, float z, float w);

    Link(const Link&) = default;
    Link(Link&&) = default;
    Link& operator=(const Link&) = default;
    Link& operator=(Link&&) = default;
    Link operator+(Link); // ADD 2 Vectors
    Link operator-(Link); // Subtraction
    float operator^(Link); // Dot Product
    Link operator*(Link); // Cross Product
    float getX();
    float getY();
    float getZ();
    float getW();
    float magnitude();
    //glm::vec4 rotationFromTo(const glm::vec4& from, const glm::vec4& to);

    friend std::ostream& operator<<(std::ostream&, const Link&);
    ~Link() = default;
};

