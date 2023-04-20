#include "Link.h"
Link::Link()
    :m_link{0.f,0.f,0.f,0.f}
{}

Link::Link(float x, float y, float z, float w)
{
    m_link.x = x;
    m_link.y = y;
    m_link.z = z;
    m_link.w = w;
}

Link Link::operator+(Link link)
{
    float x = m_link.x + link.m_link.x;
    float y = m_link.y + link.m_link.y;
    float z = m_link.z + link.m_link.z;
    float w = m_link.w + link.m_link.w;
    return Link(x,y,z,w);
}

Link Link::operator-(Link link)
{
    float x = m_link.x - link.m_link.x;
    float y = m_link.y - link.m_link.y;
    float z = m_link.z - link.m_link.z;
    float w = m_link.w - link.m_link.w;
    return Link(x, y , z, w);
}

float Link::operator^(Link link)
{
    float x1 = m_link.x * link.m_link.x;
    float y1 = m_link.y * link.m_link.y;
    float z1 = m_link.z * link.m_link.z;
    return (x1 + y1 + z1);
}

Link Link::operator*(Link link)
{
    float x1, y1, z1;
    x1 = m_link.y * link.m_link.z - m_link.z * link.m_link.y;
    y1 = m_link.z * link.m_link.x - m_link.x * link.m_link.z;
    z1 = m_link.x * link.m_link.y - m_link.y * link.m_link.x;
    return Link(x1, y1, z1, 0.f);
}

float Link::magnitude()
{
    return sqrtf(powf(m_link.x, 2) + powf(m_link.y, 2) + powf(m_link.z, 2));
}

float Link::getX()
{
    return m_link.x;
}

float Link::getY()
{
    return m_link.y;
}
float Link::getZ()
{
    return m_link.z;
}
float Link::getW()
{
    return m_link.w;
}
std::ostream& operator<<(std::ostream& out, const Link& link)
{
    out << link.m_link.x << "i ";
    if (link.m_link.y >= 0)
        out << "+ ";
    out << link.m_link.y << "j ";
    if (link.m_link.z >= 0)
        out << "+ ";
    out << link.m_link.z << "k ";
    if (link.m_link.w >= 0)
        out << "+ ";
    out << link.m_link.w << "w" << std::endl;
    return out;
}