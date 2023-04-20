#include "TestHarness.h"
#include "Link.h"

TEST(LinkCreate_getx, Link)
{
	Link link1(1.0f, 2.0f, 3.0f, 1.f);
	CHECK_EQUAL(1.0f, link1.getX());
}
TEST(LinkCreate_gety, Link)
{
	Link link1(1.0f, 2.0f, 3.0f, 1.f);
	CHECK_EQUAL(2.0f, link1.getY());
}
TEST(LinkCreate_getz, Link)
{
	Link link1(1.0f, 2.0f, 3.0f, 1.f);
	CHECK_EQUAL(3.0f, link1.getZ());
}
TEST(Link_add, Link)
{
	Link link1(1.0f, 2.0f, 3.0f, 1.f);
	Link link2(2.0f, 3.0f, 4.0f, 1.f);
	Link link3 = link1 + link2;
	CHECK_EQUAL((3.0f, 5.0f, 7.0f), (link3.getX(),link3.getY(),link3.getZ()));
}
TEST(Link_dot, Link)
{
	Link link1(1.0f, 2.0f, 3.0f, 1.f);
	Link link2(2.0f, 3.0f, 4.0f, 1.f);
	CHECK_EQUAL((20.0f), (link1 ^ link2));
}
TEST(Link_cross, Link)
{
	Link link1(1.0f, 2.0f, 3.0f, 1.f);
	Link link2(2.0f, 3.0f, 4.0f, 1.f);
	Link link3 = link1 * link2;
	CHECK_EQUAL((-1.f,2.0f,-1.0f), (link3.getX(), link3.getY(), link3.getZ()));
}